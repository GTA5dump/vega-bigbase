#pragma once
#include "PrecompiledHeader.hpp"
#include "Invoker.hpp"
#include "FunctionTypes.hpp"
#include "Reconstructed Classes/Declarations.hpp"

namespace Vega
{
	constexpr auto MAX_PLAYERS = 32Ui64;
	class Pointers {
	public:
		Pointers();
		~Pointers();
	public:
		template <typename T, typename ...TArgs>
		decltype(auto) call(T func, TArgs&&... args) {
			__try {
				return func(std::forward<TArgs>(args)...);
			}
			__except (EXCEPTION_EXECUTE_HANDLER) {
			}
		}
	public:
		rage::scrNativeRegistrationTable* m_NativeRegistrationTable;
	public:
		FunctionTypes::DoesCamExist* m_DoesCamExist;
		FunctionTypes::GetLabelText* m_GetLabelText;
		FunctionTypes::GetEventData* m_GetEventData;
		FunctionTypes::GetNativeHandler* m_GetNativeHandler;
		FunctionTypes::FixVectors* m_FixVectors;
		FunctionTypes::DrawRect* m_DrawRect;
		FunctionTypes::DrawSprite* m_DrawSprite;
		FunctionTypes::HasTextureDictLoaded* m_HasTextureDictLoaded;
		FunctionTypes::RequestTextureDict* m_RequestTextureDict;
		FunctionTypes::BeginTextDisplay* m_BeginTextDisplay;
		FunctionTypes::AddTextComponent* m_AddTextComponent;
		FunctionTypes::EndTextDisplay* m_EndTextDisplay;
		FunctionTypes::IsGamePaused* m_IsGamePaused;
		FunctionTypes::PointerToHandle* m_PointerToHandle;
		FunctionTypes::GetNetworkTime* m_GetNetworkTime;
		FunctionTypes::SetMobileRadioEnabledDuringGameplay* m_SetMobileRadioEnabledDuringGameplay;
		FunctionTypes::SetVehicleForwardSpeed* m_SetVehicleForwardSpeed;
		FunctionTypes::IsPlayerPressingHorn* m_IsPlayerPressingHorn;
		FunctionTypes::TriggerScriptEvent* m_TriggerScriptEvent;
	public:
		ReconstructedClasses::CTextInfo* m_TextInfo;
		ReconstructedClasses::CResolution* m_Resolution;
		ReconstructedClasses::CNetworkPlayerMgr** m_NetworkPlayerMgr;
		ReconstructedClasses::CPedFactory** m_PedFactory;
		ReconstructedClasses::CEntityPool** m_EntityPool;
		ReconstructedClasses::CPedPool** m_PedPool;
		ReconstructedClasses::CVehiclePool** m_VehiclePool;
		ReconstructedClasses::CObjectPool** m_ObjectPool;
		ReconstructedClasses::CPickupPool** m_PickupPool;
	public:
		std::uint32_t* m_FrameCount;
		Enums::GameState* m_GameState;
		bool* m_IsSessionStarted;
		std::uintptr_t** m_GlobalTable;
		HWND m_Window;
	};
	inline std::unique_ptr<Pointers> g_Pointers;
}