#include "MiniginPCH.h"
#include "Renderer.h"
#include <SDL.h>
#include "SceneManager.h"
#include "Texture2D.h"

void Balbino::Renderer::Init(SDL_Window * window)
{
	m_Renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (m_Renderer == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateRenderer Error: ") + SDL_GetError());
	}
}

void Balbino::Renderer::Draw() const
{
	SDL_RenderClear(m_Renderer);

	SceneManager::GetInstance().Draw();
	
	SDL_RenderPresent(m_Renderer);
}

void Balbino::Renderer::Destroy()
{
	if (m_Renderer != nullptr)
	{
		SDL_DestroyRenderer(m_Renderer);
		m_Renderer = nullptr;
	}
}

void Balbino::Renderer::RenderTexture(const Texture2D& texture, const float x, const float y) const
{
	SDL_Rect dst;
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	SDL_QueryTexture(texture.GetSDLTexture(), nullptr, nullptr, &dst.w, &dst.h);
	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
}

void Balbino::Renderer::RenderTexture(const Texture2D& texture, const float x, const float y, const float width, const float height) const
{
	SDL_Rect dst;
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	dst.w = static_cast<int>(width);
	dst.h = static_cast<int>(height);
	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
}

void Balbino::Renderer::RenderTexture( SDL_Texture* texture, float x, float y ) const
{
	SDL_Rect dst;
	dst.x = static_cast< int >( x );
	dst.y = static_cast< int >( y );
	SDL_QueryTexture( texture, nullptr, nullptr, &dst.w, &dst.h );
	SDL_RenderCopy( GetSDLRenderer(), texture, nullptr, &dst );
}

void Balbino::Renderer::RenderTexture( SDL_Texture* texture, float x, float y, float width, float height ) const
{
	SDL_Rect dst;
	dst.x = static_cast< int >( x );
	dst.y = static_cast< int >( y );
	dst.w = static_cast< int >( width );
	dst.h = static_cast< int >( height );
	SDL_QueryTexture( texture, nullptr, nullptr, &dst.w, &dst.h );
	SDL_RenderCopy( GetSDLRenderer(), texture, nullptr, &dst );
}

//void Balbino::Renderer::RenderTexture( const Texture2D& texture, const Color& overlay, float x, float y, float width, float height ) const
//{
//}
//
//void Balbino::Renderer::RenderTexture( const Texture2D& texture, const Color& overlay, float x, float y ) const
//{
//}
//
//void Balbino::Renderer::RenderTexture( const Color& color, const Texture2D& alphaMap, float x, float y ) const
//{
//}
//
//void Balbino::Renderer::RenderTexture( const Color& color, const Texture2D& alphaMap, float x, float y, float width, float height ) const
//{
//}
