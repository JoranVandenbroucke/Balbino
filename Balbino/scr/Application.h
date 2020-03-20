#pragma once
#include "Core.h"
struct SDL_Window;

namespace Balbino
{
	class BALBINO_API Application
	{
	public:
		Application() = default;
		~Application() = default;

		void Initialize();
		void LoadGame() const;
		void Cleanup();
		void Run();
	private:
		SDL_Window* m_Window{};
	};
	//to be defined in client
	Application* CreateApplication();
}