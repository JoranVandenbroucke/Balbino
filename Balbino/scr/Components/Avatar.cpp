#include "BalbinoPCH.h"
#include "Avatar.h"
#include "Texture2D.h"
#include "../Renderer.h"
#include "../GameObject/GameObject.h"

Balbino::Avatar::Avatar( std::weak_ptr<GameObject> origine )
	:Component{ origine }
	, m_Texture{}
{
}

void Balbino::Avatar::Draw() const
{
	if( m_Texture )
	{
		Renderer::Get().RenderTexture( *m_Texture, 0, 0 );
	}
}

void Balbino::Avatar::Fire()
{
	m_Texture->SetTexture( "Fire.png" );
}

void Balbino::Avatar::Duck()
{
	m_Texture->SetTexture( "Duck.png" );
}

void Balbino::Avatar::Jump()
{
	m_Texture->SetTexture( "Jump.png" );
}

void Balbino::Avatar::Fart()
{
	m_Texture->SetTexture( "Fart.png" );
}