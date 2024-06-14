#pragma once
#include <cstdint>
#include "../Types.hpp"
#include "BGRAColor.hpp"
#pragma pack(push, 1)

namespace Vega::ReconstructedClasses
{
	struct CTextInfo
	{
		BGRAColor color;
		Vector2 scale;
		Vector2 wrap;
		std::int64_t font;
		std::int16_t align;
		bool shadow;
		bool outline;
	};
}

#pragma pack(pop)
