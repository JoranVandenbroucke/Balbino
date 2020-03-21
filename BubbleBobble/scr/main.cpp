#include "pch.h"
#include "Background.h"

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
		Balbino::Scene& firsScene = Balbino::SceneManager::Get().CreateScene( "First Scene" );
		std::shared_ptr<BubbleBobble::Background> bg = std::make_shared<BubbleBobble::Background>();
		bg->Create();
		firsScene.Add( bg );
	}
}

Balbino::Application* Balbino::CreateApplication()
{
	return new BubbleBobble::BubbleBobbleGame{};
}