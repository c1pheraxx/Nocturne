#pragma once
#include <windows.h>
#include <dwmapi.h>
#include <d3d11.h>
#include <string>

namespace nocturne::overlay {

class Overlay {
public:
    bool create(const std::wstring& targetWindow);
    void destroy();
    bool render_frame();
    bool is_running() const { return running_; }

    int width() const { return width_; }
    int height() const { return height_; }

    // Drawing primitives (simplified; integrate ImGui for real UI)
    void draw_text(int x, int y, const std::string& text, uint32_t color);
    void draw_line(int x1, int y1, int x2, int y2, uint32_t color);
    void draw_rect(int x, int y, int w, int h, uint32_t color);
    void draw_circle(int x, int y, int radius, uint32_t color);

private:
    HWND overlay_hwnd_ = nullptr;
    HWND target_hwnd_ = nullptr;
    ID3D11Device* device_ = nullptr;
    ID3D11DeviceContext* context_ = nullptr;
    IDXGISwapChain* swapchain_ = nullptr;
    ID3D11RenderTargetView* rtv_ = nullptr;
    bool running_ = false;
    int width_ = 1920;
    int height_ = 1080;

    bool init_d3d();
    void cleanup_d3d();
    static LRESULT CALLBACK wnd_proc(HWND hwnd, UINT msg, WPARAM w, LPARAM l);
};

} // namespace nocturne::overlay
