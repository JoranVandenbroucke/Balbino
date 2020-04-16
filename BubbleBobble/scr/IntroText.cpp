#include "pch.h"
#include "IntroText.h"

void BubbleBobble::IntroText::Create()
{
	std::shared_ptr<Balbino::Transform> transform = AddComponent<Balbino::Transform>();
	transform->SetPosition( 0.f, -0.4f, 0.f );

	std::shared_ptr<Balbino::Text> text = AddComponent<Balbino::Text>();
	text->SetFont( "Lingua.otf", 42 );
	text->SetText( "Programming 4 Assignment" );

	SetName( "Text" );
	LoadComponents();
}