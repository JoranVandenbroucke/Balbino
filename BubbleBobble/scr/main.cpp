#include "pch.h"
#include "Background.h"
#include "DAELogo.h"
#include "IntroText.h"
#include "FPSCounter.h"

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
		std::shared_ptr<Balbino::SceneObject> go = std::make_shared<Background>();
		go->Create();
		firsScene.Add( go );

		go = std::make_shared<DAELogo>();
		go->Create();
		firsScene.Add( go );
		
		auto font = Balbino::ResourceManager::Get().LoadFont( "Lingua.otf", 36 );
		go = std::make_shared<IntroText>( font );
		go->Create();
		firsScene.Add( go );

		go = std::make_shared<FPSCounter>();
		go->Create();
		firsScene.Add( go );

		//go = std::make_shared<Avatar>();
		//go->Create();
		//firsScene.Add( go );
	}
}

Balbino::Application* Balbino::CreateApplication()
{
	return new BubbleBobble::BubbleBobbleGame{};
}