#include "pch.h"
#include "IntroText.h"

BubbleBobble::IntroText::IntroText( std::shared_ptr<Balbino::Font> font )
	: SceneObject{}
	, m_Text{ "Programming 4 Asignment", font }
{
}

void BubbleBobble::IntroText::Create()
{
	m_Transform.SetPosition( 80, 20, 0.f );
	m_Text.SetPosition( 80, 20 );
}

void BubbleBobble::IntroText::Update()
{
}

void BubbleBobble::IntroText::Draw() const
{
	m_Text.Draw();
}
