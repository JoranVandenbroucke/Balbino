#include "MiniginPCH.h"
#include <SDL.h>
#include <SDL_ttf.h>

#include "TextObject.h"
#include "Renderer.h"
#include "Font.h"
#include "Texture2D.h"

Balbino::TextObject::TextObject( const std::string& text, const std::shared_ptr<Font>& font )
	: m_NeedsUpdate( true ), m_Text( text ), m_Font( font ), m_Texture( nullptr )
	, m_Color{255,255,255}
{
}

void Balbino::TextObject::Create()
{
	m_Color = Color{ 0x000000 };
	m_NeedsUpdate = true;
}

void Balbino::TextObject::Update()
{
	if( m_NeedsUpdate )
	{
		const SDL_Color color = { m_Color.r,m_Color.g,m_Color.b }; // only white text is supported now
		const auto surf = TTF_RenderText_Blended( m_Font->GetFont(), m_Text.c_str(), color );
		if( surf == nullptr )
		{
			throw std::runtime_error( std::string( "Render text failed: " ) + SDL_GetError() );
		}
		auto texture = SDL_CreateTextureFromSurface( Renderer::GetInstance().GetSDLRenderer(), surf );
		if( texture == nullptr )
		{
			throw std::runtime_error( std::string( "Create text texture from surface failed: " ) + SDL_GetError() );
		}
		SDL_FreeSurface( surf );
		m_Texture = std::make_shared<Texture2D>( texture );
		m_NeedsUpdate = false;
	}
}

void Balbino::TextObject::Draw() const
{
	if( m_Texture != nullptr )
	{
		const auto pos = m_Transform.GetPosition();
		Renderer::GetInstance().RenderTexture( *m_Texture, pos.x, pos.y );
	}
}

// This implementation uses the "dirty flag" pattern
void Balbino::TextObject::SetText( const std::string& text )
{
	m_Text = text;
	m_NeedsUpdate = true;
}

void Balbino::TextObject::SetColor( const Color& newColor )
{
	m_Color = newColor;
	m_NeedsUpdate = true;
}

void Balbino::TextObject::SetPosition( const float x, const float y )
{
	m_Transform.SetPosition( x, y, 0.0f );
}


