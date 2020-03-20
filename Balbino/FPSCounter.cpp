#include "BalbinoPCH.h"
#include "FPSCounter.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "Text.h"
#include "Time.h"
#include <sstream>
#include <iomanip>

void Balbino::FPSCounter::Create()
{
	auto font = ResourceManager::GetInstance().LoadFont( "Lingua.otf", 16 );
	m_Text = std::make_shared<Text>( "fps: 000.00", font );
	m_Text->SetColor( { 54, 35, 213 } );
	m_Text->SetPosition( 0, 0 );
	m_Transform.SetPosition( 4.f, 4.f, 0.f );
}

void Balbino::FPSCounter::Update()
{
	std::stringstream stringStream;
	stringStream << "fps: " << std::setprecision( 2 ) << std::fixed << ( 1.f / BTime::UnscaledDeltaTime() );
	m_Text->SetText( stringStream.str() );
}

void Balbino::FPSCounter::Draw() const
{
	m_Text->Draw();
}