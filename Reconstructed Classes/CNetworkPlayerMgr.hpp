#pragma once
#include <cstdint>
#include "Declarations.hpp"
#pragma pack(push, 1)

namespace Vega::ReconstructedClasses
{
	namespace rage {
		class netPlayerData;
		struct netPlayerMgrBase {
			virtual ~netPlayerMgrBase() = default;
			virtual void Initialize() = 0;
			virtual void Shutdown() = 0;
			virtual void unk_0x18() = 0;
			virtual CNetGamePlayer* AddPlayer_raw(void* a1, void* a2, void* a3, rage::netPlayerData* net_player_data, CNonPhysicalPlayerData* non_physical_player_data) = 0;
			virtual void RemovePlayer(CNetGamePlayer* net_game_player) = 0;
			virtual void UpdatePlayerListsForPlayer(CNetGamePlayer* net_game_player) = 0;
			virtual CNetGamePlayer* AddPlayer(void* a1, void* a2, void* a3, rage::netPlayerData* net_player_data, CNonPhysicalPlayerData* non_physical_player_data) = 0;
		}; //Size: 0x0008
	}
	struct CNetworkPlayerMgr : public rage::netPlayerMgrBase {
		char pad_0008[224]; //0x0008
		class CNetGamePlayer* m_local_net_player; //0x00E8
		char pad_00F0[144]; //0x00F0
		class CNetGamePlayer* m_player_list[32]; //0x0180
		uint16_t m_player_limit; //0x0280
		char pad_0282[10]; //0x0282
		uint16_t m_player_count; //0x028C
	}; //Size: 0x028E
}

#pragma pack(pop)
