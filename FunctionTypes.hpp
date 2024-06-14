#pragma once
#include "PrecompiledHeader.hpp"
#include "Invoker.hpp"
#include "Reconstructed Classes/Declarations.hpp"
#include "Reconstructed Classes/CVehicle.hpp"

namespace Vega
{
	namespace Natives
	{
		struct Vector3;
	}

	namespace FunctionTypes {
		using GetLabelText = const char*(std::int64_t unk, const char* label);
		using DoesCamExist = bool(std::int32_t handle);
		using GetEventData = bool(std::int32_t eventGroup, std::int32_t eventIndex, std::int64_t* args, std::int32_t argCount);
		using GetNativeHandler = rage::scrNativeHandler(rage::scrNativeRegistrationTable*, std::uint64_t);
		using FixVectors = void(rage::scrNativeCallContext*);
		using DrawRect = void(float x, float y, float width, float height, std::int32_t r, std::int32_t g, std::int32_t b, std::int32_t a);
		using DrawSprite = void(const char* dict, const char* texture, float x, float y, float width, float height, float heading, std::int32_t red, std::int32_t green, std::int32_t blue, std::int32_t alpha);
		using HasTextureDictLoaded = bool(const char* dict);
		using RequestTextureDict = void(const char* dict);
		using BeginTextDisplay = void(const char* label);
		using AddTextComponent = void(const char* string);
		using EndTextDisplay = void(float x, float y);
		using IsGamePaused = bool(std::int32_t unk);
		using PointerToHandle = std::int32_t(void* ptr);
		using AttachEntToEnt = void(void* subject, void* target, std::int32_t boneIndex, Natives::Vector3* offset, Natives::Vector3* rot, bool unk0, bool softPinning, bool collision, bool isPed, std::int32_t vertexIndex, bool fixedRot, bool unk1);
		using GetNetworkTime = std::int32_t(void);
		using SetMobileRadioEnabledDuringGameplay = void(bool);
		using SetVehicleForwardSpeed = void(ReconstructedClasses::CAutomobile* vehicle, float speed);
		using IsPlayerPressingHorn = bool(ReconstructedClasses::CPlayerInfo* playerInfo);
		using TriggerScriptEvent = void(std::int32_t eventGroup, std::uint64_t* args, std::int32_t argCount, std::uint32_t bitMask);
	}
}
