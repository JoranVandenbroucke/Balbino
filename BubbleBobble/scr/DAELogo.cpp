#include "pch.h"
#include "DAELogo.h"

BubbleBobble::DAELogo::DAELogo()
	:SceneObject{}
	, m_Texture{}
{
}

void BubbleBobble::DAELogo::Create()
{
	SetTexture( "logo.png" );
	SetPosition( 216, 180 );
}

void BubbleBobble::DAELogo::Update()
{
}

void BubbleBobble::DAELogo::Draw() const
{
	const auto pos = m_Transform.GetPosition();
	m_Texture->Draw(pos.x,pos.y);
	//Renderer::GetInstance().RenderTexture( *m_Texture, pos.x, pos.y );
}

void BubbleBobble::DAELogo::SetTexture( const std::string& filename )
{
	m_Texture = Balbino::ResourceManager::Get().LoadTexture( filename );
}

void BubbleBobble::DAELogo::SetPosition( float x, float y )
{
	m_Transform.SetPosition( x, y, 0.0f );
}
