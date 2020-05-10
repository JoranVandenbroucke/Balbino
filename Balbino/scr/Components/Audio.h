#pragma once
#include "Component.h"

struct Mix_Chunk;
namespace Balbino
{
	class GameObject;

	class Audio
	{
	public:
		Audio();

		virtual ~Audio();
		virtual void PlaySound( int soundID ) = 0;
		virtual void StopSound( int soundID ) = 0;
		virtual void StopAllSounds() = 0;

		void AddSound( const char* filePath );
		void SetVolume( const int soundID, const int volume );
	protected:
		std::vector<Mix_Chunk*> m_pMixChunks;

	};

	class ConsoleAudio: public Balbino::Audio, public Balbino::Component
	{
	public:
		ConsoleAudio( const GameObject* const origine );

		virtual void Create() override;
		virtual void Update() override;
		virtual void Draw() const override;

		virtual void Save( std::ostream& file )override;
		virtual void Load( std::istream& file )override;
#ifdef _DEBUG
		virtual void DrawInpector() override;
#endif // _DEBUG

		virtual void PlaySound( int soundID );
		virtual void StopSound( int soundID );
		virtual void StopAllSounds();
	};

	class LoggedAudio: public Balbino::Audio, public Balbino::Component
	{
	public:
		LoggedAudio( const GameObject* const origine );


		virtual void Create() override;
		virtual void Update() override;
		virtual void Draw() const override;

		virtual void Save( std::ostream& file )override;
		virtual void Load( std::istream& file )override;
#ifdef _DEBUG
		virtual void DrawInpector() override;
#endif // _DEBUG

		virtual void PlaySound( int soundID );
		virtual void StopSound( int soundID );
		virtual void StopAllSounds();

	private:
		void Log( const char* message );
	};
}