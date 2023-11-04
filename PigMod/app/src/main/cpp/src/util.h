//
// Created by nguye on 10/25/2023.
//

#ifndef PIGMOD_UTIL_H
#define PIGMOD_UTIL_H

#include <dlfcn.h>
#include "stdint.h"
#include <sys/mman.h>
#include <cstdio>
#include <chrono>
#include <sstream>

uintptr_t get_libBase(const char* libName) {
    uintptr_t addr = 0;
    FILE *fp = fopen("/proc/self/maps", "rt");
    char buf[1024];
    if (fp) {
        int posRp = 0;
        while (fgets(buf, sizeof (buf), fp)) {
            if (strstr(buf, libName) && strstr(buf, "r--p")) {
                posRp++;
            }
            if ((strstr(buf, libName) && strstr(buf, "r-xp")) || posRp == 2) {
                sscanf(buf, "%lx-%*s", &addr);
                break;
            }
        }
    }
    fclose(fp);
    return addr;
}

Il2CppBaseRange get_libBaseRange(const char* libName) {
    Il2CppBaseRange r;
    FILE *fp = fopen("/proc/self/maps", "rt");
    char buf[1024];
    if (fp) {
        int posRp = 0;
        while (fgets(buf, sizeof (buf), fp)) {
            if (strstr(buf, libName) && strstr(buf, "r--p")) {
                posRp++;
            }
            if ((strstr(buf, libName) && strstr(buf, "r-xp")) || posRp == 2) {
                sscanf(buf, "%lx-%lx %*s", &r.start, &r.end);
                break;
            }
        }
    }
    fclose(fp);
    return r;
}

bool unprotect(void*  offset, unsigned  int size) {
    int result = mprotect(offset, size, PROT_READ | PROT_WRITE | PROT_EXEC);
    if (result == 0) {
        LOG_E("unprotect oke");
        return true;
    } else {
#ifdef IS_DEBUG
        int error = errno;
        const char* errorMessage = strerror(error);
        LOG_E("%s", errorMessage);
#endif
        return false;
    }
}

bool protect(void*  offset, unsigned  int size) {
    int result = mprotect(offset, size, PROT_READ | PROT_EXEC);
    if (result == 0) {
        LOG_E("protect oke");
        return true;
    } else {
#ifdef IS_DEBUG
        int error = errno;
        const char* errorMessage = strerror(error);
        LOG_E("%s", errorMessage);
#endif
        return false;
    }
}

uint64_t getMs() {
    return duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()
    ).count();
}

uintptr_t hexStrToPtr(std::string hexString) {
    uintptr_t v;
    std::stringstream ss;
    ss << std::hex << hexString;
    ss >> v;
    return v;
}

uint8_t hexStrToByte(std::string hexString) {
    return ((hexString[0] - '0') << 4) | (hexString[1] - '0');
}

std::string getPackageName() {
    pid_t pid = getpid();
    LOG_E( "process id %d", pid);
    char path[64] = { 0 };
    sprintf(path, "/proc/%d/cmdline", pid);
    FILE *cmdline = fopen(path, "r");
    if (cmdline) {
        char application_id[64] = { 0 };
        fread(application_id, sizeof(application_id), 1, cmdline);
        LOG_E("application id %s\n", application_id);
        fclose(cmdline);
        return std::string(application_id);
    }
    return std::string();
}

#endif //PIGMOD_UTIL_H
