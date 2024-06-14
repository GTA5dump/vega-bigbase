#pragma once
#include "../PrecompiledHeader.hpp"
#include "AbstractOption.hpp"
#include "OptionFlags.hpp"

namespace Vega::UserInterface
{
	class BreakOption : public AbstractOption
	{
	public:
		explicit BreakOption()
		{
			m_Flags |= OptionFlags::Break;
		}
		~BreakOption() override = default;

		BreakOption& setText(const char* const text)
		{
			AbstractOption::setLeftText(text);
			return *this;
		}
	};
}
