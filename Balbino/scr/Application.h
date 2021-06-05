#pragma once
#include "Core.h"

struct SDL_Window;
namespace Balbino
{
	class Renderer;
#ifdef BL_EDITOR
	class Interface;
#endif
	
	class BALBINO_API Application
	{
	public:
		Application();
		virtual ~Application();
		Application( const Application& ) = delete;
		Application( Application&& ) = delete;
		Application& operator=( const Application& ) = delete;
		Application& operator=( Application&& ) = delete;

		void Initialize();
		virtual void LoadGame() const;
		void Cleanup() const;
		void Run() const;

	private:
		SDL_Window* m_pWindow;

		Renderer* m_pRenderer;
#ifdef BL_EDITOR
		Interface* m_pInterface;;
#endif
	};

	//to be defined in client
	Application* CreateApplication();
}
