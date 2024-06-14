#include "PrecompiledHeader.hpp"
#include "ScriptGlobal.hpp"
#include "Pointers.hpp"

namespace Vega
{
	void* ScriptGlobal::indexToPtr(const std::size_t index)
	{
		return (g_Pointers->m_GlobalTable[index >> 0x12 & 0x3F]) + (index & 0x3FFFF);
	}

	ScriptGlobal::ScriptGlobal(const std::size_t index) :
		m_Index(index)
	{
	}

	ScriptGlobal ScriptGlobal::operator()(const std::size_t index, const std::size_t size) const
	{
		return ScriptGlobal(m_Index + 1 + (index * size));
	}

	ScriptGlobal ScriptGlobal::operator[](const std::size_t index) const
	{
		return ScriptGlobal(m_Index + index);
	}

	ScriptGlobal operator ""_Global(const std::uintptr_t index)
	{
		return ScriptGlobal(index);
	}

	ScriptGlobal operator ""_Tunable(const std::uintptr_t index)
	{
		return ScriptGlobal(0x40001 + index);
	}

	ScriptGlobal operator ""_PlayerInfo(const std::uintptr_t playerId)
	{
		return ScriptGlobal(0x18C80F + 1 + (playerId * 0x22E) + 0x1EB);
	}
}
