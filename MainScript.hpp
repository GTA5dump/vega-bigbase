#pragma once
#include "PrecompiledHeader.hpp"
#include "User Interface/Manager.hpp"

namespace Vega {
	inline std::int32_t g_SelectedPlayer = 0;
	
	class MainScript {
	public:
		MainScript() = default;
		~MainScript() = default;
	public:
		void Init();
	public:
		static void Tick();
	};
	inline MainScript g_MainScript;
}