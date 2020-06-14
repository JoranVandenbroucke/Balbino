#include "BalbinoPCH.h"
#include "Enemy.h"
#include "Rigidbody2D.h"
#include "Animation.h"
#include "Transform.h"
#include "Sprite.h"
#include "../BinaryReaderWrider.h"
#include "../PhysicsWorld.h"
#include "../Time.h"
//#include "../imgui-1.75/imgui.cpp"

Balbino::Enemy::Enemy( const GameObject* const origin )
	:Component{ origin }
	, m_Action{}
	, m_SpawnDleay{ 2.f }
	, m_ChaingeMovetentTimer{}
	, m_Generator( m_RandomDevice() )
	, m_Destribution( 0, 3 )
	, m_pCurrentState{ nullptr }
	, m_EnemyType{ EnemyType::ZenChan }
{
}

void Balbino::Enemy::Create()
{
	this->Component::Create();
	m_pRigidBody = GetComponent<Rigidbody2D>();
	m_pAnimation = GetComponent<Animation>();
	if( !m_pRigidBody )
	{
		m_pRigidBody = AddComponent<Rigidbody2D>();
		m_pRigidBody->Create();
	}
	if( !m_pAnimation )
	{
		m_pAnimation = AddComponent<Animation>();
		m_pAnimation->Create();
	}
	m_pCurrentState = new IdleState{};
	switch( m_EnemyType )
	{
	case Balbino::Enemy::EnemyType::ZenChan:
	case Balbino::Enemy::EnemyType::Hidegons:
	case Balbino::Enemy::EnemyType::Banebou:
	case Balbino::Enemy::EnemyType::Pulpul:
	{
		m_pAnimation->SetAnimation( 0, "Animations/ZenChan/ZenChanLeft.bani" );
		m_pAnimation->SetAnimation( 0, "Animations/ZenChan/ZenChanRight.bani" );
		break;
	}
	case Balbino::Enemy::EnemyType::Maita:
	case Balbino::Enemy::EnemyType::Monsta:
	case Balbino::Enemy::EnemyType::Drunk:
	case Balbino::Enemy::EnemyType::Invader:
	{
		m_pAnimation->SetAnimation( 0, "Animations/Maita/MaitaLeft.bani" );
		m_pAnimation->SetAnimation( 0, "Animations/Maita/MaitaRight.bani" );
		break;
	}
	break;
	default:
		break;

	}
}

void Balbino::Enemy::Update()
{
	float dt = BTime::DeltaTime();
	m_SpawnDleay -= dt;
	m_pAnimation->eneable = true;
	m_ChaingeMovetentTimer += dt;
	State* state = m_pCurrentState->Update( *this );
	m_pOrigin->SetTag( "Enemy" );
	if( state )
	{
		delete m_pCurrentState;
		m_pCurrentState = state;
	}
}

void Balbino::Enemy::Draw() const
{
}

void Balbino::Enemy::Save( std::ostream& file )
{
	(void) file;
	BinaryReadWrite::Write( file, ComponentList::Enemy );
}

void Balbino::Enemy::Load( std::istream& file )
{
	(void) file;
}

#ifdef BALBINO_DEBUG
void Balbino::Enemy::DrawInpector()
{
	ImGui::BeginChild( "Enemy Component", ImVec2{ -1, 128 }, true );
	ImGui::Text( "Enemy Component" );
	//ImGui::
	ImGui::EndChild();
}
#endif // BALBINO_DEBUG
void Balbino::Enemy::SetPositon( unsigned char collum, unsigned char row )
{
	if( !m_pTransform )
	{
		m_pTransform = GetComponent<Transform>();
	}
	m_pTransform->SetPosition( collum * 8.f, ( 24 - row ) * 8.f, 0.f );
}
void Balbino::Enemy::SetAnimationOffset( unsigned char offset )
{
	GetComponent<Sprite>()->SetSpriteIndex( int( offset ) );
}
void Balbino::Enemy::SetSpawnDelay( float delay )
{
	m_SpawnDleay = delay;
}

bool Balbino::Enemy::WantsToDropDown() const
{
	return m_WantToGoDawn;
}

void Balbino::Enemy::SetType( EnemyType type )
{
	m_EnemyType = type;
	if( m_Created )
	{
		switch( type )
		{
		case Balbino::Enemy::EnemyType::ZenChan:
		case Balbino::Enemy::EnemyType::Hidegons:
		case Balbino::Enemy::EnemyType::Banebou:
		case Balbino::Enemy::EnemyType::Pulpul:
		{
			m_pAnimation->SetAnimation( 0, "Animations/ZenChan/ZenChanLeft.bani" );
			m_pAnimation->SetAnimation( 0, "Animations/ZenChan/ZenChanRight.bani" );
			break;
		}
		case Balbino::Enemy::EnemyType::Maita:
		case Balbino::Enemy::EnemyType::Monsta:
		case Balbino::Enemy::EnemyType::Drunk:
		case Balbino::Enemy::EnemyType::Invader:
		{
			m_pAnimation->SetAnimation( 0, "Animations/Maita/MaitaLeft.bani" );
			m_pAnimation->SetAnimation( 0, "Animations/Maita/MaitaRight.bani" );
			break;
		}
		break;
		default:
			break;

		}
	}
}

Balbino::Enemy::State* Balbino::Enemy::IdleState::Update( Enemy& enemy )
{
	if( enemy.m_SpawnDleay < 0 )
	{
		int direction = enemy.m_Destribution( enemy.m_Generator );
		if( direction == 0 )
		{
			enemy.m_ChaingeMovetentTimer = 0.f;
			return new LeftState{};
		}
		else if( direction == 1 )
		{
			enemy.m_ChaingeMovetentTimer = 0.f;
			return new RightState{};
		}
		else if( direction == 2 )
		{
			enemy.m_ChaingeMovetentTimer = 0.f;
			return new JumpState{};
		}
	}
	return nullptr;
}

Balbino::Enemy::State* Balbino::Enemy::LeftState::Update( Enemy& enemy )
{
	Balbino::Vector2 velocity = enemy.m_pRigidBody->GetVelocity();
	if( velocity.y > 0.01f )
		new InAirState{};
	velocity.x = -10.f;
	enemy.m_pRigidBody->SetVelocity( velocity );
	if( enemy.m_ChaingeMovetentTimer > enemy.m_MaxMovementTime )
	{
		int direction = enemy.m_Destribution( enemy.m_Generator );
		if( direction % 2 == 0 )
		{
			enemy.m_ChaingeMovetentTimer = 0.f;
			return new RightState{};
		}
		else if( direction % 2 == 1 )
		{
			enemy.m_ChaingeMovetentTimer = 0.f;
			return new JumpState{};
		}
	}
	return nullptr;
}

Balbino::Enemy::State* Balbino::Enemy::RightState::Update( Enemy& enemy )
{
	Balbino::Vector2 velocity = enemy.m_pRigidBody->GetVelocity();
	if( velocity.y > 0.01f )
		new InAirState{};
	velocity.x = 10.f;
	enemy.m_pRigidBody->SetVelocity( velocity );
	if( enemy.m_ChaingeMovetentTimer > enemy.m_MaxMovementTime )
	{
		int direction = enemy.m_Destribution( enemy.m_Generator );
		if( direction % 2 == 0 )
		{
			enemy.m_ChaingeMovetentTimer = 0.f;
			return new LeftState{};
		}
		else if( direction % 2 == 1 )
		{
			enemy.m_ChaingeMovetentTimer = 0.f;
			return new JumpState{};
		}
	}
	return nullptr;
}

Balbino::Enemy::State* Balbino::Enemy::InAirState::Update( Enemy& enemy )
{
	Balbino::RaycastHit hitinfo;
	if( PhysicsWorld::Raycast2D( enemy.m_pTransform->GetPosition(), Balbino::Vector2{ 0.f,-1.f }, hitinfo, 2.f ) )
	{
		enemy.m_ChaingeMovetentTimer = 0.f;
		int direction = enemy.m_Destribution( enemy.m_Generator );
		if( direction % 2 == 0 )
		{
			enemy.m_WantToGoDawn = false;
			return new LeftState{};
		}
		else if( direction % 2 == 1 )
		{
			enemy.m_WantToGoDawn = false;
			return new RightState{};
		}
	}
	return nullptr;
}

Balbino::Enemy::State* Balbino::Enemy::JumpState::Update( Enemy& enemy )
{
	Balbino::RaycastHit hitinfo;
	if( PhysicsWorld::Raycast2D( enemy.m_pTransform->GetPosition(), Balbino::Vector2{ 0.f,-1.f }, hitinfo, 2.f ) )
	{
		enemy.m_pRigidBody->AddForce( Balbino::Vector2{ 0,1024 } );
		return new InAirState{};
	}
	return nullptr;
}

Balbino::Enemy::State* Balbino::Enemy::GoDownState::Update( Enemy& enemy )
{
	enemy.m_WantToGoDawn = true;
	return new InAirState{};
}
