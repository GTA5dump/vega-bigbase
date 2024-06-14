#pragma once
#include "PrecompiledHeader.hpp"
#include "Pointers.hpp"
#include "ScriptMgr.hpp"
#include "Logger.hpp"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui_impl_win32.h"

namespace Vega {
	class LogScript {
	public:
		LogScript() = default;
		~LogScript() = default;
	public:
		void OnPresentTick();
	public:
		static void Tick();
	public:
		bool m_LockMouse = false;
	};
	inline LogScript g_LogScript;
}