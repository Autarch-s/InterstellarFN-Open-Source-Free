#pragma once
#include <windows.h>
#include "structs.h"
#include "_spoofer_stub.h"

PVOID(*UEEvent)(PVOID, PVOID, PVOID, PVOID) = nullptr;

PVOID UEEventHook(UObject* object, UObject* func, PVOID params, PVOID result) 
{
	if (!object || !func)
		return spoof_call(jmp, UEEvent, (PVOID)object, (PVOID)func, params, result);

	//your pasted exploits go here :D

	return spoof_call(jmp, UEEvent, (PVOID)object, (PVOID)func, params, result);
}