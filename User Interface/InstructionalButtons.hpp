#pragma once
#include "../PrecompiledHeader.hpp"

namespace Vega
{
	struct InstructionalButton
	{
		enum class Type
		{
			Numeric,
			String
		};

		char m_Name[32] = {};
		Type m_Type{};
		union
		{
			std::uint32_t m_NumericId;
			char m_StringId[32] = {};
		};
	};

	class InstructionalButtons
	{
	private:
		std::deque<InstructionalButton> m_Buttons{};
	public:
		explicit InstructionalButtons();
		~InstructionalButtons();

		void addCustom(const char* name, const char* custom);
		void addByInput(const char* name, std::uint32_t id);
		void addById(const char* name, std::uint32_t id);
		void draw();
	};
}
