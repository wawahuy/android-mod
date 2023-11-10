#include <unistd.h>
#include "And64InlineHook.hpp"
// #include <chrono>

#define IS_DEBUG true
#ifdef IS_DEBUG
#include <android/log.h>
#define LOG_E(...) __android_log_print(ANDROID_LOG_ERROR, "YUH", __VA_ARGS__);
#else
#define LOG_E(...)
#endif

// uint64_t getMs() {
//     return duration_cast<std::chrono::milliseconds>(
//         std::chrono::system_clock::now().time_since_epoch()
//     ).count();
// }

uintptr_t g_Il2CppBase = 0;

// uintptr_t Slingshot__OnPointerUpOffset = 0x1D475C0;
// uint64_t timeA = 0;
// typedef void (*Slingshot__OnPointerUpType)(void* __this, void* eventData, const void* method);
// void (*Slingshot__OnPointerUpOrigin)(void* __this, void* eventData, const void* method);
// void Slingshot__OnPointerUp(void* __this, void* eventData, const void* method) {
//    LOG_E("WTF");
//    if (getMs() - timeA > 100) {
//        LOG_E("WTF OKE");
//        timeA = getMs();
//        Slingshot__OnPointerUpOrigin(__this, eventData, method);
//    }
// }

extern "C" {
    __attribute__((visibility("default"))) int test(int a, int b) {
#ifdef IS_DEBUG
    LOG_E("vIJ =================================================");
#endif
// #ifndef IS_DEBUG_NOT_GAME
//     do {
//         sleep(1);
//     } while ((g_Il2CppBase = get_libBase("libil2cpp.so")) == 0);
//     g_Il2CppBaseRange = get_libBaseRange("libil2cpp.so");
// #endif

//    void* trampolineSlingshot__OnPointerUp;
//    Slingshot__OnPointerUpType firstFunc = (Slingshot__OnPointerUpType)(g_Il2CppBase + Slingshot__OnPointerUpOffset);
//    A64HookFunction((void*)firstFunc, (void*)&Slingshot__OnPointerUp, &trampolineSlingshot__OnPointerUp );
//    Slingshot__OnPointerUpOrigin = (Slingshot__OnPointerUpType) trampolineSlingshot__OnPointerUp;

#ifdef IS_DEBUG
    LOG_E("vIJ =================================================");
#endif
        return a + b + 8;
    }

}