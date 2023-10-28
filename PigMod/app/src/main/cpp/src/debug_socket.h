//
// Created by nguye on 10/28/2023.
//

#ifndef PIGMOD_DEBUG_SOCKET_H
#define PIGMOD_DEBUG_SOCKET_H
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <map>

namespace DebugSocket {
    struct Client {
        int clientSocket;
        int threadId;
    };

    static int serverFd;
    static std::map<int, Client*> clients;

    void sendGlobalMessage(void* buf, size_t size) {
        for (auto clientPair: clients) {
            Client* client = clientPair.second;
            send(client->clientSocket, buf, size, 0);
        }
    }

    void *socketClientThread(void *clientVoid) {
        Client* client = (Client*) clientVoid;
        int clientSocket = client->clientSocket;
        char buffer[1024] = { 0 };
        ssize_t valRead;

        sendGlobalMessage((void *)"hello", 5);
        LOG_E("Start thread. %i", clientSocket);
        while (true) {
            valRead = read(clientSocket, buffer, 1024 - 1);
            if (valRead == 0) {
                LOG_E("Client disconnected. %i", clientSocket);
                break;
            } else if (valRead < 0) {
                LOG_E("Error reading from client. %i", clientSocket);
                break;
            }
            LOG_E("%s", buffer);
        }
        close(clientSocket);
        delete client;
        LOG_E("Close thread. %i", clientSocket);
        pthread_exit(nullptr);
    }

    void *socketServerThread(void *) {
        struct sockaddr_in address;
        socklen_t addrLen = sizeof(address);
        int opt = 1;

        if ((serverFd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
            LOG_E("socket failed");
            exit(EXIT_FAILURE);
        }

        if (setsockopt(serverFd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt,sizeof(opt))) {
            LOG_E("setsockopt");
            exit(EXIT_FAILURE);
        }
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(DEBUG_PORT);

        if (bind(serverFd, (struct sockaddr*)&address, sizeof(address)) < 0) {
            LOG_E("bind failed");
            exit(EXIT_FAILURE);
        }

        if (listen(serverFd, 3) < 0) {
            LOG_E("listen");
            exit(EXIT_FAILURE);
        }

        int clientSocket;
        while (true) {
            clientSocket = accept(serverFd, (struct sockaddr *) &address, &addrLen);
            if (clientSocket < 0) {
                sleep(1);
                continue;
            }

            LOG_E("new connect. %i", clientSocket);
            Client* client = new Client();
            client->clientSocket = clientSocket;

            pthread_t threadId;
            pthread_create(&threadId, nullptr, socketClientThread, client);

            client->threadId = threadId;
            clients[clientSocket] = client;
        }
        close(serverFd);
        pthread_exit(nullptr);
    }

    void init() {
        pthread_t id;
        pthread_create(&id, nullptr, socketServerThread, nullptr);
    }
}

#endif //PIGMOD_DEBUG_SOCKET_H
