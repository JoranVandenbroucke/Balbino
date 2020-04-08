#include "BalbinoPCH.h"
#include "Avatar.h"
#include "Texture2D.h"
#include "Audio.h"
#include "../Renderer.h"
#include "../GameObject/GameObject.h"
#include "../InputManager.h"

Balbino::Avatar::Avatar( std::weak_ptr<GameObject> origine )
	:Component{ origine }
{
}

void Balbino::Avatar::Create()
{
	m_ConsoleAudio = GetComponent<ConsoleAudio>();
	m_ConsoleAudio.lock()->AddSound( "SoundEffects/Fire.ogg" );
	m_ConsoleAudio.lock()->AddSound( "SoundEffects/Duck.ogg" );
	m_ConsoleAudio.lock()->AddSound( "SoundEffects/Jump.ogg" );
	m_ConsoleAudio.lock()->AddSound( "SoundEffects/Fart.ogg" );

	m_LoggedAudio = GetComponent<LoggedAudio>();
	m_LoggedAudio.lock()->AddSound( "SoundEffects/Fire.ogg" );
	m_LoggedAudio.lock()->AddSound( "SoundEffects/Duck.ogg" );
	m_LoggedAudio.lock()->AddSound( "SoundEffects/Jump.ogg" );
	m_LoggedAudio.lock()->AddSound( "SoundEffects/Fart.ogg" );
	//m_LoggedAudio->SetVolume( 0, 0 );
	//m_LoggedAudio->SetVolume( 1, 0 );
	//m_LoggedAudio->SetVolume( 2, 0 );
	//m_LoggedAudio->SetVolume( 3, 0 );
}

void Balbino::Avatar::Update()
{
	std::weak_ptr<Balbino::Command> cmd = InputManager::IsPressed();
	if( cmd.lock() )
	{
		cmd.lock()->Execute( *this );
	}
}

void Balbino::Avatar::Draw() const
{

}

void Balbino::Avatar::DrawInpector() const
{
}

void Balbino::Avatar::Fire()
{
	//m_Texture->SetTexture( "Fire.png" );
	m_ConsoleAudio.lock()->PlaySound( 0 );
	m_LoggedAudio.lock()->PlaySound( 0 );
}

void Balbino::Avatar::Duck()
{
	//m_Texture->SetTexture( "Duck.png" );
	m_ConsoleAudio.lock()->PlaySound( 1 );
	m_LoggedAudio.lock()->PlaySound( 1 );
}

void Balbino::Avatar::Jump()
{
	//m_Texture->SetTexture( "Jump.png" );
	m_ConsoleAudio.lock()->PlaySound( 2 );
	m_LoggedAudio.lock()->PlaySound( 2 );
}

void Balbino::Avatar::Fart()
{
	//m_Texture->SetTexture( "Fart.png" );
	m_ConsoleAudio.lock()->PlaySound( 3 );
	m_LoggedAudio.lock()->PlaySound( 3 );
}