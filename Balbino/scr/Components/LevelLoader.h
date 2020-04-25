#pragma once
#include "Component.h"
namespace Balbino
{
	class LevelLoader :	public Component
	{
	public:
		LevelLoader( std::weak_ptr<GameObject> origine );

		LevelLoader( const LevelLoader& ) = delete;
		LevelLoader( LevelLoader&& ) = delete;
		LevelLoader& operator=( const LevelLoader& ) = delete;
		LevelLoader& operator=( LevelLoader&& ) = delete;

		~LevelLoader() = default;

		virtual void Create() override;
		virtual void Update() override;
		virtual void Draw() const override;

		virtual void Save( std::ostream & file )override;
		virtual void Load( std::istream & file )override;

		void SetLevelNr( int levelNr );
	private:
		int m_LevelNr;
	};
}