#include "BalbinoPCH.h"
#include "CharacterController.h"
#include "Rigidbody2D.h"
#include "Animation.h"
#include "BubbleManager.h"
#include "Transform.h"
#include "../BinaryReaderWrider.h"
#include "../PhysicsWorld.h"
#include "../SceneManager.h"
#include <regex>

Balbino::CharacterController::CharacterController( const GameObject* const origine )
	:Component{ origine }
	, m_CurrentDevice{ "keyboard" }
	, m_pRigidbody{ nullptr }
	, m_WantsToDropDown{}
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
	m_pOrigin->SetTag( "Player" );

	GameObject* pBubbleManagerObject = SceneManager::GetGameObjectByName( "Bubble Manager" );
	if( pBubbleManagerObject )
	{
		m_pManager = pBubbleManagerObject->GetComponent<BubbleManager>();
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
}

void Balbino::CharacterController::Update()
{
	if( !m_pManager )
	{
		GameObject* pBubbleManagerObject = SceneManager::GetGameObjectByName( "Bubble Manager" );
		if( pBubbleManagerObject )
		{
			m_pManager = pBubbleManagerObject->GetComponent<BubbleManager>();
		}
		return;
	}
	m_WantsToDropDown = false;
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

#ifdef BALBINO_DEBUG
void Balbino::CharacterController::DrawInpector()
{
	const auto allDevices = InputManager::GetAllInputDevices();
	ImGui::BeginChild( "CharacterController", ImVec2{ -1, 128 }, true );
	ImGui::Text( "Character Controller" );
	ImGui::Checkbox( "Is Dead", &m_IsDead );
	if( ImGui::BeginCombo( "CurrentDevice", m_CurrentDevice.c_str() ) )
	{
		for( const std::string& device : allDevices )
		{
			bool isSelected = device == m_CurrentDevice;
			if( ImGui::Selectable( device.c_str() ) )
			{
				std::regex numberRemover{ R"(^\d: (.+)$)" };
				std::smatch match;
				if( std::regex_match( device, match, numberRemover ) )
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
#endif // BALBINO_DEBUG


void Balbino::CharacterController::OnCollisionEnter( GameObject* pGameObject )
{
	std::string tag = pGameObject->GetTag();
	if( tag == "Enemy" )
	{
		if( m_pManager)
		{
			m_pManager->Notify( this, Event::HIT_ENEMY );
		}
	}
	else if( tag == "Item" )
	{
		if( m_pManager )
		{
			m_pManager->Notify( this, Event::HIT_ITEM );
		}
	}
	else
	{
		if( m_pManager )
		{
			m_pManager->Notify( this, Event::HIT_BUBBLE );
		}
	}
}

void Balbino::CharacterController::SetInput( const char* const device )
{
	m_CurrentDevice = device;
}

bool Balbino::CharacterController::WantsToDropDown() const
{
	return m_WantsToDropDown;
}

void Balbino::CharacterController::GoLeft()
{
	std::cout << "Go Left\n";
	//m_pRigidbody->AddForce( Balbino::Vector2{-512,0} );
	Balbino::Vector2 velocity = m_pRigidbody->GetVelocity();
	velocity.x = -8.f;
	m_pRigidbody->SetVelocity( velocity );
	m_Direction = Direction::LEFT;
}

void Balbino::CharacterController::GoRight()
{
	std::cout << "Go Right\n";
	//m_pRigidbody->AddForce( Balbino::Vector2{ 512,0 } );
	Balbino::Vector2 velocity = m_pRigidbody->GetVelocity();
	velocity.x = 8.f;
	m_pRigidbody->SetVelocity( velocity );
	m_pAnimation->SetFloat( "VelocityX", velocity.x );
	m_Direction = Direction::RIGHT;
}

void Balbino::CharacterController::Jump()
{
	std::cout << "Jump\n";
	Balbino::RaycastHit hitinfo;
	if( PhysicsWorld::Raycast2D( m_pTransform->GetPosition(), Balbino::Vector2{ 0.f,-1.f }, hitinfo, 2.f ) )
	{
		m_pRigidbody->AddForce( Balbino::Vector2{ 0, 256 } );
	}
}

void Balbino::CharacterController::Fall()
{
	m_WantsToDropDown = true;
}

void Balbino::CharacterController::Shoot()
{
	if( m_pManager )
	{
		m_pManager->Notify( this, Event::SHOOT_BUBBLE );
	}
}

void Balbino::CharacterController::SetDead( bool isDead )
{
	m_IsDead = isDead;
}

bool Balbino::CharacterController::GetDeadState()
{
	return m_IsDead;
}

Balbino::Direction Balbino::CharacterController::GetDirection()
{
	return m_Direction;
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