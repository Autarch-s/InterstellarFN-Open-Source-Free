#pragma once
#include <windows.h>
#include "structs.h"

PVOID(*UEEvent)(PVOID, PVOID, PVOID, PVOID) = nullptr;

PVOID UEEventHook(UObject* object, UObject* func, PVOID params, PVOID result) 
{
	if (!object || !func)
		UEEvent(object, func, params, result);

	//your pasted exploits go here :D

	return UEEvent(object, func, params, result);
}