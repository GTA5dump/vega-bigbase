#pragma once
#include <cstdint>
#include "../Types.hpp"
#include "Declarations.hpp"
#pragma pack(push, 1)

namespace Vega::ReconstructedClasses
{
	struct CWeaponInfo
	{
		virtual ~CWeaponInfo() = 0;
		void* N0000051B; //0x0008 
		char pad_0x0010[0x50]; //0x0010
		CAmmoInfo* pAmmoInfo; //0x0060 
		void* N00000527; //0x0068 
		char pad_0x0070[0x4]; //0x0070
		float fSpread; //0x0074 
		char pad_0x0078[0xC]; //0x0078
		float fRecoil; //0x0084 
		float fRecoilErrorTime; //0x0088 
		float fRecoilRecoveryRate; //0x008C 
		float fRecoilAccuracyToAllowHeadshotAI; //0x0090 
		float fMinHeadshotDistanceAI; //0x0094 
		float fMaxHeadshotDistanceAI; //0x0098 
		float fHeadshotDamageModifierAI; //0x009C 
		float fRecoilAccuracyToAllowHeadshotPlayer; //0x00A0 
		float fMinHeadshotDistancePlayer; //0x00A4 
		float fMaxHeadshotDistancePlayer; //0x00A8 
		float fHeadshotDamageModifierPlayer; //0x00AC 
		float fDamage; //0x00B0 
		float fDamageTime; //0x00B4 
		float fDamageTimeInVehicle; //0x00B8 
		float fDamageTimeInVehicleHeadShot; //0x00BC 
		float fHitLimbsDamageModifier; //0x00C0 
		float fNetworkHitLimbsDamageModifier; //0x00C4 
		float fLightlyArmouredDamageModifier; //0x00C8 
		char pad_0x00CC[0x4]; //0x00CC
		float fForce; //0x00D0 
		float fPedImpulse; //0x00D4 
		float fVehicleImpulse; //0x00D8 
		float fFlyingImpulse; //0x00DC 
		void* overrideForces; //0x00E0 
		char pad_0x00E8[0x8]; //0x00E8
		float fForceMaxStrengthMult; //0x00F0 
		float fForceFalloffRangeStart; //0x00F4 
		float fForceFalloffRangeEnd; //0x00F8 
		float fForceFalloffMin; //0x00FC 
		float fProjectileForce; //0x0100 
		float fFragImpulse; //0x0104 
		float fPenetration; //0x0108 
		float fVerticalLaunchAdjustment; //0x010C 
		float fDropForwardVelocity; //0x0110 
		float fSpeed; //0x0114 
		std::int32_t i32BulletsPerShot; //0x0118 
		float fMultiBulletSpread; //0x011C 
		float fReloadTimeMP; //0x0120 
		float fReloadTimeSP; //0x0124 
		float fVehicleReloadTime; //0x0128 
		float fAnimReloadRate; //0x012C 
		std::int32_t fBulletsPerAnimLoop; //0x0130 
		float fTimeToShoot; //0x0134 
		float fTimeLeftBetweenShotsWhereShouldFireIsCached; //0x0138 
		float fSpinUpTime; //0x013C 
		float fSpinTime; //0x0140 
		float fSpinDownTime; //0x0144 
		float fAlternateWaitTime; //0x0148 
		float fBulletBendingNearRadius; //0x014C 
		float fBulletBendingFarRadius; //0x0150 
		float fBulletBendingZoomedRadius; //0x0154 
		float fFirstPersonBulletBendingNearRadius; //0x0158 
		float fFirstPersonBulletBendingFarRadius; //0x015C 
		float fFirstPersonBulletBendingZoomedRadius; //0x0160 
		char pad_0x0164[0x5C]; //0x0164
		float fTracerFxChanceSP; //0x01C0 
		float fTracerFxChanceMP; //0x01C4 
		float fFlashFxChanceSP; //0x01C8 
		float fFlashFxChanceMP; //0x01CC 
		float fFlashFxAltChance; //0x01D0 
		float fFlashFxScale; //0x01D4 
		char pad_0x01D8[0x38]; //0x01D8
		Vector2 v2FlashFxLightFalloffMinMax; //0x0210 
		char pad_0x0218[0x4]; //0x0218
		float fGroundDisturbFxDist; //0x021C 
		char pad_0x0220[0x20]; //0x0220
		std::int32_t iInitialRumbleDuration; //0x0240 
		float fInitialRumbleIntensity; //0x0244 
		float fInitialRumbleIntensityTrigger; //0x0248 
		std::int32_t fRumbleDuration; //0x024C 
		float fRumbleIntensity; //0x0250 
		float fRumbleIntensityTrigger; //0x0254 
		float fRumbleDamageIntensity; //0x0258 
		std::int32_t iInitialRumbleDurationFps; //0x025C 
		float fInitialRumbleIntensityFps; //0x0260 
		std::int32_t iRumbleDurationFps; //0x0264 
		float fRumbleIntensityFps; //0x0268 
		float fNetworkPlayerDamageModifier; //0x026C 
		float fNetworkPedDamageModifier; //0x0270 
		float fNetworkHeadshotPlayerDamageModifier; //0x0274 
		float fLockonRange; //0x0278 
		float fRange; //0x027C 
		float fAiSoundRange; //0x0280 
		float fAiPotentialBlastEventRange; //0x0284 
		float fDamageFallOffRangeMin; //0x0288 
		float fDamageFallOffRangeMax; //0x028C 
		char pad_0x0290[0x3C]; //0x0290
		float fRecoilFirstPerson; //0x02CC 
		char pad_0x02D0[0x8]; //0x02D0
		float fRecoilThirdPerson; //0x02D8 
		char pad_0x02DC[0x4]; //0x02DC
		float fThirdPersonAimingFOV; //0x02E0 
		float fTing; //0x02E4 
		float fFirstPersonAimingFov; //0x02E8 
		float fFirstPersonScopeFov; //0x02EC 
		float fFirstPersonScopeAttachmentFov; //0x02F0 
		char pad_0x02F4[0x10C]; //0x02F4
		float fZoomFactorForAccurateMode; //0x0400 
		char pad_0x0404[0x1DC]; //0x0404
		char* szStatName; //0x05E0 
		std::int32_t iKnockdownCount; //0x05E8 
		float fKillshotImpulseScale; //0x05EC 
		char pad_0x05F0[0x2E0]; //0x05F0
		void* N000006F4; //0x08D0 
		char pad_0x08D8[0x78]; //0x08D8
	};
}

#pragma pack(pop)
