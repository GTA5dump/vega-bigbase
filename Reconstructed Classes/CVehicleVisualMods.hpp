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
		char pad_0x0000[0x98]; //0x0008
		char pad_0x00A2[0x2]; //0x00A2
		Color primary; //0x00A4 
		Color secondary; //0x00A8 
		Color unknown; //0x00AC 
		Color N000017F7; //0x00B0 
		char pad_0x00B4[0x8]; //0x00B4
		Color N000017F9; //0x00BC 
		char pad_0x00C0[0x60]; //0x00C0
		struct
		{
			char pad_0x0000[0x28]; //0x0000
			char* plateName; //0x0028 
			char pad_0x0030[0x88]; //0x0030
			char* tireName; //0x00B8 
			char pad_0x00C0[0x88]; //0x00C0
			char* visualName; //0x0148 
		} *N00001806; //0x0120 
		char pad_0x0128[0x8]; //0x0128
		char N00001808[8]; //0x7B32E9C0 
		char pad_0x0138[0x7AC]; //0x0138
	};
}

#pragma pack(pop)
