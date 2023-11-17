//
// Created by nguye on 10/25/2023.
//

#ifndef PIGMOD_JAVA_H
#define PIGMOD_JAVA_H
#include <iostream>

extern "C" {
    JNIEXPORT void JNICALL
    Java_com_wawahuy_pigmod_NativeMethods_onSurfaceCreated(JNIEnv * env , jclass clazz ) {}

    JNIEXPORT void JNICALL
    Java_com_wawahuy_pigmod_NativeMethods_onSurfaceChanged(JNIEnv *env, jclass clazz, jint width, jint height) {
        Renderer::SetupRenderer(width, height);
    }

    JNIEXPORT void JNICALL
    Java_com_wawahuy_pigmod_NativeMethods_onDrawFrame(JNIEnv *env, jclass clazz) {
        Renderer::Render();
        if (g_CanStartGame) {
            Renderer::g_Initialized = false;
            g_CanStartGame = false;
            startGame("com.aladinfun.clashofsky_th_pig", "com.aladinfun.piggyboom.MainAppActivity");
        }
    }

    JNIEXPORT jboolean JNICALL
    Java_com_wawahuy_pigmod_NativeMethods_hookEvent(JNIEnv * env, jclass clazz, jfloat x, jfloat y, jint action) {
        if (Renderer::g_Initialized) {
            ImGuiIO& io = ImGui::GetIO();
            switch (action) {
                case 0: // Down
                    io.AddMousePosEvent(x, y);
                    io.AddMouseButtonEvent(0, true);
                    break;
                case 1: // Up
                    io.AddMouseButtonEvent(0, false);
                    io.AddMousePosEvent(-1, -1);
                    break;
                case 2: // Mouse
                    io.AddMousePosEvent(x, y);
                    break;
            }
            if (io.WantCaptureMouse || !MenuRenderer::isFloating)
                return false;
        }
        return true;
    }

    JNIEXPORT void JNICALL
    Java_com_wawahuy_pigmod_NativeMethods_initEnv(JNIEnv *env, jclass clazz) {
        env->GetJavaVM(&g_Jvm);
        g_PackageVersion = getPackageVersion();

        if (getSaveContains(STR_SAVE_AUTO_LOGIN)) {
            auto key = getSaveString(STR_SAVE_KEY);
            memcpy(g_AuthKey, &key[0], std::min(255, (int) key.size()));
            g_AuthAuto = getSaveBool(STR_SAVE_AUTO_LOGIN);
        } else {
            setSaveBool(STR_SAVE_AUTO_LOGIN, g_AuthAutoDefault);
            g_AuthAuto = g_AuthAutoDefault;
        }
    }
}
#endif //PIGMOD_JAVA_H
