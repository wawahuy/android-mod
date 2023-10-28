//
// Created by nguye on 10/28/2023.
//

#ifndef PIGMOD_DEBUG_SOCKET_H
#define PIGMOD_DEBUG_SOCKET_H

#include "json.hpp"
#include <tiny_websockets/client.hpp>
#include <tiny_websockets/server.hpp>
#include <string>
#include <iostream>
#include <map>

using namespace nlohmann;
using namespace websockets;

namespace DebugSocket {
    std::unordered_map<int, WebsocketsClient> allClients;
    WebsocketsServer server;
    void (*onMessageHook)(const json& js);

    void pollAllClients() {
        for(auto& clientPair  : allClients) {
            WebsocketsClient& client = clientPair.second;
            client.poll();
//            allClients.erase(allClients.find(client.getSocket()));
//            LOG_E("Close client! %i", client.getSocket());
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
        server.listen(DEBUG_PORT);
        LOG_E("Create server!");

        // while the server is alive
        while(server.available()) {

            // if there is a client that wants to connect
            if (server.poll()) {
                //accept the connection and register callback
                WebsocketsClient client = server.accept();
                client.onMessage(onMessage);
                LOG_E("A new client! %i", client.getSocket());

                // store it for later use
                allClients[client.getSocket()] = client;
            }

            // check for updates in all clients
            pollAllClients();
        }

        LOG_E("Close server!");
    }

    void init() {
        pthread_t id;
        pthread_create(&id, nullptr, mainSocket, nullptr);
    }
}

#endif //PIGMOD_DEBUG_SOCKET_H
