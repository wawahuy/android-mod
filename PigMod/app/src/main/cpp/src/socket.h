//
// Created by nguye on 10/28/2023.
//

#ifndef PIGMOD_SOCKET_H
#define PIGMOD_SOCKET_H

namespace Socket {
    static int clientSocket = -1;

    class TestCallback: public X67HuySocketCallback {
    public:
        void runnable(const json& js, X67HuySocket* sk) {
            LOG_E("test!");
        }
    };

    void init() {
        X67HuySocket* sk = new X67HuySocket(SOCKET_HOST, SOCKET_PORT);
        sk->once("test", new TestCallback());
        sk->start();
    }
}

#endif //PIGMOD_SOCKET_H
