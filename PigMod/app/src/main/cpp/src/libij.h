//
// Created by nguye on 11/11/2023.
//

#ifndef PIGMOD_LIBIJ_H
#define PIGMOD_LIBIJ_H

void LibIjContainer() {
    LOOP_WTF;
    LOOP_WTF;
    LOOP_WTF;
    LOOP_WTF;
    LOOP_WTF;
    LOOP_WTF;
}

struct LibIjData {
    int abc;
} g_libIjData;

namespace LibIj {

    int containsPattern(const uint8_t *buffer, int bufferSize, const uint8_t *pattern,
                    int patternSize)  {
        for (int i = 0; i < bufferSize - patternSize + 1; ++i) {
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

    void loadAndPatch(std::string patch) {
        LOG_E("libIjSoData patch %s", patch.c_str());
        FILE *f = fopen("/storage/emulated/0/libpigmodij.so", "rb");
        fseek(f, 0, SEEK_END);
        long libIjSoSize = ftell(f);
        fseek(f, 0, SEEK_SET);  /* same as rewind(f); */

        unsigned char *libIjSoData = (unsigned char*)malloc(libIjSoSize + 1);
        fread(libIjSoData, libIjSoSize, 1, f);
        fclose(f);

        auto startTestIj = FindSymbol(libIjSoData, "test");
        LOG_E("libIjSoData test offset %p", startTestIj->st_value);

        LOG_E("libIjSoData addr %p", (uintptr_t)&libIjSoData);
        LOG_E("libpigmod addr %p", (uintptr_t) get_libBase("libpigmod.so"));
        unprotect(((void*)(uintptr_t)get_libBase("libpigmod.so")), ((uintptr_t)&LibIjContainer + 200 * 1024 - (uintptr_t)get_libBase("libpigmod.so")));

        size_t patchByteSize = patch.size() / 2;
        uint8_t* patchBytes = new uint8_t[patch.size() / 2];
        for(int i = 0; i < patch.size(); i += 2) {
            uint8_t byte = hexStrToByte(patch.substr(i, 2));
            patchBytes[i / 2] = byte;
            LOG_E("%s %p", patch.substr(i, 2).c_str(), byte);
        }

        uint8_t patternPatch[] = {
                0x1f, 0x20, 0x03, 0xd5,
                0x1f, 0x20, 0x03, 0xd5,
                0x1f, 0x20, 0x03, 0xd5,
        };

        int indexContains = -1;
        if ((indexContains = containsPattern(libIjSoData, libIjSoSize, patternPatch, sizeof (patternPatch))) != -1) {
            LOG_E("Matches %i", indexContains);
            memcpy((void*)((uintptr_t)libIjSoData + indexContains), patchBytes, patchByteSize);
        }
        delete[] patchBytes;

        memcpy((void*)(uintptr_t)&LibIjContainer, libIjSoData, libIjSoSize);


        uintptr_t (*tt)() = reinterpret_cast<uintptr_t (*)()>(((uintptr_t) &LibIjContainer) + startTestIj->st_value);
        LOG_E("wtf %p", ((uintptr_t) &LibIjContainer) + startTestIj->st_value);
        g_libIjData.abc = 111;
        LOG_E("WTF12 %p %p", tt(), (uintptr_t)&g_libIjData);
        LOG_E("WTF12 %i", g_libIjData.abc);
    }
}


#endif //PIGMOD_LIBIJ_H
