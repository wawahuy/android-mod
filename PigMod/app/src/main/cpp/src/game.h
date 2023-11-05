//
// Created by nguye on 10/26/2023.
//

#ifndef PIGMOD_GAME_H
#define PIGMOD_GAME_H

namespace Game {
    struct OffsetPatch {
        uintptr_t offset;
        std::vector<unsigned char> patch;
        std::vector<unsigned char> backup;
    };

    struct GroupPatch {
        std::string name;
        std::vector<OffsetPatch *> patches;
        bool activeDefault;
        bool active;
    };

    struct GuiGroupPatch {
        std::string name;
        std::vector<GroupPatch *> dataPatchArray;
    };

    static std::vector<GuiGroupPatch *> guiGroupPatchArray;
    static struct MenuInfo {
        std::string hash;
    } menuInfo;


    void unprotectIl2cpp() {
        g_Unprotected = unprotect((void*)g_Il2CppBaseRange.start, g_Il2CppBaseRange.end - g_Il2CppBaseRange.start);
    }

    void protectIl2cpp() {
        g_Unprotected = !protect((void*)g_Il2CppBaseRange.start, g_Il2CppBaseRange.end - g_Il2CppBaseRange.start);
    }

    void il2cppWriteMemory(uintptr_t f, void* data, size_t size) {
        memcpy((void*)(g_Il2CppBase + f), data, size);
    }

    void il2cppCopyMemory2Vector(std::vector<unsigned char>& v, uintptr_t f, size_t size) {
        memcpy(v.data(), (void*)(g_Il2CppBase + f), size);
    }

    void patch(GroupPatch* gp) {
        LOG_E("Active Feature %s", gp->name.c_str());
        bool hasUnprotected = false;
        if (!g_Unprotected) {
            unprotectIl2cpp();
            hasUnprotected = true;
        }

        const std::vector<OffsetPatch *>& lbs = gp->patches;
        for (auto lb: lbs) {
            auto sizePatch = lb->patch.size();
            // backup
            lb->backup = std::vector<unsigned char>(lb->patch.size());
            Game::il2cppCopyMemory2Vector(lb->backup, lb->offset, sizePatch);
            LOG_E("backup %p", (uintptr_t)lb->backup[0]);

            // write
            Game::il2cppWriteMemory(lb->offset, &lb->patch[0], sizePatch);
            LOG_E("write %p", (uintptr_t )lb->patch[0]);
        }
        gp->active = true;

        if (hasUnprotected) {
            protectIl2cpp();
        }
    }

    void unpatch(GroupPatch* gp) {
        LOG_E("De-active Feature %s", gp->name.c_str());
        bool hasUnprotected = false;
        if (!g_Unprotected) {
            unprotectIl2cpp();
            hasUnprotected = true;
        }

        const std::vector<OffsetPatch *>& lbs = gp->patches;
        for (auto it = lbs.rbegin(); it != lbs.rend(); ++it) {
            auto lb = *it;
            Game::il2cppWriteMemory(lb->offset, &lb->backup[0], lb->backup.size());
            LOG_E("restore %p", (uintptr_t )(void*)lb->backup[0]);
        }
        gp->active = false;

        if (hasUnprotected) {
            protectIl2cpp();
        }
    }

    bool canInit(const json& js) {
        bool isInit = !g_MenuInit;
        if (isInit || js.contains("versionHash")) {
            auto newHash = js["versionHash"].template get<std::string>();
            if (newHash != Game::menuInfo.hash) {
                isInit = true;
            }
        }
        return isInit;
    }

    void release() {
        for(auto& ggp: guiGroupPatchArray) {
            for (auto& gp: ggp->dataPatchArray) {
                if (gp->active) {
                    unpatch(gp);
                }
                for (auto& p: gp->patches) {
                    delete p;
                }
                delete gp;
            }
            delete ggp;
        }
        guiGroupPatchArray.clear();
        g_MenuInit = false;
    }

    void init(const json& js) {
        if (g_UnprotectedDefault) {
            unprotectIl2cpp();
        }

        release();

        if (js.contains("versionHash")) {
            menuInfo.hash = js["versionHash"].template get<std::string>();
        }

        auto menu = js["menu"];
        for (auto& groupItem: menu) {
            GuiGroupPatch* guiGroupPatch = new GuiGroupPatch();
            guiGroupPatch->name = groupItem["name"];

            for (auto &menuItem: groupItem["items"]) {
                const json &il2cppPatch = menuItem["il2cppPatch"];
                bool isActive = false;
                if (menuItem.contains("activeDefault")) {
                    isActive = menuItem["activeDefault"].template get<bool>();
                }
                if (menuItem.contains("active")) {
                    isActive = menuItem["active"].template get<bool>();
                }

                GroupPatch *gp = new GroupPatch();
                gp->name = menuItem["name"].template get<std::string>();
                gp->activeDefault = isActive;
                gp->active = isActive;
                guiGroupPatch->dataPatchArray.push_back(gp);

                for (auto& il2cppPatchItem: il2cppPatch) {
                    OffsetPatch* op = new OffsetPatch();
                    op->offset = hexStrToPtr(il2cppPatchItem["offset"].template get<std::string>());
                    LOG_E("%p", op->offset);

                    auto patchStr = il2cppPatchItem["patch"].template get<std::string>();
                    LOG_E("%s", patchStr.c_str());
                    for(int i = 0; i < patchStr.size(); i += 2) {
                        uint8_t byte = hexStrToByte(patchStr.substr(i, 2));
                        op->patch.push_back(byte);
                        LOG_E("%p", byte);
                    }
                    gp->patches.push_back(op);
                }
            }

            guiGroupPatchArray.push_back(guiGroupPatch);
        }

        unprotectIl2cpp();
        if (g_Unprotected) {
            for(auto& ggp: guiGroupPatchArray) {
                for (auto &gp: ggp->dataPatchArray) {
                    if (gp->activeDefault) {
                        patch(gp);
                    }
                }
            }
        }
        protectIl2cpp();

        g_MenuInit = true;
    }
}

#endif //PIGMOD_GAME_H
