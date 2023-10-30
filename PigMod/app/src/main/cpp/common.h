//
// Created by nguye on 10/25/2023.
//

#ifndef PIGMOD_COMMON_H
#define PIGMOD_COMMON_H

#include "global.h"

#ifdef IS_DEBUG
#include <android/log.h>
#define LOG_E(...) __android_log_print(ANDROID_LOG_ERROR, "YUH", __VA_ARGS__);
#else
#define LOG_E(...)
#endif


#include <jni.h>
#include <string>
#include <vector>

#include <EGL/egl.h>
#include <GLES3/gl3.h>
#include "external-lib/ImGui/imgui.h"
#include "external-lib/ImGui/imgui_internal.h"
#include "external-lib/ImGui/imgui_impl_opengl3.h"
#include "external-lib/ImGui/imgui_impl_android.h"

#include "src/util.h"
#include "src/game.h"
#ifdef IS_DEBUG
#include "src/debug_socket.h"
#include "src/debug.h"
#endif

#ifdef IS_DEBUG
#include "src/render/debug.h"
#endif
#include "src/render/common.h"
#include "src/render/menu.h"
#include "src/render/renderer.h"
#include "src/java.h"

#endif //PIGMOD_COMMON_H
