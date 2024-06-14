#pragma once
#include <cstdint>
#include "Declarations.hpp"
#pragma pack(push, 1)

namespace Vega::ReconstructedClasses
{
	struct CResolution
	{
		std::uint32_t x;
		std::uint32_t y;
	};
}

#pragma pack(pop)
