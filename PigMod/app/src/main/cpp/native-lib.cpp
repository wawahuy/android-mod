
#include "common.h"
#include "pthread.h"
#include "unistd.h"

void *hack_thread(void *) {
//    LOG_E("v1 =================================================");
//    do {
//        sleep(1);
//    } while ((g_Il2CppBase = get_libBase("libil2cpp.so")) == 0);
//    g_Il2CppBaseRange = get_libBaseRange("libil2cpp.so");
//    Game::init();
    DebugSocket::init();
    Debug::init();
    LOG_E("v2 =================================================");
    pthread_exit(nullptr);
}

__attribute__((constructor)) void lib_main() {
    pthread_t id;
    pthread_create(&id, nullptr, hack_thread, nullptr);
}