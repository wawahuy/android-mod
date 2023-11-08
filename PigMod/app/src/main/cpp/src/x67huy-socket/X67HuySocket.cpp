//
// Created by nguye on 11/4/2023.
//

#include "X67HuySocket.h"

X67HuySocket::X67HuySocket(const X67HuySocket &sk) {
}

X67HuySocket::X67HuySocket(const char *host, int port, bool isWsHybird) {
    _host = std::string(host);
    _port = port;
    _isWsHybird = isWsHybird;
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

    if (isIPAddress(_host)) {
        LOG_E("connect ip: %s", _host.c_str());
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(_port);
        serverAddr.sin_addr.s_addr = inet_addr(_host.c_str());
    } else {
        LOG_E("connect domain: %s", _host.c_str());

        struct addrinfo hints;
        struct addrinfo *result = nullptr;
        memset(&hints, 0, sizeof(hints));
        hints.ai_family = AF_UNSPEC; // IPv4 or IPv6
        hints.ai_socktype = SOCK_STREAM;

        int status = getaddrinfo(_host.c_str(), nullptr, &hints, &result);
        if (status != 0) {
            LOG_E("getaddrinfo error");
            return nullptr;
        }

        struct addrinfo *addr = result;
        while (addr != nullptr) {
            if (addr->ai_family == AF_INET) { // IPv4
                struct sockaddr_in *ipv4 = (struct sockaddr_in *)addr->ai_addr;
                serverAddr.sin_family = ipv4->sin_family;
                serverAddr.sin_port = htons(_port);
                serverAddr.sin_addr.s_addr = ipv4->sin_addr.s_addr;
                break;
            }
            addr = addr->ai_next;
        }
        freeaddrinfo(result);
    }

    LOG_E("connecting... %s", _host.c_str());
    if (connect(_socket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        LOG_E("Can't connect socket");
        close(_socket);
        sleep(1);
        start();
        return nullptr;
    }

    LOG_E("Socket connected!");
    uint8_t buffer[SOCKET_BUFFER_SIZE];
    int receivedBytes = -1;
    int totalBytesReceived = 0;

    if (_isWsHybird) {
        sendWsHeader();
        if (!recvWsHeader(buffer, totalBytesReceived)) {
            LOG_E("Socket closed!");
            sleep(1);
            start();
            return nullptr;
        }
    }

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

std::string X67HuySocket::generateWebSocketAcceptKey(const std::string &clientWebSocketKey)  {
    const std::string magicString = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";
    const std::string combinedKey = clientWebSocketKey + magicString;

    // Calculate the SHA-1 hash
    SHA1 checksum;
    checksum.update(clientWebSocketKey);
    std::string sha1Hash = checksum.final();

    // Base64 encode the SHA-1 hash
    std::string base64Encoded = base64_encode((unsigned char*)&sha1Hash[0], (unsigned int)sha1Hash.size());

    return base64Encoded;
}

void X67HuySocket::sendWsHeader() {
    std::string key = generateRandomKey(10);
    std::string webSocketKey = generateWebSocketAcceptKey(key);
    std::string headers[] = {
            "GET / HTTP/1.1\r\n",
            "Host: " + _host + "\r\n",
            "Connection: Upgrade\r\n",
            "Sec-Websocket-Extensions: permessage-deflate; client_max_window_bits\r\n",
            "Sec-Websocket-Key: " + webSocketKey + "\r\n",
            "Sec-Websocket-Version: 13\r\n",
            "Upgrade: websocket\r\n",
            "\r\n"
    };
    for (auto str: headers) {
        ::send(_socket, (void *) &str[0], str.size(), 0);
    }
}

bool X67HuySocket::recvWsHeader(uint8_t* bufferOver, int& receivedBytesOver) {
    uint8_t endOfHeader[] = { 0x0d, 0x0a, 0x0d, 0x0a };
    uint8_t buffer[SOCKET_BUFFER_SIZE];
    int receivedBytes = -1;
    int totalBytesReceived = 0;
    int indexEohContains = -1;

    emit(X67_EVENT_OPEN, json());
    do {
        receivedBytes = recv(_socket, buffer + totalBytesReceived, SOCKET_BUFFER_SIZE - totalBytesReceived, 0);
        totalBytesReceived += receivedBytes;
        if (containsPattern(buffer, totalBytesReceived, endOfHeader, sizeof (endOfHeader), indexEohContains)) {
            // check ws
            std::string strHeader((char*)&buffer[0]);
            std::transform(strHeader.begin(), strHeader.end(), strHeader.begin(),
                           [](unsigned char c){ return std::tolower(c); });
            if (strHeader.find("sec-websocket-accept") == std::string::npos) {
                LOG_E("WS Hybird FAILED");
                return false;
            }

            LOG_E("WS Hybird OKE");

            int ss = indexEohContains + sizeof (endOfHeader);
            LOG_E("ss: %i --- totalBytesReceived: %i", ss, totalBytesReceived);
            if (ss != totalBytesReceived) {
                receivedBytesOver = totalBytesReceived - ss;
                memcpy(bufferOver, buffer + ss, receivedBytesOver);
                LOG_E("bufferOver: %p %p -- num: %i", bufferOver[0], bufferOver[1], receivedBytesOver);
            }
            return true;
        }
        if (totalBytesReceived == SOCKET_BUFFER_SIZE) {
            LOG_E("WS Hybird ERR");
            return false;
        }
    } while (receivedBytes > 0);

    return false;
}

std::string X67HuySocket::generateRandomKey(int length)  {
    static const char alphanum[] =
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz"
            "0123456789";

    std::string randomKey;
    randomKey.reserve(length);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, sizeof(alphanum) - 2);

    for (int i = 0; i < length; ++i) {
        randomKey.push_back(alphanum[dist(gen)]);
    }

    return randomKey;
}

bool X67HuySocket::containsPattern(const uint8_t *buffer, int bufferSize, const uint8_t *pattern,
                                   int patternSize, int& index)  {
    for (int i = 0; i < bufferSize - patternSize + 1; ++i) {
        bool found = true;
        for (int j = 0; j < patternSize; ++j) {
            if (buffer[i + j] != pattern[j]) {
                found = false;
                break;
            }
        }
        if (found) {
            index = i;
            return true;
        }
    }
    return false;
}

bool X67HuySocket::isIPAddress(const std::string &input)  {
    struct sockaddr_in sa;
    return inet_pton(AF_INET, input.c_str(), &(sa.sin_addr)) != 0;
}