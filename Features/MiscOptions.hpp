#pragma once
#include "../PrecompiledHeader.hpp"

namespace Vega::Feature
{
	class MiscOptions
	{
	public:
		void loop();

		bool m_MobileRadio = false;
		bool m_GTAoneCam = false;
		bool m_Noclip = false;
		bool m_DisablePhone = false;
		bool m_DisableHUD = false;

		void applyMobileRadio();
		void applyGTAoneCam();
		void applyNoclip();
		void applyDisablePhone();
		void applyDisableHUD();
	};

	extern MiscOptions g_MiscOptions;
}

