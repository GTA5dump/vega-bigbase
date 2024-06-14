#pragma once

namespace Vega::UserInterface
{
	namespace OptionFlags
	{
		enum OptionFlags
		{
			Horizontal = (1 << 0),
			Submenu = (1 << 1),
			Break = (1 << 2),
			Description = (1 << 3),
			Compound = (1 << 4),
			IsToggled = (1 << 5)
		};
	}
}
