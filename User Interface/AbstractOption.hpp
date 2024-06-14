#pragma once
#include "../PrecompiledHeader.hpp"
#include "KeyPressType.hpp"
#include "OptionFlags.hpp"
#include <string>
#include "../XorString.hpp"

namespace Vega::UserInterface
{
	class AbstractOption
	{
	protected:
		std::uint64_t m_Flags = 0;

		char m_LeftText[64] = {};
		char m_RightText[64] = {};
		char m_Description[128] = {};
	
		void setLeftText(const char* const text)
		{
			std::strncpy(m_LeftText, text, sizeof(m_LeftText) - 1);
		}

		void setRightText(const char* const text)
		{
			std::strncpy(m_RightText, text, sizeof(m_RightText) - 1);
		}

		void setDescription(const char* const text)
		{
			m_Flags |= OptionFlags::Description;
			std::strncpy(m_Description, text, sizeof(m_Description) - 1);
		}

		explicit AbstractOption() = default;
	public:
		virtual ~AbstractOption() = default;

		virtual const char* getLeftText()
		{
			return m_LeftText;
		}

		virtual const char* getRightText()
		{
			return m_RightText;
		}

		virtual const char* getDescription()
		{
			return m_Description;
		}

		virtual bool isFlagSet(const std::uint64_t flag)
		{
			return static_cast<bool>(m_Flags & flag);
		}

		virtual void onKeyPress(const KeyPressType)
		{
		}

	};
}
