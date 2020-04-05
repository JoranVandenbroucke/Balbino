#include "pch.h"
#include "IntroText.h"

void BubbleBobble::IntroText::Create()
{
	std::shared_ptr<Balbino::Transform> transform = AddComponent<Balbino::Transform>();
	transform->SetPosition( 20, 80, 0.f );

	std::shared_ptr<Balbino::Texture2D> textTexture = AddComponent<Balbino::Texture2D>();

	std::shared_ptr<Balbino::Text> text = AddComponent<Balbino::Text>();
	text->SetFont( "Lingua.otf", 42 );
	text->SetText( "Programming 4 Assignment" );

	LoadComponents();
}