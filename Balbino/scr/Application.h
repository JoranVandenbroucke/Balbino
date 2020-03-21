#pragma once
#include "Core.h"
struct SDL_Window;

namespace Balbino
{
	class BALBINO_API Application
	{
	public:
		Application() = default;
		virtual ~Application() = default;

		virtual void Initialize();
		virtual void LoadGame() const;
		virtual void Cleanup();
		virtual void Run();
	private:
		SDL_Window* m_Window{};
	};
	//to be defined in client
	Application* CreateApplication();
}