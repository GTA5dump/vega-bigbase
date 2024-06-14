#include "PrecompiledHeader.hpp"
#include "Reconstructed Classes/CNetworkPlayerMgr.hpp"
#include "Reconstructed Classes/CNetGamePlayer.hpp"
#include "Reconstructed Classes/CPlayerInfo.hpp"
#include "Reconstructed Classes/CPedFactory.hpp"
#include "Reconstructed Classes/CPed.hpp"
#include "Reconstructed Classes/CNavigation.hpp"
#include "Reconstructed Classes/CModelInfo.hpp"
#include "Util.hpp"
#include "Pointers.hpp"

namespace Vega
{
	bool Util::isGameFocused(void)
	{
		return GetForegroundWindow() == g_Pointers->m_Window;
	}

	std::uint32_t Util::joaat(const char* str)
	{
		std::uint32_t hash = 0;
		while (*str)
		{
			hash += constexprToLower(*str++);
			hash += (hash << 10);
			hash ^= (hash >> 6);
		}

		hash += (hash << 3);
		hash ^= (hash >> 11);
		hash += (hash << 15);

		return hash;
	}

	bool Util::isKeyPressed(const std::uint16_t vk)
	{
		return isGameFocused() && static_cast<bool>(GetAsyncKeyState(vk) & (1 << 15));
	}

	std::int32_t Util::ptrToHandle(void* const ptr)
	{
		return g_Pointers->call(g_Pointers->m_PointerToHandle, ptr);
	}

	std::uintptr_t Util::handleToPtrNoCast(const std::int32_t handle)
	{
		if (auto entPool = *g_Pointers->m_EntityPool)
		{
			return entPool->handleToPtr(handle);
		}

		return 0;
	}

	void showCallstack()
	{
		//	StackWalkerGud kek;
		//	kek.ShowCallstack();
	}

	void Util::tryAndLog(void(*func)(void))
	{
		__try
		{
			func();
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
#	if SCRIPT_EVENT_STACKTRACE
			//showCallstack();
#	endif
		}
	}

	void Util::simpleNotify(const std::uint32_t color, const char* format, ...)
	{
		std::va_list args;

		va_start(args, format);
		auto len = std::vsnprintf(nullptr, NULL, format, args) + 1;
		auto buf = std::make_unique<char[]>(len);
		std::memset(buf.get(), 0, len);
		std::vsnprintf(buf.get(), len, format, args);
		va_end(args);

		Natives::HUD::_SET_NOTIFICATION_BACKGROUND_COLOR(color);
		Natives::HUD::_SET_NOTIFICATION_TEXT_ENTRY(XorString("STRING"));
		Natives::HUD::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(buf.get());
		Natives::HUD::_DRAW_NOTIFICATION(false, false);
	}

	ReconstructedClasses::CPlayerInfo* Util::getPlayerInfo(const std::int32_t playerId)
	{
		if (*g_Pointers->m_IsSessionStarted)
		{
			if (auto netPlayerMgr = *g_Pointers->m_NetworkPlayerMgr)
			{
				if (auto netPlayer = netPlayerMgr->m_player_list[playerId])
				{
					return netPlayer->playerInfo;
				}
			}
		}
		else if (playerId == 0)
		{
			return getLocalPlayerInfo();
		}

		return nullptr;
	}

	ReconstructedClasses::CPed* Util::getPlayerPed(const std::int32_t playerId)
	{
		if (*g_Pointers->m_IsSessionStarted)
		{
			if (auto netPlayerMgr = *g_Pointers->m_NetworkPlayerMgr)
			{
				if (auto netPlayer = netPlayerMgr->m_player_list[playerId])
				{
					if (auto playerInfo = netPlayer->playerInfo)
					{
						return playerInfo->m_ped;
					}
				}
			}
		}
		else if (playerId == 0)
		{
			return getLocalPlayerPed();
		}

		return nullptr;
	}

	ReconstructedClasses::CPlayerInfo* Util::getLocalPlayerInfo(void)
	{
		if (auto ped = getLocalPlayerPed())
		{
			return ped->m_player_info;
		}

		return nullptr;
	}

	ReconstructedClasses::CPed* Util::getLocalPlayerPed(void)
	{
		if (auto pedFactory = *g_Pointers->m_PedFactory)
		{
			return pedFactory->localPed;
		}

		return nullptr;
	}

	ReconstructedClasses::CNetGamePlayer* Util::getLocalNetPlayer(void)
	{
		if (*g_Pointers->m_IsSessionStarted)
		{
			if (auto netPlayerMgr = *g_Pointers->m_NetworkPlayerMgr)
			{
				return netPlayerMgr->m_local_net_player;
			}
		}

		return nullptr;
	}

	std::int32_t Util::getLocalPedHandle(void)
	{
		if (auto ped = PLAYER::PLAYER_PED_ID()) {
			return ped;
		}

		return -1;
	}

	std::int32_t Util::getLocalPlayerId(void)
	{
		if (*g_Pointers->m_IsSessionStarted)
		{
			if (auto netPlayerMgr = *g_Pointers->m_NetworkPlayerMgr)
			{
				if (auto localPlayer = netPlayerMgr->m_local_net_player)
				{
					return localPlayer->playerId;
				}
			}
		}

		return 0;
	}

	ReconstructedClasses::CAutomobile* Util::getLocalVehicle()
	{
		if (auto ped = Util::getLocalPlayerPed())
		{
			if (Util::isPedInVehicle(ped))
			{
				return ped->m_vehicle;
			}
		}

		return nullptr;
	}

	bool Util::isPedShooting(ReconstructedClasses::CPed* const ped)
	{
		if (ped)
		{
			return Natives::PED::IS_PED_SHOOTING(Natives::PLAYER::PLAYER_PED_ID());
		}

		return false;
	}

	bool Util::isPlayerFreeAiming(ReconstructedClasses::CPlayerInfo* const playerInfo)
	{

		return false;
	}

	const char* Util::getPlayerName(const std::int32_t playerId)
	{
		if (auto playerInfo = getPlayerInfo(playerId))
		{
			return playerInfo->m_net_player_data.m_name;
		}

		return "**UNKNOWN**";
	}

	Vector3 Util::getEntityPosition(const void* const ent)
	{
		if (ent)
		{
			if (auto nav = *(ReconstructedClasses::CNavigation**)((std::uintptr_t)(ent) + 0x30))
			{
				return nav->v3Loc;
			}
		}

		return { NAN, NAN, NAN };
	}

	Vector3 Util::getEntityPositionHandle(const std::int32_t handle)
	{
		return getEntityPosition(handleToPtr<void*>(handle));
	}

	float Util::getEntityHeading(const void* const ent)
	{
		if (ent)
		{
			if (auto nav = (ReconstructedClasses::CNavigation*)((std::uintptr_t)(ent) + 0x30))
			{
				return nav->v3Rot.z;
			}
		}

		return NAN;
	}

	std::uint32_t Util::getEntityModel(const void* const ent)
	{
		if (ent)
		{
			if (auto modelInfo = (ReconstructedClasses::CModelInfo*)((std::uintptr_t)(ent) + 0x20))
			{
				return modelInfo->modelHash;
			}
		}

		return 0;
	}

	std::uint32_t Util::getEntityModelHandle(const std::int32_t handle)
	{
		return getEntityModel(handleToPtr<void*>(handle));
	}

	bool Util::isPedInVehicle(ReconstructedClasses::CPed* const ped)
	{
		return ped && !(Natives::PED::IS_PED_IN_ANY_VEHICLE(Natives::PLAYER::PLAYER_PED_ID(), false));
	}

	Vector3 Util::rot2dir(const Vector3 rot) {
		auto xRadian = degToRad(rot.x);
		auto zRadian = degToRad(rot.z);
		auto num = std::abs(std::cosf(xRadian));

		return
		{
			(-std::sinf(zRadian)) * num,
			std::cosf(zRadian) * num,
			std::sinf(xRadian)
		};
	}

	float Util::entityDistance(void* const ent1, void* const ent2)
	{
		if (ent1 && ent2)
		{
			auto nav1 = *(ReconstructedClasses::CNavigation**)((std::uintptr_t)(ent1) + 0x30);
			auto nav2 = *(ReconstructedClasses::CNavigation**)((std::uintptr_t)(ent2) + 0x30);
			if (nav1 && nav2)
			{
				return distance3D(nav1->v3Loc, nav2->v3Loc);
			}
		}

		return NAN;
	}

	float Util::degToRad(const float deg)
	{
		return deg * 0.017453292519943295f;
	}

	float Util::distance3D(const Vector3 one, const Vector3 two)
	{
		return std::sqrtf(
			std::powf(one.x - two.x, 2.f) +
			std::powf(one.y - two.y, 2.f) +
			std::powf(one.z - two.z, 2.f));
	}
}
