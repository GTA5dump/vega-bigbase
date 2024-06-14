#pragma once
#include <cstdint>
#pragma pack(push, 1)

namespace Vega::ReconstructedClasses
{
	struct BGRAColor
	{
		std::uint8_t b;
		std::uint8_t g;
		std::uint8_t r;
		std::uint8_t a;
	};
}

#pragma pack(pop)
