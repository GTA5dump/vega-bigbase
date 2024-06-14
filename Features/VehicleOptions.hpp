#pragma once
#include "../PrecompiledHeader.hpp"

namespace Vega::Feature
{
	class VehicleOptions
	{
	public:
		void loop();

		bool m_GodMode = false;
		void applyGodMode();

		bool m_Boost = false;
		void applyBoost();


		void fullyTuneVehicle(std::int32_t vehHandle);

		bool m_SpawnInCar = true;
		bool m_FadeInSpawnedCar = true;
		bool m_ChangePlateOnSpawnedCar = true;
		bool m_SpawnFullyTuned = false;
		bool m_SpawnOnGround = true;
		bool m_SpawnParticleFX = false;
		void spawnVehicle(std::uint32_t hash, Vector3 pos);
	};

	extern VehicleOptions g_VehicleOptions;
}
