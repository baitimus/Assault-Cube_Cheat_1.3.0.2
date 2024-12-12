#pragma once

// External dependencies
#include "../../imgui/imgui.h"
#include "../../imgui/imgui_impl_dx11.h"
#include "../../imgui/imgui_impl_win32.h"
#include "../../cheat/pch.h"
#include "../../cheat/winapi.h"
#include "../../cheat/esp/esp.h"
#include "../config.h"


class Overlay {
public:
    // Singleton interface
    static Overlay& Instance();

    // Core functionality
    bool Initialize(HINSTANCE instance);
    void Run(runTimeInfo::pInfo& pInfo);
    void Shutdown();

    // Debug interface
    void AddDebugMessage(const std::string& message);
    template<typename T>
    void AddDebugMessage(const T& value) {
        std::ostringstream oss;
        oss << value;
        AddDebugMessage(oss.str());
    }

    // State management
    bool drawMenu = false;
    void ToggleInput();
    bool IsInputEnabled() const;

private:
    // Singleton implementation
    Overlay();
    ~Overlay();
    Overlay(const Overlay&) = delete;
    Overlay& operator=(const Overlay&) = delete;

    // State variables
    bool m_InputEnabled = true;
    bool m_Running;

    // Window handling
    HWND m_Overlay;
    WNDCLASSEXW m_WindowClass;
    int m_DisplayWidth;
    int m_DisplayHeight;

    // DirectX resources
    ID3D11Device* m_Device;
    ID3D11DeviceContext* m_DeviceContext;
    IDXGISwapChain* m_SwapChain;
    ID3D11RenderTargetView* m_RenderTargetView;

    // Debug logging
    std::vector<std::string> m_DebugLog;
    std::mutex m_DebugMutex;

    // Internal helpers
    bool InitializeWindow(HINSTANCE instance);
    bool InitializeDirectX();
    bool CreateRenderTarget();
    void CleanupRenderTarget();
    void Render(runTimeInfo::pInfo& pInfo);
};


