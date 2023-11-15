//
// Created by nguye on 11/14/2023.
//

#ifndef PIGMOD_TEST_H
#define PIGMOD_TEST_H

//#define IS_TEST
#ifdef IS_TEST
#include "And64InlineHook.hpp"
#include <wchar.h>

struct System_String_Fields {
    int32_t m_stringLength;
    wchar_t m_firstChar[1];

    wchar_t* getPtr() {
        return m_firstChar;
    }
};

struct System_String_o {
    void *klass;
    void *monitor;
    System_String_Fields fields;
};


struct Il2CppObject
{
    void *klass;
    void *monitor;
};

typedef uintptr_t il2cpp_array_size_t;

struct System_Byte_array {
    Il2CppObject obj;
    void *bounds;
    il2cpp_array_size_t max_length;
    uint8_t m_Items[65535];
};

struct System_Uri_Offset_Fields {
    uint16_t Scheme;
    uint16_t User;
    uint16_t Host;
    uint16_t PortValue;
    uint16_t Path;
    uint16_t Query;
    uint16_t Fragment;
    uint16_t End;
};

struct System_Uri_Offset_o {
    System_Uri_Offset_Fields fields;
};

struct System_Uri_MoreInfo_Fields {
    System_String_o* Path;
    System_String_o* Query;
    System_String_o* Fragment;
    System_String_o* AbsoluteUri;
    int32_t Hash;
    System_String_o* RemoteUrl;
};

struct System_Uri_MoreInfo_o {
    void *klass;
    void *monitor;
    System_Uri_MoreInfo_Fields fields;
};

struct System_Uri_UriInfo_Fields {
    System_String_o* Host;
    System_String_o* ScopeId;
    System_String_o* String;
    System_Uri_Offset_o Offset;
    System_String_o* DnsSafeHost;
    System_Uri_MoreInfo_o* MoreInfo;
};

struct System_Uri_UriInfo_o {
    void *klass;
    void *monitor;
    System_Uri_UriInfo_Fields fields;
};

struct System_Uri_Fields {
    System_String_o* m_String;
    System_String_o* m_originalUnicodeString;
    void* m_Syntax;
    System_String_o* m_DnsSafeHost;
    uint64_t m_Flags;
    System_Uri_UriInfo_o* m_Info;
    bool m_iriParsing;
};

struct System_Uri_o {
    void *klass;
    void *monitor;
    System_Uri_Fields fields;
};

void printByteArray(const uint8_t* array, size_t size) {
    char buffer[65535];
    buffer[0] = '\0';
    for (size_t i = 0; i < size; ++i) {
        snprintf(buffer + strlen(buffer), sizeof(buffer) - strlen(buffer), "0x%02X, ", array[i]);
    }
    LOG_E("%s", buffer);
}

void logWideString(System_String_o* o) {
    LOG_E("Wide String Len: %i", o->fields.m_stringLength);
    char* narrowString = new char[o->fields.m_stringLength + 1];
    wcstombs(narrowString, o->fields.getPtr(), o->fields.m_stringLength + 1);
    LOG_E("Wide String: %s", narrowString);
    delete[] narrowString;
}

void printString(const char *name, System_String_o* o) {
    if ((void*)o == nullptr || o->fields.m_stringLength == 0) {
        LOG_E("%s ____", name);
        return;
    }
    int len = o->fields.m_stringLength + 1;
    char* narrowString = new char[len];
    uint8_t* ptr = (uint8_t *)o->fields.getPtr();
    for (int i = 0; i < o->fields.m_stringLength; i ++) {
        narrowString[i] = *((char*)(ptr + i * 2));
    }
    narrowString[len - 1] = '\0';
    LOG_E("%s %s", name, narrowString);
    delete[] narrowString;
}

uintptr_t BestHTTP_WebSocket_WebSocket___ctorOffset = 0x18127B4;
typedef void (*BestHTTP_WebSocket_WebSocket___ctorType)(void* __this, System_Uri_o* message, const void* method);
void (*BestHTTP_WebSocket_WebSocket___ctorOrigin)(void* __this, System_Uri_o* message, const void* method);
void BestHTTP_WebSocket_WebSocket___ctor(void* __this, System_Uri_o* message, const void* method) {
    System_String_o* (*System_Uri__GetRelativeSerializationString)(System_Uri_o* __this, int32_t format, const void* method) =
            (System_String_o* (*)(System_Uri_o* __this, int32_t format, const void* method))(g_Il2CppBase + 0x343DFDC);

    printString("WS: ", message->fields.m_String);
    printString("WS 2: ", System_Uri__GetRelativeSerializationString(message, 2, nullptr));
    BestHTTP_WebSocket_WebSocket___ctorOrigin(__this, message, method);
}

uintptr_t BestHTTP_HTTPRequest__AddHeaderOffset = 0x1970980;
typedef void (*BestHTTP_HTTPRequest__AddHeaderType) (void* __this, System_String_o* name, System_String_o* value, const void* method);
BestHTTP_HTTPRequest__AddHeaderType BestHTTP_HTTPRequest__AddHeaderOrigin;
void BestHTTP_HTTPRequest__AddHeader (void* __this, System_String_o* name, System_String_o* value, const void* method) {
//    printString("ADD H NAME: ", name);
//    printString("ADD H VALUE: ", value);
    BestHTTP_HTTPRequest__AddHeaderOrigin(__this, name, value, method);
}

uintptr_t BestHTTP_HTTPRequest__SetHeaderOffset = 0x1960FAC;
typedef void (*BestHTTP_HTTPRequest__SetHeaderType) (void* __this, System_String_o* name, System_String_o* value, const void* method);
BestHTTP_HTTPRequest__SetHeaderType BestHTTP_HTTPRequest__SetHeaderOrigin;
void BestHTTP_HTTPRequest__SetHeader (void* __this, System_String_o* name, System_String_o* value, const void* method) {
//    printString("SET H NAME: ", name);
//    printString("SET H VALUE: ", value);
    BestHTTP_HTTPRequest__SetHeaderOrigin(__this, name, value, method);
}

uintptr_t System_Uri___ctorOffset = 0x3432518;
typedef void (*System_Uri___ctorType) (System_Uri_o* __this, System_String_o* uriString, const void* method);
System_Uri___ctorType System_Uri___ctorOrigin;
void System_Uri___ctor (System_Uri_o* __this, System_String_o* uriString, const void* method) {
//    printString("CC: ", uriString);
    System_Uri___ctorOrigin(__this, uriString, method);
}

typedef void GameManager_o;
uintptr_t GameManager__InitMgrOffset = 0x3A03774;
typedef void (*GameManager__InitMgrType)(GameManager_o* __this, const void* method);
void (*GameManager__InitMgrOrigin)(GameManager_o* __this, const void* method);
void GameManager__InitMgr(GameManager_o* __this, const void* method) {
    LOG_E("$$$$$$$$ INIT");
    GameManager__InitMgrOrigin(__this, method);
}

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

struct PlayerBaseData_Fields {
    int32_t _Uid_k__BackingField;
    struct System_String_o* _Name_k__BackingField;
    int32_t _HeadDefaultId_k__BackingField;
    struct System_String_o* _HeadUrl_k__BackingField;
    bool _IsVip_k__BackingField;
    int32_t _PicFrameId_k__BackingField;
    int32_t _FlagId_k__BackingField;
    int32_t _StarNum_k__BackingField;
    int32_t _Sex_k__BackingField;
    int64_t _FamilyId_k__BackingField;
    int32_t _familyIcon;
    struct System_String_o* _FamilyName_k__BackingField;
    int32_t _FamilyPosition_k__BackingField;
    int32_t _PlatformType_k__BackingField;
    bool _isSVIP;
};

struct UserData_Fields : PlayerBaseData_Fields {
    int32_t MaxIsland;
    struct System_String_o* Mtkey;
    struct System_String_o* Skey;
    struct System_String_o* CryptKey;
    struct System_String_o* FeedTag;
    struct System_String_o* Platform;
    struct System_String_o* Device;
    struct System_String_o* Channel;
//    struct System_Collections_Generic_List_string__o* SocketUrlList;
    void* SocketUrlList;
    void* WheelInfoData;
    void* IslandData;
    void* BetData;
    int64_t LoginTimeStamp;
    struct System_String_o* HabbitPurchaseType;
    bool IsDiligent;
    bool IsBackUser;
    void* _LoginRspJson_k__BackingField;
    struct Forevernine_Com_Planet_Proto_GameLoginRsp_o* _LoginRspPb_k__BackingField;
    bool _HasVipReward_k__BackingField;
    bool _IsCodeExchangeEnable_k__BackingField;
    void* FeedbackReplyMsgIdList;
    int32_t _Guide_k__BackingField;
    int32_t _RegisterDay_k__BackingField;
    int32_t _RecommendCount_k__BackingField;
    int32_t _RefuseCount_k__BackingField;
    struct System_String_o* _country;
    struct System_String_o* _DeliveryUrl_k__BackingField;
    void* _WXConfig_k__BackingField;
    int64_t _serverTime;
    bool _HasBindReward_k__BackingField;
    void* UserPropertyDict;
    void* _ChatJson_k__BackingField;
    int32_t _newguestShowBind;
};

struct UserData_o {
    void *klass;
    void *monitor;
    UserData_Fields fields;
};

UserData_o* userData_o;
uintptr_t UserData__ParsePBOffset = 0x325F914;
typedef void (*UserData__ParsePBType) (UserData_o* __this, Forevernine_Com_Planet_Proto_GameLoginRsp_o* res, const void* method);
UserData__ParsePBType UserData__ParsePBOrigin;
void UserData__ParsePB(UserData_o* __this, Forevernine_Com_Planet_Proto_GameLoginRsp_o* res, const void* method) {
    LOG_E("$$$$$$$$ PARSE");
    printString("uID_: ", res->fields.uID_);
    printString("mtKey_: ", res->fields.mtKey_);
    printString("sKey_: ", res->fields.sKey_);

    userData_o = __this;
    UserData__ParsePBOrigin(__this, res, method);
}


struct Google_Protobuf_ByteString_Fields {
    System_Byte_array* bytes;
};

struct Google_Protobuf_ByteString_o {
    void *klass;
    void *monitor;
    Google_Protobuf_ByteString_Fields fields;
};

struct Forevernine_Base_Proto_PacketHead_Fields {
    void* _unknownFields;
    int32_t cmd_;
    int32_t seq_;
    int64_t time_;
    int64_t opts_;
    System_String_o* rKey_;
    System_String_o* clientVer_;
    System_String_o* clientIP_;
    int32_t mod_;
    System_String_o* reqId_;
    bool event_;
    System_String_o* uID_;
    int32_t tableID_;
};

struct Forevernine_Base_Proto_PacketHead_o {
    void *klass;
    void *monitor;
    Forevernine_Base_Proto_PacketHead_Fields fields;
};

struct Forevernine_Base_Proto_Packet_Fields {
    void* _unknownFields;
    Forevernine_Base_Proto_PacketHead_o* head_;
    Google_Protobuf_ByteString_o* body_;
};

struct Forevernine_Base_Proto_Packet_o {
    void *klass;
    void *monitor;
    Forevernine_Base_Proto_Packet_Fields fields;
};


uintptr_t Google_Protobuf_ByteString__ToStringUtf8Offset = 0x1DD1F4C;
typedef System_String_o* (*Google_Protobuf_ByteString__ToStringUtf8Type) (Google_Protobuf_ByteString_o* __this, const void* method);
Google_Protobuf_ByteString__ToStringUtf8Type Google_Protobuf_ByteString__ToStringUtf8Origin = nullptr;
System_String_o* Google_Protobuf_ByteString__ToStringUtf8 (Google_Protobuf_ByteString_o* __this, const void* method) {
//    LOG_E("$$$$$$$$ UTF8");
    auto wtf = Google_Protobuf_ByteString__ToStringUtf8Origin(__this, method);
//    printString("OKE", wtf);
    return wtf;
}

uintptr_t Forevernine_Base_Proto_Packet__get_BodyOffset = 0x337442C;
typedef Google_Protobuf_ByteString_o* (*Forevernine_Base_Proto_Packet__get_BodyType) (Forevernine_Base_Proto_Packet_o* __this, const void* method);
Forevernine_Base_Proto_Packet__get_BodyType Forevernine_Base_Proto_Packet__get_BodyOrigin;
Google_Protobuf_ByteString_o* Forevernine_Base_Proto_Packet__get_Body (Forevernine_Base_Proto_Packet_o* __this, const void* method) {
    LOG_E("$$$$$$$$ get_Body");
    auto wtf = Forevernine_Base_Proto_Packet__get_BodyOrigin(__this, method);
    printByteArray(wtf->fields.bytes->m_Items, wtf->fields.bytes->max_length);
if (Google_Protobuf_ByteString__ToStringUtf8Origin != nullptr) {
    auto str = Google_Protobuf_ByteString__ToStringUtf8Origin(wtf, nullptr);
    printString("==> STR", str);
}
    LOG_E("========= get_Body");
    return wtf;
}

struct Forevernine_Com_Planet_Proto_Packet_Fields {
    void* _unknownFields;
    void* head_;
    Google_Protobuf_ByteString_o* body_;
};

struct Forevernine_Com_Planet_Proto_Packet_o {
    void *klass;
    void *monitor;
    Forevernine_Com_Planet_Proto_Packet_Fields fields;
};

uintptr_t GameSocket__SendPacketOffset = 0x2DCBE38;
typedef void (*GameSocket__SendPacketType)(void* __this, Forevernine_Com_Planet_Proto_Packet_o* packet, const void* method);
void (*GameSocket__SendPacketOrigin)(void* __this, Forevernine_Com_Planet_Proto_Packet_o* packet, const void* method);
void GameSocket__SendPacket(void* __this, Forevernine_Com_Planet_Proto_Packet_o* packet, const void* method) {
    if (Google_Protobuf_ByteString__ToStringUtf8Origin != nullptr) {
        auto str = Google_Protobuf_ByteString__ToStringUtf8Origin(packet->fields.body_, nullptr);
        printString("==> OOOOOOOOOOOOOO", str);
    }
    GameSocket__SendPacketOrigin(__this, packet, method);
}

 uintptr_t BestHTTP_WebSocket_WebSocket__SendOffset = 0x1814938;
 typedef void (*BestHTTP_WebSocket_WebSocket__SendType)(void* __this, System_Byte_array* buffer, const void* method);
 void (*BestHTTP_WebSocket_WebSocket__SendOrigin)(void* __this, System_Byte_array* buffer, const void* method);
 void BestHTTP_WebSocket_WebSocket__Send(void* __this, System_Byte_array* buffer, const void* method) {
//     LOG_E("$$$$$$$$$$$$$$$$ WS %i %s", buffer->max_length, buffer->m_Items);
//     printByteArray(buffer->m_Items, buffer->max_length);
     BestHTTP_WebSocket_WebSocket__SendOrigin(__this, buffer, method);
//     LOG_E("==============  WS");
 }

struct Il2CppArrayBounds
{
    il2cpp_array_size_t length;
    int32_t lower_bound;
};

struct Forevernine_Com_Planet_Proto_PiggyBetMultipleConfig_Fields {
    void* _unknownFields;
    int64_t multiple_;
    int64_t minPlanet_;
    int64_t minStamina_;
    int64_t sID_;
};

struct Forevernine_Com_Planet_Proto_PiggyBetMultipleConfig_o {
    void *klass;
    void *monitor;
    Forevernine_Com_Planet_Proto_PiggyBetMultipleConfig_Fields fields;
};

struct Forevernine_Com_Planet_Proto_PiggyBetMultipleConfig_array {
    Il2CppObject obj;
    Il2CppArrayBounds *bounds;
    il2cpp_array_size_t max_length;
    Forevernine_Com_Planet_Proto_PiggyBetMultipleConfig_o* m_Items[65535];
};

struct System_Collections_Generic_List_PiggyBetMultipleConfig__Fields {
    Forevernine_Com_Planet_Proto_PiggyBetMultipleConfig_array* _items;
    int32_t _size;
    int32_t _version;
    Il2CppObject* _syncRoot;
};

struct System_Collections_Generic_List_PiggyBetMultipleConfig__o {
    void *klass;
    void *monitor;
    System_Collections_Generic_List_PiggyBetMultipleConfig__Fields fields;
};

struct BetButton_Fields {
    intptr_t m_CachedPtr;
    void* _betShowPrefab;
    void* _superBetEffctPrefab;
    void* _particlePrefab;
    void* _betMaskSprite;
    void* _betBgSprite;
    void* _superBetBgSprite;
    void* _betShowParent;
    void* _betButton;
    void* _superBetButton;
    void* _maskImage;
    void* _coverImage;
    void* _betBgImage;
    void* _betMultiple1;
    void* _betMultiple2;
    void* _superBetMultiple;
    void* _superTip;
    void* _particleGo;
    void* _betData;
    void* _validMultipleList;
    bool _isInBetChange;
    bool _isInSuperBetChange;
    int32_t _maxMultiple;
    bool _isbetBlastOnline;
    int32_t _lastMultiple;
    bool _isInitView;
    void* _betMultipleSvipConfigAry;
    System_Collections_Generic_List_PiggyBetMultipleConfig__o* _betMultipleSvipConfigList;
};

struct BetButton_o {
    void *klass;
    void *monitor;
    BetButton_Fields fields;
};

uintptr_t BetButton__SetCurMultipleOffset = 0x1A33F9C;
typedef void (*BetButton__SetCurMultipleType) (BetButton_o* __this, int32_t curMultiple, const void* method);
BetButton__SetCurMultipleType BetButton__SetCurMultipleOrigin;
void BetButton__SetCurMultiple (BetButton_o* __this, int32_t curMultiple, const void* method) {
    LOG_E("$$$$$ SET %i", curMultiple);
    BetButton__SetCurMultipleOrigin(__this, curMultiple, method);
}

void testInit() {

    void* trampolineBestHTTP_WebSocket_WebSocket___ctor;
    BestHTTP_WebSocket_WebSocket___ctorType firstFunc3 = (BestHTTP_WebSocket_WebSocket___ctorType)(g_Il2CppBase + BestHTTP_WebSocket_WebSocket___ctorOffset);
    A64HookFunction((void*)firstFunc3, (void*)&BestHTTP_WebSocket_WebSocket___ctor, &trampolineBestHTTP_WebSocket_WebSocket___ctor );
    BestHTTP_WebSocket_WebSocket___ctorOrigin = (BestHTTP_WebSocket_WebSocket___ctorType) trampolineBestHTTP_WebSocket_WebSocket___ctor;

    void* trampolineGameManager__InitMgr;
    GameManager__InitMgrType firstGameManager__InitMgr = (GameManager__InitMgrType)(g_Il2CppBase + GameManager__InitMgrOffset);
    A64HookFunction((void*)firstGameManager__InitMgr, (void*)&GameManager__InitMgr, &trampolineGameManager__InitMgr );
    GameManager__InitMgrOrigin = (GameManager__InitMgrType) trampolineGameManager__InitMgr;

    void* trampolineUserData__ParsePB;
    UserData__ParsePBType firstUserData__ParsePB = (UserData__ParsePBType)(g_Il2CppBase + UserData__ParsePBOffset);
    A64HookFunction((void*)firstUserData__ParsePB, (void*)&UserData__ParsePB, &trampolineUserData__ParsePB );
    UserData__ParsePBOrigin = (UserData__ParsePBType) trampolineUserData__ParsePB;

    void* trampolineForevernine_Base_Proto_Packet__get_Body;
    Forevernine_Base_Proto_Packet__get_BodyType firstForevernine_Base_Proto_Packet__get_Body = (Forevernine_Base_Proto_Packet__get_BodyType)(g_Il2CppBase + Forevernine_Base_Proto_Packet__get_BodyOffset);
    A64HookFunction((void*)firstForevernine_Base_Proto_Packet__get_Body, (void*)&Forevernine_Base_Proto_Packet__get_Body, &trampolineForevernine_Base_Proto_Packet__get_Body );
    Forevernine_Base_Proto_Packet__get_BodyOrigin = (Forevernine_Base_Proto_Packet__get_BodyType) trampolineForevernine_Base_Proto_Packet__get_Body;

    void* trampolineBetButton__SetCurMultiple;
    BetButton__SetCurMultipleType firstBetButton__SetCurMultiple = (BetButton__SetCurMultipleType)(g_Il2CppBase + BetButton__SetCurMultipleOffset);
    A64HookFunction((void*)firstBetButton__SetCurMultiple, (void*)&BetButton__SetCurMultiple, &trampolineBetButton__SetCurMultiple );
    BetButton__SetCurMultipleOrigin = (BetButton__SetCurMultipleType) trampolineBetButton__SetCurMultiple;

    void* trampolineGoogle_Protobuf_ByteString__ToStringUtf8;
    Google_Protobuf_ByteString__ToStringUtf8Type firstGoogle_Protobuf_ByteString__ToStringUtf8 = (Google_Protobuf_ByteString__ToStringUtf8Type)(g_Il2CppBase + Google_Protobuf_ByteString__ToStringUtf8Offset);
    A64HookFunction((void*)firstGoogle_Protobuf_ByteString__ToStringUtf8, (void*)&Google_Protobuf_ByteString__ToStringUtf8, &trampolineGoogle_Protobuf_ByteString__ToStringUtf8 );
    Google_Protobuf_ByteString__ToStringUtf8Origin = (Google_Protobuf_ByteString__ToStringUtf8Type) trampolineGoogle_Protobuf_ByteString__ToStringUtf8;

     void* trampolineBestHTTP_WebSocket_WebSocket__Send;
     BestHTTP_WebSocket_WebSocket__SendType firstFunc = (BestHTTP_WebSocket_WebSocket__SendType)(g_Il2CppBase + BestHTTP_WebSocket_WebSocket__SendOffset);
     A64HookFunction((void*)firstFunc, (void*)&BestHTTP_WebSocket_WebSocket__Send, &trampolineBestHTTP_WebSocket_WebSocket__Send );
     BestHTTP_WebSocket_WebSocket__SendOrigin = (BestHTTP_WebSocket_WebSocket__SendType) trampolineBestHTTP_WebSocket_WebSocket__Send;

    void* trampolineBestHTTP_HTTPRequest__SetHeader;
    BestHTTP_HTTPRequest__SetHeaderType firstBestHTTP_HTTPRequest__SetHeader = (BestHTTP_HTTPRequest__SetHeaderType)(g_Il2CppBase + BestHTTP_HTTPRequest__SetHeaderOffset);
    A64HookFunction((void*)firstBestHTTP_HTTPRequest__SetHeader, (void*)BestHTTP_HTTPRequest__SetHeader, &trampolineBestHTTP_HTTPRequest__SetHeader );
    BestHTTP_HTTPRequest__SetHeaderOrigin = (BestHTTP_HTTPRequest__SetHeaderType) trampolineBestHTTP_HTTPRequest__SetHeader;

    void* trampolineBestHTTP_HTTPRequest__AddHeader;
    BestHTTP_HTTPRequest__AddHeaderType firstBestHTTP_HTTPRequest__AddHeader = (BestHTTP_HTTPRequest__AddHeaderType)(g_Il2CppBase + BestHTTP_HTTPRequest__AddHeaderOffset);
    A64HookFunction((void*)firstBestHTTP_HTTPRequest__AddHeader, (void*)BestHTTP_HTTPRequest__AddHeader, &trampolineBestHTTP_HTTPRequest__AddHeader );
    BestHTTP_HTTPRequest__AddHeaderOrigin = (BestHTTP_HTTPRequest__AddHeaderType) trampolineBestHTTP_HTTPRequest__AddHeader;

    void* trampolineSystem_Uri___ctor;
    System_Uri___ctorType firstSystem_Uri___ctor = (System_Uri___ctorType)(g_Il2CppBase + System_Uri___ctorOffset);
    A64HookFunction((void*)firstSystem_Uri___ctor, (void*)System_Uri___ctor, &trampolineSystem_Uri___ctor );
    System_Uri___ctorOrigin = (System_Uri___ctorType) trampolineSystem_Uri___ctor;

    void* trampolineGameSocket__SendPacket;
    GameSocket__SendPacketType firstGameSocket__SendPacket = (GameSocket__SendPacketType)(g_Il2CppBase + GameSocket__SendPacketOffset);
    A64HookFunction((void*)firstGameSocket__SendPacket, (void*)GameSocket__SendPacket, &trampolineGameSocket__SendPacket );
    GameSocket__SendPacketOrigin = (GameSocket__SendPacketType) trampolineGameSocket__SendPacket;
}

#endif

#endif //PIGMOD_TEST_H
