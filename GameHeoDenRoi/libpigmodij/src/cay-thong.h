#pragma once

#define ACTION_CT_AUTO_TRUNG AY_OBFUSCATE("cayThong.autoTrung")
#define ACTION_CT_BAN_NHANH AY_OBFUSCATE("cayThong.banNhanh")
#define ACTION_CT_BAN_1_CHAM AY_OBFUSCATE("cayThong.ban1Cham")
#define ACTION_CT_NAP_DAN_NHANH AY_OBFUSCATE("cayThong.napDanNhanh")

// #include <inttypes.h>

// void printByteArray(const uint8_t* array, size_t size) {
//     char buffer[65535];
//     buffer[0] = '\0';

//     for (size_t i = 0; i < size; ++i) {
//         snprintf(buffer + strlen(buffer), sizeof(buffer) - strlen(buffer), "0x%02X, ", array[i]);
//     }

//     LOG_E("%s", buffer);
// }

// struct System_String_Fields {
// 	int32_t m_stringLength;
// 	// uint16_t m_firstChar;
// 	char m_firstChar[1];

//     char *getC() {
//         return m_firstChar;
//     }
// };

// struct System_String_o {
// 	void *klass;
// 	void *monitor;
// 	System_String_Fields fields;
// };

// void printString(System_String_o* o) {
//     LOG_E("str %i", o->fields.m_stringLength);
//     LOG_E(("%." + std::to_string(o->fields.m_stringLength) + "s").c_str(), o->fields.getC());
// }

// struct Il2CppObject
// {
//     void *klass;
//     void *monitor;
// };

// typedef uintptr_t il2cpp_array_size_t;

// struct System_Byte_array {
// 	Il2CppObject obj;
// 	void *bounds;
// 	il2cpp_array_size_t max_length;
// 	uint8_t m_Items[65535];
// };

// uint64_t wtf;
// uintptr_t BestHTTP_WebSocket_WebSocket__SendOffset = 0x1814938;
// typedef void (*BestHTTP_WebSocket_WebSocket__SendType)(void* __this, System_Byte_array* buffer, const void* method);
// void (*BestHTTP_WebSocket_WebSocket__SendOrigin)(void* __this, System_Byte_array* buffer, const void* method);
// void BestHTTP_WebSocket_WebSocket__Send(void* __this, System_Byte_array* buffer, const void* method) {
//     if (m_getMs() - wtf < 14000) {
//         LOG_E("NOOOOOOOOOOO");
//         BestHTTP_WebSocket_WebSocket__SendOrigin(__this, buffer, method);
//         return;
//     }
//     LOG_E("?????????????? %i %s", buffer->max_length, buffer->m_Items);
//     printByteArray(buffer->m_Items, buffer->max_length);
//     if (buffer->max_length == 144) {
//         // for(int i=0; i<1000; i++) {
//         //     BestHTTP_WebSocket_WebSocket__SendOrigin(__this, buffer, method);
//         // }
//         // LOG_E("WTTTTTTTTTTTTF");
//     }
//     if (buffer->max_length == 80 && buffer->m_Items[0] == 0x4D  && buffer->m_Items[1] == 0x42 && buffer->m_Items[2] == 0x40) {
//         // for(int i=0; i<10000; i++) {
//         //     BestHTTP_WebSocket_WebSocket__SendOrigin(__this, buffer, method);
//         // }
//         // LOG_E("WTTTTTTTTTTTTF XOAY");
//     }
//     if (buffer->max_length == 96) {
//         // for(int i=0; i<1000; i++) {
//         //     BestHTTP_WebSocket_WebSocket__SendOrigin(__this, buffer, method);
//         // }
//         // LOG_E("WTTTTTTTTTTTTF TROM");
//     }
//     // if (buffer->max_length == 128) {
//     //     for(int i=0; i<1000; i++) {
//     //         BestHTTP_WebSocket_WebSocket__SendOrigin(__this, buffer, method);
//     //     }
//     //     LOG_E("WTTTTTTTTTTTTF Doi");
//     // }
//     // if (buffer->max_length == 112) {
//     //     for(int i=0; i<1000; i++) {
//     //         BestHTTP_WebSocket_WebSocket__SendOrigin(__this, buffer, method);
//     //     }
//     //     LOG_E("WTTTTTTTTTTTTF Chuyen");
//     // }
//     BestHTTP_WebSocket_WebSocket__SendOrigin(__this, buffer, method);
// }


// uintptr_t BestHTTP_WebSocket_WebSocket__SendStrOffset = 0x18146B4;
// typedef void (*BestHTTP_WebSocket_WebSocket__SendStrType)(void* __this, System_String_o* message, const void* method);
// void (*BestHTTP_WebSocket_WebSocket__SendStrOrigin)(void* __this, System_String_o* message, const void* method);
// void BestHTTP_WebSocket_WebSocket__SendStr(void* __this, System_String_o* message, const void* method) {
//     LOG_E("?????????????? string %i", message->fields.m_stringLength, message->fields.m_firstChar);
//     printString(message);
//     BestHTTP_WebSocket_WebSocket__SendStrOrigin(__this, message, method);
// }

// struct System_Uri_Fields {
// 	System_String_o* m_String;
// 	System_String_o* m_originalUnicodeString;
// 	void* m_Syntax;
// 	System_String_o* m_DnsSafeHost;
// 	uint64_t m_Flags;
// 	void* m_Info;
// 	bool m_iriParsing;
// };

// struct System_Uri_o {
// 	void *klass;
// 	void *monitor;
// 	System_Uri_Fields fields;
// };

// uintptr_t BestHTTP_WebSocket_WebSocket___ctorOffset = 0x18127B4;
// typedef void (*BestHTTP_WebSocket_WebSocket___ctorType)(void* __this, System_Uri_o* message, const void* method);
// void (*BestHTTP_WebSocket_WebSocket___ctorOrigin)(void* __this, System_Uri_o* message, const void* method);
// void BestHTTP_WebSocket_WebSocket___ctor(void* __this, System_Uri_o* message, const void* method) {
//     BestHTTP_WebSocket_WebSocket___ctorOrigin(__this, message, method);
// }

namespace CayThongPatch {
    std::vector<MemoryPatch> mbAutoTrungs;
    MemoryPatch mbBanNhanh;
    MemoryPatch mbBan1Cham;
    MemoryPatch mbNapDanNhanh;

    //
    bool Slingshot__OnPointerUpActive = false;
    bool Slingshot__OnPointerUpHooking = false;
    uint64_t Slingshot__OnPointerUpTime = 0;
    int Slingshot__OnPointerUpCountCall = 1;
    int Slingshot__OnPointerUpSpeed = 100;
    uintptr_t Slingshot__OnPointerUpOffset = 0x1D475C0;
    typedef void (*Slingshot__OnPointerUpType)(void* __this, void* eventData, const void* method);
    void (*Slingshot__OnPointerUpOrigin)(void* __this, void* eventData, const void* method);
    void Slingshot__OnPointerUp(void* __this, void* eventData, const void* method) {
        LOG_E("test %i", (char)((uintptr_t)__this + 0x133));
        LOG_E("Slingshot__OnPointerUp hook call");
        if (!Slingshot__OnPointerUpActive) {
            Slingshot__OnPointerUpOrigin(__this, eventData, method);
            return;
        }
        if (m_getMs() - Slingshot__OnPointerUpTime > Slingshot__OnPointerUpSpeed) {
            LOG_E("Slingshot__OnPointerUp hook wait");
            Slingshot__OnPointerUpTime = m_getMs();
            for (int i = 0; i < Slingshot__OnPointerUpCountCall; i++) {
                uintptr_t s1 = (uintptr_t)__this + 0x133;
                auto s2 = (char *)s1;
                *s2 = 1;
                Slingshot__OnPointerUpOrigin(__this, eventData, method);
            }
        }
    }


    void autoTrungAction(const nlohmann::json& js) {
        bool isActive = js[JS_THIS];
        if (isActive) {
            LOG_E("Active autoTrungAction");
            m_unprotectIl2cpp();
            MemoryPatch::executeAll(mbAutoTrungs);
            m_protectIl2cpp();
        } else {
            LOG_E("Restore autoTrungAction");
            m_unprotectIl2cpp();
            MemoryPatch::restoreAll(mbAutoTrungs);
            m_protectIl2cpp();
        }
    }

    void banNhanhAction(const nlohmann::json& js) {
        bool isActive = js[JS_THIS];
        if (isActive) {
            LOG_E("Active banNhanhAction");
            m_unprotectIl2cpp();
            mbBanNhanh.execute();
            m_protectIl2cpp();
        } else {
            LOG_E("Restore banNhanhAction");
            m_unprotectIl2cpp();
            mbBanNhanh.restore();
            m_protectIl2cpp();
        }
    }

    void ban1ChamAction(const nlohmann::json& js) {
        bool isActive = js[JS_THIS];
        if (isActive) {
            LOG_E("Active ban1ChamAction");
            m_unprotectIl2cpp();
            mbBan1Cham.execute();
            m_protectIl2cpp();
        } else {
            LOG_E("Restore ban1ChamAction");
            m_unprotectIl2cpp();
            mbBan1Cham.restore();
            m_protectIl2cpp();
        }
    }

    void napDanNhanhAction(const nlohmann::json& js) {
        bool isActive = js[JS_THIS];
        if (isActive) {
            LOG_E("Active napDanNhanhAction");
            Slingshot__OnPointerUpActive = true;

            if (js.contains("count")) {
                Slingshot__OnPointerUpCountCall = js["count"];
                LOG_E("Slingshot__OnPointerUpCountCall %i", Slingshot__OnPointerUpCountCall);
            }

            if (js.contains("speed")) {
                float sp = js["speed"].template get<float>();
                Slingshot__OnPointerUpSpeed = (int)(sp * 1000.0f);
                LOG_E("Slingshot__OnPointerUpSpeed %i", Slingshot__OnPointerUpSpeed);
            }

            if (!Slingshot__OnPointerUpHooking) {
                void* trampolineSlingshot__OnPointerUp;
                Slingshot__OnPointerUpType firstFunc = (Slingshot__OnPointerUpType)(g_il2CppBase + Slingshot__OnPointerUpOffset);
                A64HookFunction((void*)firstFunc, (void*)&Slingshot__OnPointerUp, &trampolineSlingshot__OnPointerUp );
                Slingshot__OnPointerUpOrigin = (Slingshot__OnPointerUpType) trampolineSlingshot__OnPointerUp;
                Slingshot__OnPointerUpHooking = true;
            }

            // if (!mbNapDanNhanh.isActive()) {
            //     m_unprotectIl2cpp();
            //     mbNapDanNhanh.execute();
            //     m_protectIl2cpp();
            // }
        } else {
            LOG_E("Restore napDanNhanhAction");
            Slingshot__OnPointerUpActive = false;
            // m_unprotectIl2cpp();
            // mbNapDanNhanh.restore();
            // m_protectIl2cpp();
        }
    }

    void init() {
        mbAutoTrungs = std::vector<MemoryPatch> {
            MemoryPatch(g_il2CppBase + 0x223C604, { 0x05, 0x00, 0x00, 0x14 }),
            MemoryPatch(g_il2CppBase + 0x223C64c, { 0x31, 0x00, 0x00, 0x14 })
        };
        mbBanNhanh = MemoryPatch(g_il2CppBase + 0x1D47d70, { 0x61, 0x00, 0x00, 0x14 });
        mbBan1Cham = MemoryPatch(g_il2CppBase + 0x1D4766C, { 0x1F, 0x20, 0x03, 0xD5 });
        mbNapDanNhanh = MemoryPatch(g_il2CppBase + 0x1D47620, { 0x1F, 0x20, 0x03, 0xD5 });

        // wtf = m_getMs();
        // LOG_E("123");
        // void* trampolineBestHTTP_WebSocket_WebSocket__Send;
        // BestHTTP_WebSocket_WebSocket__SendType firstFunc = (BestHTTP_WebSocket_WebSocket__SendType)(g_il2CppBase + BestHTTP_WebSocket_WebSocket__SendOffset);
        // A64HookFunction((void*)firstFunc, (void*)&BestHTTP_WebSocket_WebSocket__Send, &trampolineBestHTTP_WebSocket_WebSocket__Send );
        // BestHTTP_WebSocket_WebSocket__SendOrigin = (BestHTTP_WebSocket_WebSocket__SendType) trampolineBestHTTP_WebSocket_WebSocket__Send;

        // void* trampolineBestHTTP_WebSocket_WebSocket__SendStr;
        // BestHTTP_WebSocket_WebSocket__SendStrType firstFunc2 = (BestHTTP_WebSocket_WebSocket__SendStrType)(g_il2CppBase + BestHTTP_WebSocket_WebSocket__SendStrOffset);
        // A64HookFunction((void*)firstFunc2, (void*)&BestHTTP_WebSocket_WebSocket__SendStr, &trampolineBestHTTP_WebSocket_WebSocket__SendStr );
        // BestHTTP_WebSocket_WebSocket__SendStrOrigin = (BestHTTP_WebSocket_WebSocket__SendStrType) trampolineBestHTTP_WebSocket_WebSocket__SendStr;

        // void* trampolineBestHTTP_WebSocket_WebSocket___ctor;
        // BestHTTP_WebSocket_WebSocket___ctorType firstFunc3 = (BestHTTP_WebSocket_WebSocket___ctorType)(g_il2CppBase + BestHTTP_WebSocket_WebSocket___ctorOffset);
        // A64HookFunction((void*)firstFunc3, (void*)&BestHTTP_WebSocket_WebSocket___ctor, &trampolineBestHTTP_WebSocket_WebSocket___ctor );
        // BestHTTP_WebSocket_WebSocket___ctorOrigin = (BestHTTP_WebSocket_WebSocket___ctorType) trampolineBestHTTP_WebSocket_WebSocket___ctor;
    }
}