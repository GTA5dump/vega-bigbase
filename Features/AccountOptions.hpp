#pragma once
#include "../PrecompiledHeader.hpp"

namespace Vega::Feature
{
	class AccountOptions
	{
	public:
		bool m_GiveMoney = false;
		bool m_RemoveMoney = false;

		void loop();

		void applyGiveMoney();
		void applyRemoveMoney();
	};

	extern AccountOptions g_AccountOptions;
}
