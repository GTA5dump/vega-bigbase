#include "PrecompiledHeader.hpp"
#include "Pointers.hpp"
#include "PatternScanner.hpp"

namespace Vega {
	Pointers::Pointers() {
		m_DoesCamExist = SIG_SCAN("8B D1 48 8B 0D ? ? ? ? E8 ? ? ? ? EB 03").sub(12).as<decltype(m_DoesCamExist)>();
		m_GetLabelText = SIG_SCAN("48 0F 45 D1 48 8D 0D ? ? ? ? E8").add(12).rip().as<decltype(m_GetLabelText)>();
		m_GetEventData = SIG_SCAN("48 85 C0 74 14 4C 8B 10").sub(28).as<decltype(m_GetEventData)>();
		m_NativeRegistrationTable = SIG_SCAN("48 8D 0D ? ? ? ? 48 8B 14 FA E8 ? ? ? ? 48 85 C0 75 0A").add(3).rip().as<decltype(m_NativeRegistrationTable)>();
		m_GetNativeHandler = SIG_SCAN("48 8D 0D ? ? ? ? 48 8B 14 FA E8 ? ? ? ? 48 85 C0 75 0A").add(12).rip().as<decltype(m_GetNativeHandler)>();
		m_FixVectors = SIG_SCAN("83 79 18 00 48 8B D1 74 4A FF 4A 18 48 63 4A 18 48 8D 41 04 48 8B 4C CA").as<decltype(m_FixVectors)>();
		m_DrawRect = SIG_SCAN("89 5C 24 20 E8 ? ? ? ? 89 5F 1C").add(5).rip().as<decltype(m_DrawRect)>();
		m_DrawSprite = SIG_SCAN("48 8B C4 48 89 58 08 48 89 68 10 48 89 70 18 57 48 81 EC ? ? ? ? 0F 29 70 E8 0F 29 78 D8 48 8B F2 48 8B D9 0F 28 FA 0F 28 F3 E8 ? ? ? ? 80 BC 24").as<decltype(m_DrawSprite)>();
		m_HasTextureDictLoaded = SIG_SCAN("48 83 EC 28 4C 8B C1 48 8D 54 24 ? 48 8D 0D ? ? ? ? E8 ? ? ? ? 83 7C 24").as<decltype(m_HasTextureDictLoaded)>();
		m_RequestTextureDict = SIG_SCAN("40 53 48 83 EC 50 8A DA 4C 8B C1").as<decltype(m_RequestTextureDict)>();
		m_BeginTextDisplay = SIG_SCAN("48 83 EC 28 E8 ? ? ? ? E8 ? ? ? ? 33 C9 48 85 C0").as<decltype(m_BeginTextDisplay)>();
		m_AddTextComponent = SIG_SCAN("8B 0D ? ? ? ? 33 D2 48 C1 E1 04").sub(169).as<decltype(m_AddTextComponent)>();
		m_EndTextDisplay = SIG_SCAN("33 C9 44 8B CB 48 85 C0").sub(30).as<decltype(m_EndTextDisplay)>();
		m_IsGamePaused = SIG_SCAN("40 F6 C7 02 0F B6 DB").sub(167).as<decltype(m_IsGamePaused)>();
		m_PointerToHandle = SIG_SCAN("49 8B CE E8 ? ? ? ? 49 8D 76 20").add(4).rip().as<decltype(m_PointerToHandle)>();
		m_GetNetworkTime = SIG_SCAN("40 53 48 83 EC 20 48 8B 0D ? ? ? ? E8 ? ? ? ? 84 C0 74 19 E8 ? ? ? ? 48 8D 15").as<decltype(m_GetNetworkTime)>();
		m_SetMobileRadioEnabledDuringGameplay = SIG_SCAN("40 53 48 83 EC 20 8A D9 48 8D 0D ? ? ? ? E8 ? ? ? ? 48 8D 0D ? ? ? ? 8B D0 E8 ? ? ? ? 48 85 C0 74 10").as<decltype(m_SetMobileRadioEnabledDuringGameplay)>();
		m_SetVehicleForwardSpeed = SIG_SCAN("F3 0F 59 D6 F3 0F 59 DE F3 0F 11 54 24").sub(62).as<decltype(m_SetVehicleForwardSpeed)>();
		m_IsPlayerPressingHorn = SIG_SCAN("48 8B CB E8 ? ? ? ? 84 C0 74 DE 48 8B 83").sub(40).as<decltype(m_IsPlayerPressingHorn)>();
		m_TriggerScriptEvent = SIG_SCAN("44 8B CF 45 8B C6 48 8B C8").sub(113).as<decltype(m_TriggerScriptEvent)>();
		m_TextInfo = SIG_SCAN("48 8B 05 ? ? ? ? 48 89 44 24 ? 48 8D 05 ? ? ? ? 48 89 44 24").add(15).rip().as<decltype(m_TextInfo)>();
		m_Resolution = SIG_SCAN("66 0F 6E 05 ? ? ? ? F3 0F 10 6D").add(4).rip().as<decltype(m_Resolution)>();
		m_NetworkPlayerMgr = SIG_SCAN("80 FB 20 73 14 48 8B 05 ? ? ? ? 0F B6 CB 48 8B BC C8 ? ? ? ? EB 02 33 FF 48 85 FF 74 21").add(8).rip().as<decltype(m_NetworkPlayerMgr)>();
		m_PedFactory = SIG_SCAN("48 8B 05 ? ? ? ? 48 8B 58 08 48 8B C3 48 83 C4 20").add(3).rip().as<decltype(m_PedFactory)>();
		m_EntityPool = SIG_SCAN("48 85 C0 75 1C 48 8B 0D").add(8).rip().as<decltype(m_EntityPool)>();
		m_PedPool = SIG_SCAN("0F 85 ? ? ? ? 48 8B 05 ? ? ? ? 8B 48 20 8B 40 10 C1 E1 02 C1 F9 02 2B C1 83 F8 0A").add(9).rip().as<decltype(m_PedPool)>();
		m_VehiclePool = SIG_SCAN("48 8B 1D ? ? ? ? 48 8D 4D 90").add(3).rip().as<decltype(m_VehiclePool)&>();
		auto pat = SIG_SCAN("4C 8B 05 ? ? ? ? 48 85 FF 0F");
		m_ObjectPool = pat.add(3).rip().as<decltype(m_ObjectPool)>();
		m_PickupPool = pat.add(19).rip().as<decltype(m_PickupPool)>();
		m_FrameCount = SIG_SCAN("F3 0F 10 0D ? ? ? ? 44 89 6B 08").add(4).rip().sub(8).as<decltype(m_FrameCount)>();
		m_GameState = SIG_SCAN("48 85 C9 74 4B 83 3D").add(7).rip().as<decltype(m_GameState)>();
		m_IsSessionStarted = SIG_SCAN("38 1D ? ? ? ? 74 1C").add(2).rip().as<decltype(m_IsSessionStarted)>();
		m_GlobalTable = SIG_SCAN("4C 8D 4D 08 48 8D 15 ? ? ? ? 4C 8B C0").add(7).rip().as<decltype(m_GlobalTable)>();
		m_Window = FindWindowA("grcWindow", "Grand Theft Auto V");
	}
	Pointers::~Pointers() {
	}
}