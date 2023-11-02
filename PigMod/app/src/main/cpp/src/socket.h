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

namespace Socket {
    static int clientSocket = -1;

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
        do {
            receivedBytes = recv(clientSocket, buffer + totalBytesReceived, SOCKET_BUFFER_SIZE - totalBytesReceived, 0);
            totalBytesReceived += receivedBytes;
            if (totalBytesReceived == SOCKET_BUFFER_SIZE) {
                LOG_E("%s", buffer);
                totalBytesReceived = 0;
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
