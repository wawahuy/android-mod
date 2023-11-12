//
// Created by nguye on 10/28/2023.
//

#ifndef PIGMOD_SOCKET_H
#define PIGMOD_SOCKET_H



namespace Socket {
    static bool isOpen = false;

    void handleLogin() {
        json js;
        js["package"] = getPackageName();
        js["key"] = g_AuthKey;
        js["version"] = g_PackageVersion;

        g_SystemMessage[0] = 0;
        g_AuthStage = AuthStage::Doing;
        g_Socket->send(STR_COMMAND_S_LOGIN, js);
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
            auto isLogin = js["isLogin"];
            if (isLogin) {
                if (js.contains("libIjHash")) {
                    std::string libIjHash = js["libIjHash"].template get<std::string>();
                    g_AuthStage = AuthStage::WaitIJ;
                    g_Socket->send(STR_COMMAND_S_GET_LIB_IJ, json());
                } else {
                    g_AuthStage = AuthStage::Oke;
                    g_Socket->send(STR_COMMAND_S_GET_MENU, json());
                }
            } else {
                g_AuthStage = AuthStage::None;
#ifndef IS_DEBUG_NOT_GAME
                Menu::release();
#endif
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
#ifndef IS_DEBUG_NOT_GAME
            if (Menu::canInit(js)) {
                Menu::init(js);
            }
#endif
        }
    };

    class OnLibIJCallback: public X67HuySocketCallback {
    public:
        void runnable(const json& js, X67HuySocket* sk) {
#ifndef IS_DEBUG_NOT_GAME
            auto data = (char *)js["data"].template get<uintptr_t>();
            int size = js["size"];
            LibIj::saveLib(data, size);
            LibIj::loadLib();
            g_AuthStage = AuthStage::Oke;
            g_Socket->send(STR_COMMAND_S_GET_MENU, json());
#endif
        }
    };

    void init() {
        g_Socket = new X67HuySocket(SOCKET_HOST, SOCKET_PORT, true);
        g_Socket->on(X67_EVENT_OPEN, new OnOpenCallback());
        g_Socket->on(X67_EVENT_ESTABLISH, new OnEstablishCallback());
        g_Socket->on(X67_EVENT_CLOSE, new OnCloseCallback());
        g_Socket->on(X67_EVENT_LIB_IJ, new OnLibIJCallback());

        g_Socket->on(STR_COMMAND_R_IS_LOGIN, new OnIsLoginCallback());
        g_Socket->on(STR_COMMAND_R_SYS_MSG, new OnSysMsgCallback());
        g_Socket->on(STR_COMMAND_R_MENU, new OnMenuCallback());
        g_Socket->start();
    }
}

#endif //PIGMOD_SOCKET_H
