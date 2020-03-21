#include "BalbinoPCH.h"
#include "Application.h"
#include <chrono>
#include <thread>
#include <SDL.h>
#include "Time.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "ResourceManager.h"
//
//#if _DEBUG
//// ReSharper disable once CppUnusedIncludeDirective
//#include <vld.h>
//#endif
//
//#include "SDL.h"
//#include "BalbinoCore.h"

using namespace std;
using namespace std::chrono;

void Balbino::Application::Initialize()
{
	if( SDL_Init( SDL_INIT_VIDEO ) != 0 )
	{
		throw std::runtime_error( std::string( "SDL_Init Error: " ) + SDL_GetError() );
	}

	m_Window = SDL_CreateWindow(
		"Programming 4 assignment",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		640,
		480,
		SDL_WINDOW_OPENGL
	);
	if( m_Window == nullptr )
	{
		throw std::runtime_error( std::string( "SDL_CreateWindow Error: " ) + SDL_GetError() );
	}
	Renderer::Get().Init( m_Window );
}

void Balbino::Application::LoadGame() const
{

}

void Balbino::Application::Cleanup()
{
	Renderer::Get().Destroy();
	SDL_DestroyWindow( m_Window );
	m_Window = nullptr;
	SDL_Quit();
}

void Balbino::Application::Run()
{
	Initialize();
	InputManager::Init();

	// tell the resource manager where he can find the game data
	ResourceManager::Get().Init( "../Data/" );

	LoadGame();

	{
		auto& renderer = Renderer::Get();
		auto& sceneManager = SceneManager::Get();

		bool doContinue = true;
		std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();
		while( doContinue )
		{
			std::chrono::steady_clock::time_point t2 = std::chrono::steady_clock::now();

			// Calculate elapsed time
			float deltaTime = std::chrono::duration<float>( t2 - t1 ).count();
			// Update current time
			t1 = t2;

			BTime::Get().SetDT( deltaTime );

			doContinue = InputManager::ProcessInput();
			sceneManager.Update();
			renderer.Draw();
		}
	}

	Cleanup();
}