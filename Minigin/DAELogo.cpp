#include "MiniginPCH.h"
#include "DAELogo.h"
#include "ResourceManager.h"
#include "Renderer.h"

void Balbino::DAELogo::Create()
{
	SetTexture( "logo.png" );
	SetPosition( 216, 180 );
}

void Balbino::DAELogo::Update()
{
}

void Balbino::DAELogo::Draw() const
{
	const auto pos = m_Transform.GetPosition();
	Renderer::GetInstance().RenderTexture( *m_Texture, pos.x, pos.y );
}

Balbino::DAELogo::DAELogo()
	:SceneObject{}
	, m_Texture{}
{
}

void Balbino::DAELogo::SetTexture( const std::string& filename )
{
	m_Texture = ResourceManager::GetInstance().LoadTexture( filename );
}

void Balbino::DAELogo::SetPosition( float x, float y )
{
	m_Transform.SetPosition( x, y, 0.0f );
}
