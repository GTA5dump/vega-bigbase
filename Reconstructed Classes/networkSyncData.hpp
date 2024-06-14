#pragma once
#include <cstdint>
#pragma pack(push, 1)

namespace Vega::ReconstructedClasses
{
	namespace rage
	{
		struct networkSyncDataULBase
		{
			virtual ~networkSyncDataULBase() {}
			virtual void m_8() {}
			virtual void m_10() {}
			virtual void m_18() {}
			virtual void m_20() {}
			virtual void m_28() {}
			virtual void m_30() {}
			virtual void m_38() {}
			virtual void m_40() {}
			virtual void m_48() {}
			virtual void m_50() {}
			virtual void SetCloningFrequency(std::int32_t player, std::int32_t frequency)
			{
				UNREFERENCED_PARAMETER(player);
				UNREFERENCED_PARAMETER(frequency);
			}

			std::uint8_t pad_10h[49];
			std::uint8_t ownerId;
			std::uint8_t nextOwnerId;
			std::uint8_t isRemote;
			std::uint8_t wantsToDelete : 1; // netobj+76
			std::uint8_t unk1 : 1;
			std::uint8_t shouldNotBeDeleted : 1;
			std::uint8_t pad_4Dh[3];
			std::uint8_t pad_50h[32];
			std::uint32_t creationAckedPlayers; // netobj+112
			std::uint32_t m64;
			std::uint32_t m68;
			std::uint32_t m6C;

			bool IsCreationAckedByPlayer(std::int32_t index)
			{
				return static_cast<bool>(creationAckedPlayers & (1 << index));
			}
		};
	}
}
#pragma pack(pop)
