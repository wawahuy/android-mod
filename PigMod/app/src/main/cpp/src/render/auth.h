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
        if (g_AuthStage == AuthStage::WaitIJ) {
            ImGui::Text(STR_AUTH_WAIT_IJ);
        }

        renderSystemMessage();
        ImGui::Text(STR_INPUT_KEY);
        ImGui::InputText("", g_AuthKey, sizeof(g_AuthKey));
        ImGui::SameLine();

        if (ImGui::Button(STR_INPUT_KEY_PASTE)) {
            auto str = getClipboard();
            memcpy(g_AuthKey, &str[0], std::min(254, (int)str.size()));
            size_t ss = std::min(255, (int)str.size());
            g_AuthKey[ss] = '\0';
        }

        ImGui::NewLine();
        ImGui::Text(STR_AUTH_AUTO);
        ImGui::SameLine();

        if (ToggleButton("g_AuthAuto", &g_AuthAuto)) {
            SaveData::saveBool(STR_SAVE_AUTO_LOGIN, g_AuthAuto);
        }

        if (ImGui::Button(STR_AUTH_LOGIN)) {
            if (g_AuthStage == AuthStage::None) {
                g_AuthTrial = false;
                Socket::handleLogin();
                SaveData::saveString(STR_SAVE_KEY, g_AuthKey);
            }
        }

        ImGui::SameLine();
        if (!g_DisableTrial && ImGui::Button(STR_AUTH_TRIAL_LOGIN)) {
            if (g_AuthStage == AuthStage::None) {
                g_AuthTrial = true;
                g_AuthKey[0] = '\0';
                Socket::handleLogin();
                SaveData::saveString(STR_SAVE_KEY, g_AuthKey);
            }
        }
        ImGui::NewLine();

        ImGui::Text("Mua key lien he telegram: wawahuy");
        ImGui::NewLine();

        ImGui::End();
    }
}

#endif //PIGMOD_AUTH_H
