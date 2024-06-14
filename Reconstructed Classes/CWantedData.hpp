#pragma once
#include <cstdint>
#include "../Types.hpp"
#include "Declarations.hpp"
#pragma pack(push, 1)

namespace Vega::ReconstructedClasses
{
	struct CWantedData
	{
		char pad_0x0000[0x50]; //0x0000
		Vector3 radius1; //0x0050
		char pad_0x005C[0x4]; //0x005C
		Vector3 radius2; //0x0060
		char pad_0x006C[0x4]; //0x006C
		Vector3 radius3; //0x0070
		char pad_0x007C[0x6]; //0x007C
		std::uint8_t ignore; //0x0082
		char pad_0x0083[0x31]; //0x0083
		std::int32_t fakeWantedLevel; //0x00B4
		std::int32_t wantedLevel; //0x00B8
		char pad_0x00BC[0x4C]; //0x00BC
	};
}

#pragma pack(pop)
