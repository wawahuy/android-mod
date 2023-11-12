//
// Created by nguye on 11/12/2023.
//

#ifndef PIGMOD_COMMON_INC_H
#define PIGMOD_COMMON_INC_H

//#define IS_DEBUG true
//#define IS_DEBUG_NOT_GAME true

#ifdef IS_DEBUG
#include <android/log.h>
#define LOG_E(...) __android_log_print(ANDROID_LOG_ERROR, "YUH", __VA_ARGS__);
#else
#define LOG_E(...)
#endif


#endif //PIGMOD_COMMON_H
