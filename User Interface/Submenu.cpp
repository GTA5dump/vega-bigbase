#include "../PrecompiledHeader.hpp"
#include "Submenu.hpp"
#include "OptionFlags.hpp"

namespace Vega::UserInterface
{
	bool Submenu::operator==(Submenu const& that)
	{
		return that.m_Id == m_Id;
	}

	std::uint32_t Submenu::getId() const
	{
		return m_Id;
	}

	Submenu& Submenu::setId(const std::uint32_t id)
	{
		m_Id = id;
		return *this;
	}

	const char* Submenu::getName()
	{
		return m_Name;
	}

	Submenu& Submenu::setName(const char* const text)
	{
		std::strncpy(m_Name, text, sizeof(m_Name));
		return *this;
	}

	Submenu& Submenu::setFunction(std::function<Submenu::Function>&& function)
	{
		m_Function = std::move(function);
		return *this;
	}

	Submenu& Submenu::clear()
	{
		m_Options.clear();
		return *this;
	}

	Submenu& Submenu::execute()
	{
		if (m_Function)
		{
			std::invoke(m_Function, this);
		}

		if (m_CurrentOption && m_CurrentOption >= m_Options.size())
		{
			m_CurrentOption = m_Options.size() - 1;
		}

		return *this;
	}

	std::deque<std::unique_ptr<AbstractOption>>& Submenu::getOptions()
	{
		return m_Options;
	}

	std::size_t Submenu::getCurrentOption()
	{
		return m_CurrentOption;
	}

	Submenu& Submenu::setCurrentOption(std::size_t currentOption)
	{
		m_CurrentOption = currentOption;
		return *this;
	}

	std::size_t Submenu::getOptionCountWithoutBreaks()
	{
		std::size_t count = 0;

		for (decltype(auto) i : m_Options)
		{
			if (!i->isFlagSet(OptionFlags::Break))
			{
				++count;
			}
		}

		return count;
	}

	std::size_t Submenu::getSelectedWithoutBreaks()
	{
		std::size_t count = 0;
		for (std::size_t i = 0, end = m_Options.size(); i < end && i < m_CurrentOption; ++i)
		{
			if (!m_Options[i]->isFlagSet(OptionFlags::Break))
			{
				++count;
			}
		}

		return count;
	}
}
