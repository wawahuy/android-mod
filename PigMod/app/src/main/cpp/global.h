//
// Created by nguye on 10/25/2023.
//

#ifndef PIGMOD_GLOBAL_H
#define PIGMOD_GLOBAL_H
#pragma once
#include "stdint.h"

#define IS_DEBUG true

#ifdef IS_DEBUG
#endif

int g_ScreenWidth = 0;
int g_ScreenHeight = 0;

uintptr_t g_Il2CppBase = 0;
struct Il2CppBaseRange {
    uintptr_t start;
    uintptr_t end;
} g_Il2CppBaseRange;

bool g_UnprotectedDefault = false;
bool g_Unprotected = g_UnprotectedDefault;

#endif //PIGMOD_GLOBAL_H
