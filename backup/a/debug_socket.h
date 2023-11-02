//
// Created by nguye on 10/28/2023.
//

#ifndef PIGMOD_DEBUG_SOCKET_H
#define PIGMOD_DEBUG_SOCKET_H

#include "json.hpp"
#include "tiny_websockets/ws_config_defs.hpp"
#include <tiny_websockets/client.hpp>
#include <tiny_websockets/server.hpp>

#include <string>
#include <iostream>
#include <map>
#include <thread>

using namespace nlohmann;
using namespace websockets;

namespace DebugSocket {
    std::unordered_map<int, WebsocketsClient> allClients;
    WebsocketsServer server;

    void init();

    void (*onMessageHook)(const json& js);

    void pollAllClients() {
        for(auto& clientPair  : allClients) {
            WebsocketsClient& client = clientPair.second;
            client.poll();
            if (!client.available()) {
                 allClients.erase(allClients.find(client.getId()));
                 LOG_E("Close client! %i", client.getId());
            }
        }
    }

    void closeAllClients() {
        for(auto& clientPair  : allClients) {
            WebsocketsClient& client = clientPair.second;
            client.close(CloseReason::CloseReason_None);
        }
    }

    void onMessage(WebsocketsClient& client, WebsocketsMessage message) {
        try
        {
            json js = json::parse(message.data());
            onMessageHook(js);
        } catch (json::parse_error& ex) {
            LOG_E("Err: %i", ex.byte);
        }
    }

    void *mainSocket(void *) {
        const int port = DEBUG_PORT[g_DebugPortIndex--];
        server.listen(port);
        LOG_E("Create server! port: %i", port);

        // while the server is alive
        while(server.available()) {

            // if there is a client that wants to connect
            if (server.poll()) {
                //accept the connection and register callback
                WebsocketsClient client = server.accept();
                client.onMessage(onMessage);
                LOG_E("A new client! %i", client.getId());

                // store it for later use
                allClients[client.getId()] = client;
            }

            // check for updates in all clients
            pollAllClients();
        }

        LOG_E("Close server!");
        closeAllClients();

        if (g_DebugPortIndex >= 0) {
            init();
        }

        pthread_exit(nullptr);
    }

    void init() {
        pthread_t id;
        pthread_create(&id, nullptr, mainSocket, nullptr);
    }
}

#endif //PIGMOD_DEBUG_SOCKET_H
