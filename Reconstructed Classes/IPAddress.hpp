#pragma once
#include <cstdint>
#pragma pack(push, 1)

namespace Vega::ReconstructedClasses
{
	struct IPAddress
	{
		union
		{
			struct
			{
				std::uint8_t four;
				std::uint8_t three;
				std::uint8_t two;
				std::uint8_t one;
			} fields;

			std::uint32_t packed;
		};
	};
}

#pragma pack(pop)
