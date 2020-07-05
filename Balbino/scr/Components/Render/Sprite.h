#pragma once
#include "Texture2D.h"

namespace Balbino
{
	class Sprite final : public Texture2D
	{
	public:
		explicit Sprite( const GameObject* const origine );
		virtual ~Sprite();

		Sprite( const Sprite& ) = delete;
		Sprite( Sprite&& ) = delete;
		Sprite& operator=( const Sprite& ) = delete;
		Sprite& operator=( Sprite&& ) = delete;

		virtual void Create();
		virtual void Save( std::ostream& file )override;
		virtual void Load( std::istream& file )override;

		void SetSpriteIndex( int index );
		void LoadUV();

		int GetSpriteIndex()const{ return m_SpriteIndex; }
		const Vector2& GetPivot()const;
		const std::vector<Vector2>& uv()const;
		const float GetWidth()const;
		const float GetHeight()const;
#ifdef BALBINO_DEBUG
		virtual void DrawInpector() override;
#endif // BALBINO_DEBUG


	private:
		int m_SpriteIndex;
		Vector2 m_Pivot;
		std::vector<Vector2> m_UV{ {0,0}, {1,1} };
	};
}