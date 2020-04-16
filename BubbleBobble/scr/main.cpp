#include "pch.h"
#include "Background.h"
#include "DAELogo.h"
#include "IntroText.h"
#include "FPSCounter.h"
#include "Players.h"

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

		//std::shared_ptr<Balbino::GameObject> go = std::make_shared<Bub>();
		//firsScene.Add( go );

		//go = std::make_shared<FPSCounter>();
		//firsScene.Add( go );
		//
		//go = std::make_shared<IntroText>();
		//firsScene.Add( go );

		//go = std::make_shared<DAELogo>();
		//firsScene.Add( go );

		//go = std::make_shared<Background>();
		//firsScene.Add( go );
	}
}

Balbino::Application* Balbino::CreateApplication()
{
	return new BubbleBobble::BubbleBobbleGame{};
}