#pragma once
#include "Core.h"
#include <ctime>
#include <ratio>
#include <chrono>
#include <SDL.h>
//struct SDL_Window;

namespace Balbino
{
	class BALBINO_API Application
	{
	public:
		Application();
		virtual ~Application();

		virtual void Initialize();
		virtual void LoadGame() const;
		virtual void Cleanup();
		virtual void Run();
	private:
		// The window we render to
		SDL_Window* m_pWindow;
		// OpenGL context
		SDL_GLContext m_pContext;
	};
	//to be defined in client
	Application* CreateApplication();
}