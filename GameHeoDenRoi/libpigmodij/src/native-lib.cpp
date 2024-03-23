#include "common.h"


extern "C" {
    __attribute__((visibility("default"))) void initMethod(
        uintptr_t il2CppBase,
        uintptr_t getMs,
        uintptr_t unprotectIl2cpp,
        uintptr_t protectIl2cpp,
        uintptr_t sendCommand
    ) {
        LOG_E("==================== OKE ===================");
        LOG_E("==================== OKE ===================");
        g_il2CppBase = il2CppBase;
        m_getMs = (uint64_t (*)())getMs;
        m_unprotectIl2cpp = (void (*)())unprotectIl2cpp;
        m_protectIl2cpp = (void (*)())protectIl2cpp;
        m_sendCommand = (m_sendCommandType)sendCommand;

        uintptr_t il2cpp_domain_getOffset = 0xefe56c; 
        uintptr_t il2cpp_thread_detachOffset = 0xefe9f4;
        uintptr_t il2cpp_thread_attachOffset = 0xefe9f0;
        il2cpp_thread_detach = (void (*)(void *))(g_il2CppBase + il2cpp_thread_detachOffset);
        il2cpp_thread_attach = (void* (*)(void *))(g_il2CppBase + il2cpp_thread_attachOffset);
        il2cpp_domain_get = (void* (*)())(g_il2CppBase + il2cpp_domain_getOffset);

        CayThongPatch::init();
        BetPatch::init();
        UserDataPatch::init();
    }

    __attribute__((visibility("default"))) void runAction(const std::string& action, nlohmann::json& js) {
        LOG_E("OKE %s %s", action.c_str(), js.dump().c_str());
        if (action == std::string(ACTION_CT_AUTO_TRUNG)) {
            CayThongPatch::autoTrungAction(js);
        } else if (action == std::string(ACTION_CT_BAN_NHANH)) {
            CayThongPatch::banNhanhAction(js);
        } else if (action == std::string(ACTION_CT_BAN_1_CHAM)) {
            CayThongPatch::ban1ChamAction(js);
        } else if (action == std::string(ACTION_CT_NAP_DAN_NHANH)) {
            CayThongPatch::napDanNhanhAction(js);
        } else if (action == std::string(ACTION_BET_X)) {
            BetPatch::betXAction(js);
        } else if (action == std::string(ACTION_USER_DATA_REQUEST)) {
            UserDataPatch::userDataRequest(js);
        }
    }

}
