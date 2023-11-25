#pragma once

#define ACTION_USER_DATA_REQUEST AY_OBFUSCATE("nangluong.userData")
#include <pthread.h>
#include <unistd.h>

typedef void Player_o ;

struct Il2CppType
{
    void* data;
    unsigned int bits;
};

struct Il2CppClass_1
{
    void* image;
    void* gc_desc;
    void* name;
    void* namespaze;
    Il2CppType byval_arg;
    Il2CppType this_arg;
    void* element_class;
    void* castClass;
    void* declaringType;
    void* parent;
    void *generic_class;
    void* typeMetadataHandle;
    void* interopData;
    void* klass;
    void* fields;
    void* events;
    void* properties;
    void* methods;
    void** nestedTypes;
    void** implementedInterfaces;
    void* interfaceOffsets;
};

struct UnityEngine_Vector3_Fields {
	float x;
	float y;
	float z;
};

struct UnityEngine_Vector3_o {
	UnityEngine_Vector3_Fields fields;
};

struct Main_StaticFields {
	bool isLastAutoGame;
	bool isLastAutoGameSub;
	bool isLastAutoInSetting;
	UnityEngine_Vector3_o scale;
	void* flyText;
	Player_o* myPlayer;
};

struct Main_c {
	Il2CppClass_1 _1;
	Main_StaticFields* static_fields;
};

namespace UserDataPatch {
    typedef void (*Player__activeOneHitType)(Player_o* __this, const void* method);
    Player__activeOneHitType Player__activeOneHit;
    
    uintptr_t mainCBaseOffset = 0x220FC68;
    Main_c* mainCBase;
    Player_o* playerBase;


    Player_o* player_o;
    void *activeOneHitThread(void *pl) {
        void* t = il2cpp_thread_attach(il2cpp_domain_get());
        Player__activeOneHit(pl, nullptr);
        il2cpp_thread_detach(t);
        pthread_exit(nullptr);
    }

    
    //
    uintptr_t  Player___ctorOffset = 0x7AECA8;
    typedef void (* Player___ctorType) (Player_o* __this, const void* method);
    Player___ctorType  Player___ctorOrigin;
    void  Player___ctor(Player_o* __this, const void* method) {
        player_o = __this;
        LOG_E("$$$$$$$$ current %p", (uintptr_t)(void*)player_o);
        Player___ctorOrigin(__this, method);
        
        // playerBase = mainCBase->static_fields->myPlayer;
        // LOG_E("$$$$$$$$ base %p", (uintptr_t)(void*)playerBase);
        // if (playerBase != 0x00) {
        //     pthread_t id;
        //     pthread_create(&id, nullptr, activeOneHitThread, nullptr);;
        // }
    }

    uintptr_t  Player__setSpeedOffset = 0x797FBC;
    typedef void (* Player__setSpeedType) (Player_o* __this, float sp, const void* method);
    Player__setSpeedType  Player__setSpeedOrigin;
    void  Player__setSpeed(Player_o* __this, float sp, const void* method) {
        LOG_E("$$$$$$$$ speed %f", sp);
        Player__setSpeedOrigin(__this, sp, method);
    }

    uintptr_t  Player__setTimeScaleOffset = 0x792554;
    typedef void (* Player__setTimeScaleType) (Player_o* __this, float sp, const void* method);
    Player__setTimeScaleType  Player__setTimeScaleOrigin;
    void  Player__setTimeScale(Player_o* __this, float sp, const void* method) {
        LOG_E("$$$$$$$$ scale %f", sp);
        Player__setTimeScaleOrigin(__this, sp, method);
    }

    uintptr_t  Player__hitOffset = 0x78813C;
    typedef void (* Player__hitType) (Player_o* __this, int damage, int currHp, bool dd, bool isCrit, const void* method);
    Player__hitType  Player__hitOrigin;
    void  Player__hit(Player_o* __this, int damage, int currHp, bool dd, bool isCrit, const void* method) {
        LOG_E("$$$$$$$$ hit %i %i %i %i", damage, currHp, dd, isCrit);
        Player__hitOrigin(__this, damage, currHp, dd, isCrit , method);
    }

    uintptr_t  Session_ME__connectOffset = 0x8EDA80;
    typedef void (* Session_ME__connectType) (void* __this, System_String_o* host, int32_t port, const void* method);
    Session_ME__connectType  Session_ME__connectOrigin;
    void  Session_ME__connect(void* __this, System_String_o* host, int32_t port, const void* method) {
        LOG_E("$$$$$$$$ socket %s %i", System_String_o_ToString(host).c_str(), port);
        std::string newHost = "192.168.1.21";
        StringToSystem_String_o(newHost, host);
        host->fields.m_stringLength = newHost.size();
        Session_ME__connectOrigin(__this, host, port, method);
    }

    void init() {
        LOG_E("$$$$$$$$ WTF");
        void* trampolinePlayer___ctor;
        Player___ctorType firstPlayer___ctor = (Player___ctorType)(g_il2CppBase + Player___ctorOffset);
        A64HookFunction((void*)firstPlayer___ctor, (void*)&Player___ctor, &trampolinePlayer___ctor );
        Player___ctorOrigin = (Player___ctorType) trampolinePlayer___ctor;

        void* trampolinePlayer__setSpeed;
        Player__setSpeedType firstPlayer__setSpeed = (Player__setSpeedType)(g_il2CppBase + Player__setSpeedOffset);
        A64HookFunction((void*)firstPlayer__setSpeed, (void*)&Player__setSpeed, &trampolinePlayer__setSpeed );
        Player__setSpeedOrigin = (Player__setSpeedType) trampolinePlayer__setSpeed;

        void* trampolinePlayer__setTimeScale;
        Player__setTimeScaleType firstPlayer__setTimeScale = (Player__setTimeScaleType)(g_il2CppBase + Player__setTimeScaleOffset);
        A64HookFunction((void*)firstPlayer__setTimeScale, (void*)&Player__setTimeScale, &trampolinePlayer__setTimeScale );
        Player__setTimeScaleOrigin = (Player__setTimeScaleType) trampolinePlayer__setTimeScale;

        void* trampolinePlayer__hit;
        Player__hitType firstPlayer__hit = (Player__hitType)(g_il2CppBase + Player__hitOffset);
        A64HookFunction((void*)firstPlayer__hit, (void*)&Player__hit, &trampolinePlayer__hit );
        Player__hitOrigin = (Player__hitType) trampolinePlayer__hit;

        void* trampolineSession_ME__connect;
        Session_ME__connectType firstSession_ME__connect = (Session_ME__connectType)(g_il2CppBase + Session_ME__connectOffset);
        A64HookFunction((void*)firstSession_ME__connect, (void*)&Session_ME__connect, &trampolineSession_ME__connect );
        Session_ME__connectOrigin = (Session_ME__connectType) trampolineSession_ME__connect;

        Player__activeOneHit = (Player__activeOneHitType)(g_il2CppBase + 0x79DEE4);
        mainCBase = (Main_c*)(g_il2CppBase + mainCBaseOffset);
    }
}