#pragma once
#include "cheat/pch.h"

#include "cheat/winapi.h"
#include "cheat/offsets.h"
#include "cheat/entity.h"
#include "cheat/math.h"
#include "cheat/draw.h"
#include "cheat/aimbot.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"




int displayWidth = GetSystemMetrics(SM_CXSCREEN);
int displayHeight = GetSystemMetrics(SM_CYSCREEN);
// classes 
runTimeInfo::pInfo pInfo;
draw Draw;
aim aimbot;
runTimeInfo run;
entity ent;
entity localPlayer;

//imgui
#define IM_COL32(R, G, B, A) (((ImU32)(A) << 24) | ((ImU32)(B) << 16) | ((ImU32)(G) << 8) | ((ImU32)(R)))

	// RGB struct to store color components
typedef struct {
	int R, G, B;
} RGB;

// Function to convert RGB to ImU32 using IM_COL32
ImU32 Color(RGB color) {
	return IM_COL32(color.R, color.G, color.B, 255); // Default alpha = 255 (opaque)
}

// Updated drawEsp function
void drawEsp1(entity ent, runTimeInfo::pInfo pInfo, entity localPlayer) {
	Offsets offsets;
	myMath math;
	myMath::Vec2 screen;
	std::vector<entity> entities = ent.readEntityList(pInfo);
	ent.readLocalplayer(pInfo, localPlayer);

	for (auto& entity : entities) {
		if (entity.entHealth <= 0) {
			continue; // Skip dead entities
		}
		
		ReadProcessMemory(pInfo.pHandle, (LPCVOID)(offsets.viewMatrix + pInfo.baseAddr), &ent.viewMatrix, sizeof(ent.viewMatrix), NULL);

		// Calculate 3D distance directly in the function
		float distance = sqrtf(powf(localPlayer.headX - entity.headX, 2) +
			powf(localPlayer.headY - entity.headY, 2) +
			powf(localPlayer.headZ - entity.headZ, 2));

		// Scale box size based on distance
		float scaleFactor = 44.0f / (distance + 0.09f); // Avoid division by zero
		int w = static_cast<int>(35 * scaleFactor); // Width scaled by distance
		int h = static_cast<int>(75 * scaleFactor); // Height scaled by distance

		// Transform entity position to screen coordinates
		if (math.WorldToScreen(entity, &screen, ent.viewMatrix, pInfo.windowWidth, pInfo.windowHeight)) {
			// Adjust the box to align with the body
			float headOffset = h * 0.09f; // Align box so that the body fills it

			// Calculate health color
			int red = static_cast<int>(255 * (100 - entity.entHealth) / 100.0f);
			int green = static_cast<int>(255 * entity.entHealth / 100.0f);
			ImU32 healthColor = IM_COL32(red, green, 0, 255);

			// Display health text above the box
			std::string healthText = std::to_string(entity.entHealth) + "HP";
			ImGui::GetBackgroundDrawList()->AddText(
				ImVec2(screen.x - ImGui::CalcTextSize(healthText.c_str()).x / 2, screen.y - headOffset - 15),
				healthColor,
				healthText.c_str()
			);

			// Display distance text
			std::string distanceText = std::to_string(static_cast<int>(distance)) + "m";
			ImGui::GetBackgroundDrawList()->AddText(
				ImVec2(screen.x - ImGui::CalcTextSize(distanceText.c_str()).x / 2, screen.y + h - 5),
				IM_COL32(255, 255, 255, 255), // White color for distance text
				distanceText.c_str()
			);

			// Draw ESP box
			if (entity.teamId == localPlayer.teamId) {
				// Green for teammates
				ImGui::GetBackgroundDrawList()->AddRect(
					ImVec2(screen.x - w / 2, screen.y - headOffset),
					ImVec2(screen.x + w / 2, screen.y + h - headOffset),
					IM_COL32(0, 255, 0, 255)
				);
			}
			else {
				// Red for enemies
				ImGui::GetBackgroundDrawList()->AddRect(
					ImVec2(screen.x - w / 2, screen.y - headOffset),
					ImVec2(screen.x + w / 2, screen.y + h - headOffset),
					IM_COL32(255, 0, 0, 255)
				);
			}
		}
	}
}



extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


LRESULT CALLBACK window_procedure(HWND window, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// set up ImGui window procedure handler
	if (ImGui_ImplWin32_WndProcHandler(window, msg, wParam, lParam))
		return true;

	// switch that disables alt application and checks for if the user tries to close the window.
	switch (msg)
	{
	case WM_SYSCOMMAND:
		if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu (imgui uses it in their example :shrug:)
			return 0;
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_CLOSE:
		return 0;
	}

	// define the window procedure
	return DefWindowProc(window, msg, wParam, lParam);
}

INT APIENTRY  WinMain(HINSTANCE instance, HINSTANCE, PSTR, INT cmd_show)
{


	run.setup(pInfo.pHandle, pInfo);


	WNDCLASSEXW wc = {  };
	wc.cbSize = sizeof(WNDCLASSEXW);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = window_procedure;
	wc.hInstance = instance;
	wc.lpszClassName = L"AC_Cheat";



	RegisterClassExW(&wc);


	const HWND overlay = CreateWindowExW(WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_LAYERED, wc.lpszClassName, L"AC_Cheat", WS_POPUP, 0, 0, displayWidth, displayHeight, nullptr, nullptr, wc.hInstance, nullptr);


	SetLayeredWindowAttributes(overlay, RGB(0, 0, 0), BYTE(255), LWA_ALPHA);
	{

		RECT clientArea;
		GetClientRect(overlay, &clientArea);

		RECT windowArea;
		GetWindowRect(overlay, &windowArea);

		POINT diff{};
		ClientToScreen(overlay, &diff);

		const MARGINS margins = {
			windowArea.left + (diff.x - windowArea.left),
			windowArea.top + (diff.y - windowArea.top),
			clientArea.right,
			clientArea.bottom
		};

		DwmExtendFrameIntoClientArea(overlay, &margins);
	}



	DXGI_SWAP_CHAIN_DESC scd = {};
	scd.BufferDesc.RefreshRate.Numerator = 144; // fps
	scd.BufferDesc.RefreshRate.Denominator = 1U;
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.SampleDesc.Count = 1U;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.BufferCount = 2U;
	scd.OutputWindow = overlay;
	scd.Windowed = TRUE;
	scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	constexpr D3D_FEATURE_LEVEL levels[2]{

		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_0


	};


	ID3D11Device* device = {nullptr};
	ID3D11DeviceContext* deviceContext = { nullptr };
	IDXGISwapChain* swapChain = { nullptr };
	ID3D11RenderTargetView* renderTargetView = { nullptr };
	D3D_FEATURE_LEVEL level{};


	//create device

	D3D11CreateDeviceAndSwapChain(nullptr, 
		D3D_DRIVER_TYPE_HARDWARE, 
		nullptr, 
		0U, 
		levels
		,2U
		, D3D11_SDK_VERSION
		, &scd
		, &swapChain
		, &device
		,&level
		,&deviceContext);


	ID3D11Texture2D* backBuffer = { nullptr };
	swapChain->GetBuffer(0U,IID_PPV_ARGS(&backBuffer));
	if (backBuffer)
	{
		device->CreateRenderTargetView(backBuffer, nullptr, &renderTargetView);
		backBuffer->Release();


	}
	else
	{
		return 1;
	}
	ShowWindow(overlay, cmd_show);
	UpdateWindow(overlay);

	ImGui::CreateContext();
	ImGui::StyleColorsDark();


	ImGui_ImplWin32_Init(overlay);
	ImGui_ImplDX11_Init(device, deviceContext);

	bool overlayRunning = true;

	while (overlayRunning)
	{
		MSG msg;
		while (PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (msg.message == WM_QUIT)
			{
				overlayRunning = false;
			}
		}

		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
		drawEsp1(ent, pInfo, localPlayer);
		//if right mouse button is pressed
		
		ImGui::Render();

		float color[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
		deviceContext->OMSetRenderTargets(1U,&renderTargetView, nullptr);
		deviceContext->ClearRenderTargetView(renderTargetView, color);

		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		swapChain->Present(0U, 0U);
			




	}






	//exiting

	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
	if (swapChain)
	{

		swapChain->Release();
	}
	if (deviceContext)
	{
		deviceContext->Release();
	}
	if (device)
	{
		device->Release();
	}
	if (renderTargetView)
	{
		renderTargetView->Release();
	}
	DestroyWindow(overlay);
	UnregisterClassW(wc.lpszClassName, wc.hInstance);
}