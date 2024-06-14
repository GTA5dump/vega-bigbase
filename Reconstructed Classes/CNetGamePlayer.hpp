#pragma once
#include <cstdint>
#include "Declarations.hpp"
#pragma pack(push, 1)

namespace Vega::ReconstructedClasses
{
	struct CNetGamePlayer
	{
		virtual ~CNetGamePlayer() = 0;
		char pad_0x0008[0x8]; //0x0008
		CNonPhysicalPlayerData* nonPhysicalPlayerData; //0x0010 
		char pad_0x0018[0x15]; //0x0018
		std::uint8_t playerId; //0x002D 
		char pad_0x002E[0x6A]; //0x002E
		std::uint8_t N00001BF3; //0x0098 
		char pad_0x0099[0xF]; //0x0099
		CPlayerInfo* playerInfo; //0x00A8 
		char pad_0x00B0[0xF1]; //0x00B0
		std::uint8_t isRockstarDev; //0x01A1 
		char pad_0x01A2[0x1]; //0x01A2
		std::uint8_t isCheater; //0x01A3 
		char pad_0x01A4[0xFC]; //0x01A4
	};
}

#pragma pack(pop)
