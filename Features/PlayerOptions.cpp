#include "../PrecompiledHeader.hpp"
#include "PlayerOptions.hpp"

#include "../Reconstructed Classes/CPedFactory.hpp"
#include "../Reconstructed Classes/CPed.hpp"
#include "../Reconstructed Classes/CPlayerInfo.hpp"
#include "../Natives.hpp"
#include "../Pointers.hpp"
#include "../ScriptGlobal.hpp"
#include "../Util.hpp"

namespace Vega::Feature
{
	PlayerOptions g_PlayerOptions;

	void PlayerOptions::loop()
	{
		if (m_GodMode)
		{
			applyGodMode();
		}

		if (m_SemiGodMode)
		{
			applySemiGodMode();
		}

		if (m_NeverWanted)
		{
			applyNeverWanted();
		}

		if (m_SuperRun)
		{
			applySuperRun();
		}

		if (m_SuperJump)
		{
			applyFrameFlag(Enums::FrameFlagSuperJump);
		}

		if (m_OffRadar)
		{
			//applyOffRadar();
		}

		if (m_RevealPlayers)
		{
			//applyRevealPlayers();
		}

		if (m_CopsTurnBlindEye)
		{
			//applyCopsTurnBlindEye();
		}

		if (m_WidthToggle)
		{
			//applyWidth();
		}

		if (m_HeightToggle)
		{
			//applyHeight();
		}

		if (m_RagdollG)
		{
			applyRagdollG();
		}
	}

	void PlayerOptions::applyGodMode()
	{
		auto ped = PLAYER::PLAYER_PED_ID();
		PLAYER::SET_PLAYER_INVINCIBLE(ped, m_GodMode);
	}

	void PlayerOptions::applySemiGodMode()
	{
		if (auto ped = Util::getLocalPlayerPed())
		{
			ped->m_health = ped->m_maxhealth;
		}
	}

	void PlayerOptions::applyNeverWanted()
	{
		PLAYER::SET_PLAYER_WANTED_LEVEL(PLAYER::PLAYER_PED_ID(), 0, false);
		PLAYER::SET_PLAYER_WANTED_LEVEL_NOW(PLAYER::PLAYER_PED_ID(), true);
	}

	void PlayerOptions::applyWantedLevel()
	{
		PLAYER::SET_PLAYER_WANTED_LEVEL(PLAYER::PLAYER_PED_ID(), m_WantedLevel, false);
	}

	void PlayerOptions::applyInvisibility(bool toggle)
	{
		ENTITY::SET_ENTITY_VISIBLE(PLAYER::PLAYER_PED_ID(), !toggle, false);
	}

	void PlayerOptions::applySuperRun()
	{
		if (Util::isGameFocused() && Util::isKeyPressed(VK_SHIFT))
		{
			auto pedHandle = Util::getLocalPedHandle();
			if (!PED::IS_PED_RUNNING_RAGDOLL_TASK(pedHandle) && !PED::IS_PED_RAGDOLL(pedHandle))
			{
				ENTITY::APPLY_FORCE_TO_ENTITY(pedHandle, 1, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 0, true, true, true, false, true);
			}
		}
	}
	
	void PlayerOptions::setAlpha(std::int32_t alpha)
	{
		ENTITY::SET_ENTITY_ALPHA(PLAYER::PLAYER_PED_ID(), static_cast<std::int32_t>(2.55f * static_cast<float>(alpha)), 0);
	}

	void PlayerOptions::applyOffRadar()
	{
		if (*g_Pointers->m_IsSessionStarted)
		{
			auto time = g_Pointers->call(g_Pointers->m_GetNetworkTime);
			ScriptGlobal(2423644)(Util::getLocalPlayerId(), 406)[196].as<bool&>() = m_OffRadar;
			ScriptGlobal(2436641)[70].as<std::int32_t&>() = m_OffRadar ? time + 757000 : time - 60000;
		}
	}

	void PlayerOptions::applyRevealPlayers()
	{
		if (*g_Pointers->m_IsSessionStarted)
		{
			auto time = g_Pointers->call(g_Pointers->m_GetNetworkTime);
			ScriptGlobal(2423644)(Util::getLocalPlayerId(), 406)[199].as<bool&>() = m_RevealPlayers;
			ScriptGlobal(2436641)[71].as<std::int32_t&>() = m_RevealPlayers ? time + 757000 : time - 60000;
		}
	}

	void PlayerOptions::applyCopsTurnBlindEye()
	{
		auto networkTime = g_Pointers->call(g_Pointers->m_GetNetworkTime);

		decltype(auto) base = 2519572_Global;
		decltype(auto) time = base[4544].as<std::uint32_t&>();
		decltype(auto) bitset = base[4541].as<std::uint32_t&>();

		if (m_CopsTurnBlindEye)
		{
			bitset |= 1 << 0;

			m_WantedLevel = 0;
			applyWantedLevel();

			time = networkTime + 757000 - 120000;
		}
		else
		{
			bitset = 0;
			time = networkTime - 60000;
		}
	}

	void PlayerOptions::applyFrameFlag(const std::uint32_t flag)
	{
		auto playerInfo = Util::getLocalPlayerInfo();
		if (playerInfo)
		{
			playerInfo->m_frame_flags |= flag;
		}
	}

	void PlayerOptions::applyWidth()
	{
		auto ped = Util::getLocalPlayerPed();
		if (ped)
		{
			//ped->matrix.floats[1] = m_WidthAmount;
		}
	}

	void PlayerOptions::applyHeight()
	{
		auto ped = Util::getLocalPlayerPed();
		if (ped)
		{
			//ped->matrix.floats[10] = m_HeightAmount;
		}
	}

	void PlayerOptions::applyRagdollG()
	{
		if (Util::isGameFocused() && Util::isKeyPressed(VK_KEY_G))
		{
			auto pedHandle = Util::getLocalPedHandle();
			PED::SET_PED_TO_RAGDOLL(pedHandle, 2000, 2000, 0, false, false, false);
		}
	}
}
