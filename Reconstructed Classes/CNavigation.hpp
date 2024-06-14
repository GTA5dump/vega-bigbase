#pragma once
#include <cstdint>
#include "../Types.hpp"
#include "Declarations.hpp"
#pragma pack(push, 1)

namespace Vega::ReconstructedClasses
{
	struct CNavigation
	{
		virtual ~CNavigation() = 0;
		char pad_0x0000[0x18]; //0x0008
		float fHeading; //0x0020
		float fHeading2; //0x0024
		char pad_0x0028[0x8]; //0x002
		Vector3 v3Rot; //0x0030
		char pad_0x003C[0x14]; //0x003C
		Vector3 v3Loc; //0x0050
	};
}

#pragma pack(pop)
