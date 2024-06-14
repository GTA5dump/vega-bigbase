#include "../PrecompiledHeader.hpp"
#include "WorldOptions.hpp"

#include "../Reconstructed Classes/CPed.hpp"
#include "../Reconstructed Classes/CVehicle.hpp"
#include "../Reconstructed Classes/CVehiclePool.hpp"
#include "../Reconstructed Classes/CPedPool.hpp"
#include "../Reconstructed Classes/CNavigation.hpp"

#include "../ScriptGlobal.hpp"
#include "../Util.hpp"
#include "../Natives.hpp"
#include "../Pointers.hpp"

namespace Vega::Feature
{
	WorldOptions g_WorldOptions;

	void WorldOptions::loop()
	{
		if (m_BlackHoleToggle)
		{
			applyBlackHole();
		}
	}

	void WorldOptions::trafficManagerLaunch()
	{
		auto me = Util::getLocalPlayerPed();
		auto myVehicle = me ? me->m_vehicle : nullptr;
		(*g_Pointers->m_VehiclePool)->forAll([=](ReconstructedClasses::CAutomobile* vehicle)
		{
			if (vehicle != myVehicle && Util::entityDistance(me, vehicle) <= m_TrafficManagerRadius)
			{
				Natives::ENTITY::APPLY_FORCE_TO_ENTITY(Util::ptrToHandle(vehicle), 1, 0.f, 0.f, 50.f, 0.f, 0.f, 0.f, 1, false, true, true, true, true);
			}
		});
	}
	
	void WorldOptions::trafficManagerBoostForward()
	{
		auto me = Util::getLocalPlayerPed();
		auto myVehicle = me ? me->m_vehicle : nullptr;
		(*g_Pointers->m_VehiclePool)->forAll([=](ReconstructedClasses::CAutomobile* vehicle)
		{
			if (vehicle != myVehicle && Util::entityDistance(me, vehicle) <= m_TrafficManagerRadius)
			{
				Natives::VEHICLE::SET_VEHICLE_FORWARD_SPEED(Util::ptrToHandle(vehicle), 80.f);
			}
		});
	}
	
	void WorldOptions::trafficManagerBoostBackward()
	{
		auto me = Util::getLocalPlayerPed();
		auto myVehicle = me ? me->m_vehicle : nullptr;
		(*g_Pointers->m_VehiclePool)->forAll([=](ReconstructedClasses::CAutomobile* vehicle)
		{
			if (vehicle != myVehicle && Util::entityDistance(me, vehicle) <= m_TrafficManagerRadius)
			{
				Natives::VEHICLE::SET_VEHICLE_FORWARD_SPEED(Util::ptrToHandle(vehicle), -80.f);
			}
		});
	}
	
	void WorldOptions::trafficManagerKickflip()
	{
		auto me = Util::getLocalPlayerPed();
		auto myVehicle = me ? me->m_vehicle : nullptr;
		(*g_Pointers->m_VehiclePool)->forAll([=](ReconstructedClasses::CAutomobile* vehicle)
		{
			if (vehicle != myVehicle && Util::entityDistance(me, vehicle) <= m_TrafficManagerRadius)
			{
				Natives::ENTITY::APPLY_FORCE_TO_ENTITY(Util::ptrToHandle(vehicle), 1, 0.0, 0.0, 10.71f, 5.f, 0.f, 0.f, 1, false, true, true, true, true);
			}
		});
	}
	
	void WorldOptions::trafficManagerExplode()
	{
		auto me = Util::getLocalPlayerPed();
		auto myVehicle = me ? me->m_vehicle : nullptr;
		(*g_Pointers->m_VehiclePool)->forAll([=](ReconstructedClasses::CAutomobile* vehicle)
		{
			if (vehicle != myVehicle && Util::entityDistance(me, vehicle) <= m_TrafficManagerRadius)
			{
				if (vehicle->m_navigation)
				{
					auto& pos = vehicle->m_navigation->v3Loc;
					Natives::FIRE::ADD_EXPLOSION(pos.x, pos.y, pos.z, 15, 1.f, true, false, 0.1f, false);
				}
			}
		});
	}
	
	void WorldOptions::trafficManagerKillDriver()
	{
		auto me = Util::getLocalPlayerPed();
		auto myVehicle = me ? me->m_vehicle : nullptr;
		(*g_Pointers->m_VehiclePool)->forAll([=](ReconstructedClasses::CAutomobile* vehicle)
		{
			if (vehicle != myVehicle && Util::entityDistance(me, vehicle) <= m_TrafficManagerRadius)
			{
				(*g_Pointers->m_PedPool)->forAll([=](ReconstructedClasses::CPed* ped)
				{
					if (ped->m_vehicle == vehicle)
					{
						ped->m_health = 0.f;
					}
				});

			}
		});
	}

	void WorldOptions::trafficManagerRandomPaintJob()
	{
		auto me = Util::getLocalPlayerPed();
		auto myVehicle = me ? me->m_vehicle : nullptr;
		(*g_Pointers->m_VehiclePool)->forAll([=](ReconstructedClasses::CAutomobile* vehicle)
		{
			if (vehicle != myVehicle && Util::entityDistance(me, vehicle) <= m_TrafficManagerRadius)
			{
				Natives::VEHICLE::SET_VEHICLE_CUSTOM_PRIMARY_COLOUR(Util::ptrToHandle(vehicle), rand() % 0xFF, rand() % 0xFF, rand() % 0xFF);
			}
		});
	}

	void WorldOptions::trafficManagerNegativeGravity()
	{
		auto me = Util::getLocalPlayerPed();
		auto myVehicle = me ? me->m_vehicle : nullptr;
		(*g_Pointers->m_VehiclePool)->forAll([=](ReconstructedClasses::CAutomobile* vehicle)
		{
			if (vehicle != myVehicle && Util::entityDistance(me, vehicle) <= m_TrafficManagerRadius)
			{
				*(float*)((std::uintptr_t)vehicle + 0xBCC) = -9.81f;
			}
		});
	}
	
	void WorldOptions::trafficManagerEarthGravity()
	{
		auto me = Util::getLocalPlayerPed();
		auto myVehicle = me ? me->m_vehicle : nullptr;
		(*g_Pointers->m_VehiclePool)->forAll([=](ReconstructedClasses::CAutomobile* vehicle)
		{
			if (vehicle != myVehicle && Util::entityDistance(me, vehicle) <= m_TrafficManagerRadius)
			{
				*(float*)((std::uintptr_t)vehicle + 0xBCC) = 9.81f;
			}
		});
	}
	
	void WorldOptions::trafficManagerMoonGravity()
	{
		auto me = Util::getLocalPlayerPed();
		auto myVehicle = me ? me->m_vehicle : nullptr;
		(*g_Pointers->m_VehiclePool)->forAll([=](ReconstructedClasses::CAutomobile* vehicle)
		{
			if (vehicle != myVehicle && Util::entityDistance(me, vehicle) <= m_TrafficManagerRadius)
			{
				*(float*)((std::uintptr_t)vehicle + 0xBCC) = 1.62f;
			}
		});
	}
	
	void WorldOptions::trafficManagerSunGravity()
	{
		auto me = Util::getLocalPlayerPed();
		auto myVehicle = me ? me->m_vehicle : nullptr;
		(*g_Pointers->m_VehiclePool)->forAll([=](ReconstructedClasses::CAutomobile* vehicle)
		{
			if (vehicle != myVehicle && Util::entityDistance(me, vehicle) <= m_TrafficManagerRadius)
			{
				*(float*)((std::uintptr_t)vehicle + 0xBCC) = 100.f;
			}
		});
	}
	
	void WorldOptions::trafficManagerNoGravity()
	{
		auto me = Util::getLocalPlayerPed();
		auto myVehicle = me ? me->m_vehicle : nullptr;
		(*g_Pointers->m_VehiclePool)->forAll([=](ReconstructedClasses::CAutomobile* vehicle)
		{
			if (vehicle != myVehicle)
			{
				*(float*)((std::uintptr_t)vehicle + 0xBCC) = 0.f;
			}
		});
	}

	void WorldOptions::applyBlackHole()
	{
		static auto timer = GetTickCount();
		if (((GetTickCount() - timer) >= 500))
			return;
		else
			timer = GetTickCount();

		auto func = [this](std::int32_t handle, Vector3 pos)
		{
			auto distance = Util::distance3D(m_BlackholePosition, pos);

			if (distance < 5.f)
			{
				Natives::ENTITY::SET_ENTITY_COORDS(handle, pos.x, pos.y, pos.z, true, true, true, false);
			}
			else if (distance > 20.f)
			{
				Vector3 pos_bhPos = m_BlackholePosition - pos;
				float x = pos_bhPos.x < 0.f ? -pos_bhPos.x : pos_bhPos.x,
					y = pos_bhPos.y < 0.f ? -pos_bhPos.y : pos_bhPos.y;
				float ratio = 1000.f / (x > y ? x : y);

				Natives::ENTITY::APPLY_FORCE_TO_ENTITY(handle, 1 << 0, pos_bhPos.x * ratio, pos_bhPos.y * ratio, pos_bhPos.z, 2, 1, -1, 2, false, false, false, 0, 0);
			}
		};

		auto me = Util::getLocalPlayerPed();
		auto myVehicle = me ? me->m_vehicle : nullptr;

		(*g_Pointers->m_PedPool)->forAll([=](ReconstructedClasses::CPed* ped)
		{
			if (ped != me && ped->m_navigation)
			{
				func(Util::ptrToHandle(ped), ped->m_navigation->v3Loc);
			}
		});

		(*g_Pointers->m_VehiclePool)->forAll([=](ReconstructedClasses::CAutomobile* vehicle)
		{
			if (vehicle != myVehicle && vehicle->m_navigation)
			{
				func(Util::ptrToHandle(vehicle), vehicle->m_navigation->v3Loc);
			}
		});

	}
}
