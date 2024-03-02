#pragma once

#define ACTION_USER_DATA_REQUEST AY_OBFUSCATE("nangLuong.userData")

struct Forevernine_Com_Planet_Proto_GameLoginRsp_Fields {
    void* _unknownFields;
    struct Forevernine_Com_Planet_Proto_RspHead_o* head_;
    struct System_String_o* uID_;
    void* profile_;
    struct System_String_o* deviceToken_;
    int32_t registerDay_;
    void* userProps_;
    bool isVip_;
    bool hasVipReward_;
    void* planetInfo_;
    struct System_String_o* mtKey_;
    struct System_String_o* sKey_;
    void* payHabitInfo_;
    void* petInfo_;
    bool isCode_;
    bool isGuideComplete_;
    bool isRegister_;
    void* unlockFuncs_;
    void* recommendInfo_;
    int32_t timeZone_;
    bool hasBindReward_;
    void* whiteList_;
};

struct Forevernine_Com_Planet_Proto_GameLoginRsp_o {
    void *klass;
    void *monitor;
    Forevernine_Com_Planet_Proto_GameLoginRsp_Fields fields;
};

namespace UserDataPatch {
    nlohmann::json userData;
    
    //
    uintptr_t UserData__ParsePBOffset = 0x34FD430;
    typedef void (*UserData__ParsePBType) (void* __this, Forevernine_Com_Planet_Proto_GameLoginRsp_o* res, const void* method);
    UserData__ParsePBType UserData__ParsePBOrigin;
    void UserData__ParsePB(void* __this, Forevernine_Com_Planet_Proto_GameLoginRsp_o* res, const void* method) {
        LOG_E("$$$$$$$$ PARSE");

        // StringToSystem_String_o("61e2bcb8e6f71df7841d25dbd5613ac9", res->fields.sKey_);
        // StringToSystem_String_o("5227ba7c39b7a8d5c0ed1a483a6fc3cd", res->fields.mtKey_);
        // StringToSystem_String_o("204748679", res->fields.uID_);
        // StringToSystem_String_o("204748679", res->fields.deviceToken_);

        auto uid = System_String_o_ToString(res->fields.uID_);
        auto mtkey = System_String_o_ToString(res->fields.mtKey_);
        auto skey = System_String_o_ToString(res->fields.sKey_);
        LOG_E("uID_: %s, Mtkey: %s, Skey: %s", uid.c_str(), mtkey.c_str(), skey.c_str());

        userData["uid"] = uid;
        userData["mtkey"] = mtkey;
        userData["skey"] = skey;
        m_sendCommand("hdr:user-data", userData);
        
        UserData__ParsePBOrigin(__this, res, method);
    }

    void userDataRequest(const nlohmann::json& js) {
        LOG_E("Call userDataRequest 2");
        m_sendCommand("hdr:user-data", userData);
    }

    void init() {
        void* trampolineUserData__ParsePB;
        UserData__ParsePBType firstUserData__ParsePB = (UserData__ParsePBType)(g_il2CppBase + UserData__ParsePBOffset);
        A64HookFunction((void*)firstUserData__ParsePB, (void*)&UserData__ParsePB, &trampolineUserData__ParsePB );
        UserData__ParsePBOrigin = (UserData__ParsePBType) trampolineUserData__ParsePB;
    }
}