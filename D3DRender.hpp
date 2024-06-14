#pragma once
#include "PrecompiledHeader.hpp"
#include "ImGui/imgui.h"
#include "ImGui/imgui_internal.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui_impl_win32.h"
#include <mutex>
#include <functional>
#include <optional>
#include <dxgi.h>

IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
namespace Vega {
	class VMT {
	public:
		explicit VMT(void* object, std::size_t numFuncs) :
			m_Object(reinterpret_cast<std::uintptr_t**>(object)), m_NumFuncs(numFuncs),
			m_Original(*m_Object), m_New(std::make_unique<std::uintptr_t[]>(m_NumFuncs + 1)) {
			std::copy_n(m_Original - 1, m_NumFuncs + 1, m_New.get());
		}
		~VMT() = default;
		void Enable() {
			*m_Object = &m_New[1];
		}
		void Disable() {
			*m_Object = m_Original;
		}
		void Hook(void* replacement, std::size_t index) {
			m_New[index + 1] = (std::uintptr_t)replacement;
		}
		void Unhook(std::size_t index) {
			m_New[index + 1] = m_Original[index];
		}
		template <typename T>
		T GetOriginal(std::size_t index) {
			return (T)m_Original[index];
		}
	private:
		std::uintptr_t** m_Object;
		std::size_t m_NumFuncs;
		std::uintptr_t* m_Original;
		std::unique_ptr<std::uintptr_t[]> m_New;
	};
	class D3DRender {
	public:
		D3DRender();
		~D3DRender();
	public:
		void Hook();
		void Unhook();
	public:
		void InitStyle();
		ImFont* m_Font;
		ImFontConfig m_FontCFG{};
	public:
		void D3DPresent();
		void PreResize();
		void PostResize();
	public:
		void WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	public:
		IDXGISwapChain* m_Swapchain;
		ID3D11Device* m_Device;
		ID3D11DeviceContext* m_Context;
	public:
		std::unique_ptr<VMT> m_dxVMT;
	};

	inline std::unique_ptr<D3DRender> g_D3DRender;

}