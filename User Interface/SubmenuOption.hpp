#pragma once
#include "AbstractOption.hpp"
#include "OptionFlags.hpp"
#include "Manager.hpp"

namespace Vega::UserInterface
{
	class SubmenuOption : public AbstractOption
	{
	private:
		std::uint32_t m_SubId;
		std::function<void()> m_Function;
	public:
		explicit SubmenuOption()
		{
			m_Flags |= OptionFlags::Submenu;
		}

		~SubmenuOption() override = default;

		SubmenuOption& setLeftText(const char* const text)
		{
			AbstractOption::setLeftText(text);
			return *this;
		}

		SubmenuOption& setRightText(const char* const text)
		{
			AbstractOption::setRightText(text);
			return *this;
		}

		SubmenuOption& setDescription(const char* const text)
		{
			AbstractOption::setDescription(text);
			return *this;
		}

		SubmenuOption& setFunction(std::function<void()>&& function)
		{
			m_Function = std::move(function);
			return *this;
		}

		SubmenuOption& setSub(const std::uint32_t id)
		{
			m_SubId = id;
			return *this;
		}

		void onKeyPress(const KeyPressType type) override
		{
			if (type == KeyPressType::Enter)
			{
				g_UIManager->switchToSub(m_SubId);
				if (m_Function)
				{
					std::invoke(m_Function);
				}
			}
		}
	};
}
