#pragma once
#include "Declarations.hpp"
#pragma pack(push, 1)

namespace Vega::ReconstructedClasses
{
	struct CPedFactory
	{
		virtual ~CPedFactory() = 0;
		CPed* localPed;
	};
}

#pragma pack(pop)
