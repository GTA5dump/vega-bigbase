#include "../PrecompiledHeader.hpp"
#include "MiscOptions.hpp"
#include "../Natives.hpp"
#include "../Pointers.hpp"
#include "../Util.hpp"

namespace Vega::Feature
{
	MiscOptions g_MiscOptions;

	void MiscOptions::loop()
	{
		if (m_GTAoneCam)
		{
			applyGTAoneCam();
		}

		if (m_Noclip)
		{
			applyNoclip();
		}

		if (m_DisablePhone)
		{
			applyDisablePhone();
		}

		if (m_DisableHUD)
		{
			applyDisableHUD();
		}
	}

	void MiscOptions::applyMobileRadio()
	{
		g_Pointers->call(g_Pointers->m_SetMobileRadioEnabledDuringGameplay, m_MobileRadio);
	}

	void MiscOptions::applyGTAoneCam()
	{
		static std::int32_t cam = 0;
		std::int32_t ped = Util::getLocalPedHandle();
		if (!Natives::CAM::DOES_CAM_EXIST(cam))
		{
			cam = Natives::CAM::CREATE_CAM("DEFAULT_SCRIPTED_CAMERA", 1);
			Natives::CAM::RENDER_SCRIPT_CAMS(1, true, 700, 1, 1, NULL);
			Natives::CAM::SET_CAM_ACTIVE(cam, 1);
		}

		if (!m_GTAoneCam)
		{
			Natives::CAM::RENDER_SCRIPT_CAMS(0, true, 700, 1, 1, NULL);
			Natives::CAM::DESTROY_CAM(cam, 1);
		}

		Natives::CAM::SET_CAM_ROT(cam, -90, 0, Natives::ENTITY::GET_ENTITY_ROTATION(ped, 0).z, 0);
		Vector3 c = Natives::ENTITY::GET_ENTITY_COORDS(ped, true);
		Natives::CAM::SET_CAM_COORD(cam, c.x, c.y, c.z + 30.f);
	}

	void MiscOptions::applyNoclip()
	{
		auto ped = Util::getLocalPedHandle();
		auto coord = Natives::ENTITY::GET_ENTITY_COORDS(ped, true);
		auto dir = Util::rot2dir(Natives::CAM::GET_GAMEPLAY_CAM_ROT(0));
		auto speed = 3.0f;

		Natives::ENTITY::SET_ENTITY_COORDS_NO_OFFSET(ped, coord.x, coord.y, coord.z, 0, 0, 0);

		if (!Util::isGameFocused())
			return;

		if (Util::isKeyPressed(VK_SPACE))
		{
			speed -= 2.6f;
		}

		if (Util::isKeyPressed(VK_LSHIFT) || Util::isKeyPressed(VK_RSHIFT))
		{
			speed += 1.0f;
		}

		if (Util::isKeyPressed(VK_KEY_W))
		{
			auto dest = coord + (dir * speed);
			Natives::ENTITY::SET_ENTITY_COORDS_NO_OFFSET(ped, dest.x, dest.y, dest.z, 0, 0, 0);
		}

		if (Util::isKeyPressed(VK_KEY_S))
		{
			speed *= -1;
			auto dest = coord + (dir * speed);
			Natives::ENTITY::SET_ENTITY_COORDS_NO_OFFSET(ped, dest.x, dest.y, dest.z, 0, 0, 0);
		}
	}

	void MiscOptions::applyDisablePhone()
	{
		Natives::MOBILE::_DISABLE_PHONE_THIS_FRAME(true);
	}

	void MiscOptions::applyDisableHUD()
	{
		Natives::HUD::HIDE_HUD_AND_RADAR_THIS_FRAME();
	}
}
