#include "pch.h"
#include "DAELogo.h"

void BubbleBobble::DAELogo::Create()
{
	std::shared_ptr<Balbino::Transform> transform = AddComponent<Balbino::Transform>();
	transform->SetPosition( 216.f, 180.f, 0.f );

	std::shared_ptr<Balbino::Texture2D> texture = AddComponent<Balbino::Texture2D>();
	texture->SetTexture( "logo.png" );

	LoadComponents();
}