#pragma once
#include "imports.h"
#include "_spoofer_stub.h"

namespace discord
{

    bool crhk(uintptr_t originalPresent, uintptr_t hookFunction, uintptr_t pOriginal)
    {
        static uintptr_t addrCreateHook = NULL;

        if (!addrCreateHook)
            addrCreateHook = sigscan(E("DiscordHook64.dll"), E("40 53 55 56 57 41 54 41 56 41 57 48 83 EC 60"));

        if (!addrCreateHook)
            return false;
        using CreateHook_t = uint64_t(__fastcall*)(LPVOID, LPVOID, LPVOID*);
        auto fnCreateHook = (CreateHook_t)addrCreateHook;

        return spoof_call(jmp, fnCreateHook, (void*)originalPresent, (void*)hookFunction, (void**)pOriginal) == 0 ? true : false;
    }

    bool enhk(uintptr_t pTarget, bool toggle)
    {
        static uintptr_t addrEnableHook = NULL;

        if (!addrEnableHook)
            addrEnableHook = sigscan(E("DiscordHook64.dll"),
                E("48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 41 56 41 57 48 83 EC 20 33 F6 8B FA")
            );


        if (!addrEnableHook)
            return false;

        using EnableHook_t = uint64_t(__fastcall*)(LPVOID, bool);
        auto fnEnableHook = (EnableHook_t)addrEnableHook;

        return spoof_call(jmp, fnEnableHook, (void*)pTarget, toggle) == 0 ? true : false;
    }

    bool enhkq()
    {
        static uintptr_t addrEnableHookQueu = NULL;

        if (!addrEnableHookQueu)
            addrEnableHookQueu = sigscan(E("DiscordHook64.dll"),
                E("48 89 5C 24 ? 48 89 6C 24 ? 48 89 7C 24 ? 41 57"));

        if (!addrEnableHookQueu)
            return false;

        using EnableHookQueu_t = uint64_t(__stdcall*)(VOID);
        auto fnEnableHookQueu = (EnableHookQueu_t)addrEnableHookQueu;

        return spoof_call(jmp, fnEnableHookQueu) == 0 ? true : false;
    }

    short GetAsyncKeyState(const int vKey)
    {
        static uintptr_t addrGetAsyncKeyState = NULL;

        if (!addrGetAsyncKeyState)
            addrGetAsyncKeyState = sigscan(E("DiscordHook64.dll"),
                E("40 53 48 83 EC 20 8B D9 FF 15 ? ? ? ?"));

        if (!addrGetAsyncKeyState)
            return false;

        using GetAsyncKeyState_t = short(__fastcall*)(int);
        auto fnGetAyncKeyState = (GetAsyncKeyState_t)addrGetAsyncKeyState;

        return spoof_call(jmp, fnGetAyncKeyState, vKey);
    }



    bool HookFunction(uintptr_t originalFunction, uintptr_t hookedFunction, uintptr_t pOriginalPresent)
    {
        if (crhk(originalFunction, hookedFunction, pOriginalPresent))
            if (enhk(originalFunction, true))
                if (enhkq())
                    return true;

        return false;
    }
}