#include "../PrecompiledHeader.hpp"
#include "VehicleOptions.hpp"
#include "../Lists.hpp"
#include "../Util.hpp"

#include "../ScriptCallback.hpp"

#include "../Reconstructed Classes/CPed.hpp"
#include "../Reconstructed Classes/CNavigation.hpp"
#include "../Reconstructed Classes/CVehicle.hpp"

namespace Vega::Feature
{
	VehicleOptions g_VehicleOptions;

	void VehicleOptions::loop(void)
	{
		if (m_GodMode)
		{
			applyGodMode();
		}

		if (m_Boost)
		{
			applyBoost();
		}
	}

	void VehicleOptions::applyGodMode(void)
	{
		if (auto vehicle = Util::getLocalVehicle())
		{
			vehicle->m_deform_god = m_GodMode;
		}
	}

	void VehicleOptions::applyBoost(void)
	{
		if (auto vehicle = Util::getLocalVehicle())
		{
			if (auto playerInfo = Util::getLocalPlayerInfo(); playerInfo && g_Pointers->call(g_Pointers->m_IsPlayerPressingHorn, playerInfo))
			{
				g_Pointers->call(g_Pointers->m_SetVehicleForwardSpeed, vehicle, 50.f);
			}
		}
	}

	void VehicleOptions::fullyTuneVehicle(const std::int32_t vehHandle)
	{
		Natives::VEHICLE::SET_VEHICLE_MOD_KIT(vehHandle, 0);
		for (std::int32_t i = 0; i < 1000; ++i)
		{
			auto numMods = Natives::VEHICLE::GET_NUM_VEHICLE_MODS(vehHandle, i);
			if (numMods > 0)
			{
				Natives::VEHICLE::SET_VEHICLE_MOD(vehHandle, i, numMods - 1, false);
			}
		}
	}

	void VehicleOptions::spawnVehicle(const std::uint32_t hash, const Vector3 pos)
	{
		if (Natives::STREAMING::IS_MODEL_A_VEHICLE(hash))
		{
			g_ScriptCallbackManager->addCallback(ModelScriptCallback(hash, [=]
			{
				auto vehHandle = Natives::VEHICLE::CREATE_VEHICLE(hash, pos.x, pos.y, pos.z, 0.f, true, true, NULL);

				auto pedHandle = Util::getLocalPedHandle();
				if (vehHandle != -1 && pedHandle != -1)
				{
					if (*g_Pointers->m_IsSessionStarted)
					{
						Natives::DECORATOR::DECOR_SET_INT(vehHandle, "MPBitset", 0);
					}

					if (m_SpawnOnGround)
					{
						Natives::VEHICLE::SET_VEHICLE_ON_GROUND_PROPERLY(vehHandle, NULL);
					}

					if (m_SpawnInCar)
					{
						Natives::PED::SET_PED_INTO_VEHICLE(pedHandle, vehHandle, Enums::VehicleSeatDriver);
					}

					if (m_FadeInSpawnedCar)
					{
						Natives::NETWORK::NETWORK_FADE_IN_ENTITY(vehHandle, false, NULL);
					}

					if (m_ChangePlateOnSpawnedCar)
					{
						Natives::VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT(vehHandle, XorString(MENUNAME_NORMAL));
						Natives::VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT_INDEX(vehHandle, Enums::NumberPlateTypeNorthYankton);
					}

					if (m_SpawnFullyTuned)
					{
						fullyTuneVehicle(vehHandle);
					}

					if (m_SpawnParticleFX)
					{
						g_ScriptCallbackManager->addCallback(ParticleFXScriptCallback(XorString("scr_rcbarry2"), [=]
						{
							Natives::GRAPHICS::START_PARTICLE_FX_NON_LOOPED_ON_ENTITY(XorString("scr_exp_clown"), vehHandle, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 1.f, false, false, false);
						}));
					}
				}

			}));
		}
	}
}
