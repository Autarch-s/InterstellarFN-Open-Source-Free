#pragma once
#include "xorstr.h"
#include "scanner.h"
#include "Discord.h"
#include "Offsets_Signatures.h"
#include "processevent.h"
#include "present.h"
#include "w2s.h"

namespace hooking
{
	bool processeventhooking()
	{
#pragma region patterns

		//had to shift because xor was causing issues...
		CONST CHAR* s_ProcessEvent = E("40 55 56 57 41 54 41 55 41 56 41 57 48 81 EC ? ? ? ? 48 8D 6C 24 ? 48 89 9D ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C5 48 89 85 ? ? ? ? 8B 41 0C 45 33 F6 3B 05 ? ? ? ? 4D 8B F8 48 8B F2 4C 8B E1 41 B8 ? ? ? ? 7D 2A");

#pragma endregion patterns


		auto processeventsig = sigscan(E("FortniteClient-Win64-Shipping.exe"), s_ProcessEvent);
		if (processeventsig)
			if (discord::HookFunction(processeventsig, (uintptr_t)UEEventHook, (uintptr_t)&UEEvent))
				return true;
		else
		return false;
	}

	bool presenthooking()
	{
		uintptr_t pcall_present_discord = sigscan(E("DiscordHook64.dll"), E("FF 15 ? ? ? ? 8B D8 E8 ? ? ? ? E8 ? ? ? ? EB 10"));

		if (!pcall_present_discord)
			return false;
		const auto poriginal_present = reinterpret_cast<f_present*>(pcall_present_discord + *reinterpret_cast<int32_t*>(pcall_present_discord + 0x2) + 0x6);
		
		if (!*poriginal_present)
			return false;

		swapchainA = *poriginal_present;
		*poriginal_present = presenthook;

		const auto presentaddy = sigscan(E("DiscordHook64.dll"), E("48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC 20 48 8B D9 41 8B F8"));
		discord::HookFunction(presentaddy, (uintptr_t)presenthook, (uintptr_t)&PresentOriginal);

		return false;
	}
}