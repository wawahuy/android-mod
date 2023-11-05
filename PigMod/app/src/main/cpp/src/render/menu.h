//
// Created by nguye on 10/30/2023.
//

#ifndef PIGMOD_MENU_H
#define PIGMOD_MENU_H

namespace MenuRenderer {
#ifdef IS_DEBUG
    static bool isFloating = false;
#elif
    static bool isFloating = true;
#endif
    uint64_t lastMapAccessTime = getMs();
    std::vector<std::string> mapStrVector;

    void RenderCommandVIl2CppMapList() {
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


    void RenderFloatButton() {
        ImGui::SetNextWindowPos({ g_ScreenWidth / 2.0f - 50.0f, 50.0f });
        ImGui::SetNextWindowBgAlpha(0);
        ImGui::Begin("", &isFloating, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration);
        ImGui::SetNextWindowSize(ImVec2(300, 200));
        if (ImGui::Button(STR_BUTTON_FLOAT)) {
            isFloating = false;
        }
        ImGui::End();
    }

    void Render() {
        if (isFloating) {
            RenderFloatButton();
            return;
        }
        ImGui::SetNextWindowPos({ 25, 25 });
        ImGui::SetNextWindowSize(ImVec2(g_ScreenWidth - 50, 0));
        ImGui::Begin(STR_WINDOWS_MENU);
        renderSystemMessage();

        if (ImGui::Button(STR_BUTTON_HIDE_MENU)) {
            isFloating = true;
        }

        ImGui::SameLine();
        ImGui::Checkbox(STR_MENU_AUTO_RELOAD, &g_MenuAutoUpdate);

        ImGui::SameLine();
        if (ImGui::Button(STR_BUTTON_LOGOUT)) {
            g_AuthStage = AuthStage::None;
        }

        if (g_MenuInit) {
            ImGui::Separator();
            ImGui::Text("Hash: %s", Game::menuInfo.hash.c_str());
            for (auto &ggp: Game::guiGroupPatchArray) {
                ImGui::SeparatorText(ggp->name.c_str());
                for (auto &gp: ggp->dataPatchArray) {
                    ImGui::BulletText("%s", gp->name.c_str());

                    ImGui::SameLine();
                    bool active = gp->active;
                    if (ToggleButton(gp->name.c_str(), &active)) {
                        if (active) {
                            Game::patch(gp);
                        } else {
                            Game::unpatch(gp);
                        }
                    }
                }
            }
        } else {
            ImGui::Text(STR_MENU_STARTING);
        }

        ImGui::End();
    }
}

#endif //PIGMOD_MENU_H
