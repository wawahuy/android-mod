#pragma once

#define ACTION_CT_AUTO_TRUNG AY_OBFUSCATE("cayThong.autoTrung")
#define ACTION_CT_BAN_NHANH AY_OBFUSCATE("cayThong.banNhanh")
#define ACTION_CT_BAN_1_CHAM AY_OBFUSCATE("cayThong.ban1Cham")
#define ACTION_CT_NAP_DAN_NHANH AY_OBFUSCATE("cayThong.napDanNhanh")


namespace CayThongPatch {
    std::vector<MemoryPatch> mbAutoTrungs;
    MemoryPatch mbBanNhanh;
    MemoryPatch mbBan1Cham;

    //
    bool Slingshot__OnPointerUpActive = false;
    bool Slingshot__OnPointerUpHooking = false;
    uint64_t Slingshot__OnPointerUpTime = 0;
    int Slingshot__OnPointerUpCountCall = 1;
    int Slingshot__OnPointerUpSpeed = 100;
    uintptr_t Slingshot__OnPointerUpOffset = 0x2EE16B0;
    typedef void (*Slingshot__OnPointerUpType)(void* __this, void* eventData, const void* method);
    void (*Slingshot__OnPointerUpOrigin)(void* __this, void* eventData, const void* method);
    void Slingshot__OnPointerUp(void* __this, void* eventData, const void* method) {
        LOG_E("Slingshot__OnPointerUp hook call");
        if (!Slingshot__OnPointerUpActive) {
            Slingshot__OnPointerUpOrigin(__this, eventData, method);
            return;
        }
        if (m_getMs() - Slingshot__OnPointerUpTime > Slingshot__OnPointerUpSpeed) {
            LOG_E("Slingshot__OnPointerUp hook wait");
            Slingshot__OnPointerUpTime = m_getMs();
            for (int i = 0; i < Slingshot__OnPointerUpCountCall; i++) {
                // ----------------------
                // 0x133
                // ----------------------
                uintptr_t s1 = (uintptr_t)__this + 0x133;
                auto s2 = (char *)s1;
                *s2 = 0xb3;
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
        } else {
            LOG_E("Restore napDanNhanhAction");
            Slingshot__OnPointerUpActive = false;
        }
    }

    void init() {
        // PineTree$$CalculateHitResult
        mbAutoTrungs = std::vector<MemoryPatch> {
            // do {
            //   if ((int)*(uint *)(...     <------ TARGET
            // -------------------------------------------
            // | B ??
            // -------------------------------------------
            MemoryPatch(g_il2CppBase + 0x2F9796C, { 0x05, 0x00, 0x00, 0x14 }),
            
            // if ((uVar2 & 1) != 0) {     <------- TARGET
            //   if (((*(long *)(param_4 + 0x38...
            // -------------------------------------------
            // | B ??
            // -------------------------------------------
            MemoryPatch(g_il2CppBase + 0x2F979B4, { 0x31, 0x00, 0x00, 0x14 })
        };

        // Slingshot$$Shot
        // uVar7 = DG.Tweening.DOTween$$Sequence(0);
        // ...  <-------------- TARGET (0x1D47d70)
        //          |
        // uVar7 = DG.Tweening.TweenSettingsExtensions$$AppendInterval(0x3d75c28f,uVar7,0); (0x1D47ef4)
        // uVar8 = thunk_FUN_011e88b8(DG.Tweening.TweenCallback_TypeInfo);
        // -----------------------------------------------
        // | 0x1D47ef4 - 0x1D47d70 = 184 -> B 61
        // -----------------------------------------------
        mbBanNhanh = MemoryPatch(g_il2CppBase + 0x2ee1e60, { 0x61, 0x00, 0x00, 0x14 });
        
        // Slingshot$$OnPointerUp
        // uVar1 = Slingshot$$CheckShotOrNot(fVar3,param_1);
        // if ((uVar1 & 1) != 0)            <------ TARGET
        // -----------------------------------------------
        // | NOP
        // -----------------------------------------------
        mbBan1Cham = MemoryPatch(g_il2CppBase + 0x2ee175c, { 0x1F, 0x20, 0x03, 0xD5 });
    }
}