#pragma once

#pragma region winapi
#include <Windows.h>
#include <cstdint>
#include <Psapi.h>
#include <immintrin.h>
#include <cstddef>
#include <utility>
#include <vector>

#pragma comment(lib, "Psapi.lib")
#pragma endregion winapi

#pragma region headers
#include "hooking.h"
#include "lazyimporter.h"
#include "scanner.h"
#include "Offsets_Signatures.h"
#include "Discord.h"
#include "xorstr.h"
#include "object.h"
#pragma endregion headers

#define RELATIVE_ADDR(addr, size) ((PBYTE)((UINT_PTR)(addr) + *(PINT)((UINT_PTR)(addr) + ((size) - sizeof(INT))) + (size)))