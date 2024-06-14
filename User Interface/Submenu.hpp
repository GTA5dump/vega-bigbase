#pragma once
#include "../PrecompiledHeader.hpp"
#include "AbstractOption.hpp"

namespace Vega::UserInterface
{
	class Submenu
	{
	public:
		using Function = void(Submenu*);
	protected:
		std::uint32_t m_Id;
		char m_Name[64];
		std::function<Submenu::Function> m_Function;
		std::deque<std::unique_ptr<AbstractOption>> m_Options;
		std::size_t m_CurrentOption;
	public:
		Submenu() = default;
		virtual ~Submenu() = default;

		Submenu(Submenu&&) = default;
		Submenu(Submenu const&) = delete;

		virtual bool operator==(Submenu const& that);

		virtual std::uint32_t getId() const;
		virtual Submenu& setId(std::uint32_t id);

		virtual const char* getName();
		virtual Submenu& setName(const char* text);

		virtual Submenu& clear();
		virtual Submenu& execute();
		virtual Submenu& setFunction(std::function<Submenu::Function>&& function);

		template <typename T>
		Submenu& addOption(T&& option);

		virtual std::deque<std::unique_ptr<AbstractOption>>& getOptions();
		virtual std::size_t getCurrentOption();
		virtual Submenu& setCurrentOption(std::size_t currentOption);

		virtual std::size_t getOptionCountWithoutBreaks();
		virtual std::size_t getSelectedWithoutBreaks();
	};

	template<typename T>
	inline Submenu& Submenu::addOption(T&& option)
	{
		auto ptr = std::make_unique<std::remove_reference_t<T>>(option);
		m_Options.push_back(std::move(ptr));
		return *this;
	}
}
