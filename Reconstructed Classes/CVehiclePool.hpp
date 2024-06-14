#pragma once
#include "../PrecompiledHeader.hpp"
#include "Declarations.hpp"
#pragma pack(push, 1)

namespace Vega::ReconstructedClasses
{
	struct CVehiclePool
	{
		CAutomobile** listAddress;
		std::int32_t size;
		char padding1[0x24];
		std::uint32_t* bitArray;
		char padding2[0x28];
		std::int32_t itemCount;

		bool isValid(std::size_t i)
		{
			return (bitArray[i >> 5] >> (i & 0x1F)) & 1;
		}

		CAutomobile* getAddress(std::size_t i)
		{
			return listAddress[i];
		}

		void forAll(std::function<void(CAutomobile*)>&& callback)
		{
			std::int32_t count = 0;
			if (this) for (std::int32_t i = 0; i < size; ++i)
			{
				if (!isValid(i)) continue;

				auto addr = getAddress(i);
				if (addr)
				{
					++count;
					callback((CAutomobile*)(addr));
				}
			}

			//g_Logger->Info("%i vehicles in world", count);
		}
	};
}

#pragma pack(pop)
