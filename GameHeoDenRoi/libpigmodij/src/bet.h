#pragma once

#define ACTION_BET_X AY_OBFUSCATE("bet.vongXoay")

typedef void BetButton_o;


namespace BetPatch {
    //
    uintptr_t BetButton__SetNextMultipleOffset = 0x2CA1840;
    typedef void (*BetButton__SetNextMultipleType) (BetButton_o* __this, const void* method);
    BetButton__SetNextMultipleType BetButton__SetNextMultipleOrigin;
    void BetButton__SetNextMultiple(BetButton_o* __this, const void* method) {
        LOG_E("Next");
        BetButton__SetNextMultipleOrigin(__this, method);
        LOG_E("Next End");
    }

    //
    BetButton_o* betButton_o = nullptr;
    uintptr_t BetButton___ctorOffset = 0x2CA264C;
    typedef void (*BetButton___ctorType)(BetButton_o* __this, const void* method);
    BetButton___ctorType BetButton___ctorOrigin;
    void BetButton___ctor(BetButton_o* __this, const void* method) {
        betButton_o = __this;
        BetButton___ctorOrigin(__this, method);
    }

    //
    bool BetButton__SetCurMultipleActive = false;
    bool BetButton__SetCurMultipleHooking = false;
    int32_t BetButton__SetCurMultipleNumber = 1;
    uintptr_t BetButton__SetCurMultipleOffset = 0x2CA010C;
    typedef void (*BetButton__SetCurMultipleType) (BetButton_o* __this, int32_t curMultiple, const void* method);
    BetButton__SetCurMultipleType BetButton__SetCurMultipleOrigin;
    void BetButton__SetCurMultiple (BetButton_o* __this, int32_t curMultiple, const void* method) {
        if (!BetButton__SetCurMultipleActive) {
            BetButton__SetCurMultipleOrigin(__this, curMultiple, method);
            return;
        }
        LOG_E("$$$$$ CURRENT SET %i", curMultiple);
        curMultiple = BetButton__SetCurMultipleNumber;
        LOG_E("$$$$$ NEW SET %i", curMultiple);
        BetButton__SetCurMultipleOrigin(__this, curMultiple, method);
    }

    void *BetButton__SetNextMultipleThread(void *) {
        void* t = il2cpp_thread_attach(il2cpp_domain_get());
        BetButton__SetNextMultiple(betButton_o, nullptr);
        il2cpp_thread_detach(t);
        pthread_exit(nullptr);
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
            if (betButton_o != nullptr) {
                pthread_t id;
                pthread_create(&id, nullptr, BetButton__SetNextMultipleThread, nullptr);;
            }
        } else {
            LOG_E("Restore betAction");
            BetButton__SetCurMultipleActive = false;
        }
    }

    void init() {
        void* trampolineBetButton___ctor;
        BetButton___ctorType firstBetButton___ctor = (BetButton___ctorType)(g_il2CppBase + BetButton___ctorOffset);
        A64HookFunction((void*)firstBetButton___ctor, (void*)&BetButton___ctor, &trampolineBetButton___ctor );
        BetButton___ctorOrigin = (BetButton___ctorType) trampolineBetButton___ctor;

        void* trampolineBetButton__SetNextMultiple;
        BetButton__SetNextMultipleType firstBetButton__SetNextMultiple = (BetButton__SetNextMultipleType)(g_il2CppBase + BetButton__SetNextMultipleOffset);
        A64HookFunction((void*)firstBetButton__SetNextMultiple, (void*)&BetButton__SetNextMultiple, &trampolineBetButton__SetNextMultiple );
        BetButton__SetNextMultipleOrigin = (BetButton__SetNextMultipleType) trampolineBetButton__SetNextMultiple;
    }
}