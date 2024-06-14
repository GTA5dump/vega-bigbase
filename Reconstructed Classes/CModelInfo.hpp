#pragma once
#include <cstdint>
#include "../Types.hpp"
#include "Declarations.hpp"
#pragma pack(push, 1)

namespace Vega::ReconstructedClasses
{
	struct CModelInfo
	{
		virtual ~CModelInfo() = 0;
		char pad_0x0000[0x10]; //0x0008
		std::uint32_t modelHash; //0x0018 
		char pad_0x001C[0x14]; //0x001C
		Vector3 N000004CD; //0x0030 
		float N000007AE; //0x003C 
		Vector3 N000007A5; //0x0040 
		char pad_0x004C[0x239]; //0x004C
		std::uint8_t type; //0x0285 
		char pad_0x0286[0x1EA]; //0x0286
	};
}

#pragma pack(pop)
