#pragma once
#include "FawnVision.h"

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
        FawnVision::Window m_window {};
        FawnVision::Renderer m_renderer {};
    };

    //to be defined in client
    Application* CreateApplication();
    void DestroyApplication( Application* pApplication );
}// namespace BalbinoApp
