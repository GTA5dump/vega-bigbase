#pragma once
#include <cstdint>
#include "Declarations.hpp"
#pragma pack(push, 1)

namespace Vega::ReconstructedClasses
{
	struct CPedStyle
	{
		virtual ~CPedStyle() = 0;
		char pad_0x0000[0x38]; //0x0008
		std::int32_t iEyeColor; //0x0040 
		char pad_0x0044[0xA4]; //0x0044
		std::uint8_t N00000EB2[12]; //0x00E8 
		char pad_0x00F4[0xC]; //0x00F4
		std::uint8_t N00000ED7[12]; //0x0100 
		std::uint8_t N00000EE0[12]; //0x010C 
		char pad_0x0118[0x10]; //0x0118
		std::uint8_t N00000EE7; //0x0128 
	};
}

#pragma pack(pop)
