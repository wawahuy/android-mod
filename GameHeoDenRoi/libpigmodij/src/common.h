#pragma once
#include "inc.h"
#include "json.hpp"
#include "obfuscate.h"
#include "patch.h"
#include "And64InlineHook.hpp"

#define JS_THIS "this"


uint64_t (*m_getMs)();
void (*m_unprotectIl2cpp)();
void (*m_protectIl2cpp)();
uintptr_t g_il2CppBase;

#include "cay-thong.h"
#include "bet.h"
