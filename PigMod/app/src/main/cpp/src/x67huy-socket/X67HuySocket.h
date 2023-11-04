//
// Created by nguye on 11/4/2023.
//

#ifndef PIGMOD_X67_HUY_SOCKET_H
#define PIGMOD_X67_HUY_SOCKET_H

#define IS_DEBUG true
#ifdef IS_DEBUG
#include <android/log.h>
#define LOG_E(...) __android_log_print(ANDROID_LOG_ERROR, "YUH", __VA_ARGS__);
#else
#define LOG_E(...)
#endif

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <memory>
#include <pthread.h>
#include <map>
#include "AES.h"
#include "json.hpp"
using namespace nlohmann;

/*
 * Frame (1024 byte):
 *   + Header (32 byte)
 *      (1)offset + (1)opcode + 2(size) + 16(iv) + 12(padding)
 *   + Data (992 byte)
 */
#define SOCKET_BUFFER_SIZE 1024
#define OFFSET_FIN 0
#define OFFSET_OPCODE 1
#define OFFSET_SIZE 2
#define OFFSET_IV 4
#define HEADER_SIZE 32

#define BYTE_FIN_END 0x01
#define BYTE_FIN_CONTINUE 0x00
#define BYTE_OPCODE_KEY  0b00000001
#define BYTE_OPCODE_JSON 0b00000011

class X67HuySocket;

class X67HuySocketCallback {
public:
    virtual void runnable(const json& js, X67HuySocket* sk) = 0;
    void setOnce(bool isOnce) {
        this->_isOnce = isOnce;
    }
    bool isOnce() {
        return this->_isOnce;
    }

private:
    bool _isOnce = false;
};

class X67HuySocket {
public:
    X67HuySocket(const char* host, int port);
    void start();
    void on(std::string name, X67HuySocketCallback* cb);
    void once(std::string name, X67HuySocketCallback* cb);

private:
    X67HuySocket(const X67HuySocket& sk);

    enum FrameStage {
        Continue,
        Error
    };

    struct FrameSession {
        FrameStage stage;
    };

    static void *socketThreadStatic(void *);
    void* socketThread();

    void handleFrame(uint8_t* data, size_t size, FrameSession& fs);
    void handleOpcodeKey(uint8_t* data, size_t size, FrameSession& fs);
    void handleOpcodeJson(uint8_t* data, size_t size, FrameSession& fs);

    int _socket = -1;
    int _port, _offsetAllData = 0;
    std::string _host;
    pthread_t _id;
    uint8_t *_key = nullptr, *_allData = nullptr;

    std::multimap<std::string, X67HuySocketCallback*> _listeners;
};


#endif //PIGMOD_X67_HUY_SOCKET_H
