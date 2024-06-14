#include "D3DRender.hpp"
#include "Pointers.hpp"
#include "Main.hpp"
#include "Hooking.hpp"
#include "PatternScanner.hpp"
#include "LogScript.hpp"

namespace Vega {
	D3DRender::D3DRender() {
		Hook();
		if (FAILED(m_Swapchain->GetDevice(__uuidof(ID3D11Device), (void**)(&m_Device))))
			g_Logger->Error("Failed to get D3D Device");
		m_Device->GetImmediateContext(&m_Context);
		ImGui::CreateContext();
		ImGui_ImplDX11_Init(m_Device, m_Context);
		ImGui_ImplWin32_Init(g_Pointers->m_Window);
		InitStyle();
	}
	D3DRender::~D3DRender() {
		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
		Unhook();
	}
	void D3DRender::Hook() {
		m_Swapchain = SIG_SCAN("48 8B 0D ? ? ? ? 48 8B 01 44 8D 43 01 33 D2 FF 50 40 8B C8").add(3).rip().as<decltype(m_Swapchain)&>();
		m_dxVMT = std::make_unique<VMT>(m_Swapchain, 19);
		m_dxVMT->Hook(&swapchainPresentDetour, 8);
		m_dxVMT->Hook(&resizeBuffersDetour, 13);
		m_dxVMT->Enable();
	}
	void D3DRender::Unhook() {
		m_dxVMT->Unhook(8);
		m_dxVMT->Unhook(13);
		m_dxVMT->Disable();
	}
	void D3DRender::InitStyle() {
		auto& IO = ImGui::GetIO();
		auto& Style = ImGui::GetStyle();
		auto& Color = Style.Colors;
		auto& Fonts = IO.Fonts;
		m_FontCFG.FontDataOwnedByAtlas = false;
		m_Font = Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Arial.ttf", 18.f, &m_FontCFG);
		Style.Alpha = 0.95f;
		Style.WindowPadding = ImVec2(10, 10);
		Style.WindowMinSize = ImVec2(100, 100);
		Style.WindowRounding = 0.0f;
		Style.WindowTitleAlign = ImVec2(0.0f, 0.5f);
		Style.FramePadding = ImVec2(5, 5);
		Style.FrameRounding = 0.0f;
		Style.ItemSpacing = ImVec2(5, 5);
		Style.ItemInnerSpacing = ImVec2(4, 4);
		Style.TouchExtraPadding = ImVec2(0, 0);
		Style.IndentSpacing = 21.0f;
		Style.ColumnsMinSpacing = 6.0f;
		Style.ScrollbarSize = 16.0f;
		Style.ScrollbarRounding = 9.0f;
		Style.GrabMinSize = 10.0f;
		Style.GrabRounding = 0.0f;
		Style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
		Color[ImGuiCol_Text] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
		Color[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
		Color[ImGuiCol_TextSelectedBg] = ImVec4(0.00f, 0.00f, 1.00f, 0.35f);
		Color[ImGuiCol_WindowBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
		Color[ImGuiCol_ChildBg] = ImVec4(0.10f, 0.10f, 0.10f, 0.00f);
		Color[ImGuiCol_PopupBg] = ImVec4(0.05f, 0.05f, 0.10f, 0.90f);
		Color[ImGuiCol_Border] = ImVec4(0.0f, 0.3f, 0.70f, 0);
		Color[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		Color[ImGuiCol_FrameBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
		Color[ImGuiCol_FrameBgHovered] = ImVec4(0.90f, 0.80f, 0.80f, 0.40f);
		Color[ImGuiCol_FrameBgActive] = ImVec4(0.90f, 0.65f, 0.65f, 0.45f);
		Color[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
		Color[ImGuiCol_TitleBgCollapsed] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
		Color[ImGuiCol_TitleBgActive] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
		Color[ImGuiCol_MenuBarBg] = ImVec4(0.40f, 0.40f, 0.55f, 0.80f);
		Color[ImGuiCol_ScrollbarBg] = ImVec4(0.20f, 0.25f, 0.30f, 0.60f);
		Color[ImGuiCol_ScrollbarGrab] = ImVec4(0.40f, 0.40f, 0.80f, 0.30f);
		Color[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.40f, 0.40f, 0.80f, 0.40f);
		Color[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.80f, 0.50f, 0.50f, 0.40f);
		Color[ImGuiCol_CheckMark] = ImVec4(0.00f, 0.60f, 0.90f, 0.50f);
		Color[ImGuiCol_SliderGrab] = ImVec4(1.00f, 1.00f, 1.00f, 0.30f);
		Color[ImGuiCol_SliderGrabActive] = ImVec4(0.80f, 0.50f, 0.50f, 1.00f);
		Color[ImGuiCol_Button] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
		Color[ImGuiCol_ButtonHovered] = ImVec4(0.40f, 0.00f, 0.00f, 1.00f);
		Color[ImGuiCol_ButtonActive] = ImVec4(0.70f, 0.20f, 0.00f, 0.83f);
		Color[ImGuiCol_Header] = ImVec4(0.40f, 0.40f, 0.90f, 0.45f);
		Color[ImGuiCol_HeaderHovered] = ImVec4(0.45f, 0.45f, 0.90f, 0.80f);
		Color[ImGuiCol_HeaderActive] = ImVec4(0.53f, 0.53f, 0.87f, 0.80f);
		Color[ImGuiCol_Separator] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
		Color[ImGuiCol_SeparatorHovered] = ImVec4(0.70f, 0.60f, 0.60f, 1.00f);
		Color[ImGuiCol_SeparatorActive] = ImVec4(0.90f, 0.70f, 0.70f, 1.00f);
		Color[ImGuiCol_ResizeGrip] = ImVec4(1.00f, 1.00f, 1.00f, 0.30f);
		Color[ImGuiCol_ResizeGripHovered] = ImVec4(1.00f, 1.00f, 1.00f, 0.60f);
		Color[ImGuiCol_ResizeGripActive] = ImVec4(1.00f, 1.00f, 1.00f, 0.90f);
		Color[ImGuiCol_PlotLines] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		Color[ImGuiCol_PlotLinesHovered] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
		Color[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
		Color[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
		Color[ImGuiCol_ModalWindowDimBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);
	}
	void D3DRender::D3DPresent() {
		if (g_LogScript.m_LockMouse) {
			ImGui::GetIO().MouseDrawCursor = true;
			ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_NoMouse;
		}
		else {
			ImGui::GetIO().MouseDrawCursor = false;
			ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouse;
		}
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
		g_LogScript.OnPresentTick();
		ImGui::EndFrame();
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}
	void D3DRender::PreResize() {
		ImGui_ImplDX11_InvalidateDeviceObjects();
	}
	void D3DRender::PostResize() {
		ImGui_ImplDX11_CreateDeviceObjects();
	}
	void D3DRender::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
		if (msg == WM_KEYUP && wParam == 'X') {
			g_LogScript.m_LockMouse ^= true;
		}
		if (ImGui::GetCurrentContext()) {
			ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam);
		}
	}
}