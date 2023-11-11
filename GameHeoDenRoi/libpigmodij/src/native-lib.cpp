#include <unistd.h>
#include "And64InlineHook.hpp"
#include <android/log.h>
#define LOG_E(...) __android_log_print(ANDROID_LOG_ERROR, "YUH", __VA_ARGS__);

// 00011100000000001000000011010010
// 00111100000000001000000011010010
// 01011100000000001000000011010010
// 01111100000000001000000011010010

// 0xcc
// 00011100000000001000000011010010
//            110011
// 10011100000110011000000011010010

// 0xffff
// 00011100000000001000000011010010
// 11111100111111111001111111010010


struct LibIjData {
    int abc;
} g_libIjData;

// typedef void (*Slingshot__OnPointerUpType)(void* __this, void* eventData, const void* method);
// void Slingshot__OnPointerUp(void* __this, void* eventData, const void* method) {
//     // Slingshot__OnPointerUpOrigin(__this, eventData, method);
// }

extern "C" {
    // __attribute__((visibility("default"))) int test(void* pageDataVoid, void* mappingFunctionVoid, uintptr_t addr) {
    //     auto a = (uint8_t*)pageDataVoid;

    //     uintptr_t Slingshot__OnPointerUpOffset = 0x1D475C0;
    //     void* trampolineSlingshot__OnPointerUp;
    //     Slingshot__OnPointerUpType firstFunc = (Slingshot__OnPointerUpType)(addr + Slingshot__OnPointerUpOffset);
    //     // A64HookFunction((void*)firstFunc, (void*)&Slingshot__OnPointerUp, &trampolineSlingshot__OnPointerUp );
    //     // Slingshot__OnPointerUpOrigin = (Slingshot__OnPointerUpType) trampolineSlingshot__OnPointerUp;
        
    //     return 123;
    // }

    __attribute__((visibility("default"))) uintptr_t test() {
        uintptr_t libIjDataAddr;
        __asm __volatile (
            "nop \r\n"
            "nop \r\n"
            "nop \r\n"
            "mov %0, x28 \r\n"
            : "=r" (libIjDataAddr)
        );
        LibIjData* libIjData = (LibIjData*) libIjDataAddr;
        libIjData->abc = 9999;
        return libIjDataAddr;
    }
}