//
// Created by nguye on 10/25/2023.
//

#ifndef PIGMOD_UTIL_H
#define PIGMOD_UTIL_H

#include <dlfcn.h>
#include "stdint.h"
#include <sys/mman.h>
#include <cstdio>
#include <chrono>
#include <sstream>
#include <net/if.h>

uintptr_t get_libBase(const char* libName) {
    uintptr_t addr = 0;
    FILE *fp = fopen("/proc/self/maps", "rt");
    char buf[1024];
    if (fp) {
        int posRp = 0;
        while (fgets(buf, sizeof (buf), fp)) {
            if (strstr(buf, libName) && strstr(buf, "r--p")) {
                posRp++;
            }
            if ((strstr(buf, libName) && strstr(buf, "r-xp")) || posRp == 2) {
                sscanf(buf, "%lx-%*s", &addr);
                break;
            }
        }
    }
    fclose(fp);
    return addr;
}

Il2CppBaseRange get_libBaseRange(const char* libName) {
    Il2CppBaseRange r;
    FILE *fp = fopen("/proc/self/maps", "rt");
    char buf[1024];
    if (fp) {
        int posRp = 0;
        while (fgets(buf, sizeof (buf), fp)) {
            if (strstr(buf, libName) && strstr(buf, "r--p")) {
                posRp++;
            }
            if ((strstr(buf, libName) && strstr(buf, "r-xp")) || posRp == 2) {
                sscanf(buf, "%lx-%lx %*s", &r.start, &r.end);
                break;
            }
        }
    }
    fclose(fp);
    return r;
}

uint64_t getMs() {
    return duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()
    ).count();
}

uintptr_t hexStrToPtr(std::string hexString) {
    uintptr_t v;
    std::stringstream ss;
    ss << std::hex << hexString;
    ss >> v;
    return v;
}

uint8_t hexStrToByte(const std::string& hex) {
    return strtol(hex.c_str(), NULL, 16);
}

std::string ptrToHexStr(uintptr_t address) {
    std::stringstream stream;
    stream << std::hex << address;

    std::string hexString = stream.str();
    return hexString;
}

std::string getPackageName() {
    pid_t pid = getpid();
    LOG_E( "process id %d", pid);
    char path[64] = { 0 };
    sprintf(path, "/proc/%d/cmdline", pid);
    FILE *cmdline = fopen(path, "r");
    if (cmdline) {
        char application_id[64] = { 0 };
        fread(application_id, sizeof(application_id), 1, cmdline);
        LOG_E("application id %s\n", application_id);
        fclose(cmdline);
        return std::string(application_id);
    }
    return std::string();
}

std::string getClipboard() {
    JNIEnv* env;

    JavaVMAttachArgs args;
    args.version = JNI_VERSION_1_6; // choose your JNI version
    args.name = NULL; // you might want to give the java thread a name
    args.group = NULL; // you might want to assign the java thread to a ThreadGroup
    g_Jvm->AttachCurrentThread(&env, &args);

    jclass mainAClass = env->FindClass("com/wawahuy/pigmod/MainActivity");
    if (mainAClass == nullptr) {
        LOG_E("Class not found");
        return "";
    }

    jmethodID getClipboardMethod = env->GetStaticMethodID(mainAClass, "getClipboard", "()Ljava/lang/String;");
    if (getClipboardMethod == nullptr) {
        LOG_E("Method not found");
        return "";
    }

    jstring result = (jstring)env->CallStaticObjectMethod(mainAClass, getClipboardMethod);
    const char* resultStr = env->GetStringUTFChars(result, nullptr);
    LOG_E("resultStr %s", resultStr);

    env->ReleaseStringUTFChars(result, resultStr);
    return resultStr;
}

std::string getPackageVersion() {
    JNIEnv* env;

    JavaVMAttachArgs args;
    args.version = JNI_VERSION_1_6; // choose your JNI version
    args.name = NULL; // you might want to give the java thread a name
    args.group = NULL; // you might want to assign the java thread to a ThreadGroup
    g_Jvm->AttachCurrentThread(&env, &args);

    jclass mainAClass = env->FindClass("com/wawahuy/pigmod/MainActivity");
    if (mainAClass == nullptr) {
        LOG_E("Class not found");
        return "";
    }

    jmethodID getPackageVersionMethod = env->GetStaticMethodID(mainAClass, "getPackageVersion", "()Ljava/lang/String;");
    if (getPackageVersionMethod == nullptr) {
        LOG_E("Method not found");
        return "";
    }

    jstring result = (jstring)env->CallStaticObjectMethod(mainAClass, getPackageVersionMethod);
    const char* resultStr = env->GetStringUTFChars(result, nullptr);
    LOG_E("pkg version %s", resultStr);

    env->ReleaseStringUTFChars(result, resultStr);
    return resultStr;
}

std::string getAndroidID(JNIEnv* env) {
    jclass mainAClass = env->FindClass("com/wawahuy/pigmod/MainActivity");
    jfieldID currentContextField = env->GetStaticFieldID(mainAClass, "contextCurrent", "Landroid/content/Context;");
    jobject context = env->GetStaticObjectField(mainAClass, currentContextField);
    jclass contextClass = env->GetObjectClass(context);
    jmethodID getContentResolver = env->GetMethodID(contextClass, "getContentResolver", "()Landroid/content/ContentResolver;");
    jobject contentResolver = env->CallObjectMethod(context, getContentResolver);
    jclass settingsSecureClass = env->FindClass("android/provider/Settings$Secure");
    jmethodID getString = env->GetStaticMethodID(settingsSecureClass, "getString", "(Landroid/content/ContentResolver;Ljava/lang/String;)Ljava/lang/String;");
    jstring androidIdKey = env->NewStringUTF("android_id");
    jstring androidId = static_cast<jstring>(env->CallStaticObjectMethod(settingsSecureClass, getString, contentResolver, androidIdKey));
    const char* resultStr = env->GetStringUTFChars(androidId, nullptr);
    env->DeleteLocalRef(androidIdKey);
    env->DeleteLocalRef(settingsSecureClass);
    return resultStr;
}

void startGame(std::string packageName, std::string className) {
    JNIEnv* env;

    JavaVMAttachArgs args;
    args.version = JNI_VERSION_1_6; // choose your JNI version
    args.name = NULL; // you might want to give the java thread a name
    args.group = NULL; // you might want to assign the java thread to a ThreadGroup
    g_Jvm->AttachCurrentThread(&env, &args);

    jclass mainAClass = env->FindClass("com/wawahuy/pigmod/MainActivity");
    if (mainAClass == nullptr) {
        LOG_E("Class not found");
        return;
    }

    jmethodID setSaveBoolMethod = env->GetStaticMethodID(mainAClass, "startGame",
                                                         "(Ljava/lang/String;Ljava/lang/String;)V");
    if (setSaveBoolMethod == nullptr) {
        LOG_E("Method not found");
        return;
    }

    jstring jPackageName = env->NewStringUTF(packageName.c_str());
    jstring jClassName = env->NewStringUTF(className.c_str());

    env->CallStaticVoidMethod(mainAClass, setSaveBoolMethod, jPackageName, jClassName);
}

#endif //PIGMOD_UTIL_H
