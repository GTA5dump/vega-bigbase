#pragma once
#include "../PrecompiledHeader.hpp"
#include "AbstractOption.hpp"
#include "OptionFlags.hpp"
#include "Toggleable.hpp"

#include "../CharStream.hpp"

#include "../MainScript.hpp"

namespace Vega::UserInterface
{
	template <typename NumberType>
	class NumberBoolOption : public virtual AbstractOption, public virtual Toggleable
	{
	private:
		NumberType* m_Number = nullptr;
		NumberType m_Min = 0;
		NumberType m_Max = 1;
		NumberType m_Step = 1;

		std::size_t m_Precision = 3;
		bool m_ActionOnHorizontal = true;

		bool* m_Bool = nullptr;
		bool m_DisplayInverted = false;

		std::function<void()> m_Function;
	public:
		explicit NumberBoolOption()
		{
			m_Flags |= OptionFlags::Compound;
			m_Flags |= OptionFlags::Horizontal;
		}

		~NumberBoolOption() override = default;

		NumberBoolOption& setLeftText(const char* const text)
		{
			AbstractOption::setLeftText(text);
			return *this;
		}

		NumberBoolOption& setDescription(const char* const text)
		{
			AbstractOption::setDescription(text);
			return *this;
		}

		NumberBoolOption& setFunction(std::function<void()>&& function)
		{
			m_Function = std::move(function);
			return *this;
		}

		NumberBoolOption& setNumber(NumberType* const number)
		{
			m_Number = number;
			return *this;
		}

		NumberBoolOption& setMax(const NumberType max)
		{
			m_Max = max;
			return *this;
		}

		NumberBoolOption& setMin(const NumberType min)
		{
			m_Min = min;
			return *this;
		}

		NumberBoolOption& setStep(const NumberType step)
		{
			m_Step = step;
			return *this;
		}

		NumberBoolOption& setPrecision(const std::size_t precision)
		{
			m_Precision = precision;
			return *this;
		}

		NumberBoolOption& setActionOnHorizontal(const bool actionOnHorizontal)
		{
			m_ActionOnHorizontal = actionOnHorizontal;
			return *this;
		}

		NumberBoolOption& setBool(bool* const b00l)
		{
			m_Bool = b00l;
			return *this;
		}

		NumberBoolOption& setDisplayInverted(const bool displayInverted)
		{
			m_DisplayInverted = displayInverted;
			return *this;
		}

		bool isToggled() override
		{
			return *m_Bool ^ m_DisplayInverted;
		}

		const char* getRightText() override
		{
			CharMemStream stream(m_RightText);
			stream << std::setprecision(m_Precision) << std::fixed;

			if constexpr (sizeof(NumberType) != 1)
			{
				stream << *m_Number;
			}
			else
			{
				stream << static_cast<std::uint32_t>(*m_Number);
			}

			return m_RightText;
		}

		void onKeyPress(const KeyPressType type) override
		{
			switch (type)
			{
			case KeyPressType::Enter:
				if (m_Function)
				{
					 std::invoke(m_Function);
				}

				*m_Bool ^= true;
				break;
			case KeyPressType::Left:
				if (*m_Number - m_Step < m_Min)
				{
					*m_Number = m_Max;
				}
				else
				{
					*m_Number -= m_Step;
				}

				if (m_ActionOnHorizontal)
				{
					if (m_Function)
					{
						 std::invoke(m_Function);
					}
				}

				break;
			case KeyPressType::Right:
				if (*m_Number + m_Step > m_Max)
				{
					*m_Number = m_Min;
				}
				else
				{
					*m_Number += m_Step;
				}

				if (m_ActionOnHorizontal)
				{
					if (m_Function)
					{
						 std::invoke(m_Function);
					}
				}
			default:
				break;
			}
		}

		bool isFlagSet(const std::uint64_t flag) override
		{
			if (flag == OptionFlags::IsToggled)
			{
				return *m_Bool;
			}
			else
			{
				return AbstractOption::isFlagSet(flag);
			}
		}
	};
}
