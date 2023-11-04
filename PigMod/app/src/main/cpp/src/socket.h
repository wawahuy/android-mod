//
// Created by nguye on 10/28/2023.
//

#ifndef PIGMOD_SOCKET_H
#define PIGMOD_SOCKET_H

namespace Socket {
    static bool isOpen = false;

    class OnOpenCallback: public X67HuySocketCallback {
    public:
        void runnable(const json& js, X67HuySocket* sk) {
            LOG_E("open!");
        }
    };

    class OnEstablishCallback: public X67HuySocketCallback {
    public:
        void runnable(const json& js, X67HuySocket* sk) {
            LOG_E("establish!");
            sk->send("test", 12);
            isOpen = true;
        }
    };

    class OnCloseCallback: public X67HuySocketCallback {
    public:
        void runnable(const json& js, X67HuySocket* sk) {
            LOG_E("close!");
            isOpen = false;
        }
    };

    class TestCallback: public X67HuySocketCallback {
    public:
        void runnable(const json& js, X67HuySocket* sk) {
            LOG_E("test!");
        }
    };

    void init() {
        X67HuySocket* sk = new X67HuySocket(SOCKET_HOST, SOCKET_PORT);
        sk->on(X67_EVENT_OPEN, new OnOpenCallback());
        sk->on(X67_EVENT_ESTABLISH, new OnEstablishCallback());
        sk->on(X67_EVENT_CLOSE, new OnCloseCallback());
        sk->once("test", new TestCallback());
        sk->start();
    }
}

#endif //PIGMOD_SOCKET_H
