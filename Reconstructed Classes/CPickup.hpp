#pragma once
#include <cstdint>
#include "Declarations.hpp"
#pragma pack(push, 1)

namespace Vega::ReconstructedClasses
{
	struct CPickup
	{
		char pad_0x0000[0xD0]; //0x0000
		rage::netObject* netObject; //0x00D0 
		char pad_0x00D8[0x3A8]; //0x00D8
		std::int16_t moneyAmount; //0x0480 
	};
}

#pragma pack(pop)
