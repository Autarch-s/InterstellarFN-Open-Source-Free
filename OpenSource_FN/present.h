#pragma once
#include "imports.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_internal.h"
#include <D3D11.h>
#include "Settings.h"
#include "xorstr.h"
#include "main.h"

#pragma comment(lib, "C:\\Program Files (x86)\\Microsoft DirectX SDK (June 2010)\\Lib\\x64\\d3d11.lib")

static bool firstTime = true;
static ID3D11DeviceContext* m_pContext;
static ID3D11RenderTargetView* g_pRenderTargetView;
static IDXGISwapChain* g_pSwapChain;
static ID3D11Device* uDevice;
static WNDPROC oWndProc;
HRESULT(*PresentOriginal)(IDXGISwapChain* pthis, UINT syncInterval, UINT flags) = nullptr;
using f_present = HRESULT(__stdcall*)(IDXGISwapChain* pthis, UINT sync_interval, UINT flags);
f_present swapchainA = nullptr;

bool showmenu = true;


ImGuiWindow& BeginScene() {
	ImGui_ImplDX11_NewFrame();
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 0));
	ImGui::Begin(E("##scene"), nullptr, ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoTitleBar);

	auto& io = ImGui::GetIO();
	ImGui::SetWindowPos(ImVec2(0, 0), ImGuiCond_Always);
	ImGui::SetWindowSize(ImVec2(io.DisplaySize.x, io.DisplaySize.y), ImGuiCond_Always);

	return *ImGui::GetCurrentWindow();
}

LRESULT CALLBACK WndProcHook(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (msg == WM_KEYUP && wParam == VK_F1)
	{
		if (settings::aimtype == 3)
			settings::aimtype = 1;
		else
			settings::aimtype += 1;
	}
	if (msg == WM_KEYUP && wParam == VK_F2)
	{
		settings::boxesp = !settings::boxesp;
	}
	if (msg == WM_KEYUP && wParam == VK_F3)
	{
		settings::snaplines = !settings::snaplines;
	}
	if (msg == WM_KEYUP && wParam == VK_F5)
	{
		settings::smooth -= 1;
	}
	if (msg == WM_KEYUP && wParam == VK_F6)
	{
		settings::smooth += 1;
	}

	return spoof_call(jmp, CallWindowProcW, oWndProc, hWnd, msg, wParam, lParam);
}

float hsv = 0;

HRESULT presenthook(IDXGISwapChain* swap, UINT sync_interval, UINT flags)
{
	ID3D11Texture2D* pRenderTargetTexture;

	g_pSwapChain = swap;

	if (firstTime)
	{

		swap->GetDevice(__uuidof(ID3D11Device), (void**)&uDevice);

		if (!uDevice) return false;

		uDevice->GetImmediateContext(&m_pContext);

		if (!m_pContext) return false;


		if (SUCCEEDED(swap->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pRenderTargetTexture)))
		{
			uDevice->CreateRenderTargetView(pRenderTargetTexture, NULL, &g_pRenderTargetView);
			pRenderTargetTexture->Release();
			uDevice->Release();
		}

		HWND window = iat(FindWindowA)(E("UnrealWindow"), E("Fortnite  "));
		if (!window) window = iat(GetForegroundWindow)();

		oWndProc = reinterpret_cast<WNDPROC>(iat(SetWindowLongPtrW)(window, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(WndProcHook)));

		firstTime = false;
	}


	if (!g_pRenderTargetView)
	{
		swap->GetDevice(__uuidof(ID3D11Device), (void**)&uDevice);

		if (!uDevice) return false;

		uDevice->GetImmediateContext(&m_pContext);

		if (!m_pContext) return false;

		if (SUCCEEDED(swap->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pRenderTargetTexture)))
		{
			uDevice->CreateRenderTargetView(pRenderTargetTexture, NULL, &g_pRenderTargetView);

			D3D11_TEXTURE2D_DESC backBufferDesc = { 0 };
			pRenderTargetTexture->GetDesc(&backBufferDesc);
			pRenderTargetTexture->Release();

			uDevice->Release();
		}
	}

	if (g_pRenderTargetView)
	{
		ImGui::GetIO().Fonts->AddFontFromFileTTF(E("C:\\Windows\\Fonts\\arialbd.ttf"), 12.0f);
		ImGui_ImplDX11_Init(iat(GetForegroundWindow)(), uDevice, m_pContext);
		ImGui_ImplDX11_CreateDeviceObjects();
		m_pContext->OMSetRenderTargets(1, &g_pRenderTargetView, NULL);
		auto& windowshit = BeginScene();
		windowshit.DrawList->PushClipRectFullScreen();

		if (hsv != 255)
			hsv += 1;
		else
			hsv = 0;

		if (discord::GetAsyncKeyState(VK_ADD))
			settings::fov += 1;

		if (discord::GetAsyncKeyState(VK_SUBTRACT))
			settings::fov -= 1;

		std::string mem = "";
		if (settings::aimtype == 1)
			mem = E("Mouse Aim");
		else if (settings::aimtype == 2)
			mem = E("Memory Aim");
		else if (settings::aimtype == 3)
			mem = E("None");

		std::string box = (settings::boxesp ? E("TRUE") : E("FALSE"));
		std::string snap = (settings::snaplines ? E("TRUE") : E("FALSE"));

		if (settings::smooth <= 0)
			settings::smooth = 0;

		std::string options = E("Aim Type (F1): ") + mem + E(" | Box ESP (F2): ") + box + E(" | Snaplines (F3): ") + snap + E(" | Aim Fov (+/-): ") + std::to_string(settings::fov) + E(" | Smooth (F5/F6): ") + std::to_string(settings::smooth) + E("    MEMORY AIM GIVES EXPLOIT BAN AFTER ~1H USE MOUSE AIM FOR UD!");

		windowshit.DrawList->AddText(ImVec2(50, 50), ImColor::HSV(hsv / 255.f, 255, 255), E("Interstellar Open Source Free (If You Bought This You Got Scammed)"));
		windowshit.DrawList->AddText(ImVec2(50, 75), ImColor::HSV(hsv / 255.f, 255, 255), options.c_str());

		windowshit.DrawList->AddCircle(ImVec2(spoof_call(jmp, GetSystemMetrics, 0) / 2, spoof_call(jmp, GetSystemMetrics, 1) / 2), settings::fov, ImGui::GetColorU32({ 0.f, 0.f, 0.f, 1.f }), 20, 1.f);

		cheatinit(windowshit, spoof_call(jmp, GetSystemMetrics, 0), spoof_call(jmp, GetSystemMetrics, 1));

		ImGui::End();
		ImGui::SetWindowPos(ImVec2(0, 0), ImGuiCond_Always);
		ImGui::SetWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y), ImGuiCond_Always);
		ImGui::SetNextWindowSize({ 500, 400 }, ImGuiCond_Always);
		ImGui::SetNextWindowCollapsed(false, ImGuiCond_Always);
		ImGui::Render();

		ImGui_ImplDX11_Shutdown();
	}

	return swapchainA(swap, sync_interval, flags);
}