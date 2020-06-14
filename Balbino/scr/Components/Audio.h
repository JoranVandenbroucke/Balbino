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

		Audio( const Audio& ) = delete;
		Audio( Audio&& ) = delete;
		Audio& operator=( const Audio& ) = delete;
		Audio& operator=( Audio&& ) = delete;

		virtual void PlaySound( int soundID ) = 0;
		virtual void StopSound( int soundID ) = 0;
		virtual void StopAllSounds() = 0;

		void AddSound( const char* filePath );
		void SetVolume( const int soundID, const int volume );
	protected:
		std::vector<Mix_Chunk*> m_pMixChunks;
	};

	class ConsoleAudio final : public Balbino::Audio, public Balbino::Component
	{
	public:
		explicit ConsoleAudio( const GameObject* const origine );
		virtual ~ConsoleAudio() = default;

		ConsoleAudio( const ConsoleAudio& ) = delete;
		ConsoleAudio( ConsoleAudio&& ) = delete;
		ConsoleAudio& operator=( const ConsoleAudio& ) = delete;
		ConsoleAudio& operator=( ConsoleAudio&& ) = delete;

		virtual void Create() override;
		virtual void Update() override;
		virtual void Draw() const override;

		virtual void Save( std::ostream& file )override;
		virtual void Load( std::istream& file )override;
#ifdef BALBINO_DEBUG
		virtual void DrawInpector() override;
#endif // BALBINO_DEBUG

		virtual void PlaySound( int soundID );
		virtual void StopSound( int soundID );
		virtual void StopAllSounds();
	};

	class LoggedAudio final : public Balbino::Audio, public Balbino::Component
	{
	public:
		explicit LoggedAudio( const GameObject* const origine );
		virtual ~LoggedAudio() = default;

		LoggedAudio( const LoggedAudio& ) = delete;
		LoggedAudio( LoggedAudio&& ) = delete;
		LoggedAudio& operator=( const LoggedAudio& ) = delete;
		LoggedAudio& operator=( LoggedAudio&& ) = delete;

		virtual void Create() override;
		virtual void Update() override;
		virtual void Draw() const override;

		virtual void Save( std::ostream& file )override;
		virtual void Load( std::istream& file )override;
#ifdef BALBINO_DEBUG
		virtual void DrawInpector() override;
#endif // BALBINO_DEBUG

		virtual void PlaySound( int soundID );
		virtual void StopSound( int soundID );
		virtual void StopAllSounds();

	private:
		void Log( const char* message );
	};
}