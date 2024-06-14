#pragma once
#include <cstdint>
#include "Declarations.hpp"
#pragma pack(push, 1)

namespace Vega::ReconstructedClasses
{
	struct CObject
	{
		virtual ~CObject() = 0;
		char pad_0x0000[0x18]; //0x0008
		CModelInfo* modelInfo; //0x0020 
		char pad_0x0028[0xA8]; //0x0028
		rage::netObject* netObject; //0x00D0 
	};
}

#pragma pack(pop)
