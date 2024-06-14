#include "LogScript.hpp"
#include "Natives.hpp"

namespace Vega {
	namespace { std::size_t s_LastLogCount = 0; }
	void LogScript::OnPresentTick() {
		ImGui::SetNextWindowSize(ImVec2(500, 250), ImGuiCond_Once);
		ImGui::SetNextWindowPos(ImVec2(50, 50), ImGuiCond_Once);
		if (ImGui::Begin("Log")) {
			auto msgs = g_Logger->GetMessages();
			for (std::size_t i = 0; i < msgs.second; ++i) {
				ImGui::TextUnformatted(msgs.first[i].get());
			}
			if (s_LastLogCount != msgs.second) {
				s_LastLogCount = msgs.second;
				ImGui::SetScrollHereY(1.f);
			}
			ImGui::End();
		}
	}
	void LogScript::Tick() {
		//Send/put single load functions sent here only load during runtime.
		while (true) {
			//Loop funcs here
			if (g_LogScript.m_LockMouse) {
				PAD::DISABLE_ALL_CONTROL_ACTIONS(0);
			}
			Script::GetCurrent()->ScriptYield();
		}
	}
}