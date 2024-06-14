#include "../PrecompiledHeader.hpp"
#include "SelectedPlayerSubmenu.hpp"

#include "../Reconstructed Classes/CPlayerInfo.hpp"
#include "../Util.hpp"
#include "../MainScript.hpp"

namespace Vega::UserInterface
{
	SelectedPlayerSubmenu::SelectedPlayerSubmenu()
		: Submenu()
	{}

	const char* SelectedPlayerSubmenu::getName()
	{
		if (auto playerInfo = Util::getPlayerInfo(g_SelectedPlayer))
		{
			std::strncpy(m_Name, playerInfo->m_net_player_data.m_name, sizeof(m_Name) - 1);
		}
		else
		{
			std::memset(m_Name, 0, sizeof(m_Name));
		}

		return m_Name;
	}
}
