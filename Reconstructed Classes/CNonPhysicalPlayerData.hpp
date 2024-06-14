#pragma once
#include <cstdint>
#include "../Types.hpp"
#include "Declarations.hpp"
#pragma pack(push, 1)

namespace Vega::ReconstructedClasses
{
	struct CNonPhysicalPlayerData
	{
		virtual ~CNonPhysicalPlayerData() = 0;
		std::int32_t bubbleId; //0x0008 
		std::int32_t playerId; //0x000C 
		Vector3 position; //0x0010 
		char pad_0x001C[0x4]; //0x001C
	};
}

#pragma pack(pop)
