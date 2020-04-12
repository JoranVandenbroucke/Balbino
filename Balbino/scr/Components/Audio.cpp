#include "BalbinoPCH.h"
#include "Audio.h"
#include "../GameObject/GameObject.h"
#include "../Debug.h"
#include "../ResourceManager.h"
#include <algorithm>
#include "../imgui-1.75/imgui.h"

Balbino::Audio::Audio()
	:m_pMixChunks{}
{
}

Balbino::Audio::~Audio()
{
	m_pMixChunks.clear();
}

void Balbino::Audio::AddSound( const char* filePath )
{
	m_pMixChunks.push_back( ResourceManager::LoadAudio(filePath) );
}

void Balbino::Audio::SetVolume( const int soundID, const int volume )
{
	if( int( m_pMixChunks.size() ) > soundID )
	{
		Mix_VolumeChunk( m_pMixChunks[soundID].lock().get(), (std::max)( (std::min)( MIX_MAX_VOLUME, volume ), 0 ) );
	}
}

Balbino::ConsoleAudio::ConsoleAudio( const std::weak_ptr<GameObject> origine )
	:Component{ origine }
	, Audio{}
{
}

void Balbino::ConsoleAudio::Create()
{
}

void Balbino::ConsoleAudio::Update()
{
}

void Balbino::ConsoleAudio::Draw() const
{
}

void Balbino::ConsoleAudio::DrawInpector()
{
	
}

void Balbino::ConsoleAudio::PlaySound( int soundID )
{
	if( int( m_pMixChunks.size() ) > soundID )
	{
		Mix_PlayChannel( soundID, m_pMixChunks[soundID].lock().get(), 0 );
	}
}

void Balbino::ConsoleAudio::StopSound( int soundID )
{
	Mix_HaltChannel( soundID );
}

void Balbino::ConsoleAudio::StopAllSounds()
{
	Mix_HaltChannel( -1 );
}

Balbino::LoggedAudio::LoggedAudio( const std::weak_ptr<GameObject> origine )
	:Component{origine}
	, Audio{}
{
}

void Balbino::LoggedAudio::Create()
{
}

void Balbino::LoggedAudio::Update()
{
}

void Balbino::LoggedAudio::Draw() const
{
}

#ifdef _DEBUG
#include "../imgui-1.75/imgui.h"
void Balbino::LoggedAudio::DrawInpector()
{

}
#endif

void Balbino::LoggedAudio::PlaySound( int soundID )
{
	if( int( m_pMixChunks.size() ) > soundID )
	{
		Mix_PlayChannel( soundID, m_pMixChunks[soundID].lock().get(), 0 );
	}
	Log( "Playing Sound" );
}

void Balbino::LoggedAudio::StopSound( int soundID )
{
	Mix_HaltChannel( soundID );
	Log( "Stoped Sound" );
}

void Balbino::LoggedAudio::StopAllSounds()
{
	Mix_HaltChannel( -1 );
	Log( "Stoped All Sounds" );
}

void Balbino::LoggedAudio::Log( const char* message )
{
	Debug::Log( message );
}
