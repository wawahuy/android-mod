#pragma once

#define ACTION_CT_AUTO_TRUNG AY_OBFUSCATE("cayThong.autoTrung")
#define ACTION_CT_BAN_NHANH AY_OBFUSCATE("cayThong.banNhanh")
#define ACTION_CT_BAN_1_CHAM AY_OBFUSCATE("cayThong.ban1Cham")
#define ACTION_CT_NAP_DAN_NHANH AY_OBFUSCATE("cayThong.napDanNhanh")
#define ACTION_BET_1000 AY_OBFUSCATE("bet.bet1000")
#define ACTION_BET_500 AY_OBFUSCATE("bet.bet500")
#define ACTION_BET_200 AY_OBFUSCATE("bet.bet200")


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
        LOG_E("test %p", (char)((uintptr_t)__this + 0x133));
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
                *s2 = 0xb3;
                Slingshot__OnPointerUpOrigin(__this, eventData, method);
            }
        }
    }

    //
    bool BetButton__SetCurMultipleActive = false;
    bool BetButton__SetCurMultipleHooking = false;
    int32_t BetButton__SetCurMultipleNumber = 1;
    uintptr_t BetButton__SetCurMultipleOffset = 0x1A33F9C;
    typedef void (*BetButton__SetCurMultipleType) (void* __this, int32_t curMultiple, const void* method);
    BetButton__SetCurMultipleType BetButton__SetCurMultipleOrigin;
    void BetButton__SetCurMultiple (void* __this, int32_t curMultiple, const void* method) {
        if (!BetButton__SetCurMultipleActive) {
            BetButton__SetCurMultipleOrigin(__this, curMultiple, method);
            return;
        }
        LOG_E("$$$$$ CURRENT SET %i", curMultiple);
        curMultiple = BetButton__SetCurMultipleNumber;
        LOG_E("$$$$$ NEW SET %i", curMultiple);
        BetButton__SetCurMultipleOrigin(__this, curMultiple, method);
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

    void betAction(const nlohmann::json& js) {
        bool isActive = js[JS_THIS];
        if (isActive) {
            LOG_E("Active betAction");
            BetButton__SetCurMultipleActive = true;
            if (js.contains("count")) {
                BetButton__SetCurMultipleNumber = js["count"];
                LOG_E("BetButton__SetCurMultipleNumber %i", BetButton__SetCurMultipleNumber);
            }
            if (!BetButton__SetCurMultipleHooking) {
                void* trampolineBetButton__SetCurMultiple;
                BetButton__SetCurMultipleType firstBetButton__SetCurMultiple = (BetButton__SetCurMultipleType)(g_il2CppBase + BetButton__SetCurMultipleOffset);
                A64HookFunction((void*)firstBetButton__SetCurMultiple, (void*)&BetButton__SetCurMultiple, &trampolineBetButton__SetCurMultiple );
                BetButton__SetCurMultipleOrigin = (BetButton__SetCurMultipleType) trampolineBetButton__SetCurMultiple;
                BetButton__SetCurMultipleHooking = true;
            }
        } else {
            LOG_E("Restore betAction");
            BetButton__SetCurMultipleActive = false;
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
    }
}