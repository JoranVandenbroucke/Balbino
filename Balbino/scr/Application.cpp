#include "pch.h"
#include "Application.h"

#include <SDL.h>
#include <SDL_vulkan.h>

#include "Input/InputHandler.h"
#include "Renderer/Renderer.h"
#include "Scene/Scene.h"

#ifdef BALBINO_EDITOR
#include <scr/Interface.h>
#endif

#include <ini.h>

Balbino::Application::Application()
	: m_pWindow{ nullptr }
	, m_manager{}
	, m_pRenderer{ DBG_NEW CRenderer{} }
	, m_pScene{ DBG_NEW CScene{} }
#ifdef BALBINO_EDITOR
	, m_pInterface{ nullptr }
#endif // BALBINO_EDITOR

{
}

Balbino::Application::~Application()
{
}

void Balbino::Application::Initialize()
{
	if ( SDL_Init( SDL_INIT_EVERYTHING ) != 0 )
	{
		throw std::runtime_error( std::string( "SDL_Init Error: " ) + SDL_GetError() );
	}
	SDL_DisplayMode current;
	if ( SDL_GetDesktopDisplayMode( 0, &current ) != 0 )
	{
		throw std::runtime_error( std::string( "Could not get display mode for video display 0: " ) + SDL_GetError() );
	}

	int w{ current.w / 2 }, h{ current.h / 2 };
	uint32_t flags{ SDL_WINDOW_RESIZABLE | SDL_WINDOW_VULKAN };
	const mINI::INIFile file( "DisplaySettings.ini" );
	mINI::INIStructure ini;
	if ( file.read( ini ) )
	{
		w = std::stoi( ini["WindowsClient"]["WindowedViewportWidth"] );
		h = std::stoi( ini["WindowsClient"]["WindowedViewportHeight"] );
		flags = ( uint32_t ) std::stoi( ini["WindowsClient"]["WindowedFlags"] );
	}

	m_pWindow = SDL_CreateWindow(
		"Balbino Engine",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		w,
		h,
		flags
	);
	if ( m_pWindow == nullptr )
	{
		throw std::runtime_error( std::string( "SDL_CreateWindow Error: " ) + SDL_GetError() );
	}

	m_manager.SetInputHandler( DBG_NEW CInputHandler{} );
	CManager::GetInputHandler()->Initialize();
}

void Balbino::Application::LoadGame()
{
	/*
	 * if you download Vulkan from https://vulkan.lunarg.com/home/welcome
	 * you get code for Open SDL
	 *
	 * this is code direct form it
	 */

	 // Get WSI extensions from SDL (we can add more if we like - we just can't remove these)
	unsigned int extenstionCount;
	if ( !SDL_Vulkan_GetInstanceExtensions( m_pWindow, &extenstionCount, nullptr ) )
	{
		throw std::runtime_error( std::string( "Could not get the number of required m_Instance extensions from SDL." ) );
	}
	const auto extensions{ DBG_NEW const char* [extenstionCount] };
	if ( !SDL_Vulkan_GetInstanceExtensions( m_pWindow, &extenstionCount, extensions ) )
	{
		throw std::runtime_error( std::string( "Could not get the names of required m_Instance extensions from SDL." ) );
	}
#ifdef BALBINO_EDITOR
	m_pInterface = new BalEditor::CInterface{};
	m_pRenderer->Setup( m_pWindow, extensions, extenstionCount, m_pInterface );
	m_pInterface->SetContext( m_pScene );
#else
	m_pRenderer->Setup( m_pWindow, extensions, extenstionCount );
#endif

	int32_t w, h;
	SDL_GetWindowSize( m_pWindow, &w, &h );
	m_pScene->Initialize( ( uint32_t ) w, ( uint32_t ) h );
	delete[] extensions;
}

void Balbino::Application::Cleanup()
{
#ifdef BALBINO_EDITOR
	m_pInterface->Cleanup();
	delete m_pInterface;
#endif
	//m_pRenderer->Cleanup();
	m_pScene->Cleanup();
	delete m_pScene;
	m_manager.Cleanup();
	m_pRenderer->Cleanup();
	//delete m_pRenderer;

	m_manager.SetInputHandler( nullptr );

	SDL_DestroyWindow( m_pWindow );
	SDL_Quit();

	m_pRenderer = nullptr;
	m_pWindow = nullptr;
}

void Balbino::Application::Run()
{
	LoadGame();
	bool isRunning{ true };
	auto start{ std::chrono::system_clock::now() };
	while ( isRunning )
	{
		auto end{ std::chrono::system_clock::now() };
		const float deltaTime{ std::chrono::duration<float>( end - start ).count() };
		start = end;

		SDL_Event e;
		while ( SDL_PollEvent( &e ) )
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
							bool isMin{ true };
							while ( isMin && isRunning && SDL_WaitEvent( &minEvent ) )
							{
								switch ( e.window.event )
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
							const unsigned int f = SDL_GetWindowFlags( m_pWindow );
							const mINI::INIFile file( "DisplaySettings.ini" );
							mINI::INIStructure ini;
							ini["WindowsClient"]["WindowedViewportWidth"] = std::to_string( w );
							ini["WindowsClient"]["WindowedViewportHeight"] = std::to_string( h );
							ini["WindowsClient"]["WindowedFlags"] = std::to_string( f );
							( void ) file.generate( ini, true );
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
			CManager::GetInputHandler()->ProcessEvents( e );
		}
		CManager::GetInputHandler()->Update();

		m_pScene->Update( deltaTime );
		if ( m_pRenderer->StartDraw() )continue;
		m_pScene->Draw();
		if ( m_pRenderer->EndDraw() )continue;
	}
	Cleanup();
}
