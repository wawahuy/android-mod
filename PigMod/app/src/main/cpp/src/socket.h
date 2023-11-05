//
// Created by nguye on 10/28/2023.
//

#ifndef PIGMOD_SOCKET_H
#define PIGMOD_SOCKET_H

namespace Socket {
    static bool isOpen = false;
    static X67HuySocket* socket = nullptr;

    void handleLogin() {
        json js;
        js["package"] = getPackageName();
        js["key"] = g_AuthKey;
        js["version"] = g_PackageVersion;
        g_SystemMessage[0] = 0;
        g_AuthStage = AuthStage::Doing;
        socket->send(STR_COMMAND_S_LOGIN, js);
    }

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
            isOpen = true;
            g_AuthStage = AuthStage::None;
            g_SystemMessage[0] = 0;
            if (g_AuthAuto && strlen(g_AuthKey) > 0) {
                handleLogin();
            }
        }
    };

    class OnCloseCallback: public X67HuySocketCallback {
    public:
        void runnable(const json& js, X67HuySocket* sk) {
            LOG_E("close!");
            isOpen = false;
        }
    };

    class OnIsLoginCallback: public X67HuySocketCallback {
    public:
        void runnable(const json& js, X67HuySocket* sk) {
            if (js == true) {
                g_AuthStage = AuthStage::Oke;
                socket->send(STR_COMMAND_S_GET_MENU, json());
            } else {
                g_AuthStage = AuthStage::None;
                Game::release();
            }
        }
    };

    class OnSysMsgCallback: public X67HuySocketCallback {
    public:
        void runnable(const json& js, X67HuySocket* sk) {
            std::string msg = js["msg"].template get<std::string>();
            memcpy(g_SystemMessage, &msg[0], msg.size() < 255 ? msg.size() : 255);

            if (js.contains("color")) {
                g_SystemMessageColor[0] = js["color"][0].template get<char>();
                g_SystemMessageColor[1] = js["color"][1].template get<char>();
                g_SystemMessageColor[2] = js["color"][2].template get<char>();
                g_SystemMessageColor[3] = js["color"][3].template get<char>();
            }
        }
    };

    class OnMenuCallback: public X67HuySocketCallback {
    public:
        void runnable(const json& js, X67HuySocket* sk) {
            if (Game::canInit(js)) {
                Game::init(js);
            }
        }
    };

    void init() {
        socket = new X67HuySocket(SOCKET_HOST, SOCKET_PORT);
        socket->on(X67_EVENT_OPEN, new OnOpenCallback());
        socket->on(X67_EVENT_ESTABLISH, new OnEstablishCallback());
        socket->on(X67_EVENT_CLOSE, new OnCloseCallback());
        socket->on(STR_COMMAND_R_IS_LOGIN, new OnIsLoginCallback());
        socket->on(STR_COMMAND_R_SYS_MSG, new OnSysMsgCallback());
        socket->on(STR_COMMAND_R_MENU, new OnMenuCallback());
        socket->start();
    }
}

#endif //PIGMOD_SOCKET_H
