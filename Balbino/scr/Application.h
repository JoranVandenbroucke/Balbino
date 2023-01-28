#pragma once

#include <cstdint>
#include "Core.h"
#include "Managers/System.h"

#ifdef BALBINO_EDITOR
namespace BalEditor
{
    class CInterface;
}
#endif // BALBINO_EDITOR


struct SDL_Window;

namespace Balbino
{
    class CScene;
    
    class CRenderer;
    
    #ifdef BALBINO_EDITOR
    
    class CInterface;
    
    #endif
    
    class Application
    {
    public:
        BALBINO_API Application();
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
        int32_t  m_w;
        int32_t  m_h;
        uint32_t m_windowFlags;
        SDL_Window* m_pWindow;
        
        CSystem m_system;
        CRenderer* m_pRenderer;
        #ifdef BALBINO_EDITOR
        BalEditor::CInterface* m_pInterface;
        #endif
        CScene* m_pScene;
    };
    
    //to be defined in client
    Application* CreateApplication();
    void DestroyApplication( Balbino::Application* pApplication );
}
