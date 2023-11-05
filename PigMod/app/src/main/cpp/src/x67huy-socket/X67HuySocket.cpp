//
// Created by nguye on 11/4/2023.
//

#include "X67HuySocket.h"

X67HuySocket::X67HuySocket(const X67HuySocket &sk) {
}

X67HuySocket::X67HuySocket(const char *host, int port) {
    _host = std::string(host);
    _port = port;
}

void X67HuySocket::start() {
    pthread_create(&_id, nullptr, &X67HuySocket::socketThreadStatic, (void*)this);
}

void *X67HuySocket::socketThreadStatic(void *thisVoid) {
    X67HuySocket* thiz = (X67HuySocket*) thisVoid;
    return thiz->socketThread();
}

void* X67HuySocket::socketThread() {
    LOG_E("Socket connecting... %s %i", _host.c_str(), _port);
    _socket = socket(AF_INET, SOCK_STREAM, 0);
    if (_socket < 0) {
        LOG_E("Can't create socket");
        return nullptr;
    }

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(_port);
    serverAddr.sin_addr.s_addr = inet_addr(_host.c_str());

    if (connect(_socket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        LOG_E("Can't connect socket");
        close(_socket);
        start();
        return nullptr;
    }

    LOG_E("Socket connected!");
    uint8_t buffer[SOCKET_BUFFER_SIZE];
    int receivedBytes = -1;
    int totalBytesReceived = 0;

    emit(X67_EVENT_OPEN, json());
    do {
        receivedBytes = recv(_socket, buffer + totalBytesReceived, SOCKET_BUFFER_SIZE - totalBytesReceived, 0);
        totalBytesReceived += receivedBytes;
        if (totalBytesReceived == SOCKET_BUFFER_SIZE) {
            FrameSession fs;
            handleFrame(buffer, totalBytesReceived, fs);
            if (fs.stage == FrameStage::Error) {
                LOG_E("Frame false!");
                break;
            }
            totalBytesReceived = 0;
        }
    } while (receivedBytes > 0);

    if (_allData != nullptr) {
        free(_allData);
        _allData = nullptr;
    }

    if (_key != nullptr) {
        free(_key);
        _key = nullptr;
    }

    emit(X67_EVENT_CLOSE, json());
    LOG_E("Socket closed!");
    sleep(1);
    start();
    return nullptr;
}

void X67HuySocket::handleFrame(uint8_t *buffer, size_t size, FrameSession& fs) {
    LOG_E("frame");

    uint8_t byteFin = buffer[OFFSET_FIN];
    uint8_t byteOpcode = buffer[OFFSET_OPCODE];
    short dataSize = *(short *)(buffer + OFFSET_SIZE);
    uint8_t *iv = ((uint8_t *)buffer) + OFFSET_IV;
    uint8_t *data = buffer + HEADER_SIZE;
    bool isFinEnd = byteFin & BYTE_FIN_END;

    LOG_E("-----------------");
    LOG_E("isFinEnd %i", isFinEnd);
    LOG_E("opcode %p", byteOpcode);
    LOG_E("data %p %p", *data, *(data + 1));
    LOG_E("iv %p %p ... %p %p", iv[0], iv[1], iv[6], iv[7]);

    // set key
    if (isFinEnd && byteOpcode == BYTE_OPCODE_KEY) {
        handleOpcodeKey(data, dataSize, fs);
        return;
    }

    // failed - if frame first not set key
    if (_key == nullptr) {
        LOG_E("not key");
        fs.stage = FrameStage::Error;
        return;
    }

    // alloc memory for multi-frame
    if (_offsetAllData == 0) {
        _allData = (uint8_t *) malloc(dataSize);
    } else {
        _allData = (uint8_t *) realloc(_allData, _offsetAllData + dataSize);
    }

    struct AES_ctx ctx;
    AES_init_ctx_iv(&ctx, _key, iv);
    AES_CBC_decrypt_buffer(&ctx, data, SOCKET_BUFFER_SIZE - HEADER_SIZE);
    LOG_E("%s", data);

    memcpy(_allData + _offsetAllData, data, dataSize);
    _offsetAllData += dataSize;

    if (isFinEnd) {
        LOG_E("end %p", byteOpcode);
        if (byteOpcode == BYTE_OPCODE_JSON) {
            handleOpcodeJson(data, dataSize, fs);
        }
        free(_allData);
        _offsetAllData = 0;
        _allData = nullptr;
    } else {
        LOG_E("continue size: %i", dataSize);
    }
}

void X67HuySocket::handleOpcodeKey(uint8_t *data, size_t size, FrameSession& fs) {
    if (_key != nullptr) {
        free(_key);
    }
    _key = (uint8_t*) malloc(size);
    memcpy(_key, data, size);
    LOG_E("set key %p %p", _key[0], _key[1]);
    fs.stage = FrameStage::Continue;

    emit(X67_EVENT_ESTABLISH, json());
}

void X67HuySocket::handleOpcodeJson(uint8_t *data, size_t size, FrameSession& fs) {
    LOG_E("json size: %i %p", size, *data);
    try
    {
        json js = json::parse(data);
        LOG_E("JSON Parse OKE");

        /**
         * {
         *      "command": <Listener Name>
         *      "data": <Any>
         * }
         */
        if (js.contains("command")) {
            std::string commandName = js["command"].template get<std::string>();
            emit(commandName, js.contains("data") ? js["data"] : json(false));
        }
    } catch (json::parse_error& ex) {
        LOG_E("Err: %i", ex.byte);
    }
    fs.stage = FrameStage::Continue;
}

void X67HuySocket::on(std::string name, X67HuySocketCallback *cb) {
    _listeners.emplace(name, cb);
}

void X67HuySocket::once(std::string name, X67HuySocketCallback *cb) {
    cb->setOnce(true);
    _listeners.emplace(name, cb);
}

void X67HuySocket::emit(std::string name, const json &js) {
    auto range = _listeners.equal_range(name);
    for (auto it = range.first; it != range.second; ) {
        it->second->runnable(js, this);
        if (it->second->isOnce()) {
            it = _listeners.erase(it);
        } else {
            ++it;
        }
    }
}

void X67HuySocket::send(std::string name, const json &jsData) {
    json js;
    js["command"] = name;
    js["data"] = jsData;
    std::string jsonStr = js.dump();
    LOG_E("send %s", jsonStr.c_str());

    uint8_t fin = BYTE_FIN_CONTINUE;
    int dataMaxSize = FRAME_SIZE - HEADER_SIZE;
    int dataLength = jsonStr.size();
    for (int i = 0; i < dataLength; i += dataMaxSize) {
        int e = i + dataMaxSize;
        if (e >= dataLength) {
            e = dataLength;
            fin = BYTE_FIN_END;
        }

        // need update
        std::string sub = jsonStr.substr(i, e - i);
        uint8_t header[HEADER_SIZE] = { 0 };
        header[0] = fin;
        header[1] = BYTE_OPCODE_JSON;
        header[2] = sub.size() & 0xFF;
        header[3] = (sub.size() >> 8) & 0xFF;

        for (int j = 0; j < AES_BLOCKLEN; j++) {
            header[j + 4] = (rand() % 125);
        }

        ::send(_socket, (void *) header, (size_t)HEADER_SIZE, 0);
        if (fin == BYTE_FIN_END) {
            // need update
            std::string tmp(dataMaxSize, 0);
            copy(sub.begin(), sub.end(), tmp.begin());
            sub = tmp;
        }

        struct AES_ctx ctx;
        AES_init_ctx_iv(&ctx, _key, header + OFFSET_IV);
        AES_CBC_encrypt_buffer(&ctx, (uint8_t *)(&sub[0]), dataMaxSize);
        LOG_E("data %p %p ... %p %p", sub[0], sub[1], sub[dataMaxSize - 2], sub[dataMaxSize - 1]);
        ::send(_socket, (void *) &sub[0], sub.size(), 0);
    }
}
