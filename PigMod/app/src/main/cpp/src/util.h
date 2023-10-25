//
// Created by nguye on 10/25/2023.
//

#ifndef PIGMOD_UTIL_H
#define PIGMOD_UTIL_H
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

#endif //PIGMOD_UTIL_H
