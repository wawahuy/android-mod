//
// Created by nguye on 11/11/2023.
//

#ifndef PIGMOD_LIBIJ_H
#define PIGMOD_LIBIJ_H

namespace LibIj {
    void runAction(const std::string& action, const json& js) {
        LOG_E("%s %s", action.c_str(), js.dump().c_str());
    }

    void saveLib(char* data, size_t dataSize) {
        std::string pathLibIj = "/data/data/" + getPackageName() +  "/libpigmodij.so";
        std::ofstream tempFile(pathLibIj, std::ios::binary);
        tempFile.write(data, dataSize);
        tempFile.close();
        LOG_E("Save libpigmodij");
    }

    void loadLib() {
        std::string pathLibIj = "/data/data/" + getPackageName() +  "/libpigmodij.so";
        void *handle = dlopen(pathLibIj.c_str(), RTLD_NOW);
        if (!handle) {
            LOG_E("Error loading dynamic library: %s", dlerror());
            return;
        }

        typedef void (*InitMethodFunction)(uintptr_t getMsPtr);
        InitMethodFunction initMethodFunction = (InitMethodFunction)dlsym(handle, "initMethod");
        if (!initMethodFunction) {
            LOG_E("Error locating the 'initMethodFunction' function: %s", dlerror());
            dlclose(handle);
            return;
        }
        initMethodFunction((uintptr_t)&getMs);

        typedef void (*InitFunction)(uintptr_t);
        InitFunction initFunction = (InitFunction)dlsym(handle, "init");
        if (!initFunction) {
            LOG_E("Error locating the 'init' function: %s", dlerror());
            dlclose(handle);
            return;
        }
        initFunction(g_Il2CppBase);
    }
}


#endif //PIGMOD_LIBIJ_H
