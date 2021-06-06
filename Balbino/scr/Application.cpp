#include "pch.h"
#include "Application.h"

#include <SDL.h>
#include <SDL_vulkan.h>
#include <vulkan/vulkan.hpp>

#include <iostream>
#include <vector>

#include "Renderer/Renderer.h"

#ifdef BL_EDITOR
#include <scr/Interface.h>
#endif


Balbino::Application::Application()
	: m_pWindow{ nullptr }
	, m_pRenderer{ DBG_NEW Renderer{} }
#ifdef BL_EDITOR
	, m_pInterface{ DBG_NEW Interface{} }
#endif
{
}

Balbino::Application::~Application()
{
	m_pWindow = nullptr;
}

void Balbino::Application::Initialize()
{
#ifdef BL_EDITOR
	m_pRenderer->SetInterface( m_pInterface );
#endif
	
	if ( SDL_Init( SDL_INIT_EVERYTHING ) != 0 )
	{
		throw std::runtime_error( std::string( "SDL_Init Error: " ) + SDL_GetError() );
	}
	SDL_DisplayMode current;
	if ( SDL_GetDesktopDisplayMode( 0, &current ) != 0 )
	{
		throw std::runtime_error( std::string( "Could not get display mode for video display 0: " ) + SDL_GetError() );
	}

	const SDL_WindowFlags flags{ SDL_WINDOW_VULKAN /*| SDL_WINDOW_FULLSCREEN*/ };
	m_pWindow = SDL_CreateWindow(
		"Balbino Editor",
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
	const char** extensions{ DBG_NEW const char* [extenstionCount] };
	if ( !SDL_Vulkan_GetInstanceExtensions( m_pWindow, &extenstionCount, extensions ) )
	{
		throw std::runtime_error( std::string( "Could not get the names of required m_Instance extensions from SDL." ) );
	}
	m_pRenderer->SetupVulkan( extensions, extenstionCount );
	delete[] extensions;
	m_pRenderer->SetupVulkanWindow( m_pWindow );
	m_pRenderer->Setup( m_pWindow );
}

void Balbino::Application::LoadGame() const
{
	std::cout << "Load Engine\n";
}

void Balbino::Application::Cleanup() const
{
	m_pRenderer->Cleanup();
	SDL_DestroyWindow( m_pWindow );
	//Quit SDL subsystems
	SDL_Quit();
}

void Balbino::Application::Run() const
{
	LoadGame();
	bool isRunning{ true };
	while ( isRunning )
	{
		SDL_Event e;
		while ( SDL_PollEvent( &e ) )
		{
#ifdef BL_EDITOR
			m_pInterface->HandleEvents( e );
#endif

			switch ( e.type )
			{
			case SDL_QUIT:
				isRunning = false;
				break;

			default:
				// Do nothing.
				break;
			}
		}
		m_pRenderer->Draw( m_pWindow );
	}
	Cleanup();
}
