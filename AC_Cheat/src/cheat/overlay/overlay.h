#pragma once
#include "../../imgui/imgui.h"
#include "../../imgui/imgui_impl_dx11.h"
#include "../../imgui/imgui_impl_win32.h"
#include "../pch.h"
#include <d3d11.h>
#include "../aim/aimbot.h"
#include "../entity/entity.h"
#include "../winapi.h"

class Overlay {
public:
    Overlay();
    ~Overlay();

    bool Initialize(HINSTANCE instance);
    void Run(runTimeInfo::pInfo& pInfo);
    void Shutdown();

    // Getter for the window handle if needed
    HWND GetWindow() const { return m_Overlay; }

private:
    bool InitializeWindow(HINSTANCE instance);
    bool InitializeDirectX();
    bool CreateRenderTarget();
    void CleanupRenderTarget();
    void Render(runTimeInfo::pInfo& pInfo);

    // Window related
    HWND m_Overlay;
    WNDCLASSEXW m_WindowClass;
    int m_DisplayWidth;
    int m_DisplayHeight;

    // DirectX related
    ID3D11Device* m_Device;
    ID3D11DeviceContext* m_DeviceContext;
    IDXGISwapChain* m_SwapChain;
    ID3D11RenderTargetView* m_RenderTargetView;

    bool m_Running;
};


