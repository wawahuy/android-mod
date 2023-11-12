//
// Created by nguye on 11/11/2023.
//

#ifndef PIGMOD_LIBIJ_H
#define PIGMOD_LIBIJ_H

namespace LibIj {
    typedef void (*IjRunActionType)(const std::string& action, const json& js);
    IjRunActionType ijRunAction;
    bool isLoaded = false;

    void runAction(const std::string& action, const json& js) {
        LOG_E("LIV %s %s", action.c_str(), js.dump().c_str());
        ijRunAction(action, js);
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

        typedef void (*InitMethodFunction)(uintptr_t, uintptr_t, uintptr_t, uintptr_t);
        InitMethodFunction initMethodFunction = (InitMethodFunction)dlsym(handle, "initMethod");
        if (!initMethodFunction) {
            LOG_E("Error locating the 'initMethod' function: %s", dlerror());
            dlclose(handle);
            return;
        }
        initMethodFunction(
                g_Il2CppBase,
                (uintptr_t)&getMs,
                (uintptr_t)&Game::unprotectIl2cpp,
                (uintptr_t)&Game::protectIl2cpp
                );


        ijRunAction = (IjRunActionType)dlsym(handle, "runAction");
        if (!ijRunAction) {
            LOG_E("Error locating the 'runAction' function: %s", dlerror());
            dlclose(handle);
            return;
        }

        LOG_E("Libpigmodij loaded=================");
        isLoaded = true;
    }
}


#endif //PIGMOD_LIBIJ_H
