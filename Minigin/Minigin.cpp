#include "MiniginPCH.h"
#include "Minigin.h"
#include <chrono>
#include <thread>
#include "InputManager.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include <SDL.h>
#include "Text.h"
#include "Background.h"
#include "Scene.h"
#include "FPSCounter.h"
#include "Time.h"
#include "DAELogo.h"
#include "Avatar.h"

using namespace std;
using namespace std::chrono;

void Balbino::Minigin::Initialize()
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

	Renderer::GetInstance().Init( m_Window );
}

/**
 * Code constructing the scene world starts here
 */
void Balbino::Minigin::LoadGame() const
{
	auto& scene = SceneManager::GetInstance().CreateScene( "Demo" );

	std::shared_ptr<SceneObject> go = std::make_shared<Background>();
	go->Create();
	scene.Add( go );

	go = std::make_shared<DAELogo>();
	go->Create();
	scene.Add( go );

	auto font = ResourceManager::GetInstance().LoadFont( "Lingua.otf", 36 );
	auto to = std::make_shared<Text>( "Programming 4 Assignment", font );
	to->SetPosition( 80, 20 );
	scene.Add( to );

	go = std::make_shared<FPSCounter>();
	go->Create();
	scene.Add( go );

	go = std::make_shared<Avatar>();
	go->Create();
	scene.Add( go );

	InputManager::Init();
}

void Balbino::Minigin::Cleanup()
{
	Renderer::GetInstance().Destroy();
	SDL_DestroyWindow( m_Window );
	m_Window = nullptr;
	SDL_Quit();
}

void Balbino::Minigin::Run()
{
	Initialize();

	// tell the resource manager where he can find the game data
	ResourceManager::GetInstance().Init( "../Data/" );

	LoadGame();

	{
		auto& renderer = Renderer::GetInstance();
		auto& sceneManager = SceneManager::GetInstance();

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

			BTime::Get().SetDT( deltaTime );

			doContinue = InputManager::ProcessInput();
			sceneManager.Update();
			renderer.Draw();
		}
	}

	Cleanup();
}
