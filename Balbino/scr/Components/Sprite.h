#pragma once
#include "Component.h"
#include "Texture2D.h"

namespace Balbino
{
	class Sprite : public Component
	{
	public:
		Sprite( std::weak_ptr<GameObject> origine );

		Sprite( const Sprite& ) = delete;
		Sprite( Sprite&& ) = delete;
		Sprite& operator=( const Sprite& ) = delete;
		Sprite& operator=( Sprite&& ) = delete;

		~Sprite() = default;

		virtual void Create() override;
		virtual void Update() override;
		virtual void Draw() const override;

		virtual void Save( std::ostream& file )override;
		virtual void Load( std::istream& file )override;
#ifdef _DEBUG
		virtual void DrawInpector() override;
#endif // _DEBUG

		const Vector4& GetBorder()const;
		const Vector2* GetVertices( int& size )const;
		const Vector2& GetTextureRectOffset()const;
		const Rect& GetTextureRect()const;
		const Vector2& GetPivot()const;
		const Texture2D& texture()const;
		const Vector2* uv( int& size )const;
		const Rect& GetRect()const;
		//static Sprite Create( const Texture2D& texture, const Rect& rect, const Vector2& pivot );
	private:
		Vector4 m_Border;
		Vector2 m_Verteces[4];
		Vector2 m_Offset;
		Rect m_TextureRect;
		Vector2 m_Pivot;
		Texture2D m_Texture;
		Vector2 m_UV[4];
		Rect m_Rect;
	};
}