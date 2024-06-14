#pragma once
#include <cstdint>
#include "../Types.hpp"
#include "Declarations.hpp"
#pragma pack(push, 1)

namespace Vega::ReconstructedClasses
{
	struct CAmmoInfo
	{
		virtual ~CAmmoInfo() = 0;
		void* N0000077B; //0x0008 
		char pad_0x0010[0x10]; //0x0010
		std::int32_t iAmmoMax; //0x0020 
		std::int32_t iAmmoMax50; //0x0024 
		std::int32_t iAmmoMax100; //0x0028 
		std::int32_t iAmmoMaxMP; //0x002C 
		std::int32_t iAmmoMax50MP; //0x0030 
		std::int32_t iAmmoMax100MP; //0x0034 
		char pad_0x0038[0x8]; //0x0038
		float fDamage; //0x0040 
		float fLifeTime; //0x0044 
		float fFromVehicleLifeTime; //0x0048 
		float fLifeTimeAfterImpact; //0x004C 
		float fLifeTimeAfterExplosion; //0x0050 
		float fExplosionTime; //0x0054 
		float fLaunchSpeed; //0x0058 
		float fSeparationTime; //0x005C 
		float fTimeToReachTarget; //0x0060 
		float fDamping; //0x0064 
		float fGravityFactor; //0x0068 
		float fRicochetTolerance; //0x006C 
		float fPedRicochetTolerance; //0x0070 
		float fVehicleRicochetTolerance; //0x0074 
		char pad_0x0078[0x68]; //0x0078
		Vector3 fLightColour; //0x00E0 
		char pad_0x00EC[0x4]; //0x00EC
		float fLightIntensity; //0x00F0 
		float fLightRange; //0x00F4 
		float fLightFalloffExp; //0x00F8 
		float fLightFrequency; //0x00FC 
		float fLightPower; //0x0100 
		float fCoronaSize; //0x0104 
		float fCoronaIntensity; //0x0108 
		float fCoronaZBias; //0x010C 
		float N00001CE2; //0x0110 
		char pad_0x0114[0x6C]; //0x0114
	};
}

#pragma pack(pop)
