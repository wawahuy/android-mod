//
// Created by nguye on 11/11/2023.
//

#ifndef PIGMOD_LIBIJ_H
#define PIGMOD_LIBIJ_H
#include "And64InlineHook.hpp"
#include <dlfcn.h>

void LibIjContainer() {
    LOOP_WTF;
    LOOP_WTF;
    LOOP_WTF;
    LOOP_WTF;
    LOOP_WTF;
    LOOP_WTF;
}

struct LibIjData {
    void (*A64HookFunction)(void *const symbol, void *const replace, void **result);
} g_libIjData;

namespace LibIj {

    int containsPattern(int offset, const uint8_t *buffer, int bufferSize, const uint8_t *pattern,
                    int patternSize)  {
        if (offset < 0) {
            offset = 0;
        }
        for (int i = offset; i < bufferSize - patternSize + 1; ++i) {
            bool found = true;
            for (int j = 0; j < patternSize; ++j) {
                if (buffer[i + j] != pattern[j]) {
                    found = false;
                    break;
                }
            }
            if (found) {
                return i;
            }
        }
        return -1;
    }

    void loadAndPatchV2(std::string patch) {
        LOG_E("libIjSoData patch %s", patch.c_str());
        FILE *f = fopen("/storage/emulated/0/libpigmodij.so", "rb");
        fseek(f, 0, SEEK_END);
        long libIjSoSize = ftell(f);
        fseek(f, 0, SEEK_SET);  /* same as rewind(f); */

        unsigned char *libIjSoData = (unsigned char*)malloc(libIjSoSize + 1);
        LOG_E("libIjSoData addr %p", (uintptr_t)&libIjSoData);
        fread(libIjSoData, libIjSoSize, 1, f);
        fclose(f);

        auto libIjContainerBase = (uintptr_t)&LibIjContainer;
        LOG_E("libIjContainerBase addr %p", libIjContainerBase);

        auto initFunctionPtr = FindSymbol(libIjSoData, "init");
        LOG_E("libIjSoData test offset %p", initFunctionPtr->st_value);

        uintptr_t libPigModBase = (uintptr_t)get_libBase("libpigmod.so");
        LOG_E("libPigModBase addr %p", libPigModBase);

        unprotect((void*)libPigModBase, libIjContainerBase + libIjSoSize - libPigModBase);

        // convert patch hex string to bytes
        size_t patchByteSize = patch.size() / 2;
        uint8_t* patchBytes = new uint8_t[patch.size() / 2];
        for(int i = 0; i < patch.size(); i += 2) {
            uint8_t byte = hexStrToByte(patch.substr(i, 2));
            patchBytes[i / 2] = byte;
        }

        // find data match pattern and replace it with patchBytes
        uint8_t patternPatchFind[] = {
                0x1f, 0x20, 0x03, 0xd5,
                0x1f, 0x20, 0x03, 0xd5,
                0x1f, 0x20, 0x03, 0xd5,
                0x1f, 0x20, 0x03, 0xd5,
        };

        int indexPatchFind = -1;
        do {
            indexPatchFind = containsPattern(
                indexPatchFind,
                libIjSoData,
                libIjSoSize,
                patternPatchFind, sizeof (patternPatchFind));
            if (indexPatchFind > -1) {
                LOG_E("patternPatchFind Matches %i", indexPatchFind);
                memcpy((void*)((uintptr_t)libIjSoData + indexPatchFind), patchBytes, patchByteSize);
            }
        } while (indexPatchFind > -1);
        delete[] patchBytes;

        // move libIjSoData overwrite LibIjContainer function
        memcpy((void*)(uintptr_t)&LibIjContainer, libIjSoData, libIjSoSize);

        // init function
        g_libIjData.A64HookFunction = reinterpret_cast<void (*)( void *const, void *const, void **)>(&A64HookFunction);

        void (*initFunction)(uintptr_t base) = reinterpret_cast<void (*)(uintptr_t)>(
                libIjContainerBase + initFunctionPtr->st_value);
        LOG_E("initFunction %p", libIjContainerBase + initFunctionPtr->st_value);
        initFunction(g_Il2CppBase);
    }

    void loadAndPatchV3(std::string patch) {
        FILE *f = fopen("/storage/emulated/0/libpigmodij.so", "rb");
        fseek(f, 0, SEEK_END);
        long libIjSoSize = ftell(f);
        fseek(f, 0, SEEK_SET);  /* same as rewind(f); */

        unsigned char *libIjSoData = (unsigned char*)malloc(libIjSoSize + 1);
        LOG_E("libIjSoData addr %p", (uintptr_t)&libIjSoData);
        fread(libIjSoData, libIjSoSize, 1, f);
        fclose(f);



        void* handle = dlopen("libpigmodij.so", RTLD_LAZY | RTLD_NOW);
        if (!handle) {
            LOG_E("Error loading dynamic library: %s", dlerror());
            return;
        }

        auto libPigModIjBase = get_libBaseRange("libpigmodij.so");
        unprotect((void*)libPigModIjBase.start, libPigModIjBase.end - libPigModIjBase.start);

        auto initFunctionPtr = FindSymbol(libIjSoData, "init");
        LOG_E("libIjSoData test offset %p", initFunctionPtr->st_value);

        // convert patch hex string to bytes
        size_t patchByteSize = patch.size() / 2;
        uint8_t* patchBytes = new uint8_t[patch.size() / 2];
        for(int i = 0; i < patch.size(); i += 2) {
            uint8_t byte = hexStrToByte(patch.substr(i, 2));
            patchBytes[i / 2] = byte;
        }

        // find data match pattern and replace it with patchBytes
        uint8_t patternPatchFind[] = {
                0x1f, 0x20, 0x03, 0xd5,
                0x1f, 0x20, 0x03, 0xd5,
                0x1f, 0x20, 0x03, 0xd5,
                0x1f, 0x20, 0x03, 0xd5,
        };

        int indexPatchFind = -1;
        do {
            indexPatchFind = containsPattern(
                    indexPatchFind,
                    libIjSoData,
                    libIjSoSize,
                    patternPatchFind, sizeof (patternPatchFind));
            if (indexPatchFind > -1) {
                LOG_E("patternPatchFind Matches %i", indexPatchFind);
                memcpy((void*)((uintptr_t)libIjSoData + indexPatchFind), patchBytes, patchByteSize);
            }
        } while (indexPatchFind > -1);
        delete[] patchBytes;

        // move libIjSoData overwrite LibIjContainer function
        memcpy((void*)libPigModIjBase.start, libIjSoData, libIjSoSize);

        // init function
        g_libIjData.A64HookFunction = reinterpret_cast<void (*)( void *const, void *const, void **)>(&A64HookFunction);

        void (*initFunction)(uintptr_t base) = reinterpret_cast<void (*)(uintptr_t)>(
                libPigModIjBase.start + initFunctionPtr->st_value);
        LOG_E("initFunction %p", libPigModIjBase.start + initFunctionPtr->st_value);
        initFunction(g_Il2CppBase);
    }

    void loadAndPatch(std::string patch) {
        FILE *f = fopen("/storage/emulated/0/libpigmodij.so", "rb");
        fseek(f, 0, SEEK_END);
        long libIjSoSize = ftell(f);
        fseek(f, 0, SEEK_SET);  /* same as rewind(f); */

        unsigned char *libIjSoData = (unsigned char *) malloc(libIjSoSize + 1);
        LOG_E("libIjSoData addr %p", (uintptr_t) &libIjSoData);
        fread(libIjSoData, libIjSoSize, 1, f);
        fclose(f);

        std::ofstream tempFile("/data/data/com.aladinfun.clashofsky_th_pig/libpigmodij.so", std::ios::binary);
        tempFile.write(reinterpret_cast<const char *>(libIjSoData), libIjSoSize);
        tempFile.close();

        void *handle = dlopen("/data/data/com.aladinfun.clashofsky_th_pig/libpigmodij.so", RTLD_NOW);
        if (!handle) {
            LOG_E("Error loading dynamic library: %s", dlerror());
            return;
        }

        typedef void (*HelloFunction)();

        HelloFunction hello = (HelloFunction)dlsym(handle, "test");

        if (!hello) {
            LOG_E("Error locating the 'hello' function: %s", dlerror());
            dlclose(handle);
            return;
        }

        // Call the function
        hello();

    }
}


#endif //PIGMOD_LIBIJ_H
