#include "BalbinoPCH.h"
#include "Audio.h"
#include "../GameObject/GameObject.h"
#include "../Editor/Debug.h"
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
		Mix_VolumeChunk( m_pMixChunks[soundID], (std::max)( (std::min)( MIX_MAX_VOLUME, volume ), 0 ) );
	}
}

Balbino::ConsoleAudio::ConsoleAudio( const GameObject* const origine )
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

void Balbino::ConsoleAudio::Save( std::ostream& file )
{
	(void) file;
}

void Balbino::ConsoleAudio::Load( std::istream& file )
{
	(void) file;
}
#ifdef _DEBUG
void Balbino::ConsoleAudio::DrawInpector()
{
	ImGui::BeginChild( "LoggedAudio", ImVec2{ -1, 128 }, true );
	ImGui::Text( "Audio Component" );
	ImGui::EndChild();
}
#endif // _DEBUG

void Balbino::ConsoleAudio::PlaySound( int soundID )
{
	if( int( m_pMixChunks.size() ) > soundID )
	{
		Mix_PlayChannel( soundID, m_pMixChunks[soundID], 0 );
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

Balbino::LoggedAudio::LoggedAudio( const GameObject* const origine )

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

void Balbino::LoggedAudio::Save( std::ostream& file )
{
	(void) file;
}

void Balbino::LoggedAudio::Load( std::istream& file )
{
	(void) file;
}

#ifdef _DEBUG
void Balbino::LoggedAudio::DrawInpector()
{
	ImGui::BeginChild( "LoggedAudio", ImVec2{ -1, 128 }, true );
	ImGui::Text( "Logged Audio Component" );
	ImGui::EndChild();
}
#endif

void Balbino::LoggedAudio::PlaySound( int soundID )
{
	if( int( m_pMixChunks.size() ) > soundID )
	{
		Mix_PlayChannel( soundID, m_pMixChunks[soundID], 0 );
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
