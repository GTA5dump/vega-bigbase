#pragma once
#include "../PrecompiledHeader.hpp"
#include "AbstractOption.hpp"
#include "Toggleable.hpp"

namespace Vega::UserInterface
{
	class BoolOption : public virtual AbstractOption, public virtual Toggleable
	{
	private:
		bool* m_Bool = nullptr;
		bool m_DisplayInverted = false;
		std::function<void()> m_Function;
	public:
		explicit BoolOption() = default;
		~BoolOption() override = default;

		BoolOption& setLeftText(const char* const text)
		{
			AbstractOption::setLeftText(text);
			return *this;
		}

		BoolOption& setDescription(const char* const text)
		{
			AbstractOption::setDescription(text);
			return *this;
		}

		BoolOption& setFunction(std::function<void()>&& function)
		{
			m_Function = std::move(function);
			return *this;
		}

		BoolOption& setBool(bool* const b00l)
		{
			m_Bool = b00l;
			return *this;
		}
		
		BoolOption& setDisplayInverted(const bool displayInverted)
		{
			m_DisplayInverted = displayInverted;
			return *this;
		}

		const char* getRightText() override
		{
			std::strncpy(m_RightText, (*m_Bool ^ m_DisplayInverted) ? XorString("~g~ON") : XorString("~r~OFF"), sizeof(m_RightText) - 1);
			return m_RightText;
		}

		void onKeyPress(const KeyPressType type) override
		{
			if (type == KeyPressType::Enter)
			{
				*m_Bool ^= true;
				if (m_Function)
				{
					 std::invoke(m_Function);
				}
			}
		}

		bool isToggled() override
		{
			return *m_Bool;
		}
	};
}
