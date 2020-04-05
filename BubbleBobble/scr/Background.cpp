#include "pch.h"
#include "Background.h"

BubbleBobble::Background::~Background() = default;

void BubbleBobble::Background::Create()
{
	std::shared_ptr<Balbino::Transform> transform = AddComponent<Balbino::Transform>();
	transform->SetPosition( 0.f, 0.f, 0.f );
	std::shared_ptr<Balbino::Texture2D> texture = AddComponent<Balbino::Texture2D>();
	texture->SetTexture( "background.jpg" );

	LoadComponents();
}