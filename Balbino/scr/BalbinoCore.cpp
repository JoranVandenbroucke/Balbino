#include "BalbinoPCH.h"
#include "BalbinoCore.h"
#include <chrono>
#include <thread>
//#include "InputManager.h"
//#include "SceneManager.h"
//#include "Renderer.h"
//#include "ResourceManager.h"
#include <SDL.h>
//#include "Components/Text.h"
//#include "Scene.h"
//#include "Time.h"
//#include "Components/Avatar.h"

using namespace std;
using namespace std::chrono;

void Balbino::Balbino::Initialize()
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

/**
 * Code constructing the scene world starts here
 */
void Balbino::Balbino::LoadGame() const
{
	///*auto& scene =*/ SceneManager::Get().CreateScene( "Demo" );

	//std::shared_ptr<SceneObject> go = std::make_shared<Background>();
	//go->Create();
	//scene.Add( go );

	//go = std::make_shared<DAELogo>();
	//go->Create();
	//scene.Add( go );
	//
	//auto font = ResourceManager::Get().LoadFont( "Lingua.otf", 36 );
	//go = std::make_shared<IntroText>( font );
	//go->Create();
	//scene.Add( go );

	//go = std::make_shared<FPSCounter>();
	//go->Create();
	//scene.Add( go );

	//go = std::make_shared<Avatar>();
	//go->Create();
	//scene.Add( go );

	//InputManager::Init();
}

void Balbino::Balbino::Cleanup()
{
	Renderer::Get().Destroy();
	SDL_DestroyWindow( m_Window );
	m_Window = nullptr;
	SDL_Quit();
}

void Balbino::Balbino::Run()
{
	Initialize();

	// tell the resource manager where he can find the game data
	//ResourceManager::Get().Init( "../Data/" );

	LoadGame();

	{
		//auto& renderer = Renderer::Get();
		//auto& sceneManager = SceneManager::Get();

		bool doContinue = true;
		std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();
		while( doContinue )
		{
			std::chrono::steady_clock::time_point t2 = std::chrono::steady_clock::now();

			// Calculate elapsed time
			float deltaTime = std::chrono::duration<float>( t2 - t1 ).count();
			std::cout << deltaTime << '\n';
			// Update current time
			t1 = t2;

			//BTime::Get().SetDT( deltaTime );

			//doContinue = InputManager::ProcessInput();
			//sceneManager.Update();
			//renderer.Draw();
		}
	}

	Cleanup();
}
