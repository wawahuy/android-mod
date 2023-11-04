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
#include "ImGui/imgui.h"
#include "ImGui/imgui_internal.h"
#include "ImGui/imgui_impl_opengl3.h"
#include "ImGui/imgui_impl_android.h"

#include "src/x67huy-socket/X67HuySocket.h"
#include "src/util.h"
#include "src/game.h"
#include "src/socket.h"
#ifdef IS_DEBUG
#endif

#ifdef IS_DEBUG
#endif
#include "src/render/common.h"
#include "src/render/menu.h"
#include "src/render/renderer.h"
#include "src/java.h"

#endif //PIGMOD_COMMON_H
