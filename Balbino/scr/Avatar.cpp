#include "BalbinoPCH.h"
#include "Avatar.h"
#include "Texture2D.h"
#include "Renderer.h"
#include "ResourceManager.h"

void Balbino::Avatar::Draw() const
{
	if( m_Texture )
	{
		Renderer::GetInstance().RenderTexture( *m_Texture, 0, 0 );
	}
}

void Balbino::Avatar::Fire()
{
	m_Texture = ResourceManager::GetInstance().LoadTexture( "Fire.png" );
}

void Balbino::Avatar::Duck()
{
	m_Texture = ResourceManager::GetInstance().LoadTexture( "Duck.png" );
}

void Balbino::Avatar::Jump()
{
	m_Texture = ResourceManager::GetInstance().LoadTexture( "Jump.png" );
}

void Balbino::Avatar::Fart()
{
	m_Texture = ResourceManager::GetInstance().LoadTexture( "Fart.png" );
}