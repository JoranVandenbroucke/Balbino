#pragma once
#include "Component.h"
#include "../Core.h"

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
		std::vector<std::weak_ptr<Mix_Chunk>> m_pMixChunks;
	};

#pragma warning(push)
#pragma warning(disable:4251)
#pragma warning(disable:4275)
	class BALBINO_API ConsoleAudio: public Audio, public Component
	{
	public:
		ConsoleAudio( const std::weak_ptr<GameObject> origine );

		virtual void Create() override;
		virtual void Update() override;
		virtual void Draw() const override;
#ifdef _DEBUG
		virtual void DrawInpector() override;
#endif // _DEBUG

		virtual void PlaySound( int soundID );
		virtual void StopSound( int soundID );
		virtual void StopAllSounds();
	};

	class BALBINO_API LoggedAudio: public Audio, public Component
	{
	public:
		LoggedAudio( const std::weak_ptr<GameObject> origine );

		virtual void Create() override;
		virtual void Update() override;
		virtual void Draw() const override;
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
#pragma warning(pop)