#pragma once
#include "Core.h"
#include "Managers/Manager.h"

struct SDL_Window;

namespace Balbino
{
	class CRenderer;

#ifdef BL_EDITOR
	class CInterface;
#endif

	class BALBINO_API Application
	{
	public:
		Application();
		virtual ~Application();
		Application(const Application&) = delete;
		Application(Application&&) = delete;
		Application& operator=(const Application&) = delete;
		Application& operator=(Application&&) = delete;

		void Initialize();
		virtual void LoadGame() const;
		void Cleanup();
		void Run();

	private:
		SDL_Window* m_pWindow;

		CManager m_manager;
		CRenderer* m_pRenderer;
	};

	//to be defined in client
	Application* CreateApplication();
}
