#pragma once
#include "Component.h"
namespace Balbino
{
	class LevelLoader :	public Component
	{
	public:
		explicit LevelLoader( const GameObject* const origine );
		virtual ~LevelLoader() = default;

		LevelLoader( const LevelLoader& ) = delete;
		LevelLoader( LevelLoader&& ) = delete;
		LevelLoader& operator=( const LevelLoader& ) = delete;
		LevelLoader& operator=( LevelLoader&& ) = delete;

		virtual void Create() override;
		virtual void Update() override;
		virtual void Draw() const override;

		virtual void Save( std::ostream & file )override;
		virtual void Load( std::istream & file )override;

		void SetLevelNr( int levelNr );

#ifdef _DEBUG
		virtual void DrawInpector() override;
#endif // _DEBUG

	private:
		int m_LevelNr;
		bool m_OverSave;
	};
}