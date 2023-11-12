#include <unistd.h>
#include "inc.h"
#include "And64InlineHook.hpp"


uint64_t (*getMs)();

uintptr_t Slingshot__OnPointerUpOffset = 0x1D475C0;
uint64_t timeA = 0;
typedef void (*Slingshot__OnPointerUpType)(void* __this, void* eventData, const void* method);
void (*Slingshot__OnPointerUpOrigin)(void* __this, void* eventData, const void* method);
void Slingshot__OnPointerUp(void* __this, void* eventData, const void* method) {
   if (getMs() - timeA > 100) {
       timeA = getMs();
       Slingshot__OnPointerUpOrigin(__this, eventData, method);
   }
}

extern "C" {
    __attribute__((visibility("default"))) void initMethod(uintptr_t getMsPtr) {
        getMs = (uint64_t (*)())getMsPtr;
    }

    __attribute__((visibility("default"))) void init(uintptr_t il2CppBase) {
        LOG_E("==================== OKE ===================");
        LOG_E("==================== OKE ===================");
        void* trampolineSlingshot__OnPointerUp;
        Slingshot__OnPointerUpType firstFunc = (Slingshot__OnPointerUpType)(il2CppBase + Slingshot__OnPointerUpOffset);
        A64HookFunction((void*)firstFunc, (void*)&Slingshot__OnPointerUp, &trampolineSlingshot__OnPointerUp );
        Slingshot__OnPointerUpOrigin = (Slingshot__OnPointerUpType) trampolineSlingshot__OnPointerUp;
    }
}
