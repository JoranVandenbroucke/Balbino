#include "BalbinoPCH.h"
#include "Text.h"
#include "GameObject.h"
#include "Renderer.h"
#include "Font.h"
#include "Texture2D.h"
#include "ResourceManager.h"

#include <SDL.h>
#include <SDL_ttf.h>

Balbino::Text::Text( const std::weak_ptr<GameObject> origin )
	: Component{ origin }
	, m_Text{ "Text" }
	, m_Font{ Balbino::ResourceManager::Get().LoadFont( "Lingua.otf", 12 ) }
	, m_Texture{ nullptr }
	, m_Color{ 255, 255, 255, 255 }
	, m_NeedsUpdate{ true }
{
}

void Balbino::Text::Create()
{
	this->Component::Create();
}

void Balbino::Text::Update()
{
	if( m_NeedsUpdate )
	{
		const SDL_Color color = { m_Color.r, m_Color.g, m_Color.b }; // only white text is supported now
		const auto surf = TTF_RenderText_Blended( m_Font->GetFont(), m_Text.c_str(), color );
		if( surf == nullptr )
		{
			throw std::runtime_error( std::string( "Render text failed: " ) + SDL_GetError() );
		}
		auto texture = SDL_CreateTextureFromSurface( Renderer::Get().GetSDLRenderer(), surf );
		if( texture == nullptr )
		{
			throw std::runtime_error( std::string( "Create text texture from surface failed: " ) + SDL_GetError() );
		}
		SDL_FreeSurface( surf );
		m_Texture->SetTexture( texture );
	}
}

void Balbino::Text::Draw() const
{
	if( m_Texture != nullptr )
	{
		const auto pos = m_Transform.lock()->GetPosition();
		Renderer::Get().RenderTexture( *m_Texture, pos.x, pos.y );
	}
}

// This implementation uses the "dirty flag" pattern
void Balbino::Text::SetText( const std::string& text )
{
	m_Text = text;
	m_NeedsUpdate = true;
}

void Balbino::Text::SetColor( const Color& newColor )
{
	m_Color = newColor;
	m_NeedsUpdate = true;
}

void Balbino::Text::SetPosition( const float x, const float y )
{
	m_Transform.lock()->SetPosition( x, y, 0.0f );
}

void Balbino::Text::SetFont( const std::string path, int size )
{
	m_Font = ResourceManager::LoadFont( path, size );
}
