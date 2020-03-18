#include "MiniginPCH.h"
#include "Avatar.h"
#include "InputManager.h"
#include "ResourceManager.h"
#include "Renderer.h"

void Balbino::Avatar::Create()
{
}

void Balbino::Avatar::Update()
{
	if( InputManager::IsPressed( Balbino::ControllerButton::ButtonA ) )
	{
		Fire();
	}
	else if( InputManager::IsPressed( Balbino::ControllerButton::ButtonB ) )
	{
		Duck();
	}
	else if( InputManager::IsPressed( Balbino::ControllerButton::ButtonX ) )
	{
		Jump();
	}
	else if( InputManager::IsPressed( Balbino::ControllerButton::ButtonY ) )
	{
		Fart();
	}
}

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
