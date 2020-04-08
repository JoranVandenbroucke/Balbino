#include "BalbinoPCH.h"
#include "Texture2D.h"
#include "Transform.h"
#include "../GameObject/GameObject.h"
#include "../Renderer.h"
#include "../ResourceManager.h"
#include <SDL.h>

Balbino::Texture2D::Texture2D( const std::weak_ptr<GameObject> origine )
	: Component{ origine }
	, m_Texture{ nullptr }
{
}

Balbino::Texture2D::~Texture2D()
{
	SDL_DestroyTexture( m_Texture );
}

void Balbino::Texture2D::SetTexture( const std::string path )
{
	m_Texture = ResourceManager::LoadTexture( path );
}

void Balbino::Texture2D::SetTexture( SDL_Texture* texture )
{
	if( m_Texture )
	{
		SDL_DestroyTexture( m_Texture );
	}
	m_Texture = texture;
}

void Balbino::Texture2D::Create()
{
	this->Component::Create();
}

void Balbino::Texture2D::Update()
{
}

void Balbino::Texture2D::Draw() const
{
	vec3 pos = m_Transform.lock()->GetPosition();
	Renderer::Get().RenderTexture( m_Texture, pos.x, pos.y );
}

void Balbino::Texture2D::DrawInpector() const
{
}

SDL_Texture* Balbino::Texture2D::GetSDLTexture() const
{
	return m_Texture;
}