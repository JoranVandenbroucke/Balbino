#include "pch.h"
#include "Application.h"

#include <SDL.h>
#include <SDL_vulkan.h>

#include "Input/InputHandler.h"
#include "Managers/CameraManager.h"
#include "Managers/MaterialManager.h"
#include "Managers/MeshManager.h"
#include "Managers/ShaderManager.h"
#include "Managers/TextureManager.h"
#include "Renderer/Renderer.h"


Balbino::Application::Application()
	: m_pWindow{ nullptr }
	, m_manager{}
	, m_pRenderer{ DBG_NEW CRenderer{} }
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

	constexpr SDL_WindowFlags flags{ SDL_WINDOW_VULKAN /*| SDL_WINDOW_FULLSCREEN*/ };
	m_pWindow = SDL_CreateWindow(
		"Balbino Engine",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		current.w / 2,
		current.h / 2,
		flags
	);
	if ( m_pWindow == nullptr )
	{
		throw std::runtime_error( std::string( "SDL_CreateWindow Error: " ) + SDL_GetError() );
	}

	m_manager.SetInputHandler( DBG_NEW CInputHandler{} );
	m_manager.SetCameraManager( DBG_NEW CCameraManager{} );
	m_manager.SetMeshManager( DBG_NEW CMeshManager{} );
	m_manager.SetShaderManager( DBG_NEW CShaderManager{} );
	m_manager.SetTextureManager( DBG_NEW CTextureManager{} );
	m_manager.SetMaterialManager( DBG_NEW CMaterialManager{} );
	m_manager.GetInputHandler()->Initialize();
	m_manager.GetCameraManager()->SetRenderer(m_pRenderer);
}

void Balbino::Application::LoadGame() const
{
	std::cout << "Load Engine\n";
	CManager::GetCameraManager()->AddCamera( { 0, 1, 0 } );

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
	auto extensions{ DBG_NEW const char* [extenstionCount] };
	if ( !SDL_Vulkan_GetInstanceExtensions( m_pWindow, &extenstionCount, extensions ) )
	{
		throw std::runtime_error( std::string( "Could not get the names of required m_Instance extensions from SDL." ) );
	}
	m_pRenderer->Setup( m_pWindow, extensions, extenstionCount );
	delete[] extensions;
}

void Balbino::Application::Cleanup()
{
	//m_pRenderer->Cleanup();
	m_manager.Cleanup();
	m_pRenderer->Cleanup();
	//delete m_pRenderer;

	m_manager.SetInputHandler( nullptr );
	m_manager.SetCameraManager( nullptr );
	m_manager.SetMeshManager( nullptr );
	m_manager.SetShaderManager( nullptr );
	m_manager.SetTextureManager( nullptr );
	m_manager.SetMaterialManager( nullptr );

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
			switch ( e.type )
			{
				case SDL_QUIT:
					isRunning = false;
					break;
				default:
					// Do nothing.
					break;
			}
			CManager::GetInputHandler()->ProcessEvents( e );
		}
		CManager::GetCameraManager()->Update( deltaTime );
		CManager::GetInputHandler()->Update();
		m_pRenderer->Draw();
	}
	Cleanup();
}
