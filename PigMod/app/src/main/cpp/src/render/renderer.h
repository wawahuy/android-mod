//
// Created by nguye on 10/25/2023.
//

#ifndef PIGMOD_RENDERER_H
#define PIGMOD_RENDERER_H

namespace Renderer {
    static bool g_Initialized = false;

    void SetupRenderer(int w, int  h) {
        g_ScreenWidth = w;
        g_ScreenHeight = h;
        if (g_Initialized) {
            return;
        }
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        ImGui::StyleColorsDark();

        ImGui_ImplAndroid_Init(nullptr);
        ImGui_ImplOpenGL3_Init("#version 300 es");

        ImFontConfig font_cfg;
        // 22 3.0
        font_cfg.SizePixels = 36.0f;
        io.Fonts->AddFontDefault(&font_cfg);
        ImGui::GetStyle().ScaleAllSizes(4.0f);
        g_Initialized = true;
    }

    void Render() {
        ImGuiIO& io = ImGui::GetIO();
        // Our state
        // (we use static, which essentially makes the variable globals, as a convenience to keep the example code easy to follow)
        static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 0.00f);

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplAndroid_NewFrame(g_ScreenWidth, g_ScreenHeight);
        ImGui::NewFrame();

        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
#ifdef IS_DEBUG
#endif

//        ImGui::ShowDemoWindow();

        if (!Socket::isOpen) {
            ImGui::SetNextWindowPos({ 25, g_ScreenHeight / 2 - 200.0f });
            ImGui::SetNextWindowSize(ImVec2(g_ScreenWidth - 50, 0));
            ImGui::Begin(STR_WINDOWS_SERVER);
            ImGui::Text(STR_SOCKET_CONNECTING);
            ImGui::End();
        } else if (g_AuthStage != AuthStage::Oke) {
            AuthRenderer::Render();
        } else {
            MenuRenderer::Render();
        }

        // Rendering
        ImGui::Render();
        glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
}

#endif //PIGMOD_RENDERER_H
