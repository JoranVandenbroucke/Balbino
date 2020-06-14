#include "BalbinoPCH.h"
#include "BubbleManager.h"
#include "Rigidbody2D.h"
#include "Transform.h"
#include "Bubble.h"
#include "Sprite.h"
#include "CharacterController.h"
#include "Animation.h"
#include "../BinaryReaderWrider.h"
#include "../Time.h"
#include "../SceneManager.h"

Balbino::BubbleManager::BubbleManager( const GameObject* const origine )
	:Component{ origine }
	, m_NumObservers{}
	, m_OverSave{}
{
	for( int i = 0; i < MAX_OBSERVERS; i++ )
	{
		m_pObservers[0] = nullptr;
	}
}

void Balbino::BubbleManager::Create()
{
	if( m_Created ) return;
	this->Component::Create();
	m_pObservers[0] = DBG_NEW BubbleObserver{};
	++m_NumObservers;
	if( !m_OverSave )
	{
		for( int i = 0; i < MAX_BUBBLES; i++ )
		{
			GameObject* bubble = SceneManager::AddGameObjectToScene();
			m_pObservers[0]->OnNotify( bubble->AddComponent<Bubble>(), Event::NEW_BUBBLE );
			bubble->Create();
			bubble->AddComponent<Sprite>()->Create();
			bubble->SetName( (std::string( "Bubble " ) + std::to_string( i ) ).c_str() );
		}
	}
	else
	{
		for( int i = 0; i < MAX_BUBBLES; i++ )
		{
			GameObject* bubble = SceneManager::GetGameObjectByName( std::string( "Bubble " ) + std::to_string( i ) );
			if( bubble )
			{
				m_pObservers[0]->OnNotify( bubble->GetComponent<Bubble>(), Event::NEW_BUBBLE );
			}
		}
	}
}

void Balbino::BubbleManager::Update()
{
	for( int i = 0; i < MAX_OBSERVERS; i++ )
	{
		if( m_pObservers[i] )
		{
			m_pObservers[i]->Update();
		}
	}
}

void Balbino::BubbleManager::Draw() const
{
}

void Balbino::BubbleManager::Save( std::ostream& file )
{
	(void) file;
	BinaryReadWrite::Write( file, int( ComponentList::BubbleManager ) );
}

void Balbino::BubbleManager::Load( std::istream& file )
{
	(void) file;
	m_OverSave = true;
}

#ifdef _DEBUG
void Balbino::BubbleManager::DrawInpector()
{
	ImGui::BeginChild( "BubbleManager", ImVec2{ -1, 128 }, true );
	ImGui::Text( "Bubble Manager" );
	ImGui::EndChild();
}
#endif // _DEBUG


void Balbino::BubbleManager::Notify( const Component* player, Event event )
{
	for( int i = 0; i < m_NumObservers; i++ )
	{
		m_pObservers[i]->OnNotify( player, event );
	}
}

Balbino::BubbleManager::BubbleObserver::BubbleObserver()

	:Observer{}
	, m_BubBubbleDelay{ 0.f }
	, m_BobBubbleDelay{ 0.f }
	, m_NrOfBubbles{ 0 }
{
	for( int i = 0; i < MAX_BUBBLES; i++ )
	{
		m_pBubbles[i] = nullptr;
	}
};

void Balbino::BubbleManager::BubbleObserver::OnNotify( const Component* entity, Event event )
{
	switch( event )
	{
	case Balbino::Event::SHOOT_BUBBLE:
	{
		std::string name = entity->GetGameObject()->GetName();
		Balbino::Vector3 pos = entity->GetComponent<Transform>()->GetPosition();
		
		if( name == "Bub" )
		{
			if( m_BubBubbleDelay < 0 )
			{
				m_BubBubbleDelay = m_MaxBubbleDelay;
				for( int i = 0; i < MAX_BUBBLES; i++ )
				{
					if( !m_pBubbles[i]->IsAlive() )
					{
						m_pBubbles[i]->SetAlive();
						m_pBubbles[i]->SetPosition( pos );
						m_pBubbles[i]->SetDirection( ( (CharacterController*) entity )->GetDirection() );
						m_pBubbles[i]->GetComponent<Animation>()->SetAnimation( 0, "Animations/Bubble/GreenBubble.bani" );
						break;
					}
				}
			}
		}
		else
		{
			if( m_BobBubbleDelay < 0 )
			{
				m_BobBubbleDelay = m_MaxBubbleDelay;
				for( int i = 0; i < MAX_BUBBLES; i++ )
				{
					if( !m_pBubbles[i]->IsAlive() )
					{
						m_pBubbles[i]->SetAlive();
						m_pBubbles[i]->SetPosition( pos );
						m_pBubbles[i]->SetDirection( ( (CharacterController*) entity )->GetDirection() );
						m_pBubbles[i]->GetComponent<Animation>()->SetAnimation( 0, "Animations/Bubble/BlueBubble.bani" );
						break;
					}
				}
			}
		}
		break;
	}
	case Balbino::Event::HIT_BUBBLE:
	{
		std::string name = entity->GetGameObject()->GetName();
		if( name.find( "Enemy" ) )
		{

		}
		else
		{

		}
		break;
	}
	case Balbino::Event::HIT_ENEMY:
	{
		std::string name = entity->GetGameObject()->GetName();
		if( name == "Bub" )
		{
		}
		else
		{
		}
		break;
	}
	case Balbino::Event::HIT_ITEM:
	{
		std::string name = entity->GetGameObject()->GetName();
		if( name == "Bub" )
		{
		}
		else
		{
		}
		break;
	}
	case Balbino::Event::NEW_BUBBLE:
	{
		if( m_NrOfBubbles < MAX_BUBBLES )
		{
			m_pBubbles[m_NrOfBubbles++] = (Bubble*) entity;
		}
		break;
	}
	default:
		break;
	}
}

void Balbino::BubbleManager::BubbleObserver::Update()
{
	float dt = BTime::DeltaTime();
	if( m_BubBubbleDelay >= 0 )
		m_BubBubbleDelay -= dt;
	if( m_BobBubbleDelay >= 0 )
		m_BobBubbleDelay -= dt;
	std::sort( m_pBubbles, m_pBubbles + m_NrOfBubbles, []( Bubble* pB1, Bubble* pB2 )
	{
		return pB1->IsAlive() > pB2->IsAlive();
	} );
}
