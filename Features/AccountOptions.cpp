#include "../PrecompiledHeader.hpp"
#include "AccountOptions.hpp"
#include "../Natives.hpp"
#include "../Util.hpp"
#include "../Pointers.hpp"

namespace Vega::Feature
{
	AccountOptions g_AccountOptions;

	void AccountOptions::loop()
	{
		if (*g_Pointers->m_IsSessionStarted)
		{
		}
	}

	void AccountOptions::applyGiveMoney()
	{
		static auto timer = GetTickCount();
		if ((GetTickCount() - timer) >= 1000)
		{
			timer = GetTickCount();

			std::int32_t transactionId = 0;
			Natives::NETSHOP::_NETWORK_SHOP_BEGIN_SERVICE(&transactionId, 1474183246, 312105838, 1445302971, 10000000, 4);
			Natives::NETSHOP::_NETWORK_SHOP_CHECKOUT_START(transactionId);
		}
	}

	void AccountOptions::applyRemoveMoney()
	{
		static auto timer = GetTickCount();
		if ((GetTickCount() - timer) >= 1000)
		{
			timer = GetTickCount();

			std::int32_t transactionId = 0;
			Natives::NETSHOP::_NETWORK_SHOP_BEGIN_SERVICE(&transactionId, 1474183246, 2039302543, 1445302971, 1000000, 4);
			Natives::NETSHOP::_NETWORK_SHOP_CHECKOUT_START(transactionId);
		}
	}
}
