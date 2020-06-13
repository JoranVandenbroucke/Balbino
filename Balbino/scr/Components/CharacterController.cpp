#include "BalbinoPCH.h"
#include "CharacterController.h"
#include "Rigidbody2D.h"
#include "Animation.h"
#include "../BinaryReaderWrider.h"
#include <regex>

Balbino::CharacterController::CharacterController( const GameObject* const origine )
	:Component{ origine }
	, m_CurrentDevice{ "keyboard" }
	, m_pRigidbody{ nullptr }
{
}

void Balbino::CharacterController::Create()
{
	if( m_Created ) return; this->Component::Create();
	m_pRigidbody = GetComponent<Rigidbody2D>();
	m_pAnimation = GetComponent<Animation>();
	if( !m_pRigidbody )
	{
		m_pRigidbody = AddComponent<Rigidbody2D>();
		m_pRigidbody->Create();
	}
	if( !m_pAnimation )
	{
		m_pAnimation = AddComponent<Animation>();
		m_pAnimation->Create();
	}

	//Command
	InputManager::AddButton( "left", new GoLeftCommand );
	InputManager::AddButton( "right", new GoRightCommand );
	InputManager::AddButton( "jump", new JumpCommand );
	InputManager::AddButton( "fall", new FallCommand );
	InputManager::AddButton( "shoot", new ShootCommand );
}

void Balbino::CharacterController::FixedUpdate()
{
	auto command = InputManager::IsPressed( m_CurrentDevice );
	if( command )
	{
		command->Execute( this );
	}
	if( m_pRigidbody->GetVelocity().y < 0.001f )
	{
		m_IsJumping = false;
	}
}

void Balbino::CharacterController::Update()
{
	Balbino::Vector2 velocity = m_pRigidbody->GetVelocity();
	m_pAnimation->SetFloat( "VelocityX", velocity.x );
}

void Balbino::CharacterController::Draw() const
{
}

void Balbino::CharacterController::Save( std::ostream& file )
{
	(void) file;
	BinaryReadWrite::Write( file, int( ComponentList::Avatar ) );
	BinaryReadWrite::Write( file, m_CurrentDevice );
}

void Balbino::CharacterController::Load( std::istream& file )
{
	(void) file;
	BinaryReadWrite::Read( file, m_CurrentDevice );
}

void Balbino::CharacterController::DrawInpector()
{
	const auto allDevices = InputManager::GetAllInputDevices();
	ImGui::BeginChild( "CharacterController", ImVec2{ -1, 128 }, true );
	ImGui::Text( "Character Controller" );
	if( ImGui::BeginCombo( "CurrentDevice", m_CurrentDevice.c_str() ) )
	{
		for( const std::string& device : allDevices )
		{
			bool isSelected = device == m_CurrentDevice;
			if( ImGui::Selectable( device.c_str() ) )
			{
				std::regex numberRemover{ R"(^\d: (.+)$)" };
				std::smatch match;
				if( std::regex_match( device, match, numberRemover ))
				{
					m_CurrentDevice = match[1].str();
				}
			}
			if( isSelected )
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}
	ImGui::EndChild();
}

void Balbino::CharacterController::SetInput( const char* const device )
{
	m_CurrentDevice = device;
}

void Balbino::CharacterController::GoLeft()
{
	std::cout << "Go Left\n";
	//m_pRigidbody->AddForce( Balbino::Vector2{-512,0} );
	Balbino::Vector2 velocity = m_pRigidbody->GetVelocity();
	velocity.x = -10.f;
	m_pRigidbody->SetVelocity( velocity );
}

void Balbino::CharacterController::GoRight()
{
	std::cout << "Go Right\n";
	//m_pRigidbody->AddForce( Balbino::Vector2{ 512,0 } );
	Balbino::Vector2 velocity = m_pRigidbody->GetVelocity();
	velocity.x = 10.f;
	m_pRigidbody->SetVelocity( velocity );
	m_pAnimation->SetFloat( "VelocityX", velocity.x );
}

void Balbino::CharacterController::LookLeft()
{
	std::cout << "Look Left\n";
}

void Balbino::CharacterController::LookRight()
{
	std::cout << "Look Right\n";
}

void Balbino::CharacterController::Jump()
{
	std::cout << "Jump\n";
	if( !m_IsJumping )
	{
		m_pRigidbody->AddForce( Balbino::Vector2{ 0, 1024 } );
		m_IsJumping = true;
	}
}

void Balbino::CharacterController::Fall()
{
	std::cout << "Fall\n";
}

void Balbino::CharacterController::Shoot()
{
	std::cout << "Shoot\n";
}

void Balbino::JumpCommand::Execute( CharacterController* actor )
{
	actor->Jump();
}
void Balbino::GoLeftCommand::Execute( CharacterController* actor )
{
	actor->GoLeft();
}
void Balbino::GoRightCommand::Execute( CharacterController* actor )
{
	actor->GoRight();
}
void Balbino::FallCommand::Execute( CharacterController* actor )
{
	actor->Fall();
}
void Balbino::ShootCommand::Execute( CharacterController* actor )
{
	actor->Shoot();
}