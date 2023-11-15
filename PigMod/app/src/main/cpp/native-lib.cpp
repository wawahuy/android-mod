
#include "common.h"
#include "pthread.h"
#include "unistd.h"

void *hack_thread(void *) {
    LOG_E("v1 =================================================");
    Socket::init();
    
#ifdef IS_DEBUG
#endif
    LOG_E("v2 =================================================");
    pthread_exit(nullptr);
}


__attribute__((constructor)) void lib_main() {
    pthread_t id;
    pthread_create(&id, nullptr, hack_thread, nullptr);
}
