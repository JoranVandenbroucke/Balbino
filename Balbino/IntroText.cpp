#include "BalbinoPCH.h"
#include "IntroText.h"
#include "ResourceManager.h"

Balbino::IntroText::IntroText( std::shared_ptr<Balbino::Font> font )
	:SceneObject{}
	, m_Text{ "Programming 4 Asignment", font }
{
}

void Balbino::IntroText::Create()
{
	m_Transform.SetPosition( 80, 20, 0.f );
	m_Text.SetPosition( 80, 20 );
}

void Balbino::IntroText::Update()
{
}

void Balbino::IntroText::Draw() const
{
	m_Text.Draw();
}
