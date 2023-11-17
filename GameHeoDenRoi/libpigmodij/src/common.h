#pragma once
#include "inc.h"
#include "json.hpp"
#include "obfuscate.h"
#include "patch.h"
#include "And64InlineHook.hpp"
#include "util.h"

#define JS_THIS "this"


uint64_t (*m_getMs)();
void (*m_unprotectIl2cpp)();
void (*m_protectIl2cpp)();
uintptr_t g_il2CppBase;

void (*il2cpp_thread_detach)(void *);
void* (*il2cpp_thread_attach)(void *);
void* (*il2cpp_domain_get)();

#include "cay-thong.h"
#include "bet.h"
