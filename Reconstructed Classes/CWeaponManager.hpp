#pragma once
#include <cstdint>
#include "Declarations.hpp"
#pragma pack(push, 1)

namespace Vega::ReconstructedClasses
{
	struct CWeaponManager
	{
		virtual ~CWeaponManager() = 0;
		char pad[0x18];
		CWeaponInfo* currentWeapon;
	};
}

#pragma pack(pop)
