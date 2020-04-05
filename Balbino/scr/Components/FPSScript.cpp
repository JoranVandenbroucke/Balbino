#include "BalbinoPCH.h"
#include "FPSScript.h"
#include "../GameObject/GameObject.h"
#include "Text.h"
#include "../Time.h"
#include <sstream>
#include <iomanip>

Balbino::FPSScript::FPSScript( const std::weak_ptr<GameObject> origine )
	:Component{ origine }
{

}


void Balbino::FPSScript::Create()
{
	m_Text = GetComponent<Text>();
}

void Balbino::FPSScript::Update()
{
	std::stringstream stringStream;
	stringStream << "fps: " << std::setprecision( 2 ) << std::fixed << ( 1.f / BTime::UnscaledDeltaTime() );
	m_Text->SetText( stringStream.str() );
}

void Balbino::FPSScript::Draw() const
{
}