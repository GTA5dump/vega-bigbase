#pragma once
#include "../PrecompiledHeader.hpp"
#include "Declarations.hpp"
#pragma pack(push, 1)

namespace Vega::ReconstructedClasses
{
	struct CEntityPool
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
			std::int64_t num1 = byteArray[i] & 0x80; // check for high bit.
			return ~((num1 | -num1) >> 63);
		}

		std::uintptr_t handleToPtr(std::int32_t handle)
		{
			return *reinterpret_cast<std::uintptr_t*>(getAddress(handle >> 8) + 8);
		}

		void forAll(std::function<void(std::uintptr_t)>&& callback)
		{
			std::int32_t count = 0;
			if (this) for (std::int32_t i = 0; i < size; ++i)
			{
				auto addr = getAddress(i);
				if (addr)
				{
					++count;
					callback(*reinterpret_cast<std::uintptr_t*>(addr + 8));
				}
			}

			//g_Logger->Info("%i entities in world", count);
		}

		void forAllVehicles(std::function<void(CAutomobile*)>&& callback)
		{
			if (this) forAll([=](std::uintptr_t ent)
			{
				if (*(BYTE*)(ent + 0x28) == Enums::EntityTypeVehicle)
				{
					callback((CAutomobile*)(ent));
				}
			});
		}

		void forAllPeds(std::function<void(CPed*)>&& callback)
		{
			if (this) forAll([=](std::uintptr_t ent)
			{
				if (*(BYTE*)(ent + 0x28) == Enums::EntityTypePed)
				{
					callback((CPed*)(ent));
				}
			});
		}

		void forAllObjects(std::function<void(CObject*)>&& callback)
		{
			if (this) forAll([=](std::uintptr_t ent)
			{
				if (*(BYTE*)(ent + 0x28) == Enums::EntityTypeObject)
				{
					callback((CObject*)(ent));
				}
			});
		}
	};
}

#pragma pack(pop)
