#include "BalbinoPCH.h"
#include "Application.h"
#include "Time.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "ResourceManager.h"

#include "imgui-1.75/imgui.h"
#include "imgui-1.75/imgui_sdl.h"
#include "imgui-1.75/imgui_impl_sdl.h"
//#include <imgui_impl_opengl3.h>

using namespace std;
using namespace std::chrono;

Balbino::Application::Application()
	:m_pWindow{nullptr}
	,m_pContext{ nullptr }
{
}

Balbino::Application::~Application()
{
	SDL_GL_DeleteContext( m_pContext );

	SDL_DestroyWindow( m_pWindow );
	m_pWindow = nullptr;

	//Quit SDL subsystems
	Mix_Quit();
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

void Balbino::Application::Initialize()
{
	if( SDL_Init( SDL_INIT_VIDEO ) != 0 )
	{
		throw std::runtime_error( std::string( "SDL_Init Error: " ) + SDL_GetError() );
	}
#ifdef _DEBUG
	m_pWindow = SDL_CreateWindow(
		"Programming 4 assignment",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		1024,
		768,
		SDL_WINDOW_OPENGL | SDL_WINDOW_MAXIMIZED
	);
#else
	m_pWindow = SDL_CreateWindow(
		"Programming 4 assignment",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		640,
		480,
		SDL_WINDOW_OPENGL | SDL_WINDOW_FUL
	);
#endif
	int w{}, h{};
	SDL_GetWindowSize( m_pWindow, &w, &h);
	if( m_pWindow == nullptr )
	{
		throw std::runtime_error( std::string( "SDL_CreateWindow Error: " ) + SDL_GetError() );
	}

	// Create OpenGL context 
	m_pContext = SDL_GL_CreateContext( m_pWindow );
	if( m_pContext == nullptr )
	{
		std::cerr << "Core::Initialize( ), error when calling SDL_GL_CreateContext: " << SDL_GetError() << std::endl;
		return;
	}

	// Set the swap interval for the current OpenGL context,
	// synchronize it with the vertical retrace

	if( SDL_GL_SetSwapInterval( 1 ) < 0 )
	{
		std::cerr << "Core::Initialize( ), error when calling SDL_GL_SetSwapInterval: " << SDL_GetError() << std::endl;
		return;
	}

	// Set the Projection matrix to the identity matrix
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();

	// Set up a two-dimensional orthographic viewing region.
	gluOrtho2D( 0, w, 0, h ); // y from bottom to top

	// Set the viewport to the client window area
	// The viewport is the rectangular region of the window where the image is drawn.
	glViewport( 0, 0, w , h );

	// Set the Modelview matrix to the identity matrix
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

	// Enable color blending and use alpha blending
	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

	Renderer::Get().Init( m_pWindow );
	ResourceManager::Get().Init( "../Data/" );

#ifdef _DEBUG
	IMGUI_CHECKVERSION();

	ImGui::CreateContext();
	ImGuiSDL::Initialize( Renderer::Get().GetSDLRenderer(), w, h );
	ImGui_ImplSDL2_InitForOpenGL( m_pWindow, m_pContext );
#endif // _DEBUG

}

void Balbino::Application::LoadGame() const
{

}

void Balbino::Application::Cleanup()
{
	ResourceManager::Cleanup();
	Renderer::Get().Destroy();
	ImGuiSDL::Deinitialize();
	ImGui::DestroyContext();
}

void Balbino::Application::Run()
{
	Initialize();
	InputManager::Init();

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

			sceneManager.Update();
			doContinue = InputManager::ProcessInput();

			renderer.Draw();
		}
	}

	Cleanup();
}