#include "Submenu.hpp"

namespace Vega::UserInterface
{
	class SelectedPlayerSubmenu : public Submenu
	{
	public:
		SelectedPlayerSubmenu();
		virtual ~SelectedPlayerSubmenu() override = default;

		SelectedPlayerSubmenu(SelectedPlayerSubmenu const&) = delete;
		SelectedPlayerSubmenu(SelectedPlayerSubmenu&&) = default;

		const char* getName() override;

	};
}
