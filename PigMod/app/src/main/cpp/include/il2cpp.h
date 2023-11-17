//
// Created by nguye on 11/17/2023.
//

#ifndef PIGMOD_IL2CPP_H
#define PIGMOD_IL2CPP_H
#include "inc.h"
#include <wchar.h>
#include <string>

struct System_String_Fields {
    int32_t m_stringLength;
    wchar_t m_firstChar[1];

    wchar_t* getPtr() {
        return m_firstChar;
    }
};

struct System_String_o {
    void *klass;
    void *monitor;
    System_String_Fields fields;
};

std::string System_String_o_ToString(System_String_o* o);

#endif //PIGMOD_IL2CPP_H
