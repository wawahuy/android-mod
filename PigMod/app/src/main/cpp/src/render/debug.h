//
// Created by nguye on 10/28/2023.
//

#ifndef PIGMOD_DEBUG_RENDER_H
#define PIGMOD_DEBUG_RENDER_H
#include <vector>
#include <string>

namespace DebugRenderer {
    uint64_t lastMapAccessTime = getMs();
    std::vector<std::string> mapStrVector;

    void RenderUnprotect() {
        ImGui::Text("il2cpp: %p", g_Il2CppBase);
        if (g_Unprotected) {
            ImGui::Text("il2cpp unprotect %p - %p", g_Il2CppBaseRange.start, g_Il2CppBaseRange.end);
        } else {
            if (ImGui::Button("il2cpp unprotect")) {
                Game::unprotectIl2cpp();
            }
        }
    }

    void RenderMap() {
        uint64_t currentTime = getMs();
        if (currentTime - lastMapAccessTime >= 1000) {
            mapStrVector.clear();
            FILE *fp = fopen("/proc/self/maps", "rt");
            char buf[1024];
            if (fp) {
                while (fgets(buf, sizeof (buf), fp)) {
                    if (strstr(buf, "libil2cpp.so")) {
                        mapStrVector.push_back(std::string(buf));
                    }
                }
                fclose(fp);
            }
            lastMapAccessTime = currentTime;
        }

        for(auto str: mapStrVector) {
            ImGui::Text("%s", str.c_str());
        }
    }

    void Render() {
        ImGui::Begin("Debug");

        if (Game::isInit) {
            RenderUnprotect();
            RenderMap();

            uintptr_t np = g_Il2CppBase + 0x1FF656C;
            const unsigned char over[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06 };
            ImGui::Text("OnExitBtnClicked");
            for (int i = 0; i < sizeof over; i++) {
                if (i != 0) {
                    ImGui::SameLine();
                }
                ImGui::Text("%p", *((unsigned  char *)(np + i)));
            }
            if (ImGui::Button("test")) {
                memcpy((void*)np, &over, sizeof over);
            }
        } else {
            ImGui::Text("find library...");
        }

        ImGui::End();
    }
}

#endif //PIGMOD_DEBUG_RENDER_H
