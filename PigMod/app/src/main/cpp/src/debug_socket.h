//
// Created by nguye on 10/28/2023.
//

#ifndef PIGMOD_DEBUG_SOCKET_H
#define PIGMOD_DEBUG_SOCKET_H

#include <tiny_websockets/client.hpp>
#include <tiny_websockets/server.hpp>
#include <iostream>
using namespace websockets;

namespace Debug {

    void init() {
        WebsocketsServer server;
        server.listen(DEBUG_PORT);

        // while the server is alive
        while(server.available()) {
            // accept a client
            WebsocketsClient client = server.accept();
            std::cout << "Client connected" << std::endl;

            while(client.available()) {
                // get a message, if it is text, return an echo
                auto message = client.readBlocking();
                if(message.isText()) {
                    client.send("Echo: " + message.data());
                    std::cout << "Sending echo: " << message.data() << std::endl;
                }
            }

            // close the connection
            if(client.available() == false) {
                std::cout << static_cast<int>(client.getCloseReason()) << std::endl;
            }
            client.close();
        }
    }
}

#endif //PIGMOD_DEBUG_SOCKET_H
