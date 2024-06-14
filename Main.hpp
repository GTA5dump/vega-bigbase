#pragma once
#include "PrecompiledHeader.hpp"

namespace Vega
{
	inline HMODULE g_hModule = nullptr;
	inline bool g_ShouldExit = false;
	using namespace std::chrono_literals;
}
using namespace Vega;
BOOL DllMain(HINSTANCE hInstance, DWORD reason, LPVOID);