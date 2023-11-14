#include "common.h"




extern "C" {
    __attribute__((visibility("default"))) void initMethod(
        uintptr_t il2CppBase,
        uintptr_t getMs,
        uintptr_t unprotectIl2cpp,
        uintptr_t protectIl2cpp
    ) {
        LOG_E("==================== OKE ===================");
        LOG_E("==================== OKE ===================");
        g_il2CppBase = il2CppBase;
        m_getMs = (uint64_t (*)())getMs;
        m_unprotectIl2cpp = (void (*)())unprotectIl2cpp;
        m_protectIl2cpp = (void (*)())protectIl2cpp;

        CayThongPatch::init();
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
        } else if (action == std::string(ACTION_BET_1000)) {
            js["count"] = 1000;
            CayThongPatch::betAction(js);
        } else if (action == std::string(ACTION_BET_500)) {
            js["count"] = 500;
            CayThongPatch::betAction(js);
        } else if (action == std::string(ACTION_BET_200)) {
            js["count"] = 200;
            CayThongPatch::betAction(js);
        }
    }

}
