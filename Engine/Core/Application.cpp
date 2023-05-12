#include "Application.h"

#include <SDL3/SDL_vulkan.h>

#include "Managers/Input/InputHandler.h"
#include "Scene/Scene.h"
#include <Renderer.h>

#include <mini/ini.h>
#include <SDL_mixer.h>

#ifdef BALBINO_EDITOR
#include <src/Interface.h>

#endif
void Balbino::Application::Initialize()
{
    if ( SDL_Init(SDL_INIT_EVERYTHING) != 0 )
    {
        throw std::invalid_argument( std::string( "SDL_Init Error: " ) + SDL_GetError());
    }
    int screenCount{};
    SDL_GetDisplays( &screenCount );
    const SDL_DisplayMode* current = SDL_GetDesktopDisplayMode( 1 );
    if ( !current )
    {
        std::cerr << "Could not get display mode for video display 0: " << SDL_GetError() << std::endl;
        current = SDL_GetCurrentDisplayMode( 1 );
        if ( !current )
        {
            throw std::invalid_argument( std::string( "Could not get display mode for video display 0: " ) + SDL_GetError());
        }
    }
    int audioFlags = MIX_INIT_FLAC /*| MIX_INIT_MOD */| MIX_INIT_MP3 | MIX_INIT_OGG | MIX_INIT_MID /*| MIX_INIT_OPUS*/;
    int initOut    = Mix_Init( audioFlags );
    if (( initOut & audioFlags ) != audioFlags )
    {
        throw std::invalid_argument( std::string( "Mix_Init: Failed to init all!\nMix_Init: " ) + Mix_GetError());
    }
    
    m_w           = current->screen_w / 2;
    m_h           = current->screen_h / 2;
    m_windowFlags = SDL_WINDOW_VULKAN;
    const mINI::INIFile     file( "DisplaySettings.ini" );
    if ( mINI::INIStructure ini; file.read( ini ))
    {
        m_w           = std::stoi( ini["WindowsClient"]["WindowedViewportWidth"] );
        m_h           = std::stoi( ini["WindowsClient"]["WindowedViewportHeight"] );
        m_windowFlags = static_cast<uint32_t>( std::stoi( ini["WindowsClient"]["WindowedFlags"] ));
    }
    
    m_pWindow = SDL_CreateWindow(
            "Balbino Engine", 360, 640, SDL_WINDOW_VULKAN | SDL_WINDOW_BORDERLESS
    );
    if ( m_pWindow == nullptr )
    {
        throw std::invalid_argument( std::string( "SDL_CreateWindow Error: " ) + SDL_GetError());
    }
    m_pRenderer = new FawnVision::CRenderer{};
    
    m_system.Initialize(m_pRenderer);
    m_system.SetInputHandler( new CInputHandler{} );
    m_system.GetInputHandler()->Initialize();
    
    m_pScene = new CScene{};
}

void Balbino::Application::LoadGame()
{
    unsigned int extensionCount;
    if ( !SDL_Vulkan_GetInstanceExtensions( &extensionCount, nullptr ))
    {
        throw std::invalid_argument(
                std::string( "Could not get the number of required m_Instance extensions from SDL." ));
    }
    const auto extensions{ new const char* [extensionCount] };
    if ( !SDL_Vulkan_GetInstanceExtensions( &extensionCount, extensions ))
    {
        throw std::invalid_argument( std::string( "Could not get the names of required m_Instance extensions from SDL." ));
    }
    #ifdef BALBINO_EDITOR
    m_pInterface = new FawnForge::CInterface{};
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
    if ( m_pInterface )
    {
        m_pInterface->Cleanup();
    }
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
        HandleEvents( isRunning );
        m_system.GetInputHandler()->Update();
        
        m_pScene->Update( deltaTime );
        if ( m_pRenderer->StartDraw())
        {
            continue;
        }
        m_pScene->PrepareDraw();
        m_pScene->Draw();
        if ( m_pRenderer->EndDraw())
        {
            continue;
        }
    }
}

void Balbino::Application::HandleEvents( bool& isRunning )
{
    SDL_Event e;
    while ( SDL_PollEvent( &e ))
    {
        #ifdef BALBINO_EDITOR
        m_pInterface->ProcessEvent( e );
        #endif
        switch ( e.type )
        {
            case SDL_EVENT_QUIT:
            {
                isRunning = false;
                break;
            }
            case SDL_EVENT_WINDOW_SHOWN:
            case SDL_EVENT_WINDOW_HIDDEN:
            case SDL_EVENT_WINDOW_EXPOSED:
            case SDL_EVENT_WINDOW_MOVED:
            case SDL_EVENT_WINDOW_RESIZED:
            case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED:
            case SDL_EVENT_WINDOW_MINIMIZED:
            case SDL_EVENT_WINDOW_MAXIMIZED:
            case SDL_EVENT_WINDOW_RESTORED:
            case SDL_EVENT_WINDOW_MOUSE_ENTER:
            case SDL_EVENT_WINDOW_MOUSE_LEAVE:
            case SDL_EVENT_WINDOW_FOCUS_GAINED:
            case SDL_EVENT_WINDOW_FOCUS_LOST:
            case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
            case SDL_EVENT_WINDOW_TAKE_FOCUS:
            case SDL_EVENT_WINDOW_HIT_TEST:
            case SDL_EVENT_WINDOW_ICCPROF_CHANGED:
            case SDL_EVENT_WINDOW_DISPLAY_CHANGED:
            {
                HandleWindowEvents( isRunning, e.type );
                break;
            }
            default:
                break;
        }
        m_system.GetInputHandler()->ProcessEvents( e );
    }
}
void Balbino::Application::HandleWindowEvents( bool& isRunning, uint32_t type )
{
    switch ( type )
    {
        case SDL_EVENT_WINDOW_MINIMIZED:
        {
            SDL_Event minEvent;
            bool      isMin{ true };
            while ( isMin && isRunning && SDL_WaitEvent( &minEvent ))
            {
                switch ( minEvent.type )
                {
                    case SDL_EVENT_WINDOW_RESTORED:
                    case SDL_EVENT_WINDOW_MAXIMIZED:
                    case SDL_EVENT_WINDOW_MINIMIZED:
                    case SDL_EVENT_WINDOW_RESIZED:
                    {
                        isMin = false;
                        break;
                    }
                    case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
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
        case SDL_EVENT_WINDOW_RESTORED:
        case SDL_EVENT_WINDOW_MAXIMIZED:
        case SDL_EVENT_WINDOW_RESIZED:
        {
            int w;
            int h;
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
}
