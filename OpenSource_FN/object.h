#pragma once
#include <windows.h>
#include <string>
#include "xorstr.h"
#include "structs.h"
#include "_spoofer_stub.h"

void Free(__int64 trashtoburn)
{
	auto fr = reinterpret_cast<__int64(__fastcall*)(__int64 a1)>(freeA);
	spoof_call(jmp, fr, trashtoburn);
}

namespace gobj
{
	std::string getobjectfirstname(PVOID object) {
		if (object == NULL)
			return "";
		auto fGetObjName = reinterpret_cast<FString * (__fastcall*)(FString * name, uintptr_t entity)>(GNameA);
		FString result;
		spoof_call(jmp, fGetObjName, &result, (uintptr_t)object);
		if (result.c_str() == NULL)
			return "";
		auto result_str = result.ToString();
		if (result.c_str() != NULL)
			Free((__int64)result.c_str());
		return result_str;
	}


	std::string getobjectname(PVOID Object) {
		if (Object == NULL)
			return E("");
		auto fGetObjName = reinterpret_cast<FString * (__fastcall*)(FString * name, uintptr_t entity)>(GNameA);
		FString result;
		spoof_call(jmp, fGetObjName, &result, (uintptr_t)Object);
		if (result.c_str() == NULL)
			return E("");
		auto result_str = result.ToString();
		if (result.c_str() != NULL)
			Free((__int64)result.c_str());
		return result_str;
	}


	PVOID findobject(std::string name) {
		for (auto array : UObjectArrayA->ObjectArray->Objects) 
		{
			auto fuObject = array;
			for (auto i = 0; i < 0x10000 && fuObject->Object; ++i, ++fuObject) {
				auto object = fuObject->Object;

				if (object->ObjectFlags != 0x41)
					continue;

				if (strstr(getobjectname(object).c_str(), name.c_str()))
					return object;
			}
		}

		return 0;
	}
}