#include "BalbinoPCH.h"
#include "Application.h"
#include "Time.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "Renderer/Renderer.h"
#include "ResourceManager.h"

using namespace std;
using namespace std::chrono;

#ifdef _DEBUG
SDL_Window* Balbino::Application::m_pWindow{ nullptr };
Balbino::Application::Application()
	: m_pContext{ nullptr }
{
}

#else
Balbino::Application::Application()
	:m_pWindow{ nullptr }
	, m_pContext{ nullptr }
{
}
#endif // _DEBUG

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

	// GL 3.3 + GLSL 330
	const char* glsl_version = "#version 330";
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_FLAGS, 0 );
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 3 );

#ifdef _DEBUG
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG );

	m_pWindow = SDL_CreateWindow(
		"Programming 4 assignment",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		1024,
		768,
		SDL_WINDOW_OPENGL | SDL_WINDOW_MAXIMIZED | SDL_WINDOW_RESIZABLE
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
	SDL_GetWindowSize( m_pWindow, &w, &h );

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
	if( gl3wInit() )
	{
		std::cerr << "failed to initialize OpenGL\n";
		return;
	}
	//glEnable( GL_CULL_FACE );	
	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	Renderer::Get().Init( m_pWindow );
	ResourceManager::Get().Init( "../Data/" );

#ifdef _DEBUG
	// Initialize OpenGL loader
#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
	bool err = gl3wInit() != 0;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
	bool err = glewInit() != GLEW_OK;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
	bool err = gladLoadGL() == 0;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLBINDING2)
	bool err = false;
	glbinding::Binding::initialize();
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLBINDING3)
	bool err = false;
	glbinding::initialize( []( const char* name )
	{
		return ( glbinding::ProcAddress )SDL_GL_GetProcAddress( name );
	} );
#else
	bool err = false; // If you use IMGUI_IMPL_OPENGL_LOADER_CUSTOM, your loader is likely to requires some form of initialization.
#endif
	if( err )
	{
		fprintf( stderr, "Failed to initialize OpenGL loader!\n" );
		return;
	}
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	//ImGuiSDL::Initialize( Renderer::Get().GetSDLRenderer(), w, h );
	ImGuiIO& io = ImGui::GetIO(); (void) io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
	//io.ConfigViewportsNoAutoMerge = true;
	//io.ConfigViewportsNoTaskBarIcon = true;

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
	ImGuiStyle& style = ImGui::GetStyle();
	if( io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable )
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	// Setup Platform/Renderer bindings
	ImGui_ImplSDL2_InitForOpenGL( m_pWindow, m_pContext );
	ImGui_ImplOpenGL3_Init( glsl_version );
#endif // _DEBUG

}

void Balbino::Application::LoadGame() const
{

}

void Balbino::Application::Cleanup()
{
	ResourceManager::Cleanup();
	Renderer::Get().Destroy();
	//ImGuiSDL::Deinitialize();
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
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
		sceneManager.SetScene( 0 );
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