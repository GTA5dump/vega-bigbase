#pragma once
#include "PrecompiledHeader.hpp"

namespace Vega
{
	class ScriptGlobal
	{
	private:
		static void* indexToPtr(std::size_t index);
	private:
		std::uintptr_t m_Index;
	public:
		ScriptGlobal(std::size_t index);

		ScriptGlobal operator()(std::size_t index, std::size_t size) const;
		ScriptGlobal operator[](std::size_t index) const;

		template <typename T>
		std::enable_if_t<std::is_pointer<T>::value, T> as() const
		{
			return reinterpret_cast<T>(indexToPtr(m_Index));
		}

		template <typename T>
		std::enable_if_t<std::is_lvalue_reference<T>::value, T> as() const
		{
			return *reinterpret_cast<std::add_pointer_t<std::remove_reference_t<T>>>(indexToPtr(m_Index));
		}

		template <typename T>
		std::enable_if_t<std::is_same<T, std::uintptr_t>::value, T> as() const
		{
			return reinterpret_cast<std::uintptr_t>(as<void*>());
		}
	};

	ScriptGlobal operator ""_Global(std::uintptr_t index);
	ScriptGlobal operator ""_Tunable(std::uintptr_t index);
	ScriptGlobal operator ""_PlayerInfo(std::uintptr_t index);

}
