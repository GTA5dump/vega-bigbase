#include "PrecompiledHeader.hpp"

#include "Lists.hpp"
#include "Remote.hpp"

#include "Features/AccountOptions.hpp"
#include "Features/PlayerOptions.hpp"
#include "Features/WeaponModifiers.hpp"
#include "Features/VehicleOptions.hpp"
#include "Features/WeaponOptions.hpp"
#include "Features/WorldOptions.hpp"
#include "Features/SpoonerOptions.hpp"
#include "Features/MiscOptions.hpp"

#include "User Interface/DrawHelper.hpp"
#include "User Interface/SimpleOption.hpp"
#include "User Interface/SubmenuOption.hpp"
#include "User Interface/NumberOption.hpp"
#include "User Interface/BoolOption.hpp"
#include "User Interface/ChooseOption.hpp"
#include "User Interface/BreakOption.hpp"
#include "User Interface/NumberBoolOption.hpp"
#include "User Interface/SelectedPlayerSubmenu.hpp"

#include "Reconstructed Classes/CNetworkPlayerMgr.hpp"
#include "Reconstructed Classes/CPlayerInfo.hpp"
#include "Reconstructed Classes/CWeaponInfo.hpp"
#include "Reconstructed Classes/CPed.hpp"
#include "Reconstructed Classes/CNavigation.hpp"
#include "Reconstructed Classes/CEntityPool.hpp"
#include "Reconstructed Classes/CPedPool.hpp"
#include "Reconstructed Classes/CVehiclePool.hpp"
#include "Reconstructed Classes/CPickupPool.hpp"
#include "Reconstructed Classes/CObjectPool.hpp"
#include "Reconstructed Classes/CPickup.hpp"
#include "Reconstructed Classes/CModelInfo.hpp"
#include "Reconstructed Classes/CVehicle.hpp"
#include "Reconstructed Classes/CTimeDate.hpp"
#include "Reconstructed Classes/CObject.hpp"
#include "Reconstructed Classes/CNavigation.hpp"

#include "MainScript.hpp"
#include "ScriptMgr.hpp"
#include "ScriptGlobal.hpp"
#include "ScriptCallback.hpp"

#include "Pointers.hpp"
#include "Main.hpp"
#include "CustomText.hpp"

namespace Vega {
	void MainScript::Init() {
		g_CustomText.initialize();
		g_CustomText.reg(XorString("ENTER_A_NUMBER"), XorString("Enter a Number"));
		g_CustomText.reg(XorString("SEARCH_PROPS"), XorString("Search Props"));

		g_UIManager = std::make_unique<UserInterface::Manager>();

#	pragma region Home
		g_UIManager->addSubmenu(UserInterface::Submenu().setName(XorString("Home")).setId("home"_joaat).setFunction([](UserInterface::Submenu* dis)
		{
			dis->addOption(UserInterface::SubmenuOption()
				.setLeftText(XorString("Player"))
				.setSub("playerOptions"_joaat));

			dis->addOption(UserInterface::BreakOption()
				.setText("Break 1"));

			dis->addOption(UserInterface::SubmenuOption()
				.setLeftText(XorString("Weapon"))
				.setSub("weaponOptions"_joaat));

			dis->addOption(UserInterface::SubmenuOption()
				.setLeftText(XorString("Vehicle"))
				.setSub("vehicleOptions"_joaat));

			dis->addOption(UserInterface::SubmenuOption()
				.setLeftText(XorString("Miscellaneous"))
				.setSub("miscOptions"_joaat));

			dis->addOption(UserInterface::SubmenuOption()
				.setLeftText(XorString("World"))
				.setSub("worldOptions"_joaat));

			dis->addOption(UserInterface::SubmenuOption()
				.setLeftText(XorString("Spooner"))
				.setSub("spoonerOptions"_joaat));

			dis->addOption(UserInterface::SubmenuOption()
				.setLeftText(XorString("Online"))
				.setSub("onlineOptions"_joaat));

			dis->addOption(UserInterface::SubmenuOption()
				.setLeftText(XorString("Player List"))
				.setSub("playerList"_joaat));

			dis->addOption(UserInterface::SubmenuOption()
				.setLeftText(XorString("Settings"))
				.setSub("settings"_joaat));
			dis->addOption(UserInterface::SimpleOption().setLeftText(XorString("Uninject")).setDescription(XorString("Unloads the menu.")).setFunction([]
			{
				g_ShouldExit = true;
			}));
		}));
#	pragma endregion
#	pragma region Online Options
		g_UIManager->addSubmenu(UserInterface::Submenu()
			.setName(XorString("Online Options"))
			.setId("onlineOptions"_joaat)
			.setFunction([](UserInterface::Submenu* dis)
		{
			dis->addOption(UserInterface::SubmenuOption()
				.setLeftText(XorString("Account"))
				.setSub("account"_joaat));

			dis->addOption(UserInterface::SubmenuOption()
				.setLeftText(XorString("Protection"))
				.setSub("self"_joaat));

			dis->addOption(UserInterface::BoolOption()
				.setLeftText(XorString("Off The Radar"))
				.setDescription(XorString("Makes you invisible on the radar for other players."))
				.setBool(&Feature::g_PlayerOptions.m_OffRadar)
				.setFunction([]
			{
				Feature::g_PlayerOptions.applyOffRadar();
			}));

			dis->addOption(UserInterface::BoolOption()
				.setLeftText(XorString("Reveal Players"))
				.setDescription(XorString("Reveals invisible players on the radar."))
				.setBool(&Feature::g_PlayerOptions.m_RevealPlayers)
				.setFunction([]
			{
				Feature::g_PlayerOptions.applyRevealPlayers();
			}));

			dis->addOption(UserInterface::BoolOption()
				.setLeftText(XorString("Cops Turn Blind Eye"))
				.setDescription(XorString("Cops will ignore any crimes you commit."))
				.setBool(&Feature::g_PlayerOptions.m_CopsTurnBlindEye)
				.setFunction([]
			{
				Feature::g_PlayerOptions.applyCopsTurnBlindEye();
			}));
		}));

		g_UIManager->addSubmenu(UserInterface::Submenu()
			.setName(XorString("Player List"))
			.setId("playerList"_joaat)
			.setFunction([](UserInterface::Submenu* dis)
		{
			for (std::int32_t i = 0; i < 32; ++i) {
				auto playerInfo = Util::getPlayerInfo(i);
				char buf[32] = {};
				std::string name = playerInfo->m_net_player_data.m_name;
				std::snprintf(buf, sizeof(buf) - 1, "%s: %u", name.c_str(), i);
				dis->addOption(UserInterface::SubmenuOption()
					.setLeftText(buf)
					.setSub("selectedPlayer"_joaat)
					.setFunction([=]
				{
					g_SelectedPlayer = i;
				}));
			}
			if (dis->getOptions().empty())
			{
				dis->addOption(UserInterface::SimpleOption()
					.setLeftText(XorString("No Players")));
			}
		}));
#	pragma endregion
#	pragma region Recovery
		g_UIManager->addSubmenu(UserInterface::Submenu()
			.setName(XorString("Account"))
			.setId("account"_joaat)
			.setFunction([](UserInterface::Submenu* dis)
		{
			dis->addOption(UserInterface::BoolOption()
				.setLeftText(XorString("Give Money"))
				.setBool(&Feature::g_AccountOptions.m_GiveMoney));
			dis->addOption(UserInterface::BoolOption()
				.setLeftText(XorString("Remove Money"))
				.setBool(&Feature::g_AccountOptions.m_RemoveMoney));
		}));
#	pragma endregion
#	pragma region Selected Player
		auto playerInfo = Util::getPlayerInfo(g_SelectedPlayer);
		g_UIManager->addSubmenu(*(UserInterface::SelectedPlayerSubmenu*)&UserInterface::SelectedPlayerSubmenu()
			.setId("selectedPlayer"_joaat)
			.setFunction([=](UserInterface::Submenu* dis)
		{
			if (playerInfo)
			{
				dis->addOption(UserInterface::SimpleOption()
					.setLeftText(XorString("Speed Up Vehicle"))
					.setFunction([]
				{
					if (auto ped = Util::getPlayerPed(g_SelectedPlayer))
					{
						if (auto vehicle = ped->m_vehicle)
						{
							auto vehicleHandle = Util::ptrToHandle(vehicle);
							g_ScriptCallbackManager->addCallback(NetworkControlScriptCallback(vehicleHandle, [=]
							{
								Natives::VEHICLE::SET_VEHICLE_FORWARD_SPEED(vehicleHandle, 70.f);
							}));
						}
					}
				}));

				dis->addOption(UserInterface::SimpleOption()
					.setLeftText(XorString("Teleport To Player"))
					.setFunction([]
				{
					if (auto ped = Util::getPlayerPed(g_SelectedPlayer))
					{
						auto handle = Util::getLocalPedHandle();
						auto pos = Util::getEntityPosition(ped);
						Natives::ENTITY::SET_ENTITY_COORDS(handle, pos.x, pos.y, pos.z, false, false, false, false);
					}
				}));

				dis->addOption(UserInterface::SimpleOption()
					.setLeftText(XorString("Kick"))
					.setFunction([]
				{
					std::int64_t args[3] = { RemoteEvent::KickToSP, g_SelectedPlayer, 0 };
					Util::triggerScriptEvent(args, 1 << g_SelectedPlayer);
				}));
			}
			else
			{
				dis->addOption(UserInterface::SimpleOption().setLeftText(XorString("Player Left")));
			}
		}));
#	pragma endregion
#	pragma region Player Options
		g_UIManager->addSubmenu(UserInterface::Submenu()
			.setName(XorString("Player Options"))
			.setId("playerOptions"_joaat)
			.setFunction([](UserInterface::Submenu* dis)
		{
			dis->addOption(UserInterface::BoolOption()
				.setLeftText(XorString("Invincibility"))
				.setDescription(XorString("Makes you invincible."))
				.setBool(&Feature::g_PlayerOptions.m_GodMode)
				.setFunction([]
			{
				Feature::g_PlayerOptions.applyGodMode();
			}));

			dis->addOption(UserInterface::BoolOption()
				.setLeftText(XorString("Semi-Invincibility"))
				.setDescription(XorString("Makes you semi-invincible."))
				.setBool(&Feature::g_PlayerOptions.m_SemiGodMode));

			dis->addOption(UserInterface::BoolOption()
				.setLeftText(XorString("Never Wanted"))
				.setDescription(XorString("Never get the police on your tail."))
				.setBool(&Feature::g_PlayerOptions.m_NeverWanted));

			dis->addOption(UserInterface::NumberOption<std::int32_t>()
				.setLeftText(XorString("Wanted Level"))
				.setDescription(XorString("Set your wanted level."))
				.setNumber(&Feature::g_PlayerOptions.m_WantedLevel)
				.setMin(0).setMax(5)
				.setTextBox(false)
				.setActionOnHorizontal(true)
				.setFunction([]
			{
				Feature::g_PlayerOptions.applyWantedLevel();
			}));

			dis->addOption(UserInterface::BoolOption()
				.setLeftText(XorString("Super Jump"))
				.setDescription(XorString("Makes you jump high."))
				.setBool(&Feature::g_PlayerOptions.m_SuperJump));
			dis->addOption(UserInterface::BoolOption()
				.setLeftText(XorString("Super Run"))
				.setDescription(XorString("Makes you run fast."))
				.setBool(&Feature::g_PlayerOptions.m_SuperRun));

			dis->addOption(UserInterface::BoolOption()
				.setLeftText(XorString("Invisible"))
				.setDescription(XorString("Makes you invisible."))
				.setBool(&Feature::g_PlayerOptions.m_Invisible)
				.setFunction([]
			{
				Feature::g_PlayerOptions.applyInvisibility(Feature::g_PlayerOptions.m_Invisible);
			}));
			dis->addOption(UserInterface::NumberOption<std::int32_t>()
				.setLeftText(XorString("Alpha"))
				.setDescription(XorString("Sets the opacity of your ped."))
				.setNumber(&Feature::g_PlayerOptions.m_Alpha)
				.setMin(0).setMax(100).setStep(20)
				.setFunction([]
			{
				Feature::g_PlayerOptions.setAlpha(Feature::g_PlayerOptions.m_Alpha);
			}));

			dis->addOption(UserInterface::BoolOption()
				.setLeftText(XorString("Ragdoll On G"))
				.setDescription(XorString("Press G to fall over."))
				.setBool(&Feature::g_PlayerOptions.m_RagdollG));

			dis->addOption(UserInterface::NumberBoolOption<float>()
				.setLeftText("Width")
				.setDescription(XorString("Set your ped's width."))
				.setBool(&Feature::g_PlayerOptions.m_WidthToggle)
				.setNumber(&Feature::g_PlayerOptions.m_WidthAmount)
				.setMin(0.f).setMax(2.f).setStep(0.01f).setPrecision(2));

			dis->addOption(UserInterface::NumberBoolOption<float>()
				.setLeftText("Height")
				.setDescription(XorString("Set your ped's height."))
				.setBool(&Feature::g_PlayerOptions.m_HeightToggle)
				.setNumber(&Feature::g_PlayerOptions.m_HeightAmount)
				.setMin(0.f).setMax(2.f).setStep(0.01f).setPrecision(2));
		}));
#	pragma endregion
#	pragma region Vehicle Options
		g_UIManager->addSubmenu(UserInterface::Submenu()
			.setName(XorString("Vehicle Options"))
			.setId("vehicleOptions"_joaat)
			.setFunction([](UserInterface::Submenu* dis)
		{
			dis->addOption(UserInterface::SubmenuOption()
				.setLeftText(XorString("Spawner"))
				.setSub("vehicleSpawner"_joaat));

			dis->addOption(UserInterface::BoolOption()
				.setLeftText(XorString("Invincibility"))
				.setDescription(XorString("Makes your vehicle invincible."))
				.setBool(&Feature::g_VehicleOptions.m_GodMode)
				.setFunction([]
			{
				Feature::g_VehicleOptions.applyGodMode();
			}));
			dis->addOption(UserInterface::BoolOption()
				.setLeftText(XorString("Boost"))
				.setDescription(XorString("Honk the horn to boost your vehicle."))
				.setBool(&Feature::g_VehicleOptions.m_Boost));
			dis->addOption(UserInterface::SimpleOption()
				.setLeftText(XorString("Fully Tune"))
				.setDescription(XorString("Upgrade your vehicle to the max."))
				.setFunction([]
			{
				if (auto vehicle = Util::getLocalVehicle())
				{
					if (auto handle = Util::ptrToHandle(vehicle); handle != -1)
					{
						Feature::g_VehicleOptions.fullyTuneVehicle(handle);
					}
				}
			}));
			dis->addOption(UserInterface::SimpleOption()
				.setLeftText(XorString("Set On Ground"))
				.setDescription(XorString("Set your vehicle on the ground."))
				.setFunction([]
			{
				if (auto vehicle = Util::getLocalVehicle())
				{
					if (auto handle = Util::ptrToHandle(vehicle); handle != -1)
					{
						Natives::VEHICLE::SET_VEHICLE_ON_GROUND_PROPERLY(handle, NULL);
					}
				}
			}));
			dis->addOption(UserInterface::SimpleOption()
				.setLeftText(XorString("Set License Plate"))
				.setDescription(XorString("Set your license plate."))
				.setFunction([]
			{
				if (auto vehicle = Util::getLocalVehicle())
				{
					if (auto handle = Util::ptrToHandle(vehicle); handle != -1)
					{
						g_ScriptCallbackManager->addCallback(KeyboardScriptCallback(XorString("License Plate"), 8, [=](const char* result)
						{
							Natives::VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT(handle, result);
						}));
					}
				}
			}));
		}));

		g_UIManager->addSubmenu(UserInterface::Submenu()
			.setName(XorString("Vehicle Spawner"))
			.setId("vehicleSpawner"_joaat)
			.setFunction([](UserInterface::Submenu* dis)
		{
			dis->addOption(UserInterface::BoolOption()
				.setLeftText(XorString("MP Vehicles in SP"))
				.setDescription(XorString("Multiplayer vehicles won't despawn in Story Mode."))
				.setBool(ScriptGlobal(0x41183A).as<bool*>()));

			dis->addOption(UserInterface::BoolOption()
				.setLeftText(XorString("Spawn In Vehicle"))
				.setDescription(XorString("Get in your vehicle after spawning it."))
				.setBool(&Feature::g_VehicleOptions.m_SpawnInCar));

			dis->addOption(UserInterface::BoolOption()
				.setLeftText(XorString("Fade In Vehicle"))
				.setDescription(XorString("Fade in your vehicle after it spawns."))
				.setBool(&Feature::g_VehicleOptions.m_FadeInSpawnedCar));

			dis->addOption(UserInterface::BoolOption()
				.setLeftText(XorString("Spawn Fully Tuned"))
				.setDescription(XorString("Fully tune your vehicle after it spawns."))
				.setBool(&Feature::g_VehicleOptions.m_SpawnFullyTuned));

			dis->addOption(UserInterface::BoolOption()
				.setLeftText(XorString("Spawn On Ground"))
				.setDescription(XorString("Set your new vehicle on the ground."))
				.setBool(&Feature::g_VehicleOptions.m_SpawnOnGround));

			dis->addOption(UserInterface::BoolOption()
				.setLeftText(XorString("Custom Plate"))
				.setDescription(XorString("Get a unique plate on your new vehicle."))
				.setBool(&Feature::g_VehicleOptions.m_ChangePlateOnSpawnedCar));

			dis->addOption(UserInterface::BoolOption().setLeftText(XorString("PTFX"))
				.setDescription(XorString("Spawn your vehicle with a particle effect."))
				.setBool(&Feature::g_VehicleOptions.m_SpawnParticleFX));

			for (auto vehicleModel : Lists::vehicleListAfterHours)
			{
				char classLabel[32] = {};
				std::snprintf(classLabel, sizeof(classLabel) - 1, "VEH_CLASS_%i", Natives::VEHICLE::GET_VEHICLE_CLASS_FROM_NAME(vehicleModel));

				char ingameName[64] = {};
				strncpy(ingameName, Natives::HUD::_GET_LABEL_TEXT(Natives::VEHICLE::GET_DISPLAY_NAME_FROM_VEHICLE_MODEL(vehicleModel)), sizeof(ingameName) - 1);

				auto fagg = Natives::HUD::_GET_LABEL_TEXT(classLabel);

				char name[64] = {};
				std::snprintf(name, sizeof(name) - 1, "%s, %s", ingameName, fagg);

				char description[64] = {};
				std::snprintf(description, sizeof(description) - 1, "Spawn %s %s.", isVowel(*name) ? "an" : "a", name);

				dis->addOption(UserInterface::SimpleOption()
					.setLeftText(name)
					.setDescription(description)
					.setFunction([=]
				{
					Vector3 pos{};
					if (auto ped = Util::getLocalPlayerPed())
					{
						if (auto nav = ped->m_navigation)
						{
							pos = nav->v3Loc;
						}
					}

					Feature::g_VehicleOptions.spawnVehicle(vehicleModel, pos);
				}));
			}
		}));

#	pragma endregion
#	pragma region Weapon Options
		g_UIManager->addSubmenu(UserInterface::Submenu()
			.setName(XorString("Weapon Options"))
			.setId("weaponOptions"_joaat)
			.setFunction([](UserInterface::Submenu* dis)
		{
			dis->addOption(UserInterface::SubmenuOption()
				.setLeftText(XorString("Weapon Modifiers"))
				.setSub("weaponModifiers"_joaat));
			dis->addOption(UserInterface::SubmenuOption()
				.setLeftText(XorString("Entity Shooter"))
				.setSub("entityShooter"_joaat));
			dis->addOption(UserInterface::SimpleOption()
				.setLeftText(XorString("Give Weapons"))
				.setDescription(XorString("Gives you all the weapons."))
				.setFunction([]
			{
				Feature::g_WeaponOptions.getWeapons();
			}));
			dis->addOption(UserInterface::SimpleOption()
				.setLeftText(XorString("Remove Weapons"))
				.setDescription(XorString("Removes all your weapons."))
				.setFunction([]
			{
				Feature::g_WeaponOptions.removeWeapons();
			}));

			dis->addOption(UserInterface::BoolOption()
				.setLeftText(XorString("Infinite Ammo"))
				.setDescription(XorString("Never run out of ammo."))
				.setBool(&Feature::g_WeaponOptions.m_InfiniteAmmo));
			dis->addOption(UserInterface::BoolOption()
				.setLeftText(XorString("Delete Gun"))
				.setDescription(XorString("Shoot at stuff to delete it."))
				.setBool(&Feature::g_WeaponOptions.m_DeleteGun));
			dis->addOption(UserInterface::BoolOption()
				.setLeftText(XorString("Gravity Gun"))
				.setDescription(XorString("Move things around with your gun."))
				.setBool(&Feature::g_WeaponOptions.m_GravityGun));
			dis->addOption(UserInterface::BoolOption()
				.setLeftText(XorString("Vacuum Cleaner Gun"))
				.setDescription(XorString("Succ things with your gun."))
				.setBool(&Feature::g_WeaponOptions.m_SuccGun));
			dis->addOption(UserInterface::BoolOption()
				.setLeftText(XorString("Fire Ammo"))
				.setDescription(XorString("Set your targets on fire."))
				.setBool(&Feature::g_WeaponOptions.m_FireAmmo));
			dis->addOption(UserInterface::BoolOption()
				.setLeftText(XorString("Explosive Ammo"))
				.setDescription(XorString("Makes your bullets explode.")).setBool(&Feature::g_WeaponOptions.m_ExplosiveAmmo));

			dis->addOption(UserInterface::BoolOption()
				.setLeftText(XorString("No Recoil"))
				.setDescription(XorString("Disables weapon recoil."))
				.setBool(&Feature::g_WeaponOptions.m_NoRecoil).setFunction([]
			{
				Feature::g_WeaponOptions.applyNoRecoil(Feature::g_WeaponOptions.m_NoRecoil);
			}));

			dis->addOption(UserInterface::BoolOption()
				.setLeftText(XorString("Laser Sight"))
				.setDescription(XorString("Enables a laser sight."))
				.setBool(&Feature::g_WeaponOptions.m_LaserSight));
			dis->addOption(UserInterface::BoolOption()
				.setLeftText(XorString("Rapid Fire"))
				.setDescription(XorString("Disable fire rate."))
				.setBool(&Feature::g_WeaponOptions.m_RapidFire));
			dis->addOption(UserInterface::BoolOption()
				.setLeftText(XorString("Cartoon Gun"))
				.setDescription(XorString("Replace muzzle flash with a cartoon effect."))
				.setBool(&Feature::g_WeaponOptions.m_CartoonGun));
			dis->addOption(UserInterface::BoolOption()
				.setLeftText(XorString("Teleport Gun"))
				.setDescription(XorString("Teleport to your bullet impact."))
				.setBool(&Feature::g_WeaponOptions.m_TeleportGun));
			dis->addOption(UserInterface::BoolOption()
				.setLeftText(XorString("Airstrike Gun"))
				.setDescription(XorString("Calls in an airstrike at your bullet's impact."))
				.setBool(&Feature::g_WeaponOptions.m_AirstrikeGun));

			dis->addOption(UserInterface::BoolOption()
				.setLeftText(XorString("Ped Gun"))
				.setDescription(XorString("Spawns a random ped on your bullet impact."))
				.setBool(&Feature::g_WeaponOptions.m_PedGun));
			dis->addOption(UserInterface::BoolOption()
				.setLeftText(XorString("Coord Gun"))
				.setDescription(XorString("Displays the coordinates of your bullet impact."))
				.setBool(&Feature::g_WeaponOptions.m_CoordGun));

			dis->addOption(UserInterface::BoolOption()
				.setLeftText(XorString("Drive It Gun"))
				.setDescription(XorString("Shoot a vehicle to drive it."))
				.setBool(&Feature::g_WeaponOptions.m_DriveItGun));
			dis->addOption(UserInterface::BoolOption()
				.setLeftText(XorString("Hash Gun"))
				.setDescription(XorString("Displays the hash of your target."))
				.setBool(&Feature::g_WeaponOptions.m_HashGun));
			dis->addOption(UserInterface::BoolOption()
				.setLeftText(XorString("Just Cause Gun"))
				.setDescription(XorString("Attach two objects with ropes by shooting them."))
				.setBool(&Feature::g_WeaponOptions.m_JustCauseGun));
			dis->addOption(UserInterface::BoolOption()
				.setLeftText(XorString("Valkyire Rocket"))
				.setDescription(XorString("Shoot a remote controlled rocket."))
				.setBool(&Feature::g_WeaponOptions.m_ValkyireRocket)
				.setFunction([]
			{
				Feature::g_WeaponOptions.applyValkyireRocket();
			}));

			dis->addOption(UserInterface::SimpleOption()
				.setLeftText(XorString("Dildo Knife"))
				.setFunction([]
			{
				Feature::g_WeaponOptions.dildoKnife();
			}));
		}));
		g_UIManager->addSubmenu(UserInterface::Submenu()
			.setName(XorString("Entity Shooter"))
			.setId("entityShooter"_joaat)
			.setFunction([](UserInterface::Submenu* dis)
		{
			dis->addOption(UserInterface::ChooseOption<const char*, std::size_t>()
				.setLeftText("Type")
				.setArray(&Lists::entityShooterList)
				.setPositon(&Feature::g_WeaponOptions.m_EntityShooterType));
			switch (Feature::g_WeaponOptions.m_EntityShooterType)
			{
			case 0:
				dis->addOption(UserInterface::ChooseOption<std::uint32_t, std::size_t>()
					.setLeftText("Vehicle")
					.setArray(&Lists::vehicleShooterList)
					.setPositon(&Feature::g_WeaponOptions.m_VehicleShooterType)
					.setNameGetter([](std::ostream* stream, std::uint32_t const* data)
				{
					*stream << Natives::HUD::_GET_LABEL_TEXT(Natives::VEHICLE::GET_DISPLAY_NAME_FROM_VEHICLE_MODEL(*data));
				}));
				dis->addOption(UserInterface::BoolOption().setLeftText(XorString("Random Color")).setBool(&Feature::g_WeaponOptions.m_RandomVehicleColor));
				break;
			case 1:
				dis->addOption(UserInterface::ChooseOption<CNameWithHash, std::size_t>().setLeftText(XorString("Prop")).setArray(&Lists::namedProps).setPositon(&Feature::g_WeaponOptions.m_PropShooterType));
				break;
			case 2:
				dis->addOption(UserInterface::ChooseOption<CNameWithHash, std::size_t>().setLeftText(XorString("Ped")).setArray(&Lists::popularPeds).setPositon(&Feature::g_WeaponOptions.m_PedShooterType));
				break;
			}
			dis->addOption(UserInterface::BoolOption()
				.setLeftText(XorString("Enable"))
				.setBool(&Feature::g_WeaponOptions.m_EntityShooter));
		}));
		g_UIManager->addSubmenu(UserInterface::Submenu()
			.setName(XorString("Weapon Modifiers"))
			.setId("weaponModifiers"_joaat)
			.setFunction([](UserInterface::Submenu* dis)
		{
			dis->addOption(UserInterface::ChooseOption<std::unique_ptr<char[]>, std::size_t>()
				.setLeftText("Choose")
				.setVector(&Feature::g_WeaponModifiers.m_Names)
				.setPositon(&Feature::g_WeaponModifiers.m_Selected));

			if (!Feature::g_WeaponModifiers.m_Infos.empty() && Feature::g_WeaponModifiers.m_Infos[Feature::g_WeaponModifiers.m_Selected])
			{
				auto kek = Feature::g_WeaponModifiers.m_Infos.at(Feature::g_WeaponModifiers.m_Selected);

				dis->addOption(UserInterface::NumberOption<float>()
					.setLeftText("TP FOV")
					.setNumber(&kek->fThirdPersonAimingFOV)
					.setUnlimited(true));
				dis->addOption(UserInterface::NumberOption<float>()
					.setLeftText("FP FOV")
					.setNumber(&kek->fFirstPersonAimingFov)
					.setUnlimited(true));
				dis->addOption(UserInterface::NumberOption<float>()
					.setLeftText("Flash FX Scale")
					.setNumber(&kek->fFlashFxScale)
					.setMin(0.1f).setMax(10.f).setStep(0.1f));
			}
		}));
#	pragma endregion
#	pragma region World Options

		g_UIManager->addSubmenu(UserInterface::Submenu()
			.setName(XorString("World Options"))
			.setId("worldOptions"_joaat)
			.setFunction([](UserInterface::Submenu* dis)
		{
			dis->addOption(UserInterface::SubmenuOption()
				.setLeftText(XorString("Traffic Manager"))
				.setSub("trafficManager"_joaat));
			dis->addOption(UserInterface::BoolOption()
				.setLeftText(XorString("Black Hole"))
				.setBool(&Feature::g_WorldOptions.m_BlackHoleToggle));
		}));

		g_UIManager->addSubmenu(UserInterface::Submenu()
			.setName(XorString("Traffic Manager"))
			.setId("trafficManager"_joaat)
			.setFunction([](UserInterface::Submenu* dis)
		{
			dis->addOption(UserInterface::SimpleOption()
				.setLeftText(XorString("Launch"))
				.setFunction([]
			{
				Feature::g_WorldOptions.trafficManagerLaunch();
			}));

			dis->addOption(UserInterface::SimpleOption()
				.setLeftText(XorString("Boost Forward"))
				.setFunction([]
			{
				Feature::g_WorldOptions.trafficManagerBoostForward();
			}));

			dis->addOption(UserInterface::SimpleOption()
				.setLeftText(XorString("Boost Backward"))
				.setFunction([]
			{
				Feature::g_WorldOptions.trafficManagerBoostBackward();
			}));

			dis->addOption(UserInterface::SimpleOption()
				.setLeftText(XorString("Kickflip"))
				.setFunction([]
			{
				Feature::g_WorldOptions.trafficManagerKickflip();
			}));

			dis->addOption(UserInterface::SimpleOption()
				.setLeftText(XorString("Explode"))
				.setFunction([]
			{
				Feature::g_WorldOptions.trafficManagerExplode();
			}));

			dis->addOption(UserInterface::SimpleOption()
				.setLeftText(XorString("Random Paint Job"))
				.setFunction([]
			{
				Feature::g_WorldOptions.trafficManagerRandomPaintJob();
			}));

			dis->addOption(UserInterface::SimpleOption()
				.setLeftText(XorString("Kill Driver"))
				.setFunction([]
			{
				Feature::g_WorldOptions.trafficManagerKillDriver();
			}));

			dis->addOption(UserInterface::SimpleOption()
				.setLeftText(XorString("Negative Gravity"))
				.setFunction([]
			{
				Feature::g_WorldOptions.trafficManagerNegativeGravity();
			}));

			dis->addOption(UserInterface::SimpleOption()
				.setLeftText(XorString("Earth Gravity"))
				.setFunction([]
			{
				Feature::g_WorldOptions.trafficManagerEarthGravity();
			}));

			dis->addOption(UserInterface::SimpleOption()
				.setLeftText(XorString("Sun Gravity"))
				.setFunction([]
			{
				Feature::g_WorldOptions.trafficManagerSunGravity();
			}));

			dis->addOption(UserInterface::SimpleOption()
				.setLeftText(XorString("Moon Gravity"))
				.setFunction([]
			{
				Feature::g_WorldOptions.trafficManagerMoonGravity();
			}));

			dis->addOption(UserInterface::SimpleOption()
				.setLeftText(XorString("No Gravity"))
				.setFunction([]
			{
				Feature::g_WorldOptions.trafficManagerNoGravity();
			}));
		}));
#	pragma endregion
#	pragma region Misc Options
		g_UIManager->addSubmenu(UserInterface::Submenu().setName(XorString("Miscellaneous Options")).setId("miscOptions"_joaat).setFunction([](UserInterface::Submenu* dis)
		{
			dis->addOption(UserInterface::BoolOption()
				.setLeftText(XorString("GTA One Camera"))
				.setBool(&Feature::g_MiscOptions.m_GTAoneCam)
				.setFunction([]
				{
					Feature::g_MiscOptions.applyGTAoneCam();
				})
				.setDescription(XorString("Sets the camera to a top down view like GTA one.")));
			dis->addOption(UserInterface::BoolOption()
				.setLeftText(XorString("Mobile Radio"))
				.setBool(&Feature::g_MiscOptions.m_MobileRadio)
				.setFunction([]
				{
					Feature::g_MiscOptions.applyMobileRadio();
				})
				.setDescription(XorString("Enables radio while not in a vehicle.")));
			dis->addOption(UserInterface::BoolOption()
				.setLeftText(XorString("Noclip"))
				.setBool(&Feature::g_MiscOptions.m_Noclip)
				.setDescription(XorString("Allows you to freely fly around the world.")));
			dis->addOption(UserInterface::BoolOption()
				.setLeftText(XorString("Disable Phone"))
				.setBool(&Feature::g_MiscOptions.m_DisablePhone)
				.setDescription(XorString("No more spam calls.")));
			dis->addOption(UserInterface::BoolOption()
				.setLeftText(XorString("Disable HUD"))
				.setBool(&Feature::g_MiscOptions.m_DisableHUD)
				.setDescription(XorString("Hides the hud.")));
		}));
#	pragma endregion
#	pragma region Spooner Options
		g_UIManager->addSubmenu(UserInterface::Submenu().setName(XorString("Spooner Options")).setId("spoonerOptions"_joaat).setFunction([](UserInterface::Submenu* dis)
		{
			dis->addOption(UserInterface::BoolOption().setLeftText(XorString("Spooner Mode")).setDescription(XorString("Move objects arround and rotate them."))
				.setBool(&Feature::g_SpoonerOptions.m_Spooner)
				.setFunction([]
			{
				Feature::g_SpoonerOptions.applySpooner();
			}));
			dis->addOption(UserInterface::SubmenuOption().setLeftText(XorString("Manage Spawned Entities")).setSub("spoonerOptions_entities"_joaat));
			dis->addOption(UserInterface::SubmenuOption().setLeftText(XorString("Spawner Settings")).setSub("spoonerSettings"_joaat));
			dis->addOption(UserInterface::SubmenuOption().setLeftText(XorString("Props")).setSub("spoonerOptions_props"_joaat));
			dis->addOption(UserInterface::SubmenuOption().setLeftText(XorString("Peds")).setSub("spoonerOptions_peds"_joaat));
		}));
		g_UIManager->addSubmenu(UserInterface::Submenu().setName(XorString("Manage Spawned Entities")).setId("spoonerOptions_entities"_joaat).setFunction([](UserInterface::Submenu* dis)
		{
			auto num = 0;
			for (decltype(auto) ent : Feature::g_SpoonerOptions.m_Entities)
			{
				if (ent.m_Handle != -1)
				{
					if (Natives::ENTITY::DOES_ENTITY_EXIST(ent.m_Handle))
					{
						num++;
						dis->addOption(UserInterface::SubmenuOption().setLeftText(ent.m_Name).setSub("spoonerOptions_editObject"_joaat).setFunction([=]
						{
							Feature::g_SpoonerOptions.m_CurrentEntity = ent;
						}));
					}
					else
					{
						ent.m_Handle = -1;
					}
				}
			}
			if (!num)
			{
				dis->addOption(UserInterface::SimpleOption().setLeftText(XorString("No Spawned Entities.")));
			}
		}));
		g_UIManager->addSubmenu(UserInterface::Submenu().setName(XorString("Spawner Settings")).setId("spoonerSettings"_joaat).setFunction([](UserInterface::Submenu* dis)
		{
			dis->addOption(UserInterface::BoolOption().setLeftText(XorString("Frozen")).setBool(&Feature::g_SpoonerOptions.m_SpawnFrozen));
			dis->addOption(UserInterface::BoolOption().setLeftText(XorString("Dynamic")).setBool(&Feature::g_SpoonerOptions.m_SpawnDynamic));
			dis->addOption(UserInterface::BoolOption().setLeftText(XorString("Collision")).setBool(&Feature::g_SpoonerOptions.m_SpawnCollision));
			dis->addOption(UserInterface::BoolOption().setLeftText(XorString("Gravity")).setBool(&Feature::g_SpoonerOptions.m_SpawnGravity));
		}));
		g_UIManager->addSubmenu(UserInterface::Submenu().setName(XorString("Edit Entity")).setId("spoonerOptions_editObject"_joaat).setFunction([](UserInterface::Submenu* dis)
		{
			Feature::g_SpoonerOptions.m_CurrentEntity.m_Position = Natives::ENTITY::GET_ENTITY_COORDS(Feature::g_SpoonerOptions.m_CurrentEntity.m_Handle, true);
			Feature::g_SpoonerOptions.m_CurrentEntity.m_Rotation = Natives::ENTITY::GET_ENTITY_ROTATION(Feature::g_SpoonerOptions.m_CurrentEntity.m_Handle, 0);
			dis->addOption(UserInterface::SimpleOption().setLeftText(XorString("Delete")).setFunction([]
			{
				Natives::ENTITY::SET_ENTITY_COORDS(Feature::g_SpoonerOptions.m_CurrentEntity.m_Handle, 0.f, 0.f, 0.f, 0, 0, 0, 0);
				Natives::ENTITY::DELETE_ENTITY(&Feature::g_SpoonerOptions.m_CurrentEntity.m_Handle);
			}));
			dis->addOption(UserInterface::BoolOption().setLeftText(XorString("Frozen"))
				.setBool(&Feature::g_SpoonerOptions.m_CurrentEntity.m_Freezed).setFunction([]
			{
				Natives::ENTITY::FREEZE_ENTITY_POSITION(Feature::g_SpoonerOptions.m_CurrentEntity.m_Handle, Feature::g_SpoonerOptions.m_CurrentEntity.m_Freezed);
			}));
			dis->addOption(UserInterface::BoolOption().setLeftText(XorString("Dynamic"))
				.setBool(&Feature::g_SpoonerOptions.m_CurrentEntity.m_Dynamic).setFunction([]
			{
				Natives::ENTITY::SET_ENTITY_DYNAMIC(Feature::g_SpoonerOptions.m_CurrentEntity.m_Dynamic, Feature::g_SpoonerOptions.m_CurrentEntity.m_Dynamic);
			}));
			dis->addOption(UserInterface::BoolOption().setLeftText(XorString("Collision"))
				.setBool(&Feature::g_SpoonerOptions.m_CurrentEntity.m_Collision).setFunction([]
			{
				Natives::ENTITY::SET_ENTITY_COLLISION(Feature::g_SpoonerOptions.m_CurrentEntity.m_Handle, Feature::g_SpoonerOptions.m_CurrentEntity.m_Collision, true);
			}));
			dis->addOption(UserInterface::BoolOption().setLeftText(XorString("Gravity"))
				.setBool(&Feature::g_SpoonerOptions.m_CurrentEntity.m_Gravity)
				.setFunction([]
			{
				Natives::ENTITY::SET_ENTITY_HAS_GRAVITY(Feature::g_SpoonerOptions.m_CurrentEntity.m_Handle, Feature::g_SpoonerOptions.m_CurrentEntity.m_Gravity);
			}));
			dis->addOption(UserInterface::SimpleOption().setLeftText(XorString("Set On Ground")).setFunction([]
			{
				float ground;
				auto coord = Util::getEntityPositionHandle(Feature::g_SpoonerOptions.m_CurrentEntity.m_Handle);
				Natives::MISC::GET_GROUND_Z_FOR_3D_COORD(coord.x, coord.y, coord.z, &ground, false);
				Natives::ENTITY::SET_ENTITY_COORDS_NO_OFFSET(Feature::g_SpoonerOptions.m_CurrentEntity.m_Handle, coord.x, coord.y, ground, 0, 0, 0);
			}));
			dis->addOption(UserInterface::SimpleOption().setLeftText(XorString("Teleport To Me")).setFunction([]
			{
				auto coord = Util::getEntityPositionHandle(Util::getLocalPedHandle());
				Natives::ENTITY::SET_ENTITY_COORDS_NO_OFFSET(Feature::g_SpoonerOptions.m_CurrentEntity.m_Handle, coord.x, coord.y, coord.z, 0, 0, 0);
			}));
			dis->addOption(UserInterface::NumberOption<float>().setLeftText(XorString("Precision")).setNumber(&Feature::g_SpoonerOptions.m_Precision)
				.setMin(-10.f).setMax(10.f).setStep(0.01f));
			dis->addOption(UserInterface::NumberOption<float>().setLeftText(XorString("X")).setNumber(&Feature::g_SpoonerOptions.m_CurrentEntity.m_Position.x)
				.setMin(-9999999999.f).setMax(9999999999.f)
				.setStep(Feature::g_SpoonerOptions.m_Precision)
				.setFunction([]
			{
				Natives::ENTITY::SET_ENTITY_COORDS_NO_OFFSET(Feature::g_SpoonerOptions.m_CurrentEntity.m_Handle, Feature::g_SpoonerOptions.m_CurrentEntity.m_Position.x, Feature::g_SpoonerOptions.m_CurrentEntity.m_Position.y, Feature::g_SpoonerOptions.m_CurrentEntity.m_Position.z, 0, 0, 0);
			}));
			dis->addOption(UserInterface::NumberOption<float>().setLeftText(XorString("Y")).setNumber(&Feature::g_SpoonerOptions.m_CurrentEntity.m_Position.y)
				.setMin(-9999999999.f).setMax(9999999999.f)
				.setStep(Feature::g_SpoonerOptions.m_Precision)
				.setFunction([]
			{
				Natives::ENTITY::SET_ENTITY_COORDS_NO_OFFSET(Feature::g_SpoonerOptions.m_CurrentEntity.m_Handle, Feature::g_SpoonerOptions.m_CurrentEntity.m_Position.x, Feature::g_SpoonerOptions.m_CurrentEntity.m_Position.y, Feature::g_SpoonerOptions.m_CurrentEntity.m_Position.z, 0, 0, 0);
			}));
			dis->addOption(UserInterface::NumberOption<float>().setLeftText(XorString("Z")).setNumber(&Feature::g_SpoonerOptions.m_CurrentEntity.m_Position.z)
				.setMin(-9999999999.f).setMax(9999999999.f)
				.setStep(Feature::g_SpoonerOptions.m_Precision)
				.setFunction([]
			{
				Natives::ENTITY::SET_ENTITY_COORDS_NO_OFFSET(Feature::g_SpoonerOptions.m_CurrentEntity.m_Handle, Feature::g_SpoonerOptions.m_CurrentEntity.m_Position.x, Feature::g_SpoonerOptions.m_CurrentEntity.m_Position.y, Feature::g_SpoonerOptions.m_CurrentEntity.m_Position.z, 0, 0, 0);
			}));
			dis->addOption(UserInterface::NumberOption<float>().setLeftText(XorString("Roll")).setNumber(&Feature::g_SpoonerOptions.m_CurrentEntity.m_Rotation.x)
				.setMin(-360.f).setMax(360.f)
				.setStep(Feature::g_SpoonerOptions.m_Precision)
				.setFunction([]
			{
				Natives::ENTITY::SET_ENTITY_ROTATION(Feature::g_SpoonerOptions.m_CurrentEntity.m_Handle, Feature::g_SpoonerOptions.m_CurrentEntity.m_Rotation.x, Feature::g_SpoonerOptions.m_CurrentEntity.m_Rotation.y, Feature::g_SpoonerOptions.m_CurrentEntity.m_Rotation.z, 0, false);
			}));
			dis->addOption(UserInterface::NumberOption<float>().setLeftText(XorString("Pitch")).setNumber(&Feature::g_SpoonerOptions.m_CurrentEntity.m_Rotation.y)
				.setMin(-360.f).setMax(360.f)
				.setStep(Feature::g_SpoonerOptions.m_Precision)
				.setFunction([]
			{
				Natives::ENTITY::SET_ENTITY_ROTATION(Feature::g_SpoonerOptions.m_CurrentEntity.m_Handle, Feature::g_SpoonerOptions.m_CurrentEntity.m_Rotation.x, Feature::g_SpoonerOptions.m_CurrentEntity.m_Rotation.y, Feature::g_SpoonerOptions.m_CurrentEntity.m_Rotation.z, 0, false);
			}));
			dis->addOption(UserInterface::NumberOption<float>().setLeftText(XorString("Yaw")).setNumber(&Feature::g_SpoonerOptions.m_CurrentEntity.m_Rotation.z)
				.setMin(-360.f).setMax(360.f)
				.setStep(Feature::g_SpoonerOptions.m_Precision)
				.setFunction([]
			{
				Natives::ENTITY::SET_ENTITY_ROTATION(Feature::g_SpoonerOptions.m_CurrentEntity.m_Handle, Feature::g_SpoonerOptions.m_CurrentEntity.m_Rotation.x, Feature::g_SpoonerOptions.m_CurrentEntity.m_Rotation.y, Feature::g_SpoonerOptions.m_CurrentEntity.m_Rotation.z, 0, false);
			}));
		}));
		g_UIManager->addSubmenu(UserInterface::Submenu().setName(XorString("Prop List")).setId("spoonerOptions_props"_joaat).setFunction([](UserInterface::Submenu* dis)
		{
			dis->addOption(UserInterface::SimpleOption().setLeftText(XorString("Search")).setRightText(Feature::g_SpoonerOptions.m_Search).setFunction([]
			{
				Feature::g_SpoonerOptions.Search();
			}));
			for (auto prop : Lists::prop_list)
			{
				if (!Feature::g_SpoonerOptions.m_Search)
				{
					dis->addOption(UserInterface::SimpleOption().setLeftText(prop).setFunction([=]
					{
						Feature::g_SpoonerOptions.spawnProp(prop);
					}));
				}
				else if (std::string(prop).find(Feature::g_SpoonerOptions.m_Search) != std::string::npos)
				{
					dis->addOption(UserInterface::SimpleOption().setLeftText(prop).setFunction([=]
					{
						Feature::g_SpoonerOptions.spawnProp(prop);
					}));
				}
			}
		}));
		g_UIManager->addSubmenu(UserInterface::Submenu().setName(XorString("Ped List")).setId("spoonerOptions_peds"_joaat).setFunction([](UserInterface::Submenu* dis)
		{
			dis->addOption(UserInterface::SimpleOption().setLeftText(XorString("Search")).setRightText(Feature::g_SpoonerOptions.m_SearchPed).setFunction([]
			{
				Feature::g_SpoonerOptions.SearchPeds();
			}));
			for (auto ped : Lists::ped_list)
			{
				if (!Feature::g_SpoonerOptions.m_SearchPed)
				{
					dis->addOption(UserInterface::SimpleOption().setLeftText(ped).setFunction([=]
					{
						Feature::g_SpoonerOptions.spawnPed(ped);
					}));
				}
				else if (std::string(ped).find(Feature::g_SpoonerOptions.m_SearchPed) != std::string::npos)
				{
					dis->addOption(UserInterface::SimpleOption().setLeftText(ped).setFunction([=]
					{
						Feature::g_SpoonerOptions.spawnPed(ped);
					}));
				}
			}
		}));
#	pragma endregion
#	pragma region Settings
		g_UIManager->addSubmenu(UserInterface::Submenu()
			.setName(XorString("Settings"))
			.setId("settings"_joaat)
			.setFunction([](UserInterface::Submenu* dis)
		{
			dis->addOption(UserInterface::SubmenuOption()
				.setLeftText(XorString("Header"))
				.setSub("headerSettings"_joaat));
			dis->addOption(UserInterface::SubmenuOption()
				.setLeftText(XorString("Topbar"))
				.setSub("topbarSettings"_joaat));
			dis->addOption(UserInterface::SubmenuOption()
				.setLeftText(XorString("Options"))
				.setSub("optionSettings"_joaat));
			dis->addOption(UserInterface::SubmenuOption()
				.setLeftText(XorString("Footer"))
				.setSub("footerSettings"_joaat));
			dis->addOption(UserInterface::SubmenuOption()
				.setLeftText(XorString("Descriptions"))
				.setSub("descriptionSettings"_joaat));
			dis->addOption(UserInterface::SubmenuOption()
				.setLeftText(XorString("Sounds"))
				.setSub("soundSettings"_joaat));
			dis->addOption(UserInterface::SubmenuOption()
				.setLeftText(XorString("Input"))
				.setSub("inputSettings"_joaat));
			dis->addOption(UserInterface::SubmenuOption()
				.setLeftText(XorString("Behaviour"))
				.setSub("behaviourSettings"_joaat));
		}));

		g_UIManager->addSubmenu(UserInterface::Submenu()
			.setName(XorString("Behaviour"))
			.setId("behaviourSettings"_joaat)
			.setFunction([](UserInterface::Submenu* dis)
		{
			dis->addOption(UserInterface::BoolOption()
				.setLeftText(XorString("Reset Selected Option On Close"))
				.setBool(&g_UIManager->m_ResetSelectedOptionOnClose));
			dis->addOption(UserInterface::BoolOption()
				.setLeftText(XorString("Reset Selected Option On Switch"))
				.setBool(&g_UIManager->m_ResetSelectedOptionOnSwitch));
			dis->addOption(UserInterface::BoolOption()
				.setLeftText(XorString("Reset Selected Option On Back"))
				.setBool(&g_UIManager->m_ResetSelectedOptionOnBack));
		}));

		g_UIManager->addSubmenu(UserInterface::Submenu().setName(XorString("Topbar")).setId("topbarSettings"_joaat).setFunction([](UserInterface::Submenu* dis)
		{
			dis->addOption(UserInterface::NumberOption<float>()
				.setLeftText(XorString("Text Size"))
				.setDescription(XorString("Sets the size of text."))
				.setNumber(&g_UIManager->m_TopbarTextSize)
				.setMin(0.05f).setMax(1.f).setStep(0.01f));
			dis->addOption(UserInterface::NumberOption<float>()
				.setLeftText(XorString("Left Padding"))
				.setDescription(XorString("Sets the left padding."))
				.setNumber(&g_UIManager->m_TopbarLeftPadding)
				.setMin(2.f).setMax(3.f).setStep(0.05f));
			dis->addOption(UserInterface::NumberOption<float>()
				.setLeftText(XorString("Right Padding"))
				.setDescription(XorString("Sets the right padding."))
				.setNumber(&g_UIManager->m_TopbarRightPadding)
				.setMin(2.f).setMax(3.f).setStep(0.05f));
		}));

		g_UIManager->addSubmenu(UserInterface::Submenu()
			.setName(XorString("Options"))
			.setId("optionSettings"_joaat)
			.setFunction([](UserInterface::Submenu* dis)
		{
			dis->addOption(UserInterface::NumberOption<float>()
				.setLeftText(XorString("Rect Size"))
				.setDescription(XorString("Sets the size of option rect."))
				.setNumber(&g_UIManager->m_OptionRectSize)
				.setMin(0.005f).setMax(0.1f).setStep(0.001f));
			dis->addOption(UserInterface::NumberOption<float>()
				.setLeftText(XorString("Text Size"))
				.setDescription(XorString("Sets the size of option text."))
				.setNumber(&g_UIManager->m_OptionTextSize)
				.setMin(0.05f).setMax(1.f).setStep(0.01f));
			dis->addOption(UserInterface::NumberOption<float>()
				.setLeftText(XorString("Left Padding"))
				.setDescription(XorString("Sets the left option padding."))
				.setNumber(&g_UIManager->m_OptionLeftPadding)
				.setMin(2.f).setMax(3.f).setStep(0.05f));
			dis->addOption(UserInterface::NumberOption<float>()
				.setLeftText(XorString("Right Padding"))
				.setDescription(XorString("Sets the right option padding."))
				.setNumber(&g_UIManager->m_OptionRightPadding)
				.setMin(2.f).setMax(3.f).setStep(0.05f));
			dis->addOption(UserInterface::NumberOption<float>()
				.setLeftText(XorString("Selected Horizontal Padding"))
				.setDescription(XorString("Sets the selected horizontal option padding."))
				.setNumber(&g_UIManager->m_OptionSelectedHorizontalPadding)
				.setMin(2.f).setMax(3.f).setStep(0.05f));
		}));

		g_UIManager->addSubmenu(UserInterface::Submenu()
			.setName(XorString("Descriptions"))
			.setId("descriptionSettings"_joaat)
			.setFunction([](UserInterface::Submenu* dis)
		{
			dis->addOption(UserInterface::BoolOption()
				.setLeftText(XorString("Enable"))
				.setDescription(XorString("Enables descriptions."))
				.setBool(&g_UIManager->m_DrawDescriptions));
			dis->addOption(UserInterface::NumberOption<float>()
				.setLeftText(XorString("Size"))
				.setDescription(XorString("Sets the size of the description bar."))
				.setNumber(&g_UIManager->m_DescriptionSize)
				.setMin(0.005f).setMax(0.1f).setStep(0.001f));

			dis->addOption(UserInterface::SubmenuOption()
				.setLeftText(XorString("Background Color"))
				.setSub("descriptionBackgroundColor"_joaat));
			dis->addOption(UserInterface::SubmenuOption()
				.setLeftText(XorString("Sprite Color"))
				.setSub("descriptionSpriteColor"_joaat));
			dis->addOption(UserInterface::SubmenuOption()
				.setLeftText(XorString("Text Color"))
				.setSub("descriptionTextColor"_joaat));
		}));

		g_UIManager->addSubmenu(UserInterface::Submenu()
			.setName(XorString("Description Background Color"))
			.setId("descriptionBackgroundColor"_joaat)
			.setFunction([](UserInterface::Submenu* dis)
		{
			dis->addOption(UserInterface::NumberOption<std::uint8_t>()
				.setLeftText(XorString("Red"))
				.setNumber(&g_UIManager->m_DescriptionBackgroundColor.r)
				.setMin(0).setMax(255));
			dis->addOption(UserInterface::NumberOption<std::uint8_t>()
				.setLeftText(XorString("Green"))
				.setNumber(&g_UIManager->m_DescriptionBackgroundColor.g)
				.setMin(0).setMax(255));
			dis->addOption(UserInterface::NumberOption<std::uint8_t>()
				.setLeftText(XorString("Blue"))
				.setNumber(&g_UIManager->m_DescriptionBackgroundColor.b)
				.setMin(0).setMax(255));
			dis->addOption(UserInterface::NumberOption<std::uint8_t>()
				.setLeftText(XorString("Alpha"))
				.setNumber(&g_UIManager->m_DescriptionBackgroundColor.a)
				.setMin(0).setMax(255));
		}));

		g_UIManager->addSubmenu(UserInterface::Submenu().setName(XorString("Description Icon Color")).setId("descriptionSpriteColor"_joaat).setFunction([](UserInterface::Submenu* dis)
		{
			dis->addOption(UserInterface::NumberOption<std::uint8_t>()
				.setLeftText(XorString("Red"))
				.setNumber(&g_UIManager->m_DescriptionSpriteColor.r)
				.setMin(0).setMax(255));
			dis->addOption(UserInterface::NumberOption<std::uint8_t>()
				.setLeftText(XorString("Green"))
				.setNumber(&g_UIManager->m_DescriptionSpriteColor.g)
				.setMin(0).setMax(255));
			dis->addOption(UserInterface::NumberOption<std::uint8_t>()
				.setLeftText(XorString("Blue"))
				.setNumber(&g_UIManager->m_DescriptionSpriteColor.b)
				.setMin(0).setMax(255));
			dis->addOption(UserInterface::NumberOption<std::uint8_t>()
				.setLeftText(XorString("Alpha"))
				.setNumber(&g_UIManager->m_DescriptionSpriteColor.a)
				.setMin(0).setMax(255));
		}));

		g_UIManager->addSubmenu(UserInterface::Submenu()
			.setName(XorString("Description Text Color"))
			.setId("descriptionTextColor"_joaat)
			.setFunction([](UserInterface::Submenu* dis)
		{
			dis->addOption(UserInterface::NumberOption<std::uint8_t>()
				.setLeftText(XorString("Red"))
				.setNumber(&g_UIManager->m_DescriptionTextColor.r)
				.setMin(0).setMax(255));
			dis->addOption(UserInterface::NumberOption<std::uint8_t>()
				.setLeftText(XorString("Green"))
				.setNumber(&g_UIManager->m_DescriptionTextColor.g)
				.setMin(0).setMax(255));
			dis->addOption(UserInterface::NumberOption<std::uint8_t>()
				.setLeftText(XorString("Blue"))
				.setNumber(&g_UIManager->m_DescriptionTextColor.b)
				.setMin(0).setMax(255));
			dis->addOption(UserInterface::NumberOption<std::uint8_t>()
				.setLeftText(XorString("Alpha"))
				.setNumber(&g_UIManager->m_DescriptionTextColor.a)
				.setMin(0).setMax(255));
		}));

		g_UIManager->addSubmenu(UserInterface::Submenu()
			.setName(XorString("Input"))
			.setId("inputSettings"_joaat)
			.setFunction([](UserInterface::Submenu* dis)
		{
			dis->addOption(UserInterface::BoolOption()
				.setLeftText(XorString("Numpad"))
				.setBool(&g_UIManager->m_InputNumpad)
				.setFunction([]
			{
				if (!g_UIManager->m_InputArrows && !g_UIManager->m_InputNumpad && !g_UIManager->m_InputController)
				{
					g_UIManager->m_InputNumpad = true;
				}
			}));
			dis->addOption(UserInterface::BoolOption().setLeftText(XorString("Arrows")).setBool(&g_UIManager->m_InputArrows).setFunction([]
			{
				if (!g_UIManager->m_InputArrows && !g_UIManager->m_InputNumpad && !g_UIManager->m_InputController)
				{
					g_UIManager->m_InputArrows = true;
				}
			}));
			dis->addOption(UserInterface::BoolOption().setLeftText(XorString("Controller")).setBool(&g_UIManager->m_InputController).setFunction([]
			{
				if (!g_UIManager->m_InputArrows && !g_UIManager->m_InputNumpad && !g_UIManager->m_InputController)
				{
					g_UIManager->m_InputArrows = true;
				}
			}));


			dis->addOption(UserInterface::NumberOption<std::int32_t>()
				.setLeftText(XorString("Scroll Delay"))
				.setNumber(&g_UIManager->m_ScrollInputDelay)
				.setMin(10).setMax(500).setStep(5));
			dis->addOption(UserInterface::NumberOption<std::int32_t>()
				.setLeftText(XorString("Horizontal Delay"))
				.setNumber(&g_UIManager->m_HorizonalInputDelay)
				.setMin(10).setMax(500).setStep(5));
			dis->addOption(UserInterface::NumberOption<std::int32_t>()
				.setLeftText(XorString("Enter Delay"))
				.setNumber(&g_UIManager->m_EnterInputDelay)
				.setMin(10).setMax(500).setStep(5));
			dis->addOption(UserInterface::NumberOption<std::int32_t>()
				.setLeftText(XorString("Back Delay"))
				.setNumber(&g_UIManager->m_BackInputDelay)
				.setMin(10).setMax(500).setStep(5));
			dis->addOption(UserInterface::NumberOption<std::int32_t>()
				.setLeftText(XorString("Open Delay"))
				.setNumber(&g_UIManager->m_OpenInputDelay)
				.setMin(10).setMax(500).setStep(5));
		}));

		g_UIManager->addSubmenu(UserInterface::Submenu()
			.setName(XorString("Sounds"))
			.setId("soundSettings"_joaat)
			.setFunction([](UserInterface::Submenu* dis)
		{
			dis->addOption(UserInterface::SimpleOption()
				.setLeftText(XorString("Enable All"))
				.setFunction([]
			{
				g_UIManager->m_SoundOpen = true;
				g_UIManager->m_SoundEnter = true;
				g_UIManager->m_SoundBack = true;
				g_UIManager->m_SoundUp = true;
				g_UIManager->m_SoundDown = true;
				g_UIManager->m_SoundLeft = true;
				g_UIManager->m_SoundRight = true;
			}));

			dis->addOption(UserInterface::SimpleOption()
				.setLeftText(XorString("Disable All"))
				.setFunction([]
			{
				g_UIManager->m_SoundOpen = false;
				g_UIManager->m_SoundEnter = false;
				g_UIManager->m_SoundBack = false;
				g_UIManager->m_SoundUp = false;
				g_UIManager->m_SoundDown = false;
				g_UIManager->m_SoundLeft = false;
				g_UIManager->m_SoundRight = false;
			}));

			dis->addOption(UserInterface::BoolOption()
				.setLeftText(XorString("Open"))
				.setBool(&g_UIManager->m_SoundOpen));
			dis->addOption(UserInterface::BoolOption()
				.setLeftText(XorString("Enter"))
				.setBool(&g_UIManager->m_SoundEnter));
			dis->addOption(UserInterface::BoolOption()
				.setLeftText(XorString("Back"))
				.setBool(&g_UIManager->m_SoundBack));
			dis->addOption(UserInterface::BoolOption()
				.setLeftText(XorString("Up"))
				.setBool(&g_UIManager->m_SoundUp));
			dis->addOption(UserInterface::BoolOption()
				.setLeftText(XorString("Down"))
				.setBool(&g_UIManager->m_SoundDown));
			dis->addOption(UserInterface::BoolOption()
				.setLeftText(XorString("Left"))
				.setBool(&g_UIManager->m_SoundLeft));
			dis->addOption(UserInterface::BoolOption()
				.setLeftText(XorString("Right"))
				.setBool(&g_UIManager->m_SoundRight));
		}));

		g_UIManager->addSubmenu(UserInterface::Submenu()
			.setName(XorString("Footer"))
			.setId("footerSettings"_joaat)
			.setFunction([](UserInterface::Submenu* dis)
		{
			dis->addOption(UserInterface::BoolOption()
				.setLeftText(XorString("Enable"))
				.setBool(&g_UIManager->m_DrawFooter));
			dis->addOption(UserInterface::BoolOption()
				.setLeftText(XorString("Dynamic"))
				.setBool(&g_UIManager->m_FooterDynamicIcon));
			dis->addOption(UserInterface::NumberOption<float>()
				.setLeftText(XorString("Size"))
				.setNumber(&g_UIManager->m_FooterSize)
				.setMin(0.005f).setMax(0.1f).setStep(0.001f));
			dis->addOption(UserInterface::SubmenuOption()
				.setLeftText(XorString("Background Color"))
				.setSub("footerBackgroundColor"_joaat));
			dis->addOption(UserInterface::SubmenuOption()
				.setLeftText(XorString("Icon Color"))
				.setSub("footerIconColor"_joaat));
		}));

		g_UIManager->addSubmenu(UserInterface::Submenu()
			.setName(XorString("Footer Background Color"))
			.setId("footerBackgroundColor"_joaat)
			.setFunction([](UserInterface::Submenu* dis)
		{
			dis->addOption(UserInterface::NumberOption<std::uint8_t>()
				.setLeftText(XorString("Red"))
				.setNumber(&g_UIManager->m_FooterBackground.r)
				.setMin(0).setMax(255));
			dis->addOption(UserInterface::NumberOption<std::uint8_t>()
				.setLeftText(XorString("Green"))
				.setNumber(&g_UIManager->m_FooterBackground.g)
				.setMin(0).setMax(255));
			dis->addOption(UserInterface::NumberOption<std::uint8_t>()
				.setLeftText(XorString("Blue"))
				.setNumber(&g_UIManager->m_FooterBackground.b)
				.setMin(0).setMax(255));
			dis->addOption(UserInterface::NumberOption<std::uint8_t>()
				.setLeftText(XorString("Alpha"))
				.setNumber(&g_UIManager->m_FooterBackground.a)
				.setMin(0).setMax(255));
		}));

		g_UIManager->addSubmenu(UserInterface::Submenu()
			.setName(XorString("Footer Icon Color"))
			.setId("footerIconColor"_joaat)
			.setFunction([](UserInterface::Submenu* dis)
		{
			dis->addOption(UserInterface::NumberOption<std::uint8_t>()
				.setLeftText(XorString("Red"))
				.setNumber(&g_UIManager->m_FooterIcon.r)
				.setMin(0).setMax(255));
			dis->addOption(UserInterface::NumberOption<std::uint8_t>()
				.setLeftText(XorString("Green"))
				.setNumber(&g_UIManager->m_FooterIcon.g)
				.setMin(0).setMax(255));
			dis->addOption(UserInterface::NumberOption<std::uint8_t>()
				.setLeftText(XorString("Blue"))
				.setNumber(&g_UIManager->m_FooterIcon.b)
				.setMin(0).setMax(255));
			dis->addOption(UserInterface::NumberOption<std::uint8_t>()
				.setLeftText(XorString("Alpha"))
				.setNumber(&g_UIManager->m_FooterIcon.a)
				.setMin(0).setMax(255));
		}));

		g_UIManager->addSubmenu(UserInterface::Submenu()
			.setName(XorString("Header"))
			.setId("headerSettings"_joaat)
			.setFunction([](UserInterface::Submenu* dis)
		{
			dis->addOption(UserInterface::ChooseOption<const char*, std::size_t>()
				.setLeftText(XorString("Type"))
				.setArray(&Lists::headerTypeFrontend)
				.setPositon(&Lists::headerTypePosition)
				.setExecuteOnHorizontal(true)
				.setFunction([]
			{
				g_UIManager->m_HeaderType = Lists::headerTypeBackend[Lists::headerTypePosition];
			}));

			switch (g_UIManager->m_HeaderType)
			{
			case UserInterface::HeaderType::Gradient:
				dis->addOption(UserInterface::SubmenuOption()
					.setLeftText(XorString("Background"))
					.setSub("headerGradientSettings"_joaat));
				break;
			case UserInterface::HeaderType::Static:
				dis->addOption(UserInterface::SubmenuOption()
					.setLeftText(XorString("Background"))
					.setSub("headerBackgroundSettings"_joaat));
			default:
				break;
			}

			dis->addOption(UserInterface::SubmenuOption()
				.setLeftText(XorString("Text"))
				.setSub("headerTextSettings"_joaat));
			dis->addOption(UserInterface::SubmenuOption()
				.setLeftText(XorString("Glare"))
				.setSub("headerGlareSettings"_joaat));
		}));

		g_UIManager->addSubmenu(UserInterface::Submenu()
			.setName(XorString("Header Gradient"))
			.setId("headerGradientSettings"_joaat)
			.setFunction([](UserInterface::Submenu* dis)
		{
#		if defined VEGA_DEV and 0
			dis->addOption(UserInterface::NumberOption<float>()
				.setLeftText(XorString("Smoothness"))
				.setNumber(&g_UIManager->m_HeaderGradientStretch)
				.setMin(0.f).setMax(0.25f).setStep(0.01f));
			dis->addOption(UserInterface::NumberOption<float>()
				.setLeftText(XorString("Filler"))
				.setNumber(&g_UIManager->m_HeaderGradientFiller)
				.setMin(0.f).setMax(1.f).setStep(0.001f));
#		endif
			dis->addOption(UserInterface::BoolOption()
				.setLeftText(XorString("Transparent"))
				.setBool(&g_UIManager->m_HeaderGradientTransparent));
			dis->addOption(UserInterface::BoolOption()
				.setLeftText(XorString("Flip"))
				.setBool(&g_UIManager->m_HeaderGradientFlip));
			dis->addOption(UserInterface::NumberOption<std::uint8_t>()
				.setLeftText(XorString("Red 1"))
				.setNumber(&g_UIManager->m_HeaderGradientColorLeft.r).setMin(0).setMax(255));
			dis->addOption(UserInterface::NumberOption<std::uint8_t>()
				.setLeftText(XorString("Green 1"))
				.setNumber(&g_UIManager->m_HeaderGradientColorLeft.g).setMin(0).setMax(255));
			dis->addOption(UserInterface::NumberOption<std::uint8_t>()
				.setLeftText(XorString("Blue 1"))
				.setNumber(&g_UIManager->m_HeaderGradientColorLeft.b).setMin(0).setMax(255));
			dis->addOption(UserInterface::NumberOption<std::uint8_t>()
				.setLeftText(XorString("Red 2"))
				.setNumber(&g_UIManager->m_HeaderGradientColorRight.r)
				.setMin(0).setMax(255));
			dis->addOption(UserInterface::NumberOption<std::uint8_t>()
				.setLeftText(XorString("Green 2"))
				.setNumber(&g_UIManager->m_HeaderGradientColorRight.g)
				.setMin(0).setMax(255));
			dis->addOption(UserInterface::NumberOption<std::uint8_t>()
				.setLeftText(XorString("Blue 2"))
				.setNumber(&g_UIManager->m_HeaderGradientColorRight.b)
				.setMin(0).setMax(255));
		}));

		g_UIManager->addSubmenu(UserInterface::Submenu()
			.setName(XorString("Header Text"))
			.setId("headerTextSettings"_joaat)
			.setFunction([](UserInterface::Submenu* dis)
		{
			dis->addOption(UserInterface::BoolOption()
				.setLeftText(XorString("Enable"))
				.setBool(&g_UIManager->m_DrawHeaderTitle));
			dis->addOption(UserInterface::BoolOption()
				.setLeftText(XorString("Shadow"))
				.setBool(&g_UIManager->m_HeaderTitleShadow));
			dis->addOption(UserInterface::BoolOption()
				.setLeftText(XorString("Outline"))
				.setBool(&g_UIManager->m_HeaderTitleOutline));

			dis->addOption(UserInterface::NumberOption<std::uint8_t>()
				.setLeftText(XorString("Red"))
				.setNumber(&g_UIManager->m_HeaderTextColor.r)
				.setMin(0).setMax(255));
			dis->addOption(UserInterface::NumberOption<std::uint8_t>()
				.setLeftText(XorString("Green"))
				.setNumber(&g_UIManager->m_HeaderTextColor.g)
				.setMin(0).setMax(255));
			dis->addOption(UserInterface::NumberOption<std::uint8_t>()
				.setLeftText(XorString("Blue"))
				.setNumber(&g_UIManager->m_HeaderTextColor.b)
				.setMin(0).setMax(255));
			dis->addOption(UserInterface::NumberOption<std::uint8_t>()
				.setLeftText(XorString("Alpha"))
				.setNumber(&g_UIManager->m_HeaderTextColor.a)
				.setMin(0).setMax(255));
		}));

		g_UIManager->addSubmenu(UserInterface::Submenu()
			.setName(XorString("Header Background"))
			.setId("headerBackgroundSettings"_joaat)
			.setFunction([](UserInterface::Submenu* dis)
		{
			dis->addOption(UserInterface::SubmenuOption()
				.setLeftText(XorString("Presets"))
				.setSub("headerColorPresets"_joaat));

			dis->addOption(UserInterface::NumberOption<std::uint8_t>()
				.setLeftText(XorString("Red"))
				.setNumber(&g_UIManager->m_HeaderBackgroundColor.r)
				.setMin(0).setMax(255));
			dis->addOption(UserInterface::NumberOption<std::uint8_t>()
				.setLeftText(XorString("Green"))
				.setNumber(&g_UIManager->m_HeaderBackgroundColor.g)
				.setMin(0).setMax(255));
			dis->addOption(UserInterface::NumberOption<std::uint8_t>()
				.setLeftText(XorString("Blue"))
				.setNumber(&g_UIManager->m_HeaderBackgroundColor.b)
				.setMin(0).setMax(255));
			dis->addOption(UserInterface::NumberOption<std::uint8_t>()
				.setLeftText(XorString("Alpha"))
				.setNumber(&g_UIManager->m_HeaderBackgroundColor.a)
				.setMin(0).setMax(255));
		}));

		g_UIManager->addSubmenu(UserInterface::Submenu()
			.setName(XorString("Header Glare"))
			.setId("headerGlareSettings"_joaat)
			.setFunction([](UserInterface::Submenu* dis)
		{
			dis->addOption(UserInterface::BoolOption()
				.setLeftText(XorString("Enable"))
				.setBool(&g_UIManager->m_DrawHeaderGlare));
			dis->addOption(UserInterface::NumberOption<std::make_signed_t<std::size_t>>()
				.setLeftText(XorString("Iterations"))
				.setNumber(&g_UIManager->m_HeaderGlareIterations)
				.setMin(1).setMax(20));
		}));

		g_UIManager->addSubmenu(UserInterface::Submenu()
			.setName(XorString("Header Background Presets"))
			.setId("headerColorPresets"_joaat)
			.setFunction([](UserInterface::Submenu* dis)
		{
			dis->addOption(UserInterface::SimpleOption()
				.setLeftText(XorString("Default"))
				.setFunction([]
			{
				g_UIManager->m_HeaderBackgroundColor = MENU_DEFAULT_HEADER_COLOR;
			}));
			dis->addOption(UserInterface::SimpleOption()
				.setLeftText(XorString("Blue"))
				.setFunction([]
			{
				g_UIManager->m_HeaderBackgroundColor = { 42, 150, 250, 255 };
			}));
		}));
#	pragma endregion
	}
	void MainScript::Tick() {
		g_MainScript.Init();
		g_ScriptCallbackManager = std::make_unique<ScriptCallbackManager>();
		Feature::g_WeaponModifiers.initialize();
		while (true) {
			Feature::g_AccountOptions.loop();
			Feature::g_PlayerOptions.loop();
			Feature::g_WeaponOptions.loop();
			Feature::g_VehicleOptions.loop();
			//Feature::g_WorldOptions.loop();
			//Feature::g_SpoonerOptions.loop();
			Feature::g_MiscOptions.loop();
			g_UIManager->onTick();
			Script::GetCurrent()->ScriptYield();
		}
	}
}