#include "pch.h"
#include "FPSCounter.h"
#include <sstream>
#include <iomanip>

void BubbleBobble::FPSCounter::Create()
{
	auto font = Balbino::ResourceManager::Get().LoadFont( "Lingua.otf", 16 );
	m_Text = std::make_shared<Balbino::Text>( "fps: 000.00", font );
	m_Text->SetColor( { 54, 35, 213 } );
	m_Text->SetPosition( 0, 0 );
	m_Transform.SetPosition( 4.f, 4.f, 0.f );
}

void BubbleBobble::FPSCounter::Update()
{
	std::stringstream stringStream;
	stringStream << "fps: " << std::setprecision( 2 ) << std::fixed << ( 1.f / Balbino::BTime::UnscaledDeltaTime() );
	m_Text->SetText( stringStream.str() );
}

void BubbleBobble::FPSCounter::Draw() const
{
	m_Text->Draw();
}