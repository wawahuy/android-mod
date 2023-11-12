
#include "common.h"
#include "pthread.h"
#include "unistd.h"

//uintptr_t Slingshot__OnPointerUpOffset = 0x1D475C0;
//uint64_t timeA = 0;
//typedef void (*Slingshot__OnPointerUpType)(void* __this, void* eventData, const void* method);
//void (*Slingshot__OnPointerUpOrigin)(void* __this, void* eventData, const void* method);
//void Slingshot__OnPointerUp(void* __this, void* eventData, const void* method) {
//    LOG_E("WTF");
//    if (getMs() - timeA > 100) {
//        LOG_E("WTF OKE");
//        timeA = getMs();
//        Slingshot__OnPointerUpOrigin(__this, eventData, method);
//    }
//}


//uintptr_t UIAttackView__StartAttackOffset = 0x1EED444;
//typedef void* (*UIAttackView__StartAttackType) (void* __this, void* partData, const void* method);
//void* (*UIAttackView__StartAttackOrigin) (void* __this, void* partData, const void* method);
//void* UIAttackView__StartAttack(void* __this, void* partData, const void* method) {
//    LOG_E("WTF2 OKE");
//    UIAttackView__StartAttackOrigin(__this, partData, method);
//    LOG_E("WTF2 OKE 2");
//    return UIAttackView__StartAttackOrigin(__this, partData, method);
//}


void *hack_thread(void *) {
    LOG_E("v1 =================================================");
#ifndef IS_DEBUG_NOT_GAME
    do {
        sleep(1);
    } while ((g_Il2CppBase = get_libBase("libil2cpp.so")) == 0);
    g_Il2CppBaseRange = get_libBaseRange("libil2cpp.so");
#endif
    Socket::init();
    Game::init();

//    void* trampolineSlingshot__OnPointerUp;
//    Slingshot__OnPointerUpType firstFunc = (Slingshot__OnPointerUpType)(g_Il2CppBase + Slingshot__OnPointerUpOffset);
//    A64HookFunction((void*)firstFunc, (void*)&Slingshot__OnPointerUp, &trampolineSlingshot__OnPointerUp );
//    Slingshot__OnPointerUpOrigin = (Slingshot__OnPointerUpType) trampolineSlingshot__OnPointerUp;

//    void* trampolineUIAttackView__StartAttack;
//    UIAttackView__StartAttackType firstFuncUIAttackView__StartAttack = (UIAttackView__StartAttackType)(g_Il2CppBase + UIAttackView__StartAttackOffset);
//    A64HookFunction((void*)firstFuncUIAttackView__StartAttack, (void*)&UIAttackView__StartAttack, &trampolineUIAttackView__StartAttack );
//    UIAttackView__StartAttackOrigin = (UIAttackView__StartAttackType) trampolineUIAttackView__StartAttack;

#ifdef IS_DEBUG
#endif
    LOG_E("v2 =================================================");
    pthread_exit(nullptr);
}


__attribute__((constructor)) void lib_main() {
    pthread_t id;
    pthread_create(&id, nullptr, hack_thread, nullptr);
}
