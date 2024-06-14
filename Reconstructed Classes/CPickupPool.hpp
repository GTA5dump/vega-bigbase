#pragma once
#include "../PrecompiledHeader.hpp"
#include "Declarations.hpp"
#pragma pack(push, 1)

namespace Vega::ReconstructedClasses
{
	struct CPickupPool
	{
		std::uintptr_t poolStartAddress;
		std::uint8_t* byteArray;
		std::int32_t size;
		std::int32_t itemSize;

		bool isValid(std::int32_t i)
		{
			return static_cast<bool>(mask(i));
		}

		std::uintptr_t getAddress(int i)
		{
			return mask(i) & (poolStartAddress + i * itemSize);
		}

		std::int64_t mask(std::int32_t i)
		{
			std::int64_t num1 = byteArray[i] & 0x80;
			return ~((num1 | -num1) >> 63);
		}

		void forAll(std::function<void(CPickup*)>&& callback)
		{
			std::int32_t count = 0;
			if (this) for (std::int32_t i = 0; i < size; ++i)
			{
				auto addr = getAddress(i);
				if (addr)
				{
					++count;
					callback((CPickup*)(addr));
				}
			}

			//g_Logger->Info("%i pickups in world", count);
		}
	};
}

#pragma pack(pop)
