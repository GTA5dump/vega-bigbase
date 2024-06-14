#pragma once
#include "PrecompiledHeader.hpp"
#include "FunctionTypes.hpp"
#include "Pointers.hpp"
#include "MainScript.hpp"
#include "Main.hpp"
#include "CustomText.hpp"
#include "D3DRender.hpp"

namespace Vega {
	bool doesCamExistDetour(std::int32_t handle);
	const char* getLabelTextDetour(std::int64_t unk, const char* label);
	bool getEventDataDetour(std::int32_t eventGroup, std::int32_t eventIndex, std::int64_t* args, std::int32_t argCount);
	LRESULT wndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	HRESULT swapchainPresentDetour(IDXGISwapChain* swapChain, UINT syncInterval, UINT flags);
	HRESULT resizeBuffersDetour(IDXGISwapChain* swapChain, UINT bufferCount, UINT width, UINT height, DXGI_FORMAT newFormat, UINT swapChainFlags);
	
	class Hooking {
	public:
		explicit Hooking();
		~Hooking();
	public:
		void Hook();
		void Unhook();
	public:
		//Orginials
		FunctionTypes::DoesCamExist* m_OriginalDoesCamExist;
		FunctionTypes::GetLabelText* m_OriginalGetLabelText;
		FunctionTypes::GetEventData* m_OriginalGetEventData;
		WNDPROC m_OriginalWndProc;
	};
	inline std::unique_ptr<Hooking> g_Hooking;
}