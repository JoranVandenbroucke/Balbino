#include "BalbinoPCH.h"
#include "FPSScript.h"
#include "../GameObject/GameObject.h"
#include "Text.h"
#include "../Time.h"
#include "../BinaryReaderWrider.h"
#include <sstream>
#include <iomanip>

Balbino::FPSScript::FPSScript( const GameObject* const origine )

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

void Balbino::FPSScript::Save( std::ostream& file )
{
	BinaryReadWrite::Write( file, int( ComponentList::FPSScript ) );
}

void Balbino::FPSScript::Load( std::istream& file )
{
	(void) file;
}

#ifdef _DEBUG
#include "../imgui-1.75/imgui.h"
void Balbino::FPSScript::DrawInpector()
{
	ImGui::BeginChild( "FPS Script Component", ImVec2{ -1, 128 }, true );
	ImGui::EndChild();
}
#endif