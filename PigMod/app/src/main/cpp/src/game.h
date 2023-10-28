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
        unprotect((void*)g_Il2CppBaseRange.start, g_Il2CppBaseRange.end - g_Il2CppBaseRange.start);
        g_Unprotected = true;
    }

    void init() {
        isInit = true;
        if (g_UnprotectedDefault) {
            unprotectIl2cpp();
        }
    }
}

#endif //PIGMOD_GAME_H
