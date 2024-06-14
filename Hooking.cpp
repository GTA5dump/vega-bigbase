#include "PrecompiledHeader.hpp"
#include <MinHook.h>

#include "Hooking.hpp"
#include "ScriptMgr.hpp"
#include "PatternScanner.hpp"

#pragma comment(lib, "MinHook.lib")

namespace Vega {
	Hooking::Hooking() {
		g_Logger->Info(XorString("MH_Initialize: %s"), MH_StatusToString(MH_Initialize()));
	}

	Hooking::~Hooking() {
		g_Logger->Info(XorString("MH_Uninitialize: %s"), MH_StatusToString(MH_Uninitialize()));
	}
	void Hooking::Hook() {
		g_Logger->Info(XorString("Does Cam Exist Create: %s"), MH_StatusToString(MH_CreateHook(g_Pointers->m_DoesCamExist, &doesCamExistDetour, reinterpret_cast<void**>(&m_OriginalDoesCamExist))));
		g_Logger->Info(XorString("Does Cam Exist Enable: %s"), MH_StatusToString(MH_EnableHook(g_Pointers->m_DoesCamExist)));

		g_Logger->Info(XorString("Get Label Text Create: %s"), MH_StatusToString(MH_CreateHook(g_Pointers->m_GetLabelText, &getLabelTextDetour, reinterpret_cast<void**>(&m_OriginalGetLabelText))));
		g_Logger->Info(XorString("Get Label Text Hook Enable: %s"), MH_StatusToString(MH_EnableHook(g_Pointers->m_GetLabelText)));
		m_OriginalWndProc = (WNDPROC)SetWindowLongPtrW(g_Pointers->m_Window, GWLP_WNDPROC, (LONG_PTR)&wndProc);
	}

	void Hooking::Unhook() {
		g_Logger->Info(XorString("Does Cam Exist Hook Disable: %s"), MH_StatusToString(MH_DisableHook(g_Pointers->m_DoesCamExist)));
		g_Logger->Info(XorString("Does Cam Exist Hook Remove: %s"), MH_StatusToString(MH_RemoveHook(g_Pointers->m_DoesCamExist)));

		g_Logger->Info(XorString("Get Label Text Hook Disable: %s"), MH_StatusToString(MH_DisableHook(g_Pointers->m_GetLabelText)));
		g_Logger->Info(XorString("Get Label Text Hook Remove: %s"), MH_StatusToString(MH_RemoveHook(g_Pointers->m_GetLabelText)));
		SetWindowLongPtrW(g_Pointers->m_Window, GWLP_WNDPROC, (LONG_PTR)m_OriginalWndProc);
	}
	namespace { std::uint32_t g_HookFrameCount{}; }
	bool doesCamExistDetour(std::int32_t handle) {
		if (!g_ShouldExit && g_HookFrameCount != *g_Pointers->m_FrameCount) {
			g_HookFrameCount = *g_Pointers->m_FrameCount;
			g_ScriptManager.ScriptTick();
		}
		return g_Hooking->m_OriginalDoesCamExist(handle);
	}
	const char* getLabelTextDetour(std::int64_t unk, const char* label) {
		auto custom = g_CustomText.get(label);
		if (!g_ShouldExit && custom) {
			return custom;
		}
		return g_Hooking->m_OriginalGetLabelText(unk, label);
	}
	bool getEventDataDetour(std::int32_t eventGroup, std::int32_t eventIndex, std::int64_t* args, std::int32_t argCount) {
		auto ret = g_Hooking->m_OriginalGetEventData(eventGroup, eventIndex, args, argCount);
		if (!g_ShouldExit && ret) {
			std::ostringstream argStr;
			for (std::size_t i = 0; i < argCount; ++i) {
				argStr << args[i];
				if (i + 1 != argCount) argStr << ", ";
			}
			g_Logger->Info("Event Group: %i", eventGroup);
			g_Logger->Info("Event Index: %i", eventIndex);
			g_Logger->Info("Event Args: %s", argStr.str().c_str());
			g_Logger->Info("Event Argcount: %i", argCount);
			g_Logger->Info("---------------");
		}
		return ret;
	}
	LRESULT wndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
		g_D3DRender->WndProc(hWnd, msg, wParam, lParam);
		return CallWindowProcW(g_Hooking->m_OriginalWndProc, hWnd, msg, wParam, lParam);
	}
	HRESULT swapchainPresentDetour(IDXGISwapChain* swapChain, UINT syncInterval, UINT flags) {
		if (!g_ShouldExit) {
			g_D3DRender->D3DPresent();
		}
		return g_D3DRender->m_dxVMT->GetOriginal<decltype(&swapchainPresentDetour)>(8)(swapChain, syncInterval, flags);
	}
	HRESULT resizeBuffersDetour(IDXGISwapChain* swapChain, UINT bufferCount, UINT width, UINT height, DXGI_FORMAT newFormat, UINT swapChainFlags) {
		if (!g_ShouldExit) {
			g_D3DRender->PreResize();
			auto hr =
				g_D3DRender->m_dxVMT->GetOriginal<decltype(&resizeBuffersDetour)>(13)(
					swapChain, bufferCount,
					width, height, newFormat,
					swapChainFlags);
			if (SUCCEEDED(hr)) {
				g_D3DRender->PostResize();
			}
			return hr;
		}
		return g_D3DRender->m_dxVMT->GetOriginal<decltype(&resizeBuffersDetour)>(13)(
			swapChain, bufferCount,
			width, height, newFormat,
			swapChainFlags);
	}
}