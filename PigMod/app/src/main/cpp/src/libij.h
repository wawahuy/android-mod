//
// Created by nguye on 11/11/2023.
//

#ifndef PIGMOD_LIBIJ_H
#define PIGMOD_LIBIJ_H

namespace LibIj {
    void loadAndPatch() {
        FILE *f = fopen("/storage/emulated/0/libpigmodij.so", "rb");
        fseek(f, 0, SEEK_END);
        long libIjSoSize = ftell(f);
        fseek(f, 0, SEEK_SET);  /* same as rewind(f); */

        unsigned char *libIjSoData = (unsigned char *) malloc(libIjSoSize + 1);
        LOG_E("libIjSoData addr %p", (uintptr_t) &libIjSoData);
        fread(libIjSoData, libIjSoSize, 1, f);
        fclose(f);

        std::string pathLibIj = "/data/data/" + getPackageName() +  "/libpigmodij.so";
        std::ofstream tempFile(pathLibIj, std::ios::binary);
        tempFile.write(reinterpret_cast<const char *>(libIjSoData), libIjSoSize);
        tempFile.close();

        void *handle = dlopen(pathLibIj.c_str(), RTLD_NOW);
        if (!handle) {
            LOG_E("Error loading dynamic library: %s", dlerror());
            return;
        }

        typedef void (*InitFunction)(uintptr_t);

        InitFunction initFunction = (InitFunction)dlsym(handle, "init");

        if (!initFunction) {
            LOG_E("Error locating the 'hello' function: %s", dlerror());
            dlclose(handle);
            return;
        }
        initFunction(g_Il2CppBase);
    }
}


#endif //PIGMOD_LIBIJ_H
