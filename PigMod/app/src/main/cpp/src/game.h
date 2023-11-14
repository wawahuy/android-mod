//
// Created by nguye on 10/26/2023.
//

#ifndef PIGMOD_GAME_H
#define PIGMOD_GAME_H



namespace Game {

    void unprotectIl2cpp() {
        g_Unprotected = MemoryPatch::unprotect((void*)g_Il2CppBaseRange.start, g_Il2CppBaseRange.end - g_Il2CppBaseRange.start);
    }

    void protectIl2cpp() {
        g_Unprotected = !MemoryPatch::protect((void*)g_Il2CppBaseRange.start, g_Il2CppBaseRange.end - g_Il2CppBaseRange.start);
    }

    void init() {
        if (g_UnprotectedDefault) {
            unprotectIl2cpp();
        }
#ifdef IS_TEST
        testInit();
#endif
    }
}

#endif //PIGMOD_GAME_H
