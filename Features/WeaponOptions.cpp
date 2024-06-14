#include "../PrecompiledHeader.hpp"
#include "WeaponOptions.hpp"
#include "PlayerOptions.hpp"
#include "../Lists.hpp"
#include "../User Interface/DrawHelper.hpp"
#include "../ScriptCallback.hpp"
#include "../Natives.hpp"
#include "../Util.hpp"
#include "../Reconstructed Classes/CWeaponManager.hpp"
#include "../Reconstructed Classes/CWeaponInfo.hpp"
#include "../Reconstructed Classes/CPlayerInfo.hpp"
#include "../Reconstructed Classes/CPed.hpp"
#include "../Reconstructed Classes/CNavigation.hpp"

namespace Vega::Feature
{
	WeaponOptions g_WeaponOptions;

	void WeaponOptions::loop()
	{
		if (m_InfiniteAmmo)
		{
			applyInfiniteAmmo();
		}

		if (m_GravityGun)
		{
			applyGravityGun();
		}
		
		if (m_DeleteGun)
		{
			applyDeleteGun();
		}

		if (m_GravityGun)
		{
			applyGravityGun();
		}

		if (m_SuccGun)
		{
			applySuccGun();
		}

		if (m_FireAmmo)
		{
			g_PlayerOptions.applyFrameFlag(1 << 12);
		}
		
		if (m_ExplosiveAmmo)
		{
			g_PlayerOptions.applyFrameFlag(1 << 11);
		}

		if (m_LaserSight)
		{
			applyLaserSight();
		}

		if (m_RapidFire)
		{
			applyRapidFire();
		}

		if (m_CartoonGun)
		{
			applyCartoonGun();
		}

		if (m_TeleportGun)
		{
			applyTeleportGun();
		}

		if (m_AirstrikeGun)
		{
			applyAirstrikeGun();
		}

		if (m_NoRecoil)
		{
			applyNoRecoil(m_NoRecoil);
		}

		if (m_DriveItGun)
		{
			applyDriveItGun();
		}

		if (m_HashGun)
		{
			applyHashGun();
		}

		if (m_JustCauseGun)
		{
			applyJustCauseGun();
		}

		if (m_PedGun)
		{
			applyPedGun();
		}

		if (m_CoordGun)
		{
			applyCoordGun();
		}

		if (m_ValkyireRocket)
		{
			applyValkyireRocket();
		}

		if (m_EntityShooter)
		{
			applyEntityShooter();
		}
	}

	void WeaponOptions::getWeapons()
	{
		auto pedHandle = Util::getLocalPedHandle();

		for (auto weaponHash : Lists::weaponListFull)
		{
			Natives::WEAPON::GIVE_DELAYED_WEAPON_TO_PED(pedHandle, weaponHash, 9999, false);
		}
	}
	
	void WeaponOptions::removeWeapons()
	{
		Natives::WEAPON::REMOVE_ALL_PED_WEAPONS(Util::getLocalPedHandle(), 1);
	}
	
	void WeaponOptions::applyInfiniteAmmo()
	{

	}
	
	Raycast WeaponOptions::raycast()
	{
		auto start = Natives::CAM::GET_GAMEPLAY_CAM_COORD() +
			(Util::rot2dir(Natives::CAM::GET_GAMEPLAY_CAM_ROT(0)) *
				Util::distance3D(Natives::CAM::GET_GAMEPLAY_CAM_COORD(), Natives::ENTITY::GET_ENTITY_COORDS(Util::getLocalPedHandle(), 1)));
		auto end = Natives::CAM::GET_GAMEPLAY_CAM_COORD() + (Util::rot2dir(Natives::CAM::GET_GAMEPLAY_CAM_ROT(0)) * 5000.f);

		Raycast result = { false,{0.f,0.f,0.f},{0.f,0.f,0.f},0,false };
		auto ray = Natives::SHAPETEST::_START_SHAPE_TEST_RAY(start.x, start.y, start.z, end.x, end.y, end.z, -1, Util::getLocalPedHandle(), 7);
		Natives::SHAPETEST::GET_SHAPE_TEST_RESULT(ray, &result.hitGround, &result.coords, &result.surface, &result.ent);

		if (result.hitGround && Natives::ENTITY::DOES_ENTITY_EXIST(result.ent))
		{
			if (auto ent = (Util::handleToPtr<void*>(result.ent)))
			{
				auto type = reinterpret_cast<std::uint8_t*>(reinterpret_cast<std::uintptr_t>(ent) + 0x28);
				if (*type == Enums::EntityTypeVehicle || *type == Enums::EntityTypePed || *type == Enums::EntityTypeObject)
				{
					result.hit = true;
				}
			}
		}
		return result;
	}

	void WeaponOptions::applyDeleteGun()
	{
		auto ray = raycast();
		if (ray.hit && Natives::PED::IS_PED_SHOOTING(Util::getLocalPedHandle()))
		{
			Natives::ENTITY::SET_ENTITY_COORDS(ray.ent, 0, 0, 0, 0, 0, 0, 0);
			Natives::ENTITY::DELETE_ENTITY(&ray.ent);
		}
	}

	void WeaponOptions::applyGravityGun()
	{
		static bool locked;
		static Natives::Entity last_ent, ent;
		static float dist;
		auto dir = Util::rot2dir(Natives::CAM::GET_GAMEPLAY_CAM_ROT(0));
		auto ray = raycast();

		if (ray.hit && !locked && ray.ent != last_ent) {
			if (Natives::ENTITY::IS_ENTITY_AN_OBJECT(ray.ent))
			{
				Natives::ENTITY::APPLY_FORCE_TO_ENTITY(ray.ent, 0, 0, 0, 0.00001f, 0, 0, 0, 0, 0, 0, 1, 0, 0);
				if (Natives::ENTITY::IS_ENTITY_STATIC(ray.ent)) { return; }
			}
			if (Natives::PAD::IS_CONTROL_PRESSED(2, Enums::ControlAim)) {
				ent = ray.ent; dist = Util::distance3D(Natives::CAM::GET_GAMEPLAY_CAM_COORD(), Natives::ENTITY::GET_ENTITY_COORDS(ray.ent, 1)); locked = true;
				//RequestControlOfEnt(gravity_ent);
			}
		}
		if (Natives::PAD::IS_CONTROL_PRESSED(2, Enums::ControlAim) && locked) {
			Vector3 targt = Natives::CAM::GET_GAMEPLAY_CAM_COORD() + (Util::rot2dir(Natives::CAM::GET_GAMEPLAY_CAM_ROT(0)) * dist);
			Natives::Vector3 minDim, maxDim;
			Natives::MISC::GET_MODEL_DIMENSIONS(Natives::ENTITY::GET_ENTITY_MODEL(ent), &minDim, &maxDim);
			targt.z -= maxDim.z / 2;

			Natives::ENTITY::SET_ENTITY_COORDS_NO_OFFSET(ent, targt.x, targt.y, targt.z, 0, 0, 0);
			if (Natives::PED::IS_PED_SHOOTING(Util::getLocalPedHandle())) {
				Natives::ENTITY::APPLY_FORCE_TO_ENTITY(ent, 1, dir.x * 500, dir.y * 500, dir.z * 500, 0, 0, 0, 0, 0, 1, 1, 0, 1);
				last_ent = ent;
				locked = false;
			}
		}
		else
		{
			locked = false;
		}
	}

	void WeaponOptions::applySuccGun()
	{
		//if (Natives::PAD::IS_CONTROL_PRESSED(0, Enums::ControlAttack) && Util::isPlayerFreeAiming(Util::getLocalPlayerInfo()))
		if (Natives::PED::IS_PED_SHOOTING(Util::getLocalPedHandle()))
		{
			if (auto ray = raycast(); ray.hit && ray.ent != -1)
			{
				auto dir = -(Util::rot2dir(Natives::CAM::GET_GAMEPLAY_CAM_ROT(0)) * 10.f);

				Natives::ENTITY::APPLY_FORCE_TO_ENTITY(
					ray.ent, 1,
					dir.x, dir.y, dir.z,
					0.f, 0.f, 0.f,
					NULL, false, true, true, false, true);
			}
		}
	}
	void WeaponOptions::applyLaserSight()
	{
		auto weapon = Natives::WEAPON::GET_CURRENT_PED_WEAPON_ENTITY_INDEX(Util::getLocalPedHandle());
		if (Natives::ENTITY::DOES_ENTITY_EXIST(weapon) && Util::isPlayerFreeAiming(Util::getLocalPlayerInfo()))
		{
			auto coord = Util::getEntityPositionHandle(weapon);
			auto end = Natives::CAM::GET_GAMEPLAY_CAM_COORD() + (Util::rot2dir(Natives::CAM::GET_GAMEPLAY_CAM_ROT(0)) * 1000.f);
			Natives::GRAPHICS::DRAW_LINE(coord.x, coord.y, coord.z, end.x, end.y, end.z, 255, 0, 0, 255);
			auto dir = Util::rot2dir(Natives::CAM::GET_GAMEPLAY_CAM_ROT(0));
			Natives::GRAPHICS::DRAW_SPOT_LIGHT(coord.x, coord.y, coord.z, dir.x, dir.y, dir.z, 255, 255, 255, 1000.f, 10.f, 10.f, 5.f, 0.f);
		}
	}

	void WeaponOptions::applyRapidFire()
	{
		auto camCoord = Natives::CAM::GET_GAMEPLAY_CAM_COORD();
		auto camRot = Natives::CAM::GET_GAMEPLAY_CAM_ROT(0);
		auto weapon = Natives::WEAPON::GET_CURRENT_PED_WEAPON_ENTITY_INDEX(Util::getLocalPedHandle());

		auto pedHandle = Util::getLocalPedHandle();

		auto start = Util::getEntityPositionHandle(weapon);
		auto end = camCoord + (Util::rot2dir(camRot) * 1000.f);

		Natives::Hash hash;
		if (
			Natives::WEAPON::GET_CURRENT_PED_WEAPON(pedHandle, &hash, true)
			&& (Natives::PAD::IS_CONTROL_PRESSED(2, Enums::ControlAttack)
			||  Natives::PAD::IS_CONTROL_PRESSED(2, Enums::ControlFrontendRt)
			||  Util::isKeyPressed(VK_LBUTTON)))
		{
			Natives::MISC::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(start.x, start.y, start.z, end.x, end.y, end.z, 50, true, hash, pedHandle, true, false, 500.f);
		}
	}

	void WeaponOptions::applyCartoonGun()
	{
		auto pedHandle = Util::getLocalPedHandle();

		if (Natives::PED::IS_PED_SHOOTING(Util::getLocalPedHandle()))
		{
			g_ScriptCallbackManager->addCallback(ParticleFXScriptCallback(XorString("scr_rcbarry2"), [=]
			{
				auto weapon = Natives::WEAPON::GET_CURRENT_PED_WEAPON_ENTITY_INDEX(pedHandle);
				auto coord = Natives::ENTITY::GET_WORLD_POSITION_OF_ENTITY_BONE(weapon, Natives::ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(weapon, "gun_muzzle"));

				Natives::Vector3 v0, v1;
				Natives::MISC::GET_MODEL_DIMENSIONS(Natives::WEAPON::GET_SELECTED_PED_WEAPON(pedHandle), &v0, &v1);

				Natives::GRAPHICS::_USE_PARTICLE_FX_ASSET_NEXT_CALL("scr_rcbarry2");
				Natives::GRAPHICS::_START_PARTICLE_FX_NON_LOOPED_ON_ENTITY_2("muz_clown", weapon, (v0.x - v1.x) / 2.f + 0.7f, 0.f, 0.f, 0.f, 180.f, 0.f, 1.f, 1, 1, 1);
			}));
		}
	}

	void WeaponOptions::applyTeleportGun()
	{
		auto player = Util::getLocalPlayerInfo();
		auto ped = player->m_ped;
		if (player && Natives::PED::IS_PED_SHOOTING(Util::getLocalPedHandle()))
		{
			//auto coord = player->v4AimingPos;
			//Natives::ENTITY::SET_ENTITY_COORDS_NO_OFFSET(Util::getLocalPedHandle(), coord.x, coord.y, coord.z, 0, 0, 0);
			//ped->m_navigation->v3Loc = coord;
		}
	}

	void WeaponOptions::applyAirstrikeGun()
	{
		auto player = Util::getLocalPlayerInfo();
		if (player && Natives::PED::IS_PED_SHOOTING(Util::getLocalPedHandle()))
		{
			auto hash = "WEAPON_AIRSTRIKE_ROCKET"_joaat;
			//auto coord = player->v4AimingPos;

			g_ScriptCallbackManager->addCallback(WeaponScriptCallback(hash, [=]
			{
				//Natives::MISC::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(coord.x, coord.y, coord.z + 50.f, coord.x, coord.y, coord.z, 250, 1, hash, Util::getLocalPedHandle(), 1, 0, 10000.f);
			}));
		}
	}

	void WeaponOptions::applyNoRecoil(const bool t)
	{
		if (auto ped = Util::getLocalPlayerPed())
		{
			ped->m_weapon_manager->currentWeapon->fRecoil = !t;
			ped->m_weapon_manager->currentWeapon->fRecoilFirstPerson = !t;
			ped->m_weapon_manager->currentWeapon->fRecoilThirdPerson = !t;
		}
	}

	void WeaponOptions::applyDriveItGun()
	{
		auto ray = raycast();
		if (ray.hit && Natives::PED::IS_PED_SHOOTING(Util::getLocalPedHandle()) && Natives::ENTITY::IS_ENTITY_A_VEHICLE(ray.ent))
		{
			auto driver = Natives::VEHICLE::GET_PED_IN_VEHICLE_SEAT(ray.ent, -1, NULL);
			//RequestControlOfEnt(driver);
			Natives::BRAIN::CLEAR_PED_TASKS_IMMEDIATELY(driver);
			Natives::BRAIN::CLEAR_PED_TASKS(driver);
			Natives::BRAIN::CLEAR_PED_SECONDARY_TASK(driver);
			Natives::PED::SET_PED_INTO_VEHICLE(Natives::PLAYER::PLAYER_PED_ID(), ray.ent, -1);
		}
	}

	void WeaponOptions::applyHashGun()
	{
		
		if (Util::isPlayerFreeAiming(Util::getLocalPlayerInfo()))
		{
			if (auto ray = raycast(); ray.hit)
			{
				if (Natives::PED::IS_PED_SHOOTING(Util::getLocalPedHandle()))
				{
					Util::simpleNotify(20, "Hash: 0x%x", Util::getEntityModelHandle(ray.ent));
				}
			}
		}
	}

	void WeaponOptions::applyJustCauseGun()
	{
		static bool attach = false;
		static Natives::Entity rope = -1;
		static Natives::Entity ent = -1;

		auto ray = raycast();
		if (Util::isPlayerFreeAiming(Util::getLocalPlayerInfo()))
		{
			if (ray.hit && Natives::PED::IS_PED_SHOOTING(Util::getLocalPedHandle()))
			{
				Natives::PHYSICS::ROPE_LOAD_TEXTURES();
				auto weapon = Natives::WEAPON::GET_CURRENT_PED_WEAPON_ENTITY_INDEX(Util::getLocalPedHandle());
				auto w_coord = Natives::ENTITY::GET_ENTITY_COORDS(weapon, 1);
				if (!attach)
				{
					rope = Natives::PHYSICS::ADD_ROPE(w_coord.x, w_coord.y, w_coord.z, 0, 0, 0,
						Util::distance3D(Natives::ENTITY::GET_ENTITY_COORDS(ray.ent, 1), w_coord), 1, 300, 0.5f, 0.5f, false, true, true, 1.0f, false, 0);
					auto c1 = Natives::ENTITY::GET_ENTITY_COORDS(ray.ent, 1);
					Natives::PHYSICS::ATTACH_ENTITIES_TO_ROPE(rope, weapon, ray.ent, w_coord.x, w_coord.y, w_coord.z, c1.x, c1.y, c1.z,
						Util::distance3D(Natives::ENTITY::GET_ENTITY_COORDS(ray.ent, 1), w_coord) + 10.f, 0, 0, NULL, NULL);
					Natives::PHYSICS::ACTIVATE_PHYSICS(rope);
					ent = ray.ent;
					attach = true;
				}
				else
				{
					Natives::PHYSICS::DELETE_ROPE(&rope);
					auto c0 = Natives::ENTITY::GET_ENTITY_COORDS(ent, 1);
					auto c1 = Natives::ENTITY::GET_ENTITY_COORDS(ray.ent, 1);
					rope = Natives::PHYSICS::ADD_ROPE(w_coord.x, w_coord.y, w_coord.z, 0, 0, 0,
						Util::distance3D(Natives::ENTITY::GET_ENTITY_COORDS(ent, 1), Natives::ENTITY::GET_ENTITY_COORDS(ray.ent, 1)), 1, 300, 0.5f, 0.5f, false, true, true, 1.0f, false, 0);
					Natives::PHYSICS::ACTIVATE_PHYSICS(rope);
					Natives::PHYSICS::ATTACH_ENTITIES_TO_ROPE(rope, ent, ray.ent, c0.x, c0.y, c0.z, c1.x, c1.y, c1.z,
						Util::distance3D(Natives::ENTITY::GET_ENTITY_COORDS(ent, 1), Natives::ENTITY::GET_ENTITY_COORDS(ray.ent, 1)), 0, 0, NULL, NULL);
					rope = -1;
					attach = false;
				}
			}
		}
		else 
		{
			Natives::PHYSICS::DELETE_ROPE(&rope);
			attach = false;
		}
	}

	void WeaponOptions::dildoKnife()
	{
		auto hash = "prop_cs_dildo_01"_joaat;
		g_ScriptCallbackManager->addCallback(ModelScriptCallback(hash, [=]
		{
			auto ped = Util::getLocalPedHandle();
			Natives::WEAPON::GIVE_DELAYED_WEAPON_TO_PED(ped, "WEAPON_KNIFE"_joaat, 1, TRUE);
			Natives::WEAPON::SET_CURRENT_PED_WEAPON(ped, "WEAPON_KNIFE"_joaat, 1);

			auto knife = Natives::WEAPON::GET_CURRENT_PED_WEAPON_ENTITY_INDEX(ped);
			auto c = Natives::ENTITY::GET_ENTITY_COORDS(knife, 1);
			auto dildo = Natives::OBJECT::CREATE_OBJECT(hash, c.x, c.y, c.z, 1, 1, 0);
			Natives::ENTITY::SET_ENTITY_ALPHA(knife, 0, 0);
			Natives::Vector3 v0, v1;
			Natives::MISC::GET_MODEL_DIMENSIONS(hash, &v0, &v1);
			Natives::ENTITY::ATTACH_ENTITY_TO_ENTITY(dildo, knife, 0, v1.z, 0, 0, 0, 0, 0, 1, 1, 0, 0, 2, 1);
		}));

	}

	void WeaponOptions::applyPedGun()
	{
		auto playerInfo = Util::getLocalPlayerInfo();
		if (playerInfo && playerInfo->m_ped && Natives::PED::IS_PED_SHOOTING(Util::getLocalPedHandle()))
		{
			//auto coord = playerInfo->v4AimingPos;
			//Natives::PED::CREATE_RANDOM_PED(coord.x, coord.y, coord.z);
		}
	}

	void WeaponOptions::applyCoordGun()
	{
		auto player = Util::getLocalPlayerInfo();
		if (player && Natives::PED::IS_PED_SHOOTING(Util::getLocalPedHandle()))
		{
			//auto coord = player->v4AimingPos;
			//Util::simpleNotify(40, ("Position: X" + std::to_string(coord.x) + " Y" + std::to_string(coord.y) + " Z" + std::to_string(coord.z)).c_str());
		}
	}

	void WeaponOptions::applyValkyireRocket()
	{
		static std::int32_t rocket, cam;
		static std::uint8_t y;
		static float bar;
		static bool init;
		if (Natives::PED::IS_PED_SHOOTING(Util::getLocalPedHandle()))
		{
			init = true;
		}

		if (init)
		{
			if (!Natives::ENTITY::DOES_ENTITY_EXIST(rocket)) {
				auto weapon = Natives::WEAPON::GET_CURRENT_PED_WEAPON_ENTITY_INDEX(Util::getLocalPedHandle());
				auto c = Natives::ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(weapon, 0.f, 1.f, 0.f);
				rocket = Natives::OBJECT::CREATE_OBJECT("w_lr_rpg_rocket"_joaat, c.x, c.y, c.z, 1, 1, 0);
				Natives::CAM::DESTROY_ALL_CAMS(true);
				cam = Natives::CAM::CREATE_CAM("DEFAULT_SCRIPTED_CAMERA", 1);
				Natives::CAM::ATTACH_CAM_TO_ENTITY(cam, rocket, 0.f, 0.f, 0.f, 1);
				Natives::CAM::RENDER_SCRIPT_CAMS(1, true, 700, 1, 1, NULL);
				Natives::CAM::SET_CAM_ACTIVE(cam, 1);
				Natives::ENTITY::SET_ENTITY_VISIBLE(rocket, 0, 0);
				bar = .5f; y = 255;
			}
			else
			{
				Natives::CAM::SET_CAM_ROT(cam, Natives::CAM::GET_GAMEPLAY_CAM_ROT(0).x, Natives::CAM::GET_GAMEPLAY_CAM_ROT(0).y, Natives::CAM::GET_GAMEPLAY_CAM_ROT(0).z, 0);
				Natives::ENTITY::SET_ENTITY_ROTATION(rocket, Natives::CAM::GET_GAMEPLAY_CAM_ROT(0).x, Natives::CAM::GET_GAMEPLAY_CAM_ROT(0).y, Natives::CAM::GET_GAMEPLAY_CAM_ROT(0).z, 0, 1);

				auto c = Natives::ENTITY::GET_ENTITY_COORDS(rocket, 1) + (Util::rot2dir(Natives::CAM::GET_GAMEPLAY_CAM_ROT(0)) * .8f);
				Natives::ENTITY::SET_ENTITY_COORDS(rocket, c.x, c.y, c.z, 0, 0, 0, 0);

				Natives::HUD::HIDE_HUD_AND_RADAR_THIS_FRAME();
				Natives::PLAYER::DISABLE_PLAYER_FIRING(Util::getLocalPedHandle(), 1);
				Natives::ENTITY::FREEZE_ENTITY_POSITION(Util::getLocalPedHandle(), 1);
				Natives::HUD::_0x0AFC4AF510774B47();

				g_DrawHelper.rect({ 0.5f, 0.5f - 0.025f }, { 0.050f, 0.002f }, { 255, 255, 255, 255 });
				g_DrawHelper.rect({ 0.5f, 0.5f + 0.025f }, { 0.050f, 0.002f }, { 255, 255, 255, 255 });
				g_DrawHelper.rect({ 0.5f - 0.025f, 0.5f }, { 0.002f, 0.050f }, { 255, 255, 255, 255 });
				g_DrawHelper.rect({ 0.5f + 0.025f, 0.5f }, { 0.002f, 0.050f }, { 255, 255, 255, 255 });
				g_DrawHelper.rect({ 0.5f + 0.05f, 0.5f }, { 0.050f, 0.002f }, { 255, 255, 255, 255 });
				g_DrawHelper.rect({ 0.5f - 0.05f, 0.5f }, { 0.050f, 0.002f }, { 255, 255, 255, 255 });
				g_DrawHelper.rect({ 0.5f, 0.5f + 0.05f }, { 0.002f, 0.050f }, { 255, 255, 255, 255 });
				g_DrawHelper.rect({ 0.5f, 0.5f - 0.05f }, { 0.002f, 0.050f }, { 255, 255, 255, 255 });
				Natives::GRAPHICS::SET_TIMECYCLE_MODIFIER("CAMERA_secuirity");

				g_DrawHelper.rect({ 0.25f, 0.5f }, { 0.03f, 0.5f }, { 255, 255, 255, 255 });
				static auto ticker = GetTickCount();
				if (GetTickCount() - ticker >= 100) {
					bar -= .01f; y -= 4; ticker = GetTickCount();
				}
				g_DrawHelper.rect({ 0.25f, 0.75f - (bar / 2) }, { 0.03f, bar }, { 255, y, 0, 255 });

				float groundZ;
				Natives::MISC::GET_GROUND_Z_FOR_3D_COORD(Natives::ENTITY::GET_ENTITY_COORDS(rocket, 1).x, Natives::ENTITY::GET_ENTITY_COORDS(rocket, 1).y, Natives::ENTITY::GET_ENTITY_COORDS(rocket, 1).z, &groundZ, 0);
				if (Natives::ENTITY::HAS_ENTITY_COLLIDED_WITH_ANYTHING(rocket) ||
					(std::abs(Natives::ENTITY::GET_ENTITY_COORDS(rocket, 1).z - groundZ) < .5f) ||
					bar <= 0.01) {
					auto impact_coord = Natives::ENTITY::GET_ENTITY_COORDS(rocket, 1); Natives::ENTITY::FREEZE_ENTITY_POSITION(Util::getLocalPedHandle(), 0);
					Natives::FIRE::ADD_EXPLOSION(impact_coord.x, impact_coord.y, impact_coord.z, 0, 10.f, 1, 0, .4f, false);
					Natives::ENTITY::DELETE_ENTITY(&rocket);
					rocket = 0;
					Natives::PLAYER::DISABLE_PLAYER_FIRING(Util::getLocalPedHandle(), 0);
					Natives::CAM::RENDER_SCRIPT_CAMS(0, true, 700, 1, 1, NULL);
					Natives::CAM::DESTROY_CAM(cam, 1);
					Natives::GRAPHICS::SET_TIMECYCLE_MODIFIER("DEFAULT");
					init = false;
				}
			}
		}

		if (!m_ValkyireRocket)
		{
			Natives::CAM::DESTROY_CAM(cam, 1);
			Natives::PLAYER::DISABLE_PLAYER_FIRING(Util::getLocalPedHandle(), 0);
			rocket = 0;
			bar = .5f;
			y = 255;
			Natives::ENTITY::FREEZE_ENTITY_POSITION(Util::getLocalPedHandle(), 0);
		}
	}

	void WeaponOptions::applyEntityShooter()
	{
		if (Natives::PED::IS_PED_SHOOTING(Util::getLocalPedHandle()))
		{
			auto rot = Natives::CAM::GET_GAMEPLAY_CAM_ROT(0);
			auto coord = Natives::CAM::GET_GAMEPLAY_CAM_COORD() + (Util::rot2dir(rot) * 12.f);

			switch (m_EntityShooterType)
			{
			case 0:
			{
				auto hash = Lists::vehicleShooterList[m_VehicleShooterType];
				g_ScriptCallbackManager->addCallback(ModelScriptCallback(hash, [=]
				{
					auto bul = Natives::VEHICLE::CREATE_VEHICLE(hash, coord.x, coord.y, coord.z, 0.f, 1, 0, NULL);
					if (m_RandomVehicleColor)
					{
						Natives::VEHICLE::SET_VEHICLE_CUSTOM_PRIMARY_COLOUR(bul, rand() % 255, rand() % 255, rand() % 255);
						Natives::VEHICLE::SET_VEHICLE_CUSTOM_SECONDARY_COLOUR(bul, rand() % 255, rand() % 255, rand() % 255);
					}
					Natives::ENTITY::SET_ENTITY_ROTATION(bul, rot.x, rot.y, rot.z, 0, 1);
					Natives::ENTITY::APPLY_FORCE_TO_ENTITY(bul, 1, 0.f, 500.f, 0.f, 0.f, 0.f, 0.f, 0, true, false, true, 0, 0);
				}));
				break;
			}

			break;
			case 1:
			{
				auto hash = Lists::namedProps[m_PropShooterType].hash;
				Natives::STREAMING::REQUEST_MODEL(hash);
				auto bul = Natives::OBJECT::CREATE_OBJECT(hash, coord.x, coord.y, coord.z, true, 0, 1);
				Natives::ENTITY::SET_ENTITY_HEADING(bul, Natives::ENTITY::GET_ENTITY_HEADING(Util::getLocalPedHandle()));
				Natives::ENTITY::APPLY_FORCE_TO_ENTITY(bul, 1, Util::rot2dir(rot).x * 500.f, Util::rot2dir(rot).y * 500.f, Util::rot2dir(rot).z * 500.f, 0, 0, 0, 0, 0, 1, 1, 0, 1); }
			break;
			case 2:
			{
				auto hash = Lists::popularPeds[m_PedShooterType].hash;
				Natives::STREAMING::REQUEST_MODEL(hash);
				auto bul = Natives::PED::CREATE_PED(0, hash, coord.x, coord.y, coord.z, Natives::ENTITY::GET_ENTITY_HEADING(Util::getLocalPedHandle()), true, false);
				Natives::ENTITY::SET_ENTITY_COORDS(bul, coord.x, coord.y, coord.z, 0, 0, 0, 0);
				Natives::ENTITY::APPLY_FORCE_TO_ENTITY(bul, 1, Util::rot2dir(rot).x * 500.f, Util::rot2dir(rot).y * 500.f, Util::rot2dir(rot).z * 500.f, 0, 0, 0, 0, 0, 1, 1, 0, 1); 
			}
			break;
			}
		}
	}
}