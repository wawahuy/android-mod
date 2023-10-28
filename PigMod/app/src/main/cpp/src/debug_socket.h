//
// Created by nguye on 10/28/2023.
//

#ifndef PIGMOD_DEBUG_SOCKET_H
#define PIGMOD_DEBUG_SOCKET_H
#include <boost/asio.hpp>
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include <functional>

typedef websocketpp::server<websocketpp::config::asio> server;

using namespace boost::asio;

namespace Debug {
    void on_message(server* s, websocketpp::connection_hdl hdl, server::message_ptr msg) {
        LOG_E("Received message: %s",  msg->get_payload().c_str());
        s->send(hdl, msg->get_payload(), msg->get_opcode());
    }

    void init() {
        boost::asio::io_service io_service;
        server srv;
        srv.init_asio(&io_service);
        srv.set_message_handler(bind(&on_message, &srv, std::placeholders::_1, std::placeholders::_2));
        srv.listen(DEBUG_PORT);
        srv.start_accept();
        io_service.run();
    }
}

#endif //PIGMOD_DEBUG_SOCKET_H
