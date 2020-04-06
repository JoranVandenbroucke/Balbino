#include "pch.h"
#include "Players.h"
#include <memory>

void BubbleBobble::Bub::Create()
{
	std::shared_ptr<Balbino::Transform> transform = AddComponent<Balbino::Transform>();
	transform->SetPosition( 200.f, 200.f, 0.f );

	std::shared_ptr<Balbino::Avatar> avatar = AddComponent<Balbino::Avatar>();

	std::shared_ptr<Balbino::ConsoleAudio> consAud = AddComponent<Balbino::ConsoleAudio>();

	std::shared_ptr<Balbino::LoggedAudio> logAud = AddComponent<Balbino::LoggedAudio>();

	LoadComponents();
}

//void BubbleBobble::Bub::Update()
//{
//	//if( InputManager::IsPressed( Balbino::ControllerButton::ButtonA ) )
//	//{
//	//	Fire();
//	//}
//	//else if( InputManager::IsPressed( Balbino::ControllerButton::ButtonB ) )
//	//{
//	//	Duck();
//	//}
//	//else if( InputManager::IsPressed( Balbino::ControllerButton::ButtonX ) )
//	//{
//	//	Jump();
//	//}
//	//else if( InputManager::IsPressed( Balbino::ControllerButton::ButtonY ) )
//	//{
//	//	Fart();
//	//}
//
//	std::weak_ptr<Balbino::Command> command{ Balbino::InputManager::IsPressed() };
//	if( command.lock() )
//	{
//		command.lock()->Execute( m_Avatar );
//	}
//}
//
//void BubbleBobble::Bub::Draw() const
//{
//	m_Avatar.Draw();
//}
