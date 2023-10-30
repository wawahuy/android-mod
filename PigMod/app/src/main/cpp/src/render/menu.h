//
// Created by nguye on 10/30/2023.
//

#ifndef PIGMOD_MENU_H
#define PIGMOD_MENU_H

namespace MenuRenderer {
    bool isFloating = true;

    void RenderFloatButton() {
        ImGui::SetNextWindowPos({ g_ScreenWidth / 2.0f - 50.0f, 50.0f });
        ImGui::SetNextWindowBgAlpha(0);
        ImGui::Begin("", &isFloating, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration);
        ImGui::SetNextWindowSize(ImVec2(300, 200));
        if (ImGui::Button("0x67Huy")) {
            isFloating = false;
        }
        ImGui::End();
    }

    void Render() {
        if (isFloating) {
            RenderFloatButton();
            return;
        }
        ImGui::SetNextWindowSize(ImVec2(300, 200));
        ImGui::Begin("0x67Huy");
        for(auto& gp: Game::dataPatchArray) {

            ImGui::Text("%s", gp->name.c_str());

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

        if (ImGui::Button("TAT MENU")) {
            isFloating = true;
        }

        ImGui::End();
    }
}

#endif //PIGMOD_MENU_H
