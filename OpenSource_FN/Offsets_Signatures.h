#pragma once

namespace offsets
{
	DWORD GameInstance = 0x180;
	DWORD LocalPlayers = 0x38;
	DWORD PlayerController = 0x30;
	DWORD AcknowledgedPawn = 0x2A0;
	DWORD PersistentLevel = 0x30;
	DWORD AActors = 0x98;
	DWORD ActorCount = 0xA0;
	DWORD PlayerCameraManager = 0x2B8;
	DWORD Mesh = 0x280;
	DWORD PlayerState = 0x240;
	DWORD TeamIndex = 0xE88;
}

namespace patterns
{
	CONST CHAR* Uworld = "48 8B 1D ? ? ? ? 48 85 DB 74 3B 41";
	CONST CHAR* UObjectArray = "48 8B 05 ? ? ? ? 4C 8D 34 CD ? ? ? ?";
	CONST CHAR* GNames = "40 53 48 83 EC 20 48 8B D9 48 85 D2 75 45 33 C0 48 89 01 48 89 41 08 8D 50 05 E8 ? ? ? ? 8B 53 08 8D 42 05 89 43 08 3B 43 0C 7E 08 48 8B CB E8 ? ? ? ? 48 8B 0B 48 8D 15 ? ? ? ? 41 B8 ? ? ? ? E8 ? ? ? ? 48 8B C3 48 83 C4 20 5B C3 48 8B 42 18";
	CONST CHAR* Free = "48 85 C9 74 2E 53 48 83 EC 20 48 8B D9 48 8B 0D ? ? ? ? 48 85 C9 75 0C";
	CONST CHAR* ProcessEvent = "40 55 56 57 41 54 41 55 41 56 41 57 48 81 EC ? ? ? ? 48 8D 6C 24 ? 48 89 9D ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C5 48 89 85 ? ? ? ? 8B 41 0C 45 33 F6 3B 05 ? ? ? ? 4D 8B F8 48 8B F2 4C 8B E1 41 B8 ? ? ? ? 7D 2A";
	CONST CHAR* BoneMatrix = "E8 ? ? ? ? 48 8B 47 30 F3 0F 10 45";
}