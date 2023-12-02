//
// Created by nguye on 10/28/2023.
//

#ifndef PIGMOD_SOCKET_H
#define PIGMOD_SOCKET_H



namespace Socket {
    static bool isOpen = false;
    static std::string libIjHashTmp;

    void requestMenu() {
        g_AuthStage = AuthStage::Oke;
        g_CanStartGame = true;

        do {
            sleep(1);
        } while ((g_Il2CppBase = get_libBase("libil2cpp.so")) == 0);
        g_Il2CppBaseRange = get_libBaseRange("libil2cpp.so");
        Game::init();
        LibIj::loadLib();

        g_Socket->send(STR_COMMAND_S_GET_MENU, json());
    }

    void handleLogin() {
        json js;
        js["package"] = getPackageName();
        js["key"] = g_AuthKey;
        js["version"] = g_PackageVersion;
        js["trial"] = g_AuthTrial;
        js["mac"] = g_AndroidID;
        js["re"] = g_AuthRe;

        g_SystemMessage[0] = 0;
        g_AuthStage = AuthStage::Doing;
        g_Socket->send(STR_COMMAND_S_LOGIN, js);
    }

    void handleLogout() {
        Menu::release();
        g_AuthStage = AuthStage::None;
        g_AuthRe = false;
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
            if ((g_AuthAuto && strlen(g_AuthKey) > 0) || g_AuthRe) {
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
                g_AppPackageName = js["packageName"];
                g_AppClassName = js["className"];
                if (js.contains("libIjHash") && !LibIj::isLoaded) {
                    LOG_E("LibIJ wait load");
                    std::string libIjHash = js["libIjHash"].template get<std::string>();
                    if (libIjHash == SaveData::getString(STR_SAVE_HASH_LIB_IJ)) {
                        LOG_E("Not need load");
                        requestMenu();
                        return;
                    }

                    libIjHashTmp = libIjHash;
                    g_AuthStage = AuthStage::WaitIJ;
                    g_Socket->send(STR_COMMAND_S_GET_LIB_IJ, json());
                } else {
                    LOG_E("LibIJ not need load");
                    g_AuthStage = AuthStage::Oke;
                    g_Socket->send(STR_COMMAND_S_GET_MENU, json());
                }
            } else {
                g_AuthRe = false;
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
            memcpy(g_SystemMessage, &msg[0], msg.size() < 254 ? msg.size() : 254);
            msg[msg.size()] = '\0';

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
            bool noReload = js.contains("noReloadIfReconnect") && js["noReloadIfReconnect"].template get<bool>();
            if (Menu::canInit(js)) {
                if (!(g_AuthRe && noReload)) {
                    LOG_E("Load menu");
                    Menu::init(js);
                } else {
                    Menu::reset();
                }
            }
            g_AuthRe = true;
        }
    };

    class OnLibIJCallback: public X67HuySocketCallback {
    public:
        void runnable(const json& js, X67HuySocket* sk) {
#ifndef IS_DEBUG_NOT_GAME
            auto data = (char *)js["data"].template get<uintptr_t>();
            int size = js["size"];

            LibIj::saveLib(data, size);
            SaveData::saveString(STR_SAVE_HASH_LIB_IJ, libIjHashTmp);
            requestMenu();
#endif
        }
    };

    class OnDestroyCallback: public X67HuySocketCallback {
    public:
        void runnable(const json& js, X67HuySocket* sk) {
            handleLogout();
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
        g_Socket->on(STR_COMMAND_R_DESTROY, new OnDestroyCallback());
        g_Socket->start();
    }
}

#endif //PIGMOD_SOCKET_H
