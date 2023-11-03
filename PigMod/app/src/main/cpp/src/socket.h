//
// Created by nguye on 10/28/2023.
//

#ifndef PIGMOD_SOCKET_H
#define PIGMOD_SOCKET_H
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>
#include <unordered_map>
#include <arpa/inet.h>
#include <memory>

#include "json.hpp"
using namespace nlohmann;

#define OFFSET_FIN 0
#define OFFSET_OPCODE 1
#define OFFSET_SIZE 2
#define OFFSET_IV 4
#define HEADER_SIZE 32

#define BYTE_FIN 0x01
#define BYTE_OPCODE_JSON 0b00000001

namespace Socket {
    static int clientSocket = -1;
    static uint8_t key[] = {0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8,0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8};

    void init();

    void reconnect() {
        clientSocket = -1;
        sleep(1);
        init();
    }

    void *socketThread(void *) {
        LOG_E("Socket connecting...");
        clientSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (clientSocket < 0) {
            LOG_E("Can't create socket");
            return nullptr;
        }

        struct sockaddr_in serverAddr;
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(SOCKET_PORT);
        serverAddr.sin_addr.s_addr = inet_addr(SOCKET_HOST);

        if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
            LOG_E("Can't connect socket");
            close(clientSocket);
            reconnect();
            return nullptr;
        }

        LOG_E("Socket connected!");
        char buffer[SOCKET_BUFFER_SIZE];
        int receivedBytes = -1;
        int totalBytesReceived = 0;

        int offsetAllBytesReceived = 0;
        char *allBytesReceived = 0;

        do {
            receivedBytes = recv(clientSocket, buffer + totalBytesReceived, SOCKET_BUFFER_SIZE - totalBytesReceived, 0);
            totalBytesReceived += receivedBytes;
            if (totalBytesReceived == SOCKET_BUFFER_SIZE) {
                LOG_E("frame");
                totalBytesReceived = 0;
                char byteFin = buffer[OFFSET_FIN];
                char byteOpcode = buffer[OFFSET_OPCODE];
                short byteSize = *(short *)(buffer + OFFSET_SIZE);
                uint8_t *iv = ((uint8_t *)buffer) + OFFSET_IV;
                void *data = buffer + HEADER_SIZE;
                bool fin = byteFin & BYTE_FIN;

                if (offsetAllBytesReceived == 0) {
                    allBytesReceived = (char *) malloc(byteSize);
                } else {
                    allBytesReceived = (char *) realloc(allBytesReceived, offsetAllBytesReceived + byteSize);
                }

                LOG_E("enc %p %p", *(unsigned char*)(data), *(unsigned char*)( (uintptr_t)data + 1));
                struct AES_ctx ctx;
                AES_init_ctx_iv(&ctx, key, iv);
                AES_CBC_decrypt_buffer(&ctx, (uint8_t*)data, SOCKET_BUFFER_SIZE - HEADER_SIZE);
                LOG_E("%s", data);

                memcpy(allBytesReceived + offsetAllBytesReceived, data, byteSize);
                offsetAllBytesReceived += byteSize;

                if (fin) {
                    LOG_E("end %p", byteOpcode);
                    if (byteOpcode == BYTE_OPCODE_JSON) {
                        LOG_E("iv %p %p %p %p", iv[0], iv[1], iv[6], iv[7]);
                        LOG_E("json size: %i %p", offsetAllBytesReceived, *allBytesReceived);

                        try
                        {
                            json js = json::parse(allBytesReceived);
                            LOG_E("JSON Parse OKE");
                        } catch (json::parse_error& ex) {
                            LOG_E("Err: %i", ex.byte);
                        }

                        free(allBytesReceived);
                        offsetAllBytesReceived = 0;
                    }
                } else {
                    LOG_E("continue size: %i", byteSize);
                }
            }
        } while (receivedBytes > 0);

        LOG_E("Socket closed!");
        reconnect();
        pthread_exit(nullptr);
    }

    void init() {
        pthread_t id;
        pthread_create(&id, nullptr, socketThread, nullptr);
    }
}

#endif //PIGMOD_SOCKET_H
