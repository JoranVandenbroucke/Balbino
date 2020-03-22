#include "pch.h"
#include "Players.h"
#include <memory>

void BubbleBobble::Bub::Create()
{
	m_Transform.SetPosition( 200.f, 200.f, 0.f );
}

void BubbleBobble::Bub::Update()
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
		command.lock()->Execute( m_Avatar );
	}
}

void BubbleBobble::Bub::Draw() const
{
	m_Avatar.Draw();
}
