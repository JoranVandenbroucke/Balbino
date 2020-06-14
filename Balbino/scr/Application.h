#pragma once
#include "Core.h"
#include <ctime>
#include <ratio>
#include <chrono>
#include <SDL.h>
#include <future>
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
		static SDL_Window* GetWindow()
		{
			return m_pWindow;
		}
	private:
		// The window we render to

		// OpenGL context
		SDL_GLContext m_pContext;
public:
		void SetImGuiStyle();
	private:
		static SDL_Window* m_pWindow;
		std::future<void> m_Future;
	};
	//to be defined in client
	Application* CreateApplication();
}