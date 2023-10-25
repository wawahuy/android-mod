//
// Created by nguye on 10/25/2023.
//

#ifndef PIGMOD_JAVA_H
#define PIGMOD_JAVA_H


extern "C" {
    JNIEXPORT void JNICALL
    Java_com_wawahuy_pigmod_NativeMethods_onSurfaceCreated(JNIEnv * env , jclass clazz ) {
    }

    JNIEXPORT void JNICALL
    Java_com_wawahuy_pigmod_NativeMethods_onSurfaceChanged(JNIEnv *env, jclass clazz, jint width, jint height) {
        Renderer::SetupRenderer(width, height);
    }

    JNIEXPORT void JNICALL
    Java_com_wawahuy_pigmod_NativeMethods_onDrawFrame(JNIEnv *env, jclass clazz) {
        Renderer::Render();
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
            if (io.WantCaptureMouse)
                return false;
        }
        return true;
    }
}
#endif //PIGMOD_JAVA_H
