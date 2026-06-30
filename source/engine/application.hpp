#pragma once

import FawnVision;
import DeerUI;

namespace BalbinoApp
{
class Application
{
  public:
    Application() noexcept                     = default;
    virtual ~Application()                     = default;
    Application(const Application&)            = delete;
    Application(Application&&)                 = delete;
    Application& operator=(const Application&) = delete;
    Application& operator=(Application&&)      = delete;

    void Initialize();
    virtual void LoadGame();
    void Cleanup();
    void Run();

  private:
    fawn_vision::Window m_window{};
    fawn_vision::Renderer m_renderer{};
    fawn_vision::RenderGraph m_renderGraph{};
    deer_ui::UIRenderer m_uiRenderer{};
    bool m_isRunning{true};

    void Draw();
    void WindowResize();
};

// to be defined in a client
Application* CreateApplication();
void DestroyApplication(const Application* pApplication);
} // namespace BalbinoApp
