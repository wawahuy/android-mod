#pragma once

#define ACTION_BET_X AY_OBFUSCATE("bet.vongXoay")

namespace BetPatch {
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

    void betXAction(const nlohmann::json& js) {
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

    }
}