#pragma once
#include <cstdint>
#include "../Types.hpp"
#include "Declarations.hpp"
#pragma pack(push, 1)

namespace Vega::ReconstructedClasses
{
	struct CVehicleMods
	{
		virtual ~CVehicleMods() = 0;
		char pad_0x0000[0x18]; //0x0008
		CVehicleVisualMods* pVisualMods; //0x0020 
		char pad_0x0028[0x378]; //0x0028
		Color neonColor; //0x03A0 
		char pad_0x03A4[0x4C]; //0x03A4
		std::int16_t wModKit; //0x03F0 
		char pad_0x03F2[0x582]; //0x03F2
	};
}

#pragma pack(pop)
