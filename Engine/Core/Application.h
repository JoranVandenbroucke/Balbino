#pragma once

#include <cstdint>
#include <Core.h>
#include "Managers/System.h"

#ifdef BALBINO_EDITOR
namespace FawnForge
{
    class CInterface;
}
#endif // BALBINO_EDITOR


struct SDL_Window;

namespace FawnVision
{
    class CRenderer;
}
namespace Balbino
{
    class CScene;
    
    class Application
    {
    public:
        BALBINO_API Application()=default;
        BALBINO_API virtual ~Application() = default;
        Application( const Application& ) = delete;
        Application( Application&& ) = delete;
        Application& operator=( const Application& ) = delete;
        Application& operator=( Application&& ) = delete;
        
        void Initialize();
        BALBINO_API virtual void LoadGame();
        void Cleanup();
        void Run();
    
    private:
        int32_t  m_w{};
        int32_t  m_h{};
        uint32_t m_windowFlags{};
        SDL_Window* m_pWindow{ nullptr };
        
        CSystem m_system{1920,1080};
        FawnVision::CRenderer* m_pRenderer{ nullptr };
        #ifdef BALBINO_EDITOR
        FawnForge::CInterface* m_pInterface{ nullptr };
        #endif
        CScene* m_pScene{ nullptr };
        void HandleEvents( bool& isRunning);
        void HandleWindowEvents( bool& isRunning, uint32_t type );
    };
    
    //to be defined in client
    Application* CreateApplication();
    void DestroyApplication( Balbino::Application* pApplication );
}
