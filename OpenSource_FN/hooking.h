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
		auto processeventsig = sigscan(E("FortniteClient-Win64-Shipping.exe"), patterns::ProcessEvent);
		if (processeventsig)
			if (discord::HookFunction(processeventsig, (uintptr_t)processevent, (uintptr_t)&processeventorig))
				return true;
		
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
		if(presentaddy)
			if(discord::HookFunction(presentaddy, (uintptr_t)presenthook, (uintptr_t)&PresentOriginal))
				return true;

		return false;
	}
}
