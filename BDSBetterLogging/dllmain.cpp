#include <Windows.h>
#include <MinHook.h>

#include <Psapi.h>
#include <iostream>

#include "mem.h"

typedef void(__fastcall* WriteToLogHook)(uintptr_t, uintptr_t, uintptr_t, const char*);
WriteToLogHook __o__log;

void LogDetour(uintptr_t instance, uintptr_t logarea, uintptr_t loglevel, const char* str) {
    switch (loglevel) {
    case 1:
        std::cout << str;
        break;
    case 2:
        std::cout << "[BDSBL]: Warning: " << str;
        break;
    case 3:
        std::cout << "[BDSBL]: Error: " << str;
        break;
    }
    return __o__log(instance, logarea, loglevel, str);
}

void Initialize(HMODULE module) {
    SetConsoleTitleA("BDSBL by yeemi<3#9764");

    if (MH_Initialize() == MH_OK) {
        uintptr_t contentLog_LogAddr = MCM::findSig("40 55 53 56 57 41 54 41 55 41 56 41 57 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 4D 8B E1 45 8B F8 8B F2");
        if (MH_CreateHook((void*)contentLog_LogAddr, &LogDetour, reinterpret_cast<LPVOID*>(&__o__log)) == MH_OK) {
            MH_EnableHook((void*)contentLog_LogAddr);
        }
    }
}

BOOL APIENTRY DllMain(HMODULE a1, DWORD a2, LPVOID a3)
{
    if (a2 == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(a1);
        CreateThread(0, 0, (LPTHREAD_START_ROUTINE)Initialize, a1, 0, 0);
    }
    return TRUE;
}