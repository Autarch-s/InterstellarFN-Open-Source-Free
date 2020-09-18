#define _CRT_SECURE_NO_WARNINGS
#include "imports.h"
#include "_spoofer_stub.h"
#include "detours.h"

void fatalerrormessage(std::string msg)
{
    std::string en = E("Following Signature Could Not Be Found: ") + msg;
    MessageBoxA(0, en.c_str(), E("Fatal Error Occured   |  INTERSTELLAR OPEN SOURCE FREE"), MB_OK | MB_ICONERROR);
    spoof_call(jmp, exit, 0);
}

#pragma region trackingfilesblocker

//real winapi function
HANDLE(WINAPI* Real_CreateFileW) (
    LPCWSTR lpFileName,
    DWORD dwDesiredAccess,
    DWORD dwShareMode,
    LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    DWORD dwCreationDisposition,
    DWORD dwFlagsAndAttributes,
    HANDLE hTemplateFile) = CreateFileW;

//real winapi function
BOOL(WINAPI* Real_CreateDirectoryW) (
    LPCWSTR lpPathName,
    LPSECURITY_ATTRIBUTES lpSecurityAttributes) = CreateDirectoryW;


//our own hooked function
HANDLE WINAPI _CreateFileW(LPCWSTR lpFileName,
    DWORD dwDesiredAccess,
    DWORD dwShareMode,
    LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    DWORD dwCreationDisposition,
    DWORD dwFlagsAndAttributes,
    HANDLE hTemplateFile)
{
    //ignore pak, sig files and anything to fortnite and pipes
    if (wcsstr(lpFileName, E(L".pak")) || wcsstr(lpFileName, E(L".sig")) || wcsstr(lpFileName, E(L"Fortnite")) || wcsstr(lpFileName, E(L"\\.\\")))
        return Real_CreateFileW(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
    else
        return Real_CreateFileW(E(L"C:\\Windows\\a"), dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
}

//our own hooked function
BOOL WINAPI _CreateDirectoryW(LPCWSTR lpPathName,
    LPSECURITY_ATTRIBUTES lpSecurityAttributes)
{
    //ignore fortnite folder creation
    if (wcsstr(lpPathName, E(L"Fortnite")))
        return Real_CreateDirectoryW(lpPathName, lpSecurityAttributes);
    else
        return Real_CreateDirectoryW(E(L"C:\\Windows\\a"), lpSecurityAttributes);
}

void trackingfilesblockinit()
{
    //using detours because i cant be arsed at this point because people will paste this anyway :rolling_eyes:

    //initializing detours
    DetourTransactionBegin();

    //attaching to winapi functions
    DetourAttach(&(PVOID&)Real_CreateFileW, _CreateFileW);
    DetourAttach(&(PVOID&)Real_CreateDirectoryW, _CreateDirectoryW);

    //start the hooking process
    LONG Error = DetourTransactionCommit();
    if (Error != NO_ERROR)
        MessageBoxA(0, E("A Fatal Exception Occured! Information Below-\n[-] Trace Blocker Hook Failed\n\nTrace Blocker Will Be Disabled! Use At Your Own RISK!"), E("Interstellar Open Source"), MB_OK | MB_ICONERROR);
}

#pragma endregion trackingfilesblocker

void init()
{

#pragma region patterns

    //Had to shift patterns here because xor was causing issues...

    CONST CHAR* s_Uworld = E("48 8B 1D ? ? ? ? 48 85 DB 74 3B 41");
    CONST CHAR* s_UObjectArray = E("48 8B 05 ? ? ? ? 4C 8D 34 CD ? ? ? ?");
    CONST CHAR* s_GNames = E("40 53 48 83 EC 20 48 8B D9 48 85 D2 75 45 33 C0 48 89 01 48 89 41 08 8D 50 05 E8 ? ? ? ? 8B 53 08 8D 42 05 89 43 08 3B 43 0C 7E 08 48 8B CB E8 ? ? ? ? 48 8B 0B 48 8D 15 ? ? ? ? 41 B8 ? ? ? ? E8 ? ? ? ? 48 8B C3 48 83 C4 20 5B C3 48 8B 42 18");
    CONST CHAR* s_Free = E("48 85 C9 74 2E 53 48 83 EC 20 48 8B D9 48 8B 0D ? ? ? ? 48 85 C9 75 0C");
    CONST CHAR* s_BoneMatrix = E("E8 ? ? ? ? 48 8B 47 30 F3 0F 10 45");

#pragma endregion patterns


    jmp = hookjmp(E(L"DiscordHook64.dll"));

    hooking::processeventhooking();

    Uworld = reinterpret_cast<PVOID>(RELATIVE_ADDR(sigscan(E("FortniteClient-Win64-Shipping.exe"), s_Uworld), 7));
    if (!Uworld) fatalerrormessage(E("UWORLD"));

    UObjectArrayA = reinterpret_cast<decltype(UObjectArrayA)>(RELATIVE_ADDR(sigscan(E("FortniteClient-Win64-Shipping.exe"), s_UObjectArray), 7));
    if (!UObjectArrayA) fatalerrormessage(E("UOBJECTARRAY"));

    GNameA = reinterpret_cast<decltype(GNameA)>(sigscan(E("FortniteClient-Win64-Shipping.exe"), s_GNames));
    if (!GNameA) fatalerrormessage(E("GETOBJECTNAME"));

    freeA = reinterpret_cast<PVOID>(sigscan(E("FortniteClient-Win64-Shipping.exe"), s_Free));
    if (!freeA) fatalerrormessage(E("FREE"));

    BoneMatrixA = reinterpret_cast<PVOID>(sigscan(E("FortniteClient-Win64-Shipping.exe"), s_BoneMatrix) + *(DWORD*)(sigscan(E("FortniteClient-Win64-Shipping.exe"), s_BoneMatrix) + 0x1) + 0x5 - 0x100000000);
    if (!BoneMatrixA) fatalerrormessage(E("GETBONEMATRIX"));

    ClientSetRotation = gobj::findobject(E("ClientSetRotation"));

    hooking::presenthooking();
}


BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        iat(DisableThreadLibraryCalls)(hModule);
        trackingfilesblockinit();
        init();
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

