//
// Created by nguye on 10/26/2023.
//

#ifndef PIGMOD_GAME_H
#define PIGMOD_GAME_H

namespace Game {
    bool isInit = false;

    struct offset_t {
        uintptr_t gameManager = 0xFCC304;
    } offset;

    struct Function {
        
    } fncptr;

    void unprotectIl2cpp() {
        g_Unprotected = unprotect((void*)g_Il2CppBaseRange.start, g_Il2CppBaseRange.end - g_Il2CppBaseRange.start);
    }

    void il2cppWriteMemory(uintptr_t f, void* data, size_t size) {
        memcpy((void*)(g_Il2CppBase + f), data, size);
    }

    void il2cppCopyMemory2Vector(std::vector<unsigned char>& v, uintptr_t f, size_t size) {
        memcpy(v.data(), (void*)(g_Il2CppBase + f), size);
    }


    void init() {
        isInit = true;
        if (g_UnprotectedDefault) {
            unprotectIl2cpp();
        }
    }
}

#endif //PIGMOD_GAME_H
