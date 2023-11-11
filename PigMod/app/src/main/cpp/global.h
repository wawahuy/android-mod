//
// Created by nguye on 10/25/2023.
//

#ifndef PIGMOD_GLOBAL_H
#define PIGMOD_GLOBAL_H
#pragma once
#include <jni.h>
#include "stdint.h"
#include "string"

#define IS_DEBUG true
//#define IS_DEBUG_NOT_GAME true

#ifdef IS_DEBUG
#define SOCKET_PORT 1235
#define SOCKET_HOST "192.168.1.21"
#else
#define SOCKET_PORT 80
#define SOCKET_HOST "x67huy-s1.j2run.com"
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
    Oke
};

AuthStage g_AuthStage = AuthStage::None;
char g_SystemMessageColor[] = { 255, 0, 0, 255 };
char g_SystemMessage[255] = { 0 };
char g_AuthKey[255];
bool g_AuthAutoDefault = true;
bool g_AuthAuto = g_AuthAutoDefault;
bool g_MenuInit = false;

#ifdef IS_DEBUG
#else
#endif

#endif //PIGMOD_GLOBAL_H
