#pragma once
#include "Invoker.hpp"

namespace Vega
{
	template <typename R, typename ...Args>
	inline R invoke(std::uint64_t hash, Args... args) {
		g_Invoker->BeginCall();
		(g_Invoker->Push(args), ...);
		g_Invoker->EndCall(hash);
		return g_Invoker->GetReturn<R>();
	}
}