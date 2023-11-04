//
// Created by nguye on 11/4/2023.
//

#ifndef PIGMOD_AUTH_H
#define PIGMOD_AUTH_H

namespace AuthRenderer {
    void Render() {
        ImGui::SetNextWindowPos({ 25, g_ScreenHeight / 2 - 200.0f });
        ImGui::SetNextWindowSize(ImVec2(g_ScreenWidth - 50, 550));

        ImGui::Begin(STR_WINDOWS_LOGIN);

        if (g_AuthStage == AuthStage::Doing) {
            ImGui::Text(STR_AUTH_DOING);
        }

        if (g_SystemMessage[0] != 0) {
            ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(
                    g_SystemMessageColor[0],
                    g_SystemMessageColor[1],
                    g_SystemMessageColor[2],
                    g_SystemMessageColor[3]
                    ));
            ImGui::Text("%s", g_SystemMessage);
            ImGui::PopStyleColor();
        }

        ImGui::Text(STR_INPUT_KEY);
        ImGui::InputText("", g_AuthKey, sizeof(g_AuthKey));
        ImGui::SameLine();
        ImGui::Button(STR_INPUT_KEY_PASTE);
        ImGui::NewLine();
        ImGui::Text(STR_AUTH_AUTO);
        ImGui::SameLine();
        ToggleButton("g_AuthAuto", &g_AuthAuto);

        if (g_AuthStage != AuthStage::Doing) {
            if (ImGui::Button(STR_AUTH_LOGIN)) {
                Socket::handleLogin();
            }
        }
        ImGui::NewLine();

        ImGui::End();
    }
}

#endif //PIGMOD_AUTH_H
