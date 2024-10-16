#pragma once

import FawnVision;
import DeerUI;

namespace BalbinoApp
{
    class Application
    {
      public:
        Application() noexcept                       = default;
        virtual ~Application()                       = default;
        Application( const Application& )            = delete;
        Application( Application&& )                 = delete;
        Application& operator=( const Application& ) = delete;
        Application& operator=( Application&& )      = delete;

        void Initialize();
        virtual void LoadGame();
        void Cleanup();
        void Run();

      private:
        FawnVision::SWindow m_window {};
        FawnVision::SRenderer m_renderer{};
        FawnVision::SRenderGraph m_renderGraph{};
        DeerUI::SUIRenderer m_uiRenderer;
        bool m_isRunning{true};

        void Draw();
        void WindowResize();
    };

    //to be defined in a client
    Application* CreateApplication();
    void DestroyApplication( Application* pApplication );
}// namespace BalbinoApp
