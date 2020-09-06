#pragma once
#include <windows.h>
#include "structs.h"

PVOID(*processeventorig)(PVOID, PVOID, PVOID, PVOID) = nullptr;

PVOID processevent(UObject* object, UObject* func, PVOID params, PVOID result) 
{
	if (!object || !func)
		processeventorig(object, func, params, result);

	//your pasted exploits go here :D

	return processeventorig(object, func, params, result);
}