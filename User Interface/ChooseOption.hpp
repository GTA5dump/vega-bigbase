#pragma once
#include "../PrecompiledHeader.hpp"
#include "AbstractOption.hpp"

#include "../CharStream.hpp"

namespace Vega
{
	template<typename S, typename T, typename = void>
	struct is_streamable : std::false_type
	{};

	template<typename S, typename T>
	struct is_streamable<S, T,
		std::void_t<decltype(std::declval<S&>() << std::declval<T>())>>
		: std::true_type
	{};

	template <typename T1, typename T2, typename T3 = void>
	inline constexpr bool is_streamable_v = is_streamable<T1, T2, T3>::value;
}

namespace Vega::UserInterface
{
	template <typename DataType, typename PositionType, typename = std::enable_if_t<is_streamable_v<CharMemStream, DataType>>>
	class ChooseOption : public virtual AbstractOption
	{
	private:
		bool m_ExecuteOnHorizontal = false;

		DataType const* m_Data = nullptr;
		std::size_t m_DataSize = 0;
		PositionType* m_Position = nullptr;

		std::function<void()> m_Function;
		std::function<void(std::ostream*, DataType const*)> m_NameGetter = [](std::ostream* stream, decltype(m_Data) data)
		{
			*stream << *data;
		};
	public:
		explicit ChooseOption()
		{
			m_Flags |= OptionFlags::Horizontal;
		}

		~ChooseOption() override = default;

		ChooseOption& setLeftText(const char* const text)
		{
			AbstractOption::setLeftText(text);
			return *this;
		}

		ChooseOption& setDescription(const char* const text)
		{
			AbstractOption::setDescription(text);
			return *this;
		}

		ChooseOption& setFunction(std::function<void()>&& function)
		{
			m_Function = std::move(function);
			return *this;
		}

		template <typename VectorType>
		ChooseOption& setVector(const VectorType* const vector)
		{
			m_Data = &(*vector)[0];
			m_DataSize = (*vector).size();
			return *this;
		}

		template <std::size_t ArraySize>
		ChooseOption& setArray(DataType const(*const arr)[ArraySize])
		{
			m_Data = *arr;
			m_DataSize = ArraySize;
			return *this;
		}

		ChooseOption& setPositon(PositionType* const position)
		{
			m_Position = position;
			return *this;
		}
		
		ChooseOption& setExecuteOnHorizontal(const bool executeOnHorizontal)
		{
			m_ExecuteOnHorizontal = executeOnHorizontal;
			return *this;
		}

		ChooseOption& setNameGetter(std::function<void(std::ostream*, DataType const*)> nameGetter)
		{
			m_NameGetter = std::move(nameGetter);
			return *this;
		}

		const char* getRightText() override
		{
			if (m_DataSize != 0)
			{
				CharMemStream stream(m_RightText);

				if (m_NameGetter)
				{
					std::invoke(m_NameGetter, &stream, m_Data + *m_Position);
				}

				stream << " ~c~[" << *m_Position + 1 << " / " << m_DataSize << "]";
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
				break;
			case KeyPressType::Left:
				if (*m_Position > 0)
				{
					--(*m_Position);
				}
				else
				{
					*m_Position = m_DataSize - 1;
				}

				if (m_ExecuteOnHorizontal && m_Function)
				{
					 std::invoke(m_Function);
				}

				break;
			case KeyPressType::Right:
				if (*m_Position < m_DataSize - 1)
				{
					++(*m_Position);
				}
				else
				{
					*m_Position = 0;
				}

				if (m_ExecuteOnHorizontal && m_Function)
				{
					 std::invoke(m_Function);
				}
			default:
				break;
			}
		}
	};
}
