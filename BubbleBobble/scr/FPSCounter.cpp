#include "pch.h"
#include "FPSCounter.h"
#include <sstream>
#include <iomanip>

void BubbleBobble::FPSCounter::Create()
{
	std::shared_ptr<Balbino::Transform> transform = AddComponent<Balbino::Transform>();
	transform->SetPosition( 4.f, 4.f, 0.f );

	std::shared_ptr<Balbino::Texture2D> textTexture = AddComponent<Balbino::Texture2D>();

	std::shared_ptr<Balbino::Text> text = AddComponent<Balbino::Text>();
	text->SetColor( Balbino::Color{ 54, 213, 35 } );
	text->SetText( "fps: 000.00" );

	std::shared_ptr<Balbino::FPSScript> fps = AddComponent<Balbino::FPSScript>();

	LoadComponents();
}