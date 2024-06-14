#pragma once
#include <cstdint>
#include "../Types.hpp"
#include "Declarations.hpp"
#pragma pack(push, 1)

namespace Vega::ReconstructedClasses
{
	struct CHandlingData
	{
		virtual ~CHandlingData() = 0;
		char pad_0x0000[0x4]; //0x0008
		float fMass; //0x000C 
		char pad_0x0010[0x4]; //0x0010
		float fDownforceModifier; //0x0014 
		char pad_0x0018[0x18]; //0x0018
		Vector3 v3IntertiaMultiplier; //0x0030 
		float fDriveInertia; //0x003C 
		float fPercentSubmerged; //0x0040 
		float fFrontWheelsAcceleration; //0x0044 
		char pad_0x0048[0x4]; //0x0048
		float fRearWheelsAcceleration; //0x004C 
		std::int32_t nInitialDriveGears; //0x0050 
		float N00000KEK; //0x0054 
		float fUpshift; //0x0058 
		float fDownshift; //0x005C 
		float fInitialDriveForce; //0x0060 
		float N00000BFB; //0x0064 
		float N00000DEF; //0x0068 
		float fBrakeForce; //0x006C 
		char pad_0x0070[0x4]; //0x0070
		float fBrakeBiasFront; //0x0074 
		float N00000BB5; //0x0078 
		float fHandbrakeForce; //0x007C 
		float fSteeringLock; //0x0080 
		float N00001784; //0x0084 
		float fTractionCurveMax; //0x0088 
		float N00000BF0; //0x008C 
		float fTractionCurveMin; //0x0090 
		float N00001KEK; //0x0094 
		float N00001786; //0x0098 
		float N00000BF2; //0x009C 
		float fTractionSpringDeltaMax; //0x00A0 
		char pad_0x00A4[0x4]; //0x00A4
		float fLowSpeedTractionLossMult; //0x00A8 
		float fCamberStiffnesss; //0x00AC 
		float fTractionBiasFront; //0x00B0 
		float fTwoMinus_fTractionBiasFront; //0x00B4 
		float fTractionLossMult; //0x00B8 
		float fSuspensionForce; //0x00BC 
		float fSuspensionCompDamp; //0x00C0 
		float fSuspensionReboundDamp; //0x00C4 
		float fSuspensionUpperLimit; //0x00C8 
		float fSuspensionLowerLimit; //0x00CC 
		float fSuspensionHeight; //0x00D0 
		char pad_0x00D4[0x8]; //0x00D4
		float fAntiRollBarForce; //0x00DC 
		char pad_0x00E0[0x8]; //0x00E0
		float fRollCentreHeightFront; //0x00E8 
		float fRollCentreHeightRear; //0x00EC 
		float fCollisionDamageMult; //0x00F0 
		float fWeaponDamageMult; //0x00F4 
		float fDeformationDamageMult; //0x00F8 
		float fEngineDamageMult; //0x00FC 
		float fPetrolTankVolume; //0x0100 
		float fOilVolume; //0x0104 
		char pad_0x0108[0x4]; //0x0108
		Vector3 v3SeatOffset; //0x010C 
		std::int32_t i32MonetaryValue; //0x0118 
		char pad_0x011C[0xAC]; //0x011C
	};
}

#pragma pack(pop)
