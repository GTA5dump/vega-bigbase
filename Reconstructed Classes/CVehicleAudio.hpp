#pragma once
#include <cstdint>
#include "../Types.hpp"
#include "Declarations.hpp"
#pragma pack(push, 1)

namespace Vega::ReconstructedClasses
{
	struct CVehicleAudio
	{
		virtual ~CVehicleAudio() = 0;
		char pad_0x0000[0x635]; //0x0008
		std::uint8_t isHornActive; //0x063D 
		char pad_0x063E[0x13E]; //0x063E
		std::int32_t boostSoundActive; //0x077C 
		char pad_0x0780[0x108]; //0x0780
	};
}

#pragma pack(pop)
