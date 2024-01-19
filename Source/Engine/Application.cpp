#include "Application.h"

#include <chrono>

namespace BalbinoApp
{
    void Application::Initialize()
    {
        // todo : initialize memory manager (ensure memory is managed before other components)
        // todo : initialize logger/assert (set up logging and assertions early for debugging)

        FawnVision::error_code errorCode { FawnVision::Initialize() };
        if ( errorCode != FawnVision::error_code::oke )
        {
            throw std::invalid_argument( FawnVision::GetWindowError() );
        }

        // todo : name by "game_name", if in editor update tytle to "balbino engine: game_name - open scene"
        constexpr FawnVision::WindowCreateInfo window_create_info { "Balbino Engine", 650, 1000, FawnVision::window_flags::borderless | FawnVision::window_flags::vulkan };
        errorCode = CreateWindow( window_create_info, m_window );
        if ( errorCode != FawnVision::error_code::oke )
        {
            throw std::invalid_argument( FawnVision::GetWindowError() );
        }
        errorCode = CreateRenderer( m_window, m_renderer );
        if ( errorCode != FawnVision::error_code::oke )
        {
            throw std::invalid_argument( FawnVision::GetRenderError() );
        }
        // todo : initialize renderer (initialize graphics rendering)
        // todo : initialize font engine (set up font rendering)
        // todo : initialize user interface (UI) components (if your application has a GUI)
        // todo : initialize randomization (if needed for procedural generation)
        // todo : initialize file manager (prepare for file I/O operations)
        // todo : initialize asset manager (manage game assets like textures, models, etc.)
        // todo : initialize input manager (handle user input)
        // todo : initialize audio engine (set up audio processing)
        // todo : initialize physics engine (if applicable, for handling physics simulations)
        // todo : initialize AI module (initialize artificial intelligence systems)
        // todo : initialize network manager (if applicable, for networking functionalities)
        // todo : initialize scene manager (manage different scenes in the application)
        // todo : initialize scripting language (if applicable, set up scripting support)
    }

    void Application::LoadGame()
    {
        // todo : initialize editor
        // todo : initialize first scene(s)

        // todo : read out desired flags;
        // todo : read out desired position;
        // todo : read out desired size;
        SetWindowFlags( m_window, FawnVision::window_flags::maximized | FawnVision::window_flags::resizable );
        SetWindowPosition( m_window );
        SetWindowSize( m_window, 860, 640 );
        ResizeRender( m_window, m_renderer );
    }

    void Application::Cleanup()
    {
        if ( ReleaseRenderer( m_renderer ) != FawnVision::error_code::oke )
        {
            throw std::invalid_argument( FawnVision::GetRenderError() );
        }
        if ( ReleaseWindow( m_window ) != FawnVision::error_code::oke )
        {
            throw std::invalid_argument( FawnVision::GetWindowError() );
        }
    }

    void Application::Run()
    {
        LoadGame();

        bool loop { true };
        while ( loop )
        {
            ProcessEvents( m_window, loop );

            // todo : update physics
            // todo : update scene(s)
            if ( StartRender( m_renderer ) != FawnVision::error_code::oke )
            {
                std::cout << "Something went wrong with starting the render call!" << std::endl;
            }
            // todo : render
            if( StopRender(m_renderer) != FawnVision::error_code::oke)
            {
                std::cout << "Something went wrong with ending the render call!" << std::endl;
            }
        }
    }
}// namespace BalbinoApp
