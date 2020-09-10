#pragma once
#include "imports.h"
#include "w2s.h"
#include "object.h"
#include <Windows.h>


Vector3 getboneloc(PVOID mesh, int bone)
{
	if (!mesh) return { 0,0,0 };
	auto fGetBoneMatrix = ((FMatrix * (__fastcall*)(PVOID, FMatrix*, int))(BoneMatrixA));
	fGetBoneMatrix(mesh, myMatrix, bone);
	return Vector3(myMatrix->M[3][0], myMatrix->M[3][1], myMatrix->M[3][2]);
}

void mousemove(float tarx, float tary, float X, float Y, int smooth)
{
	float ScreenCenterX = (X / 2);
	float ScreenCenterY = (Y / 2);
	float TargetX = 0;
	float TargetY = 0;

	if (tarx != 0)
	{
		if (tarx > ScreenCenterX)
		{
			TargetX = -(ScreenCenterX - tarx);
			TargetX /= smooth;
			if (TargetX + ScreenCenterX > ScreenCenterX * 2) TargetX = 0;
		}

		if (tarx < ScreenCenterX)
		{
			TargetX = tarx - ScreenCenterX;
			TargetX /= smooth;
			if (TargetX + ScreenCenterX < 0) TargetX = 0;
		}
	}

	if (tary != 0)
	{
		if (tary > ScreenCenterY)
		{
			TargetY = -(ScreenCenterY - tary);
			TargetY /= smooth;
			if (TargetY + ScreenCenterY > ScreenCenterY * 2) TargetY = 0;
		}

		if (tary < ScreenCenterY)
		{
			TargetY = tary - ScreenCenterY;
			TargetY /= smooth;
			if (TargetY + ScreenCenterY < 0) TargetY = 0;
		}
	}

	iat(mouse_event)(MOUSEEVENTF_MOVE, static_cast<DWORD>(TargetX), static_cast<DWORD>(TargetY), NULL, NULL);

	return;
}

bool cheatinit(ImGuiWindow& window,float X, float Y)	
{
	try
	{
		bool localpawnusable = false;
		float olddistance = FLT_MAX;

		PVOID World = read(Uworld, 0);
		if (!World) return false;

		PVOID instance = read(World, offsets::GameInstance);
		if (!instance) return false;

		PVOID localplayersmanager = read(instance, offsets::LocalPlayers);
		if (!localplayersmanager)return false;

		PVOID localplayer = read(localplayersmanager, 0);
		if (!localplayer) return false;

		PVOID localcontroller = read(localplayer, offsets::PlayerController);
		if (!localcontroller)return false;

		PVOID localpawn = read(localcontroller, offsets::AcknowledgedPawn);
		if (localpawn) localpawnusable = true; else  return false;

		PVOID localplayerstate = 0;

		if (localpawnusable)
			localplayerstate = read(localpawn, offsets::PlayerState);

		if (!hookcamera(read(localcontroller, offsets::PlayerCameraManager))) return false;

		DWORD_PTR persistentlvl = *(DWORD_PTR*)((PBYTE)World + offsets::PersistentLevel);
		if (!persistentlvl) return false;

		DWORD_PTR AActors = *(DWORD_PTR*)(persistentlvl + offsets::AActors);
		if (!AActors) return false;

		int ActorCount = *(int*)(persistentlvl + offsets::ActorCount);
		if (ActorCount == 0) return false;

		std::vector<PVOID> players;

		for (int count = 0; count < ActorCount; count++)
		{
			PVOID CurrentActor = read(AActors, count * 0x8);
			if (!CurrentActor) continue;

			std::string ObjName = gobj::getobjectname((UObject*)(CurrentActor));
			if (ObjName == "") continue;

			if (strstr(ObjName.c_str(), E("PlayerPawn_Athena_C"))) //player pawn
				players.push_back(CurrentActor);
			else if (strstr(ObjName.c_str(), E("BP_PlayerPawn_Athena_Phoebe_C"))) //bot player pawn
				players.push_back(CurrentActor);
			else if (strstr(ObjName.c_str(), E("BP_Gibson"))) //marauders
				players.push_back(CurrentActor);
			else if (strstr(ObjName.c_str(), E("BP_GasketPlayerPawn_Base_C"))) //henchmen
				players.push_back(CurrentActor);
			else if (strstr(ObjName.c_str(), E("BP_GasketPlayerPawn_DateAlpha_C"))) //henchmen
				players.push_back(CurrentActor);
			else if (strstr(ObjName.c_str(), E("BP_GasketPlayerPawn_Date_C"))) //henchmen
				players.push_back(CurrentActor);
			else if (strstr(ObjName.c_str(), E("BP_GasketPlayerPawn_Lonely_C"))) //henchmen
				players.push_back(CurrentActor);
			else if (strstr(ObjName.c_str(), E("BP_GasketPlayerPawn_TomatoAlpha_C"))) //henchmen
				players.push_back(CurrentActor);
			else if (strstr(ObjName.c_str(), E("BP_GasketPlayerPawn_Tomato_C"))) //henchmen
				players.push_back(CurrentActor);
			else if (strstr(ObjName.c_str(), E("BP_GasketPlayerPawn_Wasabi_C"))) //henchmen
				players.push_back(CurrentActor);

		}

		Vector3 aimw2s;
		Vector3 aim;

		for (PVOID enemy : players)
		{
			PVOID PlayerState = read(enemy, offsets::PlayerState);
			if (!PlayerState) continue;

			if (localpawnusable)
			{
				if (!localplayerstate) continue;

				int index = *(int*)((PBYTE)PlayerState + offsets::TeamIndex);
				int currentindex = *(int*)((PBYTE)localplayerstate + offsets::TeamIndex);

				if (index == currentindex) continue;
			}

			PVOID Mesh = read(enemy, offsets::Mesh);
			if (!Mesh) continue;

			Vector3 Head = getboneloc(Mesh, 66);
			Vector3 Heada;
			Vector3 Feet = getboneloc(Mesh, 0);

			if (Head.x == 0 && Feet.y == 0 || Feet.x == 0 && Feet.y == 0) continue;

			Heada = Head;
			Head = w2s(Head, CamRot, X, Y);
			Feet = w2s(Feet, CamRot, X, Y);

			Vector3 HeadBox = w2s(Vector3(getboneloc(Mesh, 66).x, getboneloc(Mesh, 66).y, getboneloc(Mesh, 66).z + 15), CamRot, X, Y);

			if (settings::boxesp)
			{
				float Height = HeadBox.y - Feet.y;

				if (Height < 0)
					Height = Height * (-1.f);

				float Width = Height * 0.65;

				Vector3 temp = HeadBox;

				temp.x = temp.x - (Width / 2);

				Vector3 temp1 = temp;

				temp1.x = temp1.x + (Width);

				window.DrawList->AddRectFilled(ImVec2(temp.x, temp.y), ImVec2(temp1.x, Feet.y), ImGui::GetColorU32({ 0.f, 0.f, 0.f, 0.4f }));
				window.DrawList->AddRect(ImVec2(temp.x, temp.y), ImVec2(temp1.x, Feet.y), ImGui::GetColorU32({ 1.f, 0.f, 0.f, 1.f }), 0, 0, 0.5);
			}

			if (settings::snaplines)
				window.DrawList->AddLine(ImVec2(Head.x, Head.y), ImVec2(X / 2, Y), ImGui::GetColorU32({ 255.f, 255.f, 255.f, 1.f }));

			auto x = Head.x - (X / 2);
			auto y = Head.y - (Y / 2);
			auto distance = iat(sqrtf)(x * x + y * y);

			if (localpawnusable)
				if (distance < settings::fov && distance < olddistance)
				{
					aimw2s = Head;
					aim = Heada;
					olddistance = distance;
				}
		}


		if (iat(GetAsyncKeyState)(VK_RBUTTON) && localpawnusable)
		{
			if (settings::aimtype == 1)
			{
				mousemove(aimw2s.x, aimw2s.y, X, Y, settings::smooth + 5);
			}
			else if (settings::aimtype == 2)
			{
				if (aim.x == 0 && aim.y == 0) return false;

				Vector3 ret;
				Vector3 pos = aim - CamLoc;

				float distance = pos.Length();
				ret.x = -(((float)iat(acosf)(pos.z / distance) * (float)(180.0f / M_PI)) - 90.f);
				ret.y = (float)iat(atan2f)(pos.y, pos.x) * (float)(180.0f / M_PI);

				ret.x = (ret.x - CamRot.x) / settings::smooth + CamRot.x;
				ret.y = (ret.y - CamRot.y) / settings::smooth + CamRot.y;
				ret.z = 0;

				processeventorig(localcontroller, ClientSetRotation, &ret, 0);
			}
		}

	}
	catch(...)
	{
	}

	return true;
}