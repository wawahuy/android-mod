//
// Created by nguye on 11/17/2023.
//

#include "il2cpp.h"

std::string System_String_o_ToString(System_String_o* o) {
    std::string str;
    str.resize(o->fields.m_stringLength);
    uint8_t* ptr = (uint8_t *)o->fields.getPtr();
    for (int i = 0; i < o->fields.m_stringLength; i ++) {
        str[i] = *((char*)(ptr + i * 2));
    }
    return str;
}

void StringToSystem_String_o(std::string str, System_String_o* o) {
    uint8_t* ptr = (uint8_t *)o->fields.getPtr();
    for (int i = 0; i < o->fields.m_stringLength; i ++) {
        *((char*)(ptr + i * 2)) = str[i];
    }
}