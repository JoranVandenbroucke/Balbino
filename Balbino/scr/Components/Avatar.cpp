#include "BalbinoPCH.h"
#include "Avatar.h"
#include "Texture2D.h"
#include "Audio.h"
#include "../GameObject/GameObject.h"
#include "../InputManager.h"
#include "../BinaryReaderWrider.h"

Balbino::Avatar::Avatar( const GameObject* const origine )
	:Component{ origine }
{
}

void Balbino::Avatar::Create()
{
	m_ConsoleAudio = GetComponent<ConsoleAudio>();
	m_LoggedAudio->AddSound( "SoundEffects/Converted Sounds/Shoot Bubble.wav" );
	m_ConsoleAudio->AddSound( "SoundEffects/Duck.ogg" );
	m_ConsoleAudio->AddSound( "SoundEffects/Converted Sounds/Jump.wav" );
	m_ConsoleAudio->AddSound( "SoundEffects/Fart.ogg" );

	m_LoggedAudio = GetComponent<LoggedAudio>();
	m_LoggedAudio->AddSound( "SoundEffects/Converted Sounds/Shoot Bubble.wav" );
	m_LoggedAudio->AddSound( "SoundEffects/Duck.ogg" );
	m_LoggedAudio->AddSound( "SoundEffects/Converted Sounds/Jump.wav" );
	m_LoggedAudio->AddSound( "SoundEffects/Fart.ogg" );
	//m_LoggedAudio->SetVolume( 0, 0 );
	//m_LoggedAudio->SetVolume( 1, 0 );
	//m_LoggedAudio->SetVolume( 2, 0 );
	//m_LoggedAudio->SetVolume( 3, 0 );
}

void Balbino::Avatar::Update()
{
	Balbino::Command* const cmd = InputManager::IsPressed();

	if( cmd )
	{
		cmd->Execute( *this );
	}
}

void Balbino::Avatar::Draw() const
{

}

void Balbino::Avatar::Save( std::ostream& file )
{
	BinaryReadWrite::Write( file, int( ComponentList::Avatar ) );
}

void Balbino::Avatar::Load( std::istream& file )
{
	(void) file;
}

#ifdef _DEBUG
#include "../imgui-1.75/imgui.h"
void Balbino::Avatar::DrawInpector()
{
	ImGui::BeginChild( "Avatar Component", ImVec2{ -1, 128 }, true );
	ImGui::EndChild();
}
#endif

void Balbino::Avatar::Fire()
{
	//m_Texture->SetTexture( "Fire.png" );
	m_ConsoleAudio->PlaySound( 0 );
	m_LoggedAudio->PlaySound( 0 );
}

void Balbino::Avatar::Duck()
{
	//m_Texture->SetTexture( "Duck.png" );
	m_ConsoleAudio->PlaySound( 1 );
	m_LoggedAudio->PlaySound( 1 );
}

void Balbino::Avatar::Jump()
{
	//m_Texture->SetTexture( "Jump.png" );
	m_ConsoleAudio->PlaySound( 2 );
	m_LoggedAudio->PlaySound( 2 );
}

void Balbino::Avatar::Fart()
{
	//m_Texture->SetTexture( "Fart.png" );
	m_ConsoleAudio->PlaySound( 3 );
	m_LoggedAudio->PlaySound( 3 );
}