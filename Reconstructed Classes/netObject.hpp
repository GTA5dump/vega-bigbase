#pragma once
#include <cstdint>
#include "networkSyncData.hpp"
#include "Declarations.hpp"
#pragma pack(push, 1)

namespace Vega::ReconstructedClasses
{
	namespace rage
	{
		struct netObject
		{
			std::uint16_t objectType;
			std::uint16_t objectId;
			std::uint32_t pad;

			networkSyncDataULBase syncData;

			netBlender* GetBlender()
			{
				return *(netBlender**)((std::uintptr_t)this + 88);
			}

			virtual ~netObject() = 0;

			virtual void m_8() = 0;
			virtual void m_10() = 0;
			virtual void m_18() = 0;
			virtual void* m_20() = 0;
			virtual void m_28() = 0;
			virtual netSyncTree* GetSyncTree() = 0;
			virtual void m_38() = 0;
			virtual void m_40() = 0;
			virtual void m_48() = 0;
			virtual void m_50() = 0;
			virtual void m_58() = 0;
			virtual void m_60() = 0;
			virtual void m_68() = 0;
			virtual void m_70() = 0;
			virtual void m_78() = 0;
			virtual void* GetGameObject() = 0;
			virtual void m_88() = 0;
			virtual void m_90() = 0;
			virtual void m_98() = 0;
			virtual int GetObjectFlags() = 0;
			virtual void m_A8() = 0;
			virtual void m_B0() = 0;
			virtual void m_B8() = 0;
			virtual void m_C0() = 0;
			virtual void m_C8() = 0;
			virtual int GetSyncFrequency() = 0;
			virtual void m_D8() = 0;
			virtual void m_E0() = 0;
			virtual void m_E8() = 0;
			virtual void m_F0() = 0;
			virtual void m_F8() = 0;
			virtual void Update() = 0;
			virtual void m_108() = 0;
			virtual void m_110() = 0;
			virtual void m_118() = 0;
			virtual void m_120() = 0;
			virtual void m_128() = 0;
			virtual void m_130() = 0;
			virtual void m_138() = 0;
			virtual void m_140() = 0;
			virtual void m_148() = 0;
			virtual void m_150() = 0;
			virtual bool m_158(void* player, int type, int* outReason) = 0;
			virtual void m_160() = 0;
			virtual bool m_168(int* outReason) = 0;
			virtual void m_170() = 0;
			virtual void m_178() = 0;
			virtual void m_180() = 0;
			virtual void m_188() = 0;
			virtual void m_190() = 0;
			virtual void m_198() = 0;
			virtual void m_1A0() = 0;
			virtual void m_1A8() = 0;
			virtual void m_1B0() = 0;
			virtual void m_1B8() = 0;
			virtual void m_1C0() = 0;
			virtual void m_1C8() = 0;
			virtual void m_1D0() = 0;
			virtual void m_1D8() = 0;
			virtual void m_1E0() = 0;
		};
	}
}
#pragma pack(pop)
