#pragma once
#include "PrecompiledHeader.hpp"
#include "Natives.hpp"
#include "Types.hpp"
#include "Reconstructed Classes/CEntityPool.hpp"
#include "Pointers.hpp"

namespace Vega
{
	struct Color;

#define VK_KEY_0 0x30
#define VK_KEY_1 0x31
#define VK_KEY_2 0x32
#define VK_KEY_3 0x33
#define VK_KEY_4 0x34
#define VK_KEY_5 0x35
#define VK_KEY_6 0x36
#define VK_KEY_7 0x37
#define VK_KEY_8 0x38
#define VK_KEY_9 0x39
#define VK_KEY_A 0x41
#define VK_KEY_B 0x42
#define VK_KEY_C 0x43
#define VK_KEY_D 0x44
#define VK_KEY_E 0x45
#define VK_KEY_F 0x46
#define VK_KEY_G 0x47
#define VK_KEY_H 0x48
#define VK_KEY_I 0x49
#define VK_KEY_J 0x4A
#define VK_KEY_K 0x4B
#define VK_KEY_L 0x4C
#define VK_KEY_M 0x4D
#define VK_KEY_N 0x4E
#define VK_KEY_O 0x4F
#define VK_KEY_P 0x50
#define VK_KEY_Q 0x51
#define VK_KEY_R 0x52
#define VK_KEY_S 0x53
#define VK_KEY_T 0x54
#define VK_KEY_U 0x55
#define VK_KEY_V 0x56
#define VK_KEY_W 0x57
#define VK_KEY_X 0x58
#define VK_KEY_Y 0x59
#define VK_KEY_Z 0x5A

#if SCRIPT_EVENT_STACKTRACE
#include <StackWalker.h>
	class StackWalkerGud : public StackWalker
	{
	public:
		StackWalkerGud() :
			StackWalker()
		{}
	protected:
		void OnOutput(LPCSTR szText) override
		{
			g_Logger->Info("%s", szText);
		}
	};
#endif

	namespace Util
	{
		FORCEINLINE char* uppercase(char* str)
		{
			for (; *str; ++str)
			{
				*str = static_cast<char>(toupper(*str));
			}

			return str;
		}



		bool isGameFocused();

		std::uint32_t joaat(const char* str);

		bool isKeyPressed(std::uint16_t vk);

		std::int32_t ptrToHandle(void* ptr);

		std::uintptr_t handleToPtrNoCast(std::int32_t handle);

		template <typename T>
		FORCEINLINE std::enable_if_t<std::is_pointer_v<T>, T> handleToPtr(std::int32_t handle)
		{
			return reinterpret_cast<T>(handleToPtrNoCast(handle));
		}

		void tryAndLog(void(*func)());

		void simpleNotify(std::uint32_t color, const char* format, ...);

		ReconstructedClasses::CPlayerInfo* getPlayerInfo(std::int32_t playerId);
		ReconstructedClasses::CPed* getPlayerPed(std::int32_t playerId);
		ReconstructedClasses::CPlayerInfo* getLocalPlayerInfo();
		ReconstructedClasses::CPed* getLocalPlayerPed();
		ReconstructedClasses::CNetGamePlayer* getLocalNetPlayer();
		std::int32_t getLocalPedHandle();
		std::int32_t getLocalPlayerId();
		ReconstructedClasses::CAutomobile* getLocalVehicle();

		bool isPedShooting(ReconstructedClasses::CPed* ped);
		bool isPlayerFreeAiming(ReconstructedClasses::CPlayerInfo* playerInfo);
		const char* getPlayerName(std::int32_t playerId);
		Vector3 getEntityPosition(const void* ent);
		Vector3 getEntityPositionHandle(std::int32_t handle);
		float getEntityHeading(const void* ent);
		std::uint32_t getEntityModel(const void* ent);
		std::uint32_t getEntityModelHandle(std::int32_t handle);
		bool isPedInVehicle(ReconstructedClasses::CPed* ped);

		float degToRad(float deg);
		float distance3D(Vector3 one, Vector3 two);
		Vector3 rot2dir(Vector3 rot);
		float entityDistance(void* ent1, void* ent2);

		template <std::size_t ArgCount, typename = std::enable_if_t<ArgCount != 0>>
		void triggerScriptEvent(std::int64_t(&args)[ArgCount], std::uint32_t bitMask)
		{
			g_Pointers->m_TriggerScriptEvent(1, (std::uint64_t*)&args[0], ArgCount, bitMask);
		}
	};

	inline constexpr std::uint8_t operator""_byte(std::uint64_t integer)
	{
		return static_cast<std::uint8_t>(integer);
	}

	static inline constexpr char constexprToLower(const char c)
	{
		return (c >= 'A' && c <= 'Z') ? c + ('a' - 'A') : c;
	}

	inline constexpr std::uint32_t operator ""_joaat(const char* str, std::size_t)
	{
		std::uint32_t hash = 0;
		for (; *str; ++str)
		{
			hash += constexprToLower(*str);
			hash += (hash << 10);
			hash ^= (hash >> 6);
		}

		hash += (hash << 3);
		hash ^= (hash >> 11);
		hash += (hash << 15);

		return hash;
	}

	inline bool isVowel(const char c)
	{
		switch (c)
		{
		case 'a':
		case 'e':
		case 'i':
		case 'o':
		case 'u':
		case 'A':
		case 'E':
		case 'I':
		case 'O':
		case 'U':
			return true;
		default:
			return false;
		}
	}

}
