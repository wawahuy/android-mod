//
// Created by nguye on 10/25/2023.
//

#ifndef PIGMOD_GLOBAL_H
#define PIGMOD_GLOBAL_H
#pragma once
#include "inc.h"
#include <jni.h>
#include "stdint.h"
#include "string"
#include "X67HuySocket.h"

#ifdef IS_DEBUG
#define SOCKET_PORT 1235
#define SOCKET_HOST "192.168.1.21"
#else
#define SOCKET_PORT 80
#define SOCKET_HOST "x67socket.j2run.com"
#endif

JavaVM* g_Jvm;

int g_ScreenWidth = 0;
int g_ScreenHeight = 0;
std::string g_PackageVersion;

uintptr_t g_Il2CppBase = 0;
struct Il2CppBaseRange {
    uintptr_t start;
    uintptr_t end;
} g_Il2CppBaseRange;

bool g_UnprotectedDefault = false;
bool g_Unprotected = g_UnprotectedDefault;

enum AuthStage {
    None,
    Doing,
    WaitIJ,
    Oke
};

AuthStage g_AuthStage = AuthStage::None;
char g_SystemMessageColor[] = { 255, 0, 0, 255 };
char g_SystemMessage[255] = { 0 };
char g_AuthKey[255];
bool g_AuthAutoDefault = true;
bool g_AuthAuto = g_AuthAutoDefault;
bool g_AuthTrial = false;
bool g_AuthRe = false;
bool g_MenuInit = false;
X67HuySocket* g_Socket = nullptr;

bool g_CanStartGame = false;
std::string g_AndroidID;
std::string g_AppPackageName;
std::string g_AppClassName;

#ifdef IS_DEBUG
#else
#endif

#endif //PIGMOD_GLOBAL_H
