#pragma once
struct SDL_Window;
namespace Balbino
{
	class Balbino
	{
	public:
		void Initialize();
		void LoadGame() const;
		void Cleanup();
		void Run();
	private:
		SDL_Window* m_Window{};
	};
}