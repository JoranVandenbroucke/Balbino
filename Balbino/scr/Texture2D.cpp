#include "BalbinoPCH.h"
#include "Texture2D.h"
#include <SDL.h>
#include "SceneObject.h"
#include "Renderer.h"

Balbino::Texture2D::~Texture2D()
{
	SDL_DestroyTexture(m_Texture);
}

void Balbino::Texture2D::Draw() const
{
	Renderer::Get().RenderTexture( m_Texture, 0, 0 );
}
void Balbino::Texture2D::Draw(float x, float y) const
{
	Renderer::Get().RenderTexture( m_Texture, x, y );
}

SDL_Texture* Balbino::Texture2D::GetSDLTexture() const
{
	return m_Texture;
}

Balbino::Texture2D::Texture2D(SDL_Texture* texture)
{
	m_Texture = texture;
}