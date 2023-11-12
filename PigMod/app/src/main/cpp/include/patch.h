//
// Created by nguye on 11/12/2023.
//

#ifndef PIGMOD_PATCH_H
#define PIGMOD_PATCH_H
#include "inc.h"
#include <dlfcn.h>
#include <stdint.h>
#include <cstdio>
#include <sys/mman.h>
#include <cerrno>
#include <cstring>
#include <vector>


class MemoryPatch {
public:
    static bool unprotect(void*  offset, unsigned  int size);
    static bool protect(void*  offset, unsigned  int size);
    static void executeAll(std::vector<MemoryPatch> &mps);
    static void restoreAll(std::vector<MemoryPatch> &mps);
    MemoryPatch();
    MemoryPatch(const MemoryPatch& memoryPatch);
    MemoryPatch(uintptr_t offset, const std::vector<uint8_t>& patch);
    void execute();
    void restore();
    bool isActive();

private:
    uintptr_t _offset;
    bool _isActive = false;
    std::vector<uint8_t> _patch;
    std::vector<uint8_t> _backup;
};

#endif //PIGMOD_PATCH_H
