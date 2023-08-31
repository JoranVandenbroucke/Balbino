#pragma once

#include <Core.h>
#include <System/System.h>
#include <cstdint>

#ifdef BALBINO_EDITOR
namespace FawnForge
{
    class CInterface;
}
#endif// BALBINO_EDITOR

struct SDL_Window;

namespace FawnVision
{
    class CRenderer;
}
namespace BalbinoScene
{
    class CScene;
}
namespace BalbinoApp
{
    class Application
    {
    public:
        BALBINO_API Application() noexcept           = default;
        BALBINO_API virtual ~Application()           = default;
        Application( const Application& )            = delete;
        Application( Application&& )                 = delete;
        Application& operator=( const Application& ) = delete;
        Application& operator=( Application&& )      = delete;

        void Initialize();
        BALBINO_API virtual void LoadGame();
        void Cleanup();
        void Run();

    private:
        int32_t m_w {};
        int32_t m_h {};
        uint32_t m_windowFlags {};
        SDL_Window* m_pWindow { nullptr };

        BalbinoControl::CSystem m_system { 1920, 1080 };
        FawnVision::CRenderer* m_pRenderer { nullptr };
#ifdef BALBINO_EDITOR
        FawnForge::CInterface* m_pInterface { nullptr };
#endif
        BalbinoScene::CScene* m_pScene { nullptr };
        void HandleEvents( bool& isRunning ) noexcept;
        void HandleWindowEvents( bool& isRunning, uint32_t type ) noexcept;
    };

    //to be defined in client
    Application* CreateApplication();
    void DestroyApplication( BalbinoApp::Application* pApplication );
}// namespace Balbino
