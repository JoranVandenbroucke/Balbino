#include "BalbinoPCH.h"
#include "Bubble.h"
#include "Rigidbody2D.h"
#include "Animation.h"
#include "Transform.h"
#include "BubbleManager.h"
#include "../SceneManager.h"
#include "../BinaryReaderWrider.h"
#include "../Time.h"

Balbino::Bubble::Bubble( const GameObject* const pOrigine )
	:Component{ pOrigine }
{
}

void Balbino::Bubble::Create()
{
	m_pAnimation = GetComponent<Animation>();
	m_pRigidBody = GetComponent<Rigidbody2D>();
	if( !m_pAnimation )
	{
		m_pAnimation = AddComponent<Animation>();
		m_pAnimation->Create();
	}
	if( !m_pRigidBody )
	{
		m_pRigidBody = AddComponent<Rigidbody2D>();
		m_pRigidBody->Create();
	}
	m_pRigidBody->UseGravity( false );
	GameObject* pBubbleManagerObject = SceneManager::GetGameObjectByName( "Bubble Manager" );
	if( pBubbleManagerObject )
	{
		m_pManager = pBubbleManagerObject->GetComponent<BubbleManager>();
	}
}

void Balbino::Bubble::Update()
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
	float dt = BTime::DeltaTime();
	if( m_LifeTime >= 0.f )
	{
		m_LifeTime -= dt;
		if( m_LifeTime < .5f )
		{
			if( m_pRigidBody )
			{
				Balbino::Vector2 velocity = m_pRigidBody->GetVelocity();
				velocity.x = 0;
				velocity.y = 10;
				m_pRigidBody->SetVelocity( velocity );
			}
		}
	}
}

void Balbino::Bubble::Draw() const
{
}

void Balbino::Bubble::Save( std::ostream& file )
{
	(void) file;
	BinaryReadWrite::Write( file, int( ComponentList::Bubble ) );
}

void Balbino::Bubble::Load( std::istream& file )
{
	(void) file;
}

#ifdef BALBINO_DEBUG
void Balbino::Bubble::DrawInpector()
{
	ImGui::BeginChild( "Bubble", ImVec2{ -1, 128 }, true );
	ImGui::Text( "Bubbler" );
	ImGui::EndChild();
}
#endif // _DEBUG

void Balbino::Bubble::SetPosition( const Balbino::Vector2& positon )
{
	if( !m_pTransform )
	{
		m_pTransform = GetComponent<Transform>();
	}
	m_pTransform->SetPosition( positon.x, positon.y, 0.f );
}

void Balbino::Bubble::SetAlive()
{
	m_LifeTime = 2.f;
	m_pOrigin->SetActive( true );
}

bool Balbino::Bubble::IsAlive()
{
	if( m_LifeTime < 0.f )
	{
		m_pOrigin->SetActive( false );
		return false;
	}
	return true;
}

void Balbino::Bubble::SetDirection( Direction direction )
{
	if( direction == Direction::LEFT )
		m_pRigidBody->AddForce( { -2048, 0 } );
	else
		m_pRigidBody->AddForce( { 2048,0 } );
}

void Balbino::Bubble::OnTriggerEnter( GameObject* pGameObject )
{
	if( pGameObject->GetTag() == "Enemy" )
	{
		if( m_pManager )
		{
			m_pManager->Notify( this, Event::HIT_BUBBLE );
		}
	}
}
