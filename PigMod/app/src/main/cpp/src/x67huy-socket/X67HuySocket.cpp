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
        std::string commandName = js["command"].template get<std::string>();
        auto range = _listeners.equal_range(commandName);
        for (auto it = range.first; it != range.second; ++it) {
            it->second->runnable(js["data"], this);
            if (it->second->isOnce()) {
                it = _listeners.erase(it);
            } else {
                ++it;
            }
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
