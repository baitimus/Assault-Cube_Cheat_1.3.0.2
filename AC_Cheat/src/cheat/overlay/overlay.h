#pragma once

#include "../../imgui/imgui.h"
#include "../../imgui/imgui_impl_dx11.h"
#include "../../imgui/imgui_impl_win32.h"
#include "../../cheat/pch.h"
#include "../../cheat/winapi.h"
#include "../../cheat/esp/esp.h"
#include <mutex>
#include <string>
#include <vector>
#include <sstream>

class Overlay {
public:
    // Singleton access
    static Overlay& Instance();

    // Public methods
    bool Initialize(HINSTANCE instance);
    void Run(runTimeInfo::pInfo& pInfo);
    void Shutdown();

    // Debug logging
    void AddDebugMessage(const std::string& message);

    template<typename T>
    void AddDebugMessage(const T& value) {
        std::ostringstream oss;
        oss << value;
        AddDebugMessage(oss.str());
    }
    bool drawMenu = false;

    void ToggleInput(); // Add this new method
    bool IsInputEnabled() const; // Add getter for input state

private:
    //  constructor and destructor
    Overlay();
    ~Overlay();
    bool m_InputEnabled = true;
    // Disable copy and assignment
    Overlay(const Overlay&) = delete;
    Overlay& operator=(const Overlay&) = delete;

    
    HWND m_Overlay;
    WNDCLASSEXW m_WindowClass;
    int m_DisplayWidth;
    int m_DisplayHeight;

    ID3D11Device* m_Device;
    ID3D11DeviceContext* m_DeviceContext;
    IDXGISwapChain* m_SwapChain;
    ID3D11RenderTargetView* m_RenderTargetView;

    bool m_Running;
    std::vector<std::string> m_DebugLog;
    std::mutex m_DebugMutex;
    

    // Helper methods
    bool InitializeWindow(HINSTANCE instance);
    bool InitializeDirectX();
    bool CreateRenderTarget();
    void CleanupRenderTarget();
    void Render(runTimeInfo::pInfo& pInfo);
};
