//
// Created by nguye on 10/25/2023.
//

#ifndef HOTRELOAD_UTIL_H
#define HOTRELOAD_UTIL_H

#include <unistd.h>
#include <dlfcn.h>
#include <android/log.h>
#include "stdint.h"

uintptr_t get_libBase(const char* libName) {
    uintptr_t addr = 0;
    FILE *fp = fopen("/proc/self/maps", "rt");
    char buf[1024];
    if (fp) {
        while (fgets(buf, sizeof (buf), fp)) {
            // r--p r-xp
            if (strstr(buf, libName)) {
                sscanf(buf, "%lx-%*s", &addr);
                break;
            }
        }
    }
    fclose(fp);
    return addr;
}

const char* getPathLibPigMod() {
    Dl_info dl_info;
    int result = dladdr((void*)get_libBase("libpigmod.so"), &dl_info);

    if (result && dl_info.dli_fname) {
        return dl_info.dli_fname;
    } else {
        return nullptr;
    }
}

void hotReloadLibrary() {
    const char* newLibPath = getPathLibPigMod();
    __android_log_print(ANDROID_LOG_ERROR, "LibraryPath", "%s", newLibPath);
    void* oldLib = dlopen(newLibPath, RTLD_NOW);
    if (!oldLib) {
        __android_log_print(ANDROID_LOG_ERROR, "LibraryPath", "%s", dlerror());
    } else {
        __android_log_print(ANDROID_LOG_ERROR, "LibraryPath", "close");
        if (dlclose(oldLib) != 0) {
            __android_log_print(ANDROID_LOG_ERROR, "LibraryPath", "%s", dlerror());
        }
        __android_log_print(ANDROID_LOG_ERROR, "LibraryPath", "%s", getPathLibPigMod());
    }
    void* newLib = dlopen(newLibPath, RTLD_NOW);
}

#endif //HOTRELOAD_UTIL_H
