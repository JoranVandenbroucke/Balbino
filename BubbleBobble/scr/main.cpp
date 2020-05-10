#include "pch.h"
#include <vld.h>
namespace BubbleBobble
{
	class BubbleBobbleGame: public Balbino::Application
	{
	public:
		BubbleBobbleGame() = default;

		virtual void LoadGame() const override;
	};

	void BubbleBobbleGame::LoadGame() const
	{
		Balbino::SceneManager::Get().CreateScene( "First Scene" );
	}
}

Balbino::Application* Balbino::CreateApplication()
{
	return new BubbleBobble::BubbleBobbleGame{};
}