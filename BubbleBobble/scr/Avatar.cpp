#include "pch.h"
#include "Avatar.h"
#include "InputManager.h"
#include "ResourceManager.h"
#include "Renderer.h"

void Balbino::Avatar::Create()
{
	m_Transform.SetPosition( 200.f, 200.f, 0.f );
}

void Balbino::Avatar::Update()
{
	//if( InputManager::IsPressed( Balbino::ControllerButton::ButtonA ) )
	//{
	//	Fire();
	//}
	//else if( InputManager::IsPressed( Balbino::ControllerButton::ButtonB ) )
	//{
	//	Duck();
	//}
	//else if( InputManager::IsPressed( Balbino::ControllerButton::ButtonX ) )
	//{
	//	Jump();
	//}
	//else if( InputManager::IsPressed( Balbino::ControllerButton::ButtonY ) )
	//{
	//	Fart();
	//}

	std::weak_ptr<Balbino::Command> command{ Balbino::InputManager::IsPressed() };
	if( command.lock() )
	{
		command.lock()->Execute( *this );
	}
}

void Balbino::Avatar::Draw() const
{
	if( m_Texture )
	{
		glm::vec3 pos{ m_Transform.GetPosition() };
		Renderer::GetInstance().RenderTexture( *m_Texture, pos.x, pos.y );
	}
}
