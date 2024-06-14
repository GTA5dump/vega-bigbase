#pragma once
#include "PrecompiledHeader.hpp"
#include "User Interface/HeaderType.hpp"

namespace Vega
{
	struct CNameWithHash
	{
		const char* name = nullptr;
		std::uint32_t hash = 0;
	};

	inline void operator<<(std::ostream& stream, CNameWithHash nameHash)
	{
		stream << nameHash.name;
	}

	namespace Lists
	{
		extern const char* headerTypeFrontend[2];
		extern UserInterface::HeaderType headerTypeBackend[2];
		extern std::size_t headerTypePosition;
		extern const char* entityShooterList[3];
		extern std::uint32_t vehicleShooterList[11];

		extern CNameWithHash popularPeds[15];
		extern CNameWithHash namedProps[32];

		extern const char* prop_list[5762];
		extern const char* ped_list[665];

		extern std::uint32_t rankToExperience[8000];
		extern std::uint32_t quickRankList[9];
		extern std::size_t quickRankListPos;

		extern std::uint32_t weaponListFull[61];

		extern std::uint32_t vehicleListAfterHours[14];
	}
}
