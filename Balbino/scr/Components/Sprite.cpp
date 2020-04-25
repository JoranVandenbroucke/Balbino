#include "BalbinoPCH.h"
#include "Sprite.h"

Balbino::Sprite::Sprite( std::weak_ptr<GameObject> origine )
	: Component{ origine }
	, m_Border{ 0,0,0,0 }
	, m_Verteces{ { 1, 1 }, { 1, 0 }, { 0, 0 }, { 0, 1 } }
	, m_Offset{ 0, 0 }
	, m_TextureRect{ -1 , -1, 1, 1 }
	, m_Texture{ origine }
	, m_UV{ { 1, 1 }, { 1, 0 }, { 0, 0 }, { 0, 1 } }
	, m_Rect{}
{
}

const Balbino::Vector4& Balbino::Sprite::GetBorder() const
{
	return m_Border;
}

const Balbino::Vector2* Balbino::Sprite::GetVertices( int& size ) const
{
	size = 4;
	return m_Verteces;
}

const Balbino::Vector2& Balbino::Sprite::GetTextureRectOffset() const
{
	return m_Offset;
}

const Balbino::Rect& Balbino::Sprite::GetTextureRect() const
{
	return m_TextureRect;
}

const Balbino::Vector2& Balbino::Sprite::GetPivot() const
{
	return m_Pivot;
}

const Balbino::Texture2D& Balbino::Sprite::texture() const
{
	return m_Texture;
}

const Balbino::Vector2* Balbino::Sprite::uv( int& size ) const
{
	size = 4;
	return m_UV;
}

const Balbino::Rect& Balbino::Sprite::GetRect() const
{
	return m_Rect;
}
//
//Sprite& Balbino::Sprite::Create( const Texture2D& texture, const Rect& rect, const Vector2& pivot )
//{
//	return Sprite();
//}

void Balbino::Sprite::Create()
{
}

void Balbino::Sprite::Update()
{
}

void Balbino::Sprite::Draw() const
{
}

void Balbino::Sprite::Save( std::ostream& file )
{
	(void) file;
}

void Balbino::Sprite::Load( std::istream& file )
{
	(void) file;
}

#ifdef _DEBUG
void Balbino::Sprite::DrawInpector()
{
}
#endif // _DEBUG

