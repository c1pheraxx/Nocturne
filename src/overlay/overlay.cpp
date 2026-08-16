#include "overlay.h"
#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dwmapi.lib")

namespace nocturne::overlay {

bool Overlay::create(const std::wstring& targetWindow) {
    target_hwnd_ = FindWindowW(nullptr, targetWindow.c_str());
    if (!target_hwnd_) return false;

    RECT rc;
    GetClientRect(target_hwnd_, &rc);
    width_ = rc.right - rc.left;
    height_ = rc.bottom - rc.top;

    WNDCLASSEXW wc = {};
    wc.cbSize = sizeof(wc);
    wc.lpfnWndProc = wnd_proc;
    wc.hInstance = GetModuleHandleW(nullptr);
    wc.lpszClassName = L"NocturneOverlay";
    RegisterClassExW(&wc);

    overlay_hwnd_ = CreateWindowExW(
        WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_LAYERED | WS_EX_TOOLWINDOW,
        L"NocturneOverlay", L"Nocturne",
        WS_POPUP,
        0, 0, width_, height_,
        nullptr, nullptr, wc.hInstance, nullptr
    );

    if (!overlay_hwnd_) return false;

    SetLayeredWindowAttributes(overlay_hwnd_, RGB(0, 0, 0), 0, LWA_COLORKEY);
    MARGINS margins = { -1 };
    DwmExtendFrameIntoClientArea(overlay_hwnd_, &margins);
    ShowWindow(overlay_hwnd_, SW_SHOW);

    if (!init_d3d()) return false;
    running_ = true;
    return true;
}

void Overlay::destroy() {
    running_ = false;
    cleanup_d3d();
    if (overlay_hwnd_) DestroyWindow(overlay_hwnd_);
    UnregisterClassW(L"NocturneOverlay", GetModuleHandleW(nullptr));
}

bool Overlay::render_frame() {
    if (!running_ || !swapchain_) return false;

    // Update position to match target window
    RECT rc;
    GetWindowRect(target_hwnd_, &rc);
    SetWindowPos(overlay_hwnd_, HWND_TOPMOST, rc.left, rc.top, width_, height_, SWP_SHOWWINDOW);

    // Clear
    float clearColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    context_->ClearRenderTargetView(rtv_, clearColor);

    // Present
    swapchain_->Present(1, 0);
    return true;
}

void Overlay::draw_text(int x, int y, const std::string& text, uint32_t color) {
    // Placeholder: integrate ImGui or DirectWrite for real text rendering
    (void)x; (void)y; (void)text; (void)color;
}

void Overlay::draw_line(int x1, int y1, int x2, int y2, uint32_t color) {
    (void)x1; (void)y1; (void)x2; (void)y2; (void)color;
}

void Overlay::draw_rect(int x, int y, int w, int h, uint32_t color) {
    (void)x; (void)y; (void)w; (void)h; (void)color;
}

void Overlay::draw_circle(int x, int y, int radius, uint32_t color) {
    (void)x; (void)y; (void)radius; (void)color;
}

bool Overlay::init_d3d() {
    DXGI_SWAP_CHAIN_DESC sd = {};
    sd.BufferCount = 2;
    sd.BufferDesc.Width = width_;
    sd.BufferDesc.Height = height_;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = overlay_hwnd_;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    D3D_FEATURE_LEVEL level;
    HRESULT hr = D3D11CreateDeviceAndSwapChain(
        nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr,
        D3D11_CREATE_DEVICE_BGRA_SUPPORT,
        nullptr, 0, D3D11_SDK_VERSION,
        &sd, &swapchain_, &device_, &level, &context_
    );

    if (FAILED(hr)) return false;

    ID3D11Texture2D* backBuffer = nullptr;
    swapchain_->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
    if (backBuffer) {
        device_->CreateRenderTargetView(backBuffer, nullptr, &rtv_);
        backBuffer->Release();
    }

    context_->OMSetRenderTargets(1, &rtv_, nullptr);
    return true;
}

void Overlay::cleanup_d3d() {
    if (rtv_) { rtv_->Release(); rtv_ = nullptr; }
    if (swapchain_) { swapchain_->Release(); swapchain_ = nullptr; }
    if (context_) { context_->Release(); context_ = nullptr; }
    if (device_) { device_->Release(); device_ = nullptr; }
}

LRESULT CALLBACK Overlay::wnd_proc(HWND hwnd, UINT msg, WPARAM w, LPARAM l) {
    if (msg == WM_DESTROY) PostQuitMessage(0);
    return DefWindowProcW(hwnd, msg, w, l);
}

} // namespace nocturne::overlay
