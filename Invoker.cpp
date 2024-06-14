#include "PrecompiledHeader.hpp"
#include "Entrypoints.hpp"
#include "Invoker.hpp"
#include "Hooking.hpp"
#include "Entrypoints.hpp"
#include "Pointers.hpp"

namespace Vega
{
	bool Invoker::MapNatives() {
		for (auto const& mapping : g_Crossmap) {
			rage::scrNativeHandler handler = g_Pointers->m_GetNativeHandler(
				g_Pointers->m_NativeRegistrationTable, mapping.m_New);
			m_Cache.emplace(mapping.m_Old, handler);
		}
		return false;
	}
	void Invoker::BeginCall() {
		m_Context.Reset();
	}
	void Invoker::EndCall(std::uint64_t hash) {
		if (auto cache = m_Cache.find(hash); cache != m_Cache.end()) {
			rage::scrNativeHandler handler = cache->second;
			if (!handler) {
				g_Logger->Info("Failed to invoke native %p", hash);
				return;
			}
			handler(&m_Context);
			//FV
			g_Pointers->m_FixVectors(&m_Context);
		}
		else {
			g_Logger->Error("Failed to find 0x%p native's handler.", hash);
		}
	}
}