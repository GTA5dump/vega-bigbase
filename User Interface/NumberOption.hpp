#pragma once
#include "../PrecompiledHeader.hpp"
#include "AbstractOption.hpp"
#include "OptionFlags.hpp"
#include "../ScriptMgr.hpp"
#include "../MainScript.hpp"
#include "../Util.hpp"
#include "../Hooking.hpp"

#include "../CharStream.hpp"

namespace Vega::UserInterface
{
	template <typename NumberType, typename = std::enable_if_t<std::is_arithmetic_v<NumberType>>>
	class NumberOption : public virtual AbstractOption
	{
	private:
		NumberType* m_Number = nullptr;
		NumberType m_Min = 0;
		NumberType m_Max = 1;
		NumberType m_Step = 1;
		
		std::size_t m_Precision = 3;
		bool m_Unlimited = false;
		bool m_TextBox = true;
		bool m_ActionOnHorizontal = true;

		std::function<void()> m_Function;
	public:
		explicit NumberOption()
		{
			m_Flags |= OptionFlags::Horizontal;
		}

		~NumberOption() override = default;

		NumberOption& setLeftText(const char* const text)
		{
			AbstractOption::setLeftText(text);
			return *this;
		}

		NumberOption& setDescription(const char* const text)
		{
			AbstractOption::setDescription(text);
			return *this;
		}

		NumberOption& setFunction(std::function<void()>&& function)
		{
			m_Function = std::move(function);
			return *this;
		}

		NumberOption& setNumber(NumberType* const number)
		{
			m_Number = number;
			return *this;
		}

		NumberOption& setMax(const NumberType max)
		{
			m_Max = max;
			return *this;
		}

		NumberOption& setMin(const NumberType min)
		{
			m_Min = min;
			return *this;
		}

		NumberOption& setStep(const NumberType step)
		{
			m_Step = step;
			return *this;
		}

		NumberOption& setPrecision(const std::size_t precision)
		{
			m_Precision = precision;
			return *this;
		}

		NumberOption& setUnlimited(const bool unlimited)
		{
			m_Unlimited = unlimited;
			return *this;
		}

		NumberOption& setTextBox(const bool textBox)
		{
			m_TextBox = textBox;
			return *this;
		}

		NumberOption& setActionOnHorizontal(const bool actionOnHorizontal)
		{
			m_ActionOnHorizontal = actionOnHorizontal;
			return *this;
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
				if (m_TextBox)
				{
					Natives::MISC::DISPLAY_ONSCREEN_KEYBOARD(0, XorString("ENTER_A_NUMBER"), "", "", "", "", "", 20);
					while (!Natives::MISC::UPDATE_ONSCREEN_KEYBOARD())
					{
						Script::GetCurrent()->ScriptYield();
					}

					auto result = Natives::MISC::GET_ONSCREEN_KEYBOARD_RESULT();

					if (result)
					{
						std::stringstream converter;
						converter << result;
						if constexpr (sizeof(NumberType) != 1)
						{
							NumberType num{};
							converter >> num;

							if (!converter.fail())
							{
								if (m_Unlimited || (num <= m_Max && num >= m_Min))
								{
									*m_Number = num;
								}
								else
								{
									std::ostringstream failMsg;
									failMsg << std::fixed << std::setprecision(m_Precision)
										<< g_UIManager->m_SubmenuStack.top()->getName()
										<< " -> "
										<< m_LeftText
										<< "~n~"
										<< XorString("Out of range!~n~")
										<< XorString("Entered: ")
										<< num
										<< XorString("~n~Min: ")
										<< m_Min
										<< XorString("~n~Max: ")
										<< m_Max;
									Util::simpleNotify(6, XorString("%s"), failMsg.str().c_str());
								}
							}
							else
							{
								Util::simpleNotify(6, XorString("That's not a number!"));
							}
						}
						else
						{
							std::int32_t num{};
							converter >> num;

							if (!converter.fail())
							{
								if (m_Unlimited || (num <= m_Max && num >= m_Min))
								{
									*m_Number = static_cast<NumberType>(num);
								}
								else
								{
									std::ostringstream failMsg;
									failMsg << std::fixed << std::setprecision(m_Precision)
										<< g_UIManager->m_SubmenuStack.top()->getName()
										<< " -> "
										<< m_LeftText
										<< "~n~"
										<< XorString("Out of range!~n~")
										<< XorString("Entered: ")
										<< static_cast<std::int32_t>(num)
										<< XorString("~n~Min: ")
										<< static_cast<std::int32_t>(m_Min)
										<< XorString("~n~Max: ")
										<< static_cast<std::int32_t>(m_Max);


									Util::simpleNotify(6, XorString("%s"), failMsg.str().c_str());
								}
							}
							else
							{
								Util::simpleNotify(6, XorString("That's not a number!"));
							}
						}
					}
				}

				if (m_Function)
				{
					 std::invoke(m_Function);
				}
				break;
			case KeyPressType::Left:
				if (!m_Unlimited)
				{
					if (*m_Number - m_Step < m_Min)
					{
						*m_Number = m_Max;
					}
					else
					{
						*m_Number -= m_Step;
					}
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
				if (!m_Unlimited)
				{
					if (*m_Number + m_Step > m_Max)
					{
						*m_Number = m_Min;
					}
					else
					{
						*m_Number += m_Step;
					}
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
	};
}
