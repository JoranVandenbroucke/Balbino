#include "Application.h"

#include <SDL_vulkan.h>

#include "Managers/Input/InputHandler.h"
#include "Renderer/Renderer.h"
#include "Scene/Scene.h"

//#include <chrono>
//#include <thread>
#include <ini.h>

Balbino::Application::Application()
        : m_w{ 0 }
          , m_h{ 0 }
          , m_windowFlags{ 0 }
          , m_pWindow{ nullptr }
          , m_system{ 1920, 1080 }
          , m_pRenderer{ nullptr }
        #ifdef BALBINO_EDITOR
          , m_pInterface{ nullptr }
        #endif // BALBINO_EDITOR
          , m_pScene{ nullptr }
{
}

void Balbino::Application::Initialize()
{
    if ( SDL_Init(SDL_INIT_EVERYTHING) != 0 )
    {
        throw std::runtime_error( std::string( "SDL_Init Error: " ) + SDL_GetError());
    }
    SDL_DisplayMode current;
    if ( SDL_GetDesktopDisplayMode( 0, &current ) != 0 )
    {
        throw std::runtime_error( std::string( "Could not get display mode for video display 0: " ) + SDL_GetError());
    }
    
    int audioFlags = MIX_INIT_FLAC | /*MIX_INIT_MOD |*/ MIX_INIT_MP3 | MIX_INIT_OGG | MIX_INIT_MID /*| MIX_INIT_OPUS*/;
    int initOut    = Mix_Init( audioFlags );
    if (( initOut & audioFlags ) != audioFlags )
    {
        throw std::runtime_error( std::string( "Mix_Init: Failed to init all!\nMix_Init: " ) + Mix_GetError());
    }
    
    m_w           = current.w / 2;
    m_h           = current.h / 2;
    m_windowFlags = SDL_WINDOW_VULKAN;
    const mINI::INIFile file( "DisplaySettings.ini" );
    mINI::INIStructure  ini;
    if ( file.read( ini ))
    {
        m_w           = std::stoi( ini["WindowsClient"]["WindowedViewportWidth"] );
        m_h           = std::stoi( ini["WindowsClient"]["WindowedViewportHeight"] );
        m_windowFlags = static_cast<uint32_t>( std::stoi( ini["WindowsClient"]["WindowedFlags"] ));
    }
    
    m_pWindow = SDL_CreateWindow(
            "Balbino Engine",
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            360,
            640,
            SDL_WINDOW_VULKAN | SDL_WINDOW_BORDERLESS
    );
    if ( m_pWindow == nullptr )
    {
        throw std::runtime_error( std::string( "SDL_CreateWindow Error: " ) + SDL_GetError());
    }
    m_system.Initialize();
    m_system.SetInputHandler( new CInputHandler{} );
    m_system.GetInputHandler()->Initialize();
    m_pRenderer = new CRenderer{};
    m_pScene    = new CScene{};
}

void Balbino::Application::LoadGame()
{
    unsigned int extensionCount;
    if ( !SDL_Vulkan_GetInstanceExtensions( m_pWindow, &extensionCount, nullptr ))
    {
        throw std::runtime_error(
                std::string( "Could not get the number of required m_Instance extensions from SDL." ));
    }
    const auto extensions{ new const char* [extensionCount] };
    if ( !SDL_Vulkan_GetInstanceExtensions( m_pWindow, &extensionCount, extensions ))
    {
        throw std::runtime_error( std::string( "Could not get the names of required m_Instance extensions from SDL." ));
    }
    #ifdef BALBINO_EDITOR
    m_pInterface = new BalEditor::CInterface{};
    m_pRenderer->Setup( m_pWindow, extensions, extensionCount, m_pInterface, &m_system );
    m_pRenderer->GiveSceneRenderData( m_pScene );
    m_pScene->Initialize( &m_system );
    m_pInterface->SetContext( m_pScene, &m_system );
    delete[] extensions;
    #else
    m_pRenderer->Setup( m_pWindow, extensions, extensionCount );
    m_pRenderer->GiveSceneRenderData( m_pScene );
    m_pScene->Initialize( &m_system );
    delete[] extensions;
    #endif
    m_system.SetCurrentScene( m_pScene );
    
    SDL_SetWindowBordered( m_pWindow, SDL_TRUE );
    SDL_SetWindowResizable( m_pWindow, SDL_TRUE );
    if ( m_windowFlags & SDL_WINDOW_MAXIMIZED )
    {
        SDL_MaximizeWindow( m_pWindow );
        SDL_SetWindowSize( m_pWindow, m_w, m_h );
    }
    else
    {
        SDL_SetWindowSize( m_pWindow, m_w, m_h );
        SDL_SetWindowPosition( m_pWindow, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    }
}

void Balbino::Application::Cleanup()
{
    #ifdef BALBINO_EDITOR
    m_pInterface->Cleanup();
    delete m_pInterface;
    m_pInterface = nullptr;
    #endif
    m_system.Cleanup();
    
    if ( m_pScene )
    {
        m_pScene->Cleanup();
        delete m_pScene;
        m_pScene = nullptr;
    }
    if ( m_pRenderer )
    {
        m_pRenderer->Cleanup();
        delete m_pRenderer;
        m_pRenderer = nullptr;
    }
    
    SDL_DestroyWindow( m_pWindow );
    SDL_Quit();
    Mix_Quit();
    
    m_pWindow = nullptr;
}

void Balbino::Application::Run()
{
    LoadGame();
    bool isRunning{ true };
    auto start{ std::chrono::system_clock::now() };
    while ( isRunning )
    {
        auto        end{ std::chrono::system_clock::now() };
        const float deltaTime{ std::chrono::duration<float>( end - start ).count() };
        start = end;
        
        SDL_Event e;
        while ( SDL_PollEvent( &e ))
        {
            #ifdef BALBINO_EDITOR
            m_pInterface->ProcessEvent( e );
            #endif
            switch ( e.type )
            {
                case SDL_QUIT:
                {
                    isRunning = false;
                    break;
                }
                case SDL_WINDOWEVENT:
                {
                    switch ( e.window.event )
                    {
                        case SDL_WINDOWEVENT_MINIMIZED:
                        {
                            SDL_Event minEvent;
                            bool      isMin{ true };
                            while ( isMin && isRunning && SDL_WaitEvent( &minEvent ))
                            {
                                switch ( minEvent.window.event )
                                {
                                    case SDL_WINDOWEVENT_SIZE_CHANGED:
                                    case SDL_WINDOWEVENT_MAXIMIZED:
                                    case SDL_WINDOWEVENT_MINIMIZED:
                                    case SDL_WINDOWEVENT_RESIZED:
                                    {
                                        isMin = false;
                                        break;
                                    }
                                    case SDL_WINDOWEVENT_CLOSE:
                                    {
                                        isRunning = false;
                                        break;
                                    }
                                    default:
                                        break;
                                }
                            }
                            break;
                        }
                        case SDL_WINDOWEVENT_SIZE_CHANGED:
                        case SDL_WINDOWEVENT_MAXIMIZED:
                        case SDL_WINDOWEVENT_RESIZED:
                        {
                            int w, h;
                            SDL_GetWindowSize( m_pWindow, &w, &h );
                            m_windowFlags = SDL_GetWindowFlags( m_pWindow );
                            const mINI::INIFile file( "DisplaySettings.ini" );
                            mINI::INIStructure  ini;
                            ini["WindowsClient"]["WindowedViewportWidth"]  = std::to_string( w );
                            ini["WindowsClient"]["WindowedViewportHeight"] = std::to_string( h );
                            ini["WindowsClient"]["WindowedFlags"]          = std::to_string( m_windowFlags );
                            (void) file.generate( ini, true );
                            m_system.SetWidth( w );
                            m_system.SetHeight( h );
                            break;
                        }
                        default:
                            break;
                    }
                    break;
                }
                default:
                    break;
            }
            m_system.GetInputHandler()->ProcessEvents( e );
        }
        m_system.GetInputHandler()->Update();
        
        m_pScene->Update( deltaTime );
        if ( m_pRenderer->StartDraw())
        {
            continue;
        }
        m_pScene->PrepareDraw();
//        m_pScene->DepthPass();
//        m_pRenderer->NextSubPass();
        m_pScene->Draw();
        if ( m_pRenderer->EndDraw())
        {
            continue;
        }
    }
}
