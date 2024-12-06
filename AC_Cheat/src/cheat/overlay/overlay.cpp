#include "overlay.h"


extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK WindowProc(HWND window, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(window, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU)
            return 0;
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    case WM_CLOSE:
        return 0;
    }

    return DefWindowProc(window, msg, wParam, lParam);
}

Overlay::Overlay()
    : m_Overlay(nullptr)
    , m_Device(nullptr)
    , m_DeviceContext(nullptr)
    , m_SwapChain(nullptr)
    , m_RenderTargetView(nullptr)
    , m_Running(false)
{
    m_DisplayWidth = GetSystemMetrics(SM_CXSCREEN);
    m_DisplayHeight = GetSystemMetrics(SM_CYSCREEN);
}

Overlay::~Overlay()
{
    Shutdown();
}

bool Overlay::Initialize(HINSTANCE instance)
{
    if (!InitializeWindow(instance))
        return false;

    if (!InitializeDirectX())
        return false;

    // Initialize ImGui
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplWin32_Init(m_Overlay);
    ImGui_ImplDX11_Init(m_Device, m_DeviceContext);

    return true;
}
bool Overlay::InitializeWindow(HINSTANCE instance)
{
    m_WindowClass = {};
    m_WindowClass.cbSize = sizeof(WNDCLASSEXW);
    m_WindowClass.style = CS_HREDRAW | CS_VREDRAW;
    m_WindowClass.lpfnWndProc = WindowProc;
    m_WindowClass.hInstance = instance;
    m_WindowClass.lpszClassName = L"AC_Cheat";

    RegisterClassExW(&m_WindowClass);

    m_Overlay = CreateWindowExW(
        WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_LAYERED,
        m_WindowClass.lpszClassName,
        L"AC_Cheat",
        WS_POPUP,
        0, 0,
        m_DisplayWidth, m_DisplayHeight,
        nullptr,
        nullptr,
        m_WindowClass.hInstance,
        nullptr
    );

    if (!m_Overlay)
        return false;

    SetLayeredWindowAttributes(m_Overlay, RGB(0, 0, 0), BYTE(255), LWA_ALPHA);

    // Set up margins
    RECT clientArea;
    GetClientRect(m_Overlay, &clientArea);
    RECT windowArea;
    GetWindowRect(m_Overlay, &windowArea);
    POINT diff{};
    ClientToScreen(m_Overlay, &diff);

    MARGINS margins = {
        windowArea.left + (diff.x - windowArea.left),
        windowArea.top + (diff.y - windowArea.top),
        clientArea.right,
        clientArea.bottom
    };

    DwmExtendFrameIntoClientArea(m_Overlay, &margins);

    return true;
}
bool Overlay::InitializeDirectX()
{
    DXGI_SWAP_CHAIN_DESC scd = {};
    scd.BufferDesc.RefreshRate.Numerator = 60;
    scd.BufferDesc.RefreshRate.Denominator = 1;
    scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    scd.SampleDesc.Count = 1;
    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    scd.BufferCount = 2;
    scd.OutputWindow = m_Overlay;
    scd.Windowed = TRUE;
    scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

    constexpr D3D_FEATURE_LEVEL levels[2] = {
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_0
    };

    D3D_FEATURE_LEVEL level;

    HRESULT hr = D3D11CreateDeviceAndSwapChain(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        0,
        levels,
        2,
        D3D11_SDK_VERSION,
        &scd,
        &m_SwapChain,
        &m_Device,
        &level,
        &m_DeviceContext
    );

    if (FAILED(hr))
        return false;

    return CreateRenderTarget();
}

bool Overlay::CreateRenderTarget()
{
    ID3D11Texture2D* backBuffer = nullptr;
    if (FAILED(m_SwapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer))))
        return false;

    HRESULT hr = m_Device->CreateRenderTargetView(backBuffer, nullptr, &m_RenderTargetView);
    backBuffer->Release();

    return SUCCEEDED(hr);
}

void Overlay::Run(runTimeInfo::pInfo& pInfo)
{
    ShowWindow(m_Overlay, SW_SHOW);
    UpdateWindow(m_Overlay);

    m_Running = true;

    while (m_Running)
    {

        MSG msg;
        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);

            if (msg.message == WM_QUIT)
            {
                m_Running = false;
                break;
            }
        }

        if (!m_Running)
            break;
		
		


        Render(pInfo);
    }
}
void Overlay::Render(runTimeInfo::pInfo& pInfo)
{
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();


	//aim::aimbot(pInfo);




    ImGui::Render();

    const float clear_color[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    m_DeviceContext->OMSetRenderTargets(1, &m_RenderTargetView, nullptr);
    m_DeviceContext->ClearRenderTargetView(m_RenderTargetView, clear_color);
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    m_SwapChain->Present(0, 0);
}
void Overlay::CleanupRenderTarget()
{
    if (m_RenderTargetView)
    {
        m_RenderTargetView->Release();
        m_RenderTargetView = nullptr;
    }
}

void Overlay::Shutdown()
{
    CleanupRenderTarget();

    if (m_SwapChain)
    {
        m_SwapChain->Release();
        m_SwapChain = nullptr;
    }
    if (m_DeviceContext)
    {
        m_DeviceContext->Release();
        m_DeviceContext = nullptr;
    }
    if (m_Device)
    {
        m_Device->Release();
        m_Device = nullptr;
    }

    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    if (m_Overlay)
    {
        DestroyWindow(m_Overlay);
        m_Overlay = nullptr;
    }

    UnregisterClassW(m_WindowClass.lpszClassName, m_WindowClass.hInstance);
}