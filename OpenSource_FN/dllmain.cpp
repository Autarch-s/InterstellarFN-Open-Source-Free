#define _CRT_SECURE_NO_WARNINGS
#include "imports.h"

void fatalerrormessage(std::string msg)
{
    std::string en = E("Following Signature Could Not Be Found: ") + msg;
    MessageBoxA(0, en.c_str(), E("Fatal Error Occured   |  INTERSTELLAR OPEN SOURCE FREE"), MB_OK | MB_ICONERROR);
    exit(0);
}

void init()
{

    hooking::processeventhooking();

    Uworld = reinterpret_cast<PVOID>(RELATIVE_ADDR(sigscan(E("FortniteClient-Win64-Shipping.exe"), patterns::Uworld), 7));
    if (!Uworld) fatalerrormessage(E("UWORLD"));

    UObjectArrayA = reinterpret_cast<decltype(UObjectArrayA)>(RELATIVE_ADDR(sigscan(E("FortniteClient-Win64-Shipping.exe"), patterns::UObjectArray), 7));
    if (!UObjectArrayA) fatalerrormessage(E("UOBJECTARRAY"));

    GNameA = reinterpret_cast<decltype(GNameA)>(sigscan(E("FortniteClient-Win64-Shipping.exe"), patterns::GNames));
    if (!GNameA) fatalerrormessage(E("GETOBJECTNAME"));

    freeA = reinterpret_cast<PVOID>(sigscan(E("FortniteClient-Win64-Shipping.exe"), patterns::Free));
    if (!freeA) fatalerrormessage(E("FREE"));

    BoneMatrixA = reinterpret_cast<PVOID>(sigscan(E("FortniteClient-Win64-Shipping.exe"), patterns::BoneMatrix) + *(DWORD*)(sigscan(E("FortniteClient-Win64-Shipping.exe"), patterns::BoneMatrix) + 0x1) + 0x5 - 0x100000000);
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
        init();
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

