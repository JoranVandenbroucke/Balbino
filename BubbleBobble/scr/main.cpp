#include "pch.h"
#include <Balbino.h>

class BubbleBobble: public Balbino::Application
{
public:
	BubbleBobble() = default;

	virtual void LoadGame() const override;
};

Balbino::Application* Balbino::CreateApplication()
{
	return new BubbleBobble{};
}

void BubbleBobble::LoadGame() const
{
	Balbino::Scene& firsScene = Balbino::SceneManager::Get().CreateScene("First Scene");
}