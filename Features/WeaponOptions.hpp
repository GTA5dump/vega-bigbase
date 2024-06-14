#pragma once
#include "../PrecompiledHeader.hpp"

namespace Vega::Feature
{
	class WeaponOptions
	{
	public:
		void loop();

		bool m_InfiniteAmmo = false;
		bool m_DeleteGun = false;
		bool m_GravityGun = false;
		bool m_SuccGun = false;
		bool m_FireAmmo = false;
		bool m_ExplosiveAmmo = false;
		bool m_LaserSight = false;
		bool m_RapidFire = false;
		bool m_CartoonGun = false;
		bool m_TeleportGun = false;
		bool m_AirstrikeGun = false;
		bool m_NoRecoil = false;
		bool m_DriveItGun = false;
		bool m_HashGun = false;
		bool m_JustCauseGun = false;
		bool m_PedGun = false;
		bool m_CoordGun = false;
		bool m_ValkyireRocket = false;
		bool m_EntityShooter = false;
		std::size_t m_EntityShooterType = 0;
		std::size_t m_VehicleShooterType = 0;
		bool m_RandomVehicleColor = true;
		std::size_t m_PedShooterType = 0;
		bool m_MidgetPeds = false;
		std::size_t m_PropShooterType = 0;

		void getWeapons();
		void removeWeapons();
		void applyInfiniteAmmo();
		Raycast raycast();
		void applyDeleteGun();
		void applyGravityGun();
		void applySuccGun();
		void applyLaserSight();
		void applyRapidFire();
		void applyCartoonGun();
		void applyTeleportGun();
		void applyAirstrikeGun();
		void applyNoRecoil(bool t);
		void applyDriveItGun();
		void applyHashGun();
		void applyJustCauseGun();
		void dildoKnife();
		void applyPedGun();
		void applyCoordGun();
		void applyValkyireRocket();
		void applyEntityShooter();
	};
	extern WeaponOptions g_WeaponOptions;
}
