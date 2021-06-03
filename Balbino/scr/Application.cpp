#include "pch.h"
#include "Application.h"

#include <SDL.h>
#include <SDL_vulkan.h>
#include <vulkan/vulkan.hpp>

#include <iostream>
#include <vector>

vk::Instance m_Instance;
vk::SurfaceKHR m_Surface;

Balbino::Application::Application()
	: m_pWindow{ nullptr }
{
}

Balbino::Application::~Application()
{
	m_pWindow = nullptr;
}

void Balbino::Application::Initialize()
{
	if ( SDL_Init( SDL_INIT_EVERYTHING ) != 0 )
	{
		throw std::runtime_error( std::string( "SDL_Init Error: " ) + SDL_GetError() );
	}
	SDL_DisplayMode current;
	const int shouldBeZero{ SDL_GetDesktopDisplayMode( 0, &current ) };
	if ( shouldBeZero != 0 )
	{
		throw std::runtime_error( std::string( "Could not get display mode for video display 0: " ) + SDL_GetError() );
	}

	const SDL_WindowFlags flags{ static_cast<SDL_WindowFlags>( SDL_WINDOW_VULKAN /*| SDL_WINDOW_FULLSCREEN*/ ) };
	m_pWindow = SDL_CreateWindow(
		"Balbino Editor",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		current.w/2,
		current.h/2,
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
	std::vector<const char*> extensions( extenstionCount );
	if ( !SDL_Vulkan_GetInstanceExtensions( m_pWindow, &extenstionCount, extensions.data() ) )
	{
		throw std::runtime_error( std::string( "Could not get the names of required m_Instance extensions from SDL." ) );
	}

	
}

void Balbino::Application::LoadGame() const
{
	std::cout << "Load Game\n";
}

void Balbino::Application::Cleanup()
{
	// Clean up.
	m_Instance.destroySurfaceKHR( m_Surface );
	SDL_DestroyWindow( m_pWindow );
	//Quit SDL subsystems
	SDL_Quit();
	m_Instance.destroy();
}

void Balbino::Application::Run()
{
	LoadGame();
	bool isRunning{ true };
	while ( isRunning )
	{
		SDL_Event event;
		while ( SDL_PollEvent( &event ) )
		{
			switch ( event.type )
			{
			case SDL_QUIT:
				isRunning = false;
				break;

			default:
				// Do nothing.
				break;
			}
		}
		std::cout << "Lol";
	}
	Cleanup();
}
