#pragma once
#include "../PrecompiledHeader.hpp"

namespace Vega::Feature
{
	class PlayerOptions
	{
	public:
		void loop();

		bool m_GodMode = false;
		bool m_SemiGodMode = false;
		bool m_NeverWanted = false;
		bool m_SuperJump = false;
		bool m_SuperRun = false;
		bool m_Invisible = false;
		bool m_OffRadar = false;
		bool m_RevealPlayers = false;
		bool m_CopsTurnBlindEye = false;
		std::int32_t m_Alpha = 100;
		std::int32_t m_WantedLevel = 0;
		float m_WidthAmount = 0.5f;
		bool m_WidthToggle = false;
		float m_HeightAmount = 0.5f;
		bool m_HeightToggle = false;
		bool m_RagdollG = false;
		
		void applyGodMode();
		void applySemiGodMode();
		void applyNeverWanted();
		void applyWantedLevel();
		void setAlpha(std::int32_t alpha);
		void applyOffRadar();
		void applyRevealPlayers();
		void applyCopsTurnBlindEye();
		void applyInvisibility(bool toggle);
		void applySuperRun();
		void applyFrameFlag(std::uint32_t flag);
		void applyWidth();
		void applyHeight();
		void applyRagdollG();
	};

	extern PlayerOptions g_PlayerOptions;
}

