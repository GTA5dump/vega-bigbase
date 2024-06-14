#include "../PrecompiledHeader.hpp"
#include "SpoonerOptions.hpp"
#include "../ScriptCallback.hpp"
#include "../User Interface/InstructionalButtons.hpp"

#include "../Reconstructed Classes/CPedFactory.hpp"
#include "../Reconstructed Classes/CPed.hpp"
#include "../Reconstructed Classes/CPlayerInfo.hpp"
#include "../Reconstructed Classes/CNavigation.hpp"
#include "WeaponOptions.hpp"
#include "../Util.hpp"
#include "../User Interface/DrawHelper.hpp"
#include "../User Interface/Manager.hpp"

namespace Vega::Feature
{
	SpoonerOptions g_SpoonerOptions;

	void SpoonerOptions::loop()
	{
		if (m_Spooner)
		{
			applySpooner();
		}
	}

	void SpoonerOptions::applySpooner()
	{
		static bool lock;
		static std::int32_t cam, ent;
		static float dist;
		auto rot = Natives::CAM::GET_GAMEPLAY_CAM_ROT(0);
		auto coord = Natives::CAM::GET_GAMEPLAY_CAM_COORD();
		Vector3 p_coord = { 0,0,0 };
		if (!Natives::CAM::DOES_CAM_EXIST(cam)) {
			cam = Natives::CAM::CREATE_CAM("DEFAULT_SCRIPTED_CAMERA", 1);
			Natives::CAM::SET_CAM_ROT(cam, rot.x, rot.y, rot.z, 0);
			Natives::CAM::SET_CAM_COORD(cam, coord.x, coord.y, coord.z);
		}
		if (!m_Spooner)
		{
			Natives::CAM::RENDER_SCRIPT_CAMS(false, true, 700, 1, 1, NULL);
			Natives::CAM::SET_CAM_ACTIVE(cam, 0);
			Natives::CAM::DESTROY_CAM(cam, true);
			Natives::PLAYER::DISABLE_PLAYER_FIRING(Util::getLocalPedHandle(), 0);
		}

		Natives::CAM::RENDER_SCRIPT_CAMS(true, true, 700, 1, 1, NULL);
		Natives::CAM::SET_CAM_ACTIVE(cam, 1);
		Natives::CAM::SET_CAM_ROT(cam, rot.x, rot.y, rot.z, 0);

		auto ped = Util::getLocalPlayerPed();
		if (ped)
		{
			p_coord = ped->m_navigation->v3Loc;
		}

		Natives::ENTITY::SET_ENTITY_COORDS_NO_OFFSET(Util::getLocalPedHandle(), p_coord.x, p_coord.y, p_coord.z, 0, 0, 0);
		Natives::PLAYER::DISABLE_PLAYER_FIRING(Util::getLocalPedHandle(), 1);
		Natives::HUD::HIDE_HUD_AND_RADAR_THIS_FRAME();
		Natives::HUD::_0x0AFC4AF510774B47();

		auto speed = .26f;
		if (Util::isKeyPressed(VK_LSHIFT) && Util::isGameFocused())
		{
			speed += .4f;
		}
		if (Util::isKeyPressed(VK_SPACE) && Util::isGameFocused())
		{
			speed -= .2f;
		}

		if (Util::isKeyPressed(VK_KEY_S) && Util::isGameFocused())
		{
			speed *= -1;
			auto c = Natives::CAM::GET_CAM_COORD(cam) + (Util::rot2dir(rot) * speed);
			Natives::CAM::SET_CAM_COORD(cam, c.x, c.y, c.z);
		}

		if (Util::isKeyPressed(VK_KEY_W) && Util::isGameFocused())
		{
			auto c = Natives::CAM::GET_CAM_COORD(cam) + (Util::rot2dir(rot) * speed);
			Natives::CAM::SET_CAM_COORD(cam, c.x, c.y, c.z);
		}

		Natives::GRAPHICS::REQUEST_STREAMED_TEXTURE_DICT("shared", 0);
		g_DrawHelper.sprite("shared", "menuplus_32", { 0.5f,0.5f }, { 0.015f,0.02f }, { 255,0,0,100 });

		auto start = Natives::CAM::GET_CAM_COORD(cam);
		auto end = Natives::CAM::GET_CAM_COORD(cam) + (Util::rot2dir(Natives::CAM::GET_GAMEPLAY_CAM_ROT(0)) * 5000.f);

		Raycast ray = { false,{0.f,0.f,0.f},{0.f,0.f,0.f},0,false };
		auto rayy = Natives::SHAPETEST::_START_SHAPE_TEST_RAY(start.x, start.y, start.z, end.x, end.y, end.z, -1, Util::getLocalPedHandle(), 7);
		Natives::SHAPETEST::GET_SHAPE_TEST_RESULT(rayy, &ray.hitGround, &ray.coords, &ray.surface, &ray.ent);

		if (ray.hitGround && Natives::ENTITY::DOES_ENTITY_EXIST(ray.ent))
		{
			if (auto eent = (Util::handleToPtr<void*>(ray.ent)))
			{
				auto type = reinterpret_cast<std::uint8_t*>(reinterpret_cast<std::uintptr_t>(eent) + 0x28);
				if (*type == Enums::EntityTypeVehicle || *type == Enums::EntityTypePed || *type == Enums::EntityTypeObject)
				{
					ray.hit = true;
				}
			}
		}
		
		if (ray.hit)
		{
			g_DrawHelper.sprite("shared", "menuplus_32", { 0.5f,0.5f }, { 0.015f,0.02f }, { 255,0,0,255 });
			draw_ent_box(ray.ent);
			if ((Natives::PAD::IS_CONTROL_PRESSED(2, Enums::ControlAttack) || Natives::PAD::IS_CONTROL_PRESSED(2, Enums::ControlFrontendRt) || Util::isKeyPressed(VK_LBUTTON)) && !lock)
			{
				dist = Util::distance3D(Natives::CAM::GET_CAM_COORD(cam), Natives::ENTITY::GET_ENTITY_COORDS(ray.ent, true));
				ent = ray.ent;
				lock = true;
				// net ctrl
			}
		}
		if ((Natives::PAD::IS_CONTROL_PRESSED(2, Enums::ControlAttack) || Natives::PAD::IS_CONTROL_PRESSED(2, Enums::ControlFrontendRt) || Util::isKeyPressed(VK_LBUTTON)) && lock && Util::isGameFocused())
		{
			g_DrawHelper.sprite("shared", "menuplus_32", { 0.5f,0.5f }, { 0.015f,0.02f }, { 255,0,0,255 });
			draw_ent_box(ent);
			static int axis = 2;
			auto obj_rot = Natives::ENTITY::GET_ENTITY_ROTATION(ent, 0);
			auto n_coord = Natives::CAM::GET_CAM_COORD(cam) + (Util::rot2dir(Natives::CAM::GET_GAMEPLAY_CAM_ROT(0)) * dist);

			Natives::ENTITY::SET_ENTITY_COORDS_NO_OFFSET(ent, n_coord.x, n_coord.y, n_coord.z, 0, 0, 0);

			if (Util::isKeyPressed(VK_KEY_Q))
			{
				switch (axis) {
				case 0:
					Natives::ENTITY::SET_ENTITY_ROTATION(ent, obj_rot.x -= 1, obj_rot.y, obj_rot.z, 0, 0);
					break;
				case 1:
					Natives::ENTITY::SET_ENTITY_ROTATION(ent, obj_rot.x, obj_rot.y -= 1, obj_rot.z, 0, 0);
					break;
				case 2:
					Natives::ENTITY::SET_ENTITY_ROTATION(ent, obj_rot.x, obj_rot.y, obj_rot.z -= 1, 0, 0);
					break;
				}
			}

			if (Util::isKeyPressed(VK_KEY_E))
			{
				switch (axis) {
				case 0:
					Natives::ENTITY::SET_ENTITY_ROTATION(ent, obj_rot.x += 1, obj_rot.y, obj_rot.z, 0, 0);
					break;
				case 1:
					Natives::ENTITY::SET_ENTITY_ROTATION(ent, obj_rot.x, obj_rot.y += 1, obj_rot.z, 0, 0);
					break;
				case 2:
					Natives::ENTITY::SET_ENTITY_ROTATION(ent, obj_rot.x, obj_rot.y, obj_rot.z += 1, 0, 0);
					break;
				}
			}

			static auto ticker = GetTickCount();

			if (Util::isKeyPressed(VK_MBUTTON))
			{
				Natives::ENTITY::SET_ENTITY_COORDS(ent, 0.f, 0.f, 0.f, 0, 0, 0, 0);
				Natives::ENTITY::DELETE_ENTITY(&ent);
				lock = false;
			}

			if (Util::isKeyPressed(VK_RBUTTON) && GetTickCount() - ticker >= 200)
			{
				SavedEntity t;
				std::snprintf(t.m_Name, sizeof(t.m_Name), "0x%X", Natives::ENTITY::GET_ENTITY_MODEL(ent));
				t.m_Handle = ent;
				m_Entities.push_back(std::move(t));
				ticker = GetTickCount();
			}

			if (Util::isKeyPressed(VK_TAB) && GetTickCount() - ticker >= 200)
			{
				if (axis == 2) { axis = 0; }
				else { axis++; } 
				ticker = GetTickCount();
			}
		}
		else
		{
			lock = false;
		}

		g_UIManager->m_HideInstructionalButtonsThisFrame = true;
		m_Buttons->addById(XorString("Grab Object"), 100);
		if (lock)
		{
			m_Buttons->addById(XorString("Delete Object"), 102);
			m_Buttons->addById(XorString("Add To DB"), 101);
			m_Buttons->addCustom(XorString("Change Axis"), "TAB");
			m_Buttons->addCustom(XorString("Rotate -"), "E");
			m_Buttons->addCustom(XorString("Rotate +"), "Q");
		}
		m_Buttons->addCustom(XorString("Forward"), "W");
		m_Buttons->addCustom(XorString("Backward"), "S");
		m_Buttons->draw();
	}

	void SpoonerOptions::spawnProp(const char* const hashName)
	{
		auto hash = Util::joaat(hashName);

		Vector3 coord{};

		if (m_Spooner)
		{
			coord = Natives::CAM::GET_CAM_COORD(Natives::CAM::GET_RENDERING_CAM()) + (Util::rot2dir(Natives::CAM::GET_GAMEPLAY_CAM_ROT(0)) * 10.f);
		}
		else
		{
			coord = Natives::CAM::GET_GAMEPLAY_CAM_COORD() + (Util::rot2dir(Natives::CAM::GET_GAMEPLAY_CAM_ROT(0)) * (
				Util::distance3D(Natives::CAM::GET_GAMEPLAY_CAM_COORD(), Natives::ENTITY::GET_ENTITY_COORDS(Util::getLocalPedHandle(), true)) + 5.f));
		}

		g_ScriptCallbackManager->addCallback(ModelScriptCallback(hash, [=]
		{
			SavedEntity spawned;
			std::strncpy(spawned.m_Name, hashName, sizeof(spawned.m_Name));
			spawned.m_Handle = Natives::OBJECT::CREATE_OBJECT(hash, coord.x, coord.y, coord.z, true, 0, true);
			spawned.m_Freezed = m_SpawnFrozen;
			spawned.m_Dynamic = m_SpawnDynamic;
			spawned.m_Collision = m_SpawnCollision;
			spawned.m_Gravity = m_SpawnGravity;
			Natives::ENTITY::SET_ENTITY_DYNAMIC(spawned.m_Handle, m_SpawnDynamic);
			Natives::ENTITY::FREEZE_ENTITY_POSITION(spawned.m_Handle, m_SpawnFrozen);
			Natives::ENTITY::SET_ENTITY_COLLISION(spawned.m_Handle, m_SpawnCollision, true);
			Natives::ENTITY::SET_ENTITY_HAS_GRAVITY(spawned.m_Handle, m_SpawnGravity);

			m_Entities.push_back(std::move(spawned));
		}));
	}

	void SpoonerOptions::spawnPed(const char* const hashName)
	{
		auto hash = Util::joaat(hashName);

		Vector3 coord{};

		if (m_Spooner)
		{
			coord = Natives::CAM::GET_CAM_COORD(Natives::CAM::GET_RENDERING_CAM()) + (Util::rot2dir(Natives::CAM::GET_GAMEPLAY_CAM_ROT(0)) * 10.f);
		}
		else
		{
			coord = Natives::CAM::GET_GAMEPLAY_CAM_COORD() + (Util::rot2dir(Natives::CAM::GET_GAMEPLAY_CAM_ROT(0)) * (
				Util::distance3D(Natives::CAM::GET_GAMEPLAY_CAM_COORD(), Natives::ENTITY::GET_ENTITY_COORDS(Util::getLocalPedHandle(), true)) + 5.f));
		}

		g_ScriptCallbackManager->addCallback(ModelScriptCallback(hash, [=]
		{
			SavedEntity spawned;
			std::strncpy(spawned.m_Name, hashName, sizeof(spawned.m_Name));
			spawned.m_Handle = Natives::PED::CREATE_PED(0, hash, coord.x, coord.y, coord.z, Util::getEntityHeading(Util::getLocalPlayerPed()), true, 0);
			spawned.m_Freezed = m_SpawnFrozen;
			spawned.m_Dynamic = m_SpawnDynamic;
			spawned.m_Collision = m_SpawnCollision;
			spawned.m_Gravity = m_SpawnGravity;
			Natives::ENTITY::SET_ENTITY_DYNAMIC(spawned.m_Handle, m_SpawnDynamic);
			Natives::ENTITY::FREEZE_ENTITY_POSITION(spawned.m_Handle, m_SpawnFrozen);
			Natives::ENTITY::SET_ENTITY_COLLISION(spawned.m_Handle, m_SpawnCollision, true);
			Natives::ENTITY::SET_ENTITY_HAS_GRAVITY(spawned.m_Handle, m_SpawnGravity);

			m_Entities.push_back(std::move(spawned));
		}));
	}

	void SpoonerOptions::Search() {
		g_ScriptCallbackManager->addCallback(KeyboardScriptCallback(XorString("Search"), 32, [=](const char* result)
		{
			m_Search = result;
		}));
	}

	void SpoonerOptions::SearchPeds() {
		g_ScriptCallbackManager->addCallback(KeyboardScriptCallback(XorString("Search"), 32, [=](const char* result)
		{
			m_SearchPed = result;
		}));
	}

	void SpoonerOptions::draw_ent_box(std::int32_t ent)
	{
		Natives::Vector3 minDimensions = { 0,0,0 }, maxDimensions = { 0,0,0 }; // vector ptrs fucked pl0x fix
		auto entityModel = Natives::ENTITY::GET_ENTITY_MODEL(ent);
		Natives::MISC::GET_MODEL_DIMENSIONS(entityModel, &minDimensions, &maxDimensions);

		auto one = Natives::ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(ent, maxDimensions.x / -1, maxDimensions.y / -1, +maxDimensions.z);
		auto two = Natives::ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(ent, maxDimensions.x, maxDimensions.y / -1, +maxDimensions.z);
		auto three = Natives::ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(ent, maxDimensions.x, maxDimensions.y, +maxDimensions.z);
		auto four = Natives::ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(ent, maxDimensions.x / -1, maxDimensions.y, +maxDimensions.z);

		auto one_ = Natives::ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(ent, maxDimensions.x / -1, maxDimensions.y / -1, maxDimensions.z / -1);
		auto two_ = Natives::ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(ent, maxDimensions.x, maxDimensions.y / -1, +maxDimensions.z / -1);
		auto three_ = Natives::ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(ent, maxDimensions.x, maxDimensions.y, +maxDimensions.z / -1);
		auto four_ = Natives::ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(ent, maxDimensions.x / -1, maxDimensions.y, +maxDimensions.z / -1);

		Natives::GRAPHICS::DRAW_LINE(one.x, one.y, one.z, two.x, two.y, two.z, 255, 0, 0, 255);
		Natives::GRAPHICS::DRAW_LINE(two.x, two.y, two.z, three.x, three.y, three.z, 255, 0, 0, 255);
		Natives::GRAPHICS::DRAW_LINE(three.x, three.y, three.z, four.x, four.y, four.z, 255, 0, 0, 255);
		Natives::GRAPHICS::DRAW_LINE(four.x, four.y, four.z, one.x, one.y, one.z, 255, 0, 0, 255);
		Natives::GRAPHICS::DRAW_LINE(one_.x, one_.y, one_.z, two_.x, two_.y, two_.z, 255, 0, 0, 255);
		Natives::GRAPHICS::DRAW_LINE(two_.x, two_.y, two_.z, three_.x, three_.y, three_.z, 255, 0, 0, 255);
		Natives::GRAPHICS::DRAW_LINE(three_.x, three_.y, three_.z, four_.x, four_.y, four_.z, 255, 0, 0, 255);
		Natives::GRAPHICS::DRAW_LINE(four_.x, four_.y, four_.z, one_.x, one_.y, one_.z, 255, 0, 0, 255);
		Natives::GRAPHICS::DRAW_LINE(one.x, one.y, one.z, one_.x, one_.y, one_.z, 255, 0, 0, 255);
		Natives::GRAPHICS::DRAW_LINE(two.x, two.y, two.z, two_.x, two_.y, two_.z, 255, 0, 0, 255);
		Natives::GRAPHICS::DRAW_LINE(three.x, three.y, three.z, three_.x, three_.y, three_.z, 255, 0, 0, 255);
		Natives::GRAPHICS::DRAW_LINE(four.x, four.y, four.z, four_.x, four_.y, four_.z, 255, 0, 0, 255);
	}
}
