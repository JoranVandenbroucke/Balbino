#include "pch.h"
#include <Balbino.h>

namespace Balbino
{
	class Engine: public Balbino::Application
	{
	public:
		Engine() = default;

		virtual void LoadGame() const override;
	};

	void Engine::LoadGame() const
	{
		SceneManager::Get().CreateScene();
	}
}

Balbino::Application* Balbino::CreateApplication()
{
	return new Engine{};
}