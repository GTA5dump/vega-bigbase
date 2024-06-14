#pragma once
#include <cstdint>
#pragma pack(push, 1)

namespace Vega::ReconstructedClasses
{
	struct CTimeDate
	{
		std::int32_t day;
		std::int32_t month;
		std::int32_t year;
		std::int32_t hour;
		std::int32_t minute;
		std::int32_t second;
	};
}

#pragma pack(pop)
