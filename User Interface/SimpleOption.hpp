#pragma once
#include "../PrecompiledHeader.hpp"
#include "AbstractOption.hpp"
#include "../MainScript.hpp"

namespace Vega::UserInterface
{
	class SimpleOption : public AbstractOption
	{
	private:
		std::function<void()> m_Function;
	public:
		explicit SimpleOption() = default;
		~SimpleOption() override = default;

		SimpleOption& setLeftText(const char* const text)
		{
			AbstractOption::setLeftText(text);
			return *this;
		}

		SimpleOption& setRightText(const char* const text)
		{
			AbstractOption::setRightText(text);
			return *this;
		}

		SimpleOption& setDescription(const char* const text)
		{
			AbstractOption::setDescription(text);
			return *this;
		}

		SimpleOption& setFunction(std::function<void()>&& function)
		{
			m_Function = std::move(function);
			return *this;
		}

		void onKeyPress(const KeyPressType type) override
		{
			if (type == KeyPressType::Enter)
			{
				if (m_Function)
				{
					 std::invoke(m_Function);
				}
			}
		}
	};
}
