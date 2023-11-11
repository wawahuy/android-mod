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

bool unprotect(void*  offset, unsigned  int size) {
    int result = mprotect(offset, size, PROT_READ | PROT_WRITE | PROT_EXEC);
    if (result == 0) {
        LOG_E("unprotect oke");
        return true;
    } else {
#ifdef IS_DEBUG
        int error = errno;
        const char* errorMessage = strerror(error);
        LOG_E("%s", errorMessage);
#endif
        return false;
    }
}

bool protect(void*  offset, unsigned  int size) {
    int result = mprotect(offset, size, PROT_READ | PROT_EXEC);
    if (result == 0) {
        LOG_E("protect oke");
        return true;
    } else {
#ifdef IS_DEBUG
        int error = errno;
        const char* errorMessage = strerror(error);
        LOG_E("%s", errorMessage);
#endif
        return false;
    }
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
    LOG_E("resultStr %s", resultStr);

    env->ReleaseStringUTFChars(result, resultStr);
    return resultStr;
}

bool getSaveContains(const char* name) {
    JNIEnv* env;

    JavaVMAttachArgs args;
    args.version = JNI_VERSION_1_6; // choose your JNI version
    args.name = NULL; // you might want to give the java thread a name
    args.group = NULL; // you might want to assign the java thread to a ThreadGroup
    g_Jvm->AttachCurrentThread(&env, &args);

    jclass mainAClass = env->FindClass("com/wawahuy/pigmod/MainActivity");
    if (mainAClass == nullptr) {
        LOG_E("Class not found");
        return false;
    }

    jmethodID getSaveContainsMethod = env->GetStaticMethodID(mainAClass, "getSaveContains",
                                                         "(Ljava/lang/String;)Z");
    if (getSaveContainsMethod == nullptr) {
        LOG_E("Method not found");
        return false;
    }

    jstring jName = env->NewStringUTF(name);
    jboolean result = env->CallStaticBooleanMethod(mainAClass, getSaveContainsMethod, jName);
    return result == JNI_TRUE;
}

std::string getSaveString(const char* name) {
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

    jmethodID getSaveStringMethod = env->GetStaticMethodID(mainAClass, "getSaveString",
                                                               "(Ljava/lang/String;)Ljava/lang/String;");
    if (getSaveStringMethod == nullptr) {
        LOG_E("Method not found");
        return "";
    }

    jstring jName = env->NewStringUTF(name);
    jstring result = (jstring)env->CallStaticObjectMethod(mainAClass, getSaveStringMethod, jName);
    const char* resultStr = env->GetStringUTFChars(result, nullptr);
    LOG_E("resultStr %s", resultStr);

    env->ReleaseStringUTFChars(result, resultStr);
    return resultStr;
}

void setSaveString(const char* name, const char* value) {
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

    jmethodID setSaveStringMethod = env->GetStaticMethodID(mainAClass, "setSaveString",
                                                           "(Ljava/lang/String;Ljava/lang/String;)V");
    if (setSaveStringMethod == nullptr) {
        LOG_E("Method not found");
        return;
    }

    jstring jName = env->NewStringUTF(name);
    jstring jValue = env->NewStringUTF(value);
    env->CallStaticVoidMethod(mainAClass, setSaveStringMethod, jName, jValue);
}

bool getSaveBool(const char* name) {
    JNIEnv* env;

    JavaVMAttachArgs args;
    args.version = JNI_VERSION_1_6; // choose your JNI version
    args.name = NULL; // you might want to give the java thread a name
    args.group = NULL; // you might want to assign the java thread to a ThreadGroup
    g_Jvm->AttachCurrentThread(&env, &args);

    jclass mainAClass = env->FindClass("com/wawahuy/pigmod/MainActivity");
    if (mainAClass == nullptr) {
        LOG_E("Class not found");
        return false;
    }

    jmethodID getSaveBoolMethod = env->GetStaticMethodID(mainAClass, "getSaveBool",
                                                         "(Ljava/lang/String;)Z");
    if (getSaveBoolMethod == nullptr) {
        LOG_E("Method not found");
        return false;
    }

    jstring jName = env->NewStringUTF(name);
    jboolean result = env->CallStaticBooleanMethod(mainAClass, getSaveBoolMethod, jName);
    return result == JNI_TRUE;
}

void setSaveBool(const char* name, bool value) {
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

    jmethodID setSaveBoolMethod = env->GetStaticMethodID(mainAClass, "setSaveBool",
                                                         "(Ljava/lang/String;Z)V");
    if (setSaveBoolMethod == nullptr) {
        LOG_E("Method not found");
        return;
    }

    jstring jName = env->NewStringUTF(name);
    env->CallStaticVoidMethod(mainAClass, setSaveBoolMethod, jName, value);
}

#endif //PIGMOD_UTIL_H
