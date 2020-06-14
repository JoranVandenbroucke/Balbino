#include "BalbinoPCH.h"
#include "PhysicsWorld.h"
#include "Time.h"
#include "SceneManager.h"
#include "Components/Collider2D.h"
#include "Components/Rigidbody2D.h"
#include "Components/Transform.h"
#include "Components/CharacterController.h"
#include "Components/Enemy.h"
#include "GameObject/GameObject.h"

Balbino::PhysicsWorld::PhysicsWorld()
	:m_World{ { 0.f, -9.80665f } }
	, m_CollisionCallback{}
{
	m_World.SetAllowSleeping( true );
	m_World.SetWarmStarting( true );
	m_World.SetContinuousPhysics( true );
	m_World.SetSubStepping( false );
	m_World.SetContactListener( &m_CollisionCallback );
}

void Balbino::PhysicsWorld::Initialize()
{
	Get().IUpdate();
}

void Balbino::PhysicsWorld::IUpdate()
{
	if( BTime::TimeScale() && BTime::FixedTime() + BTime::FixedDeltaTime() < BTime::Time() )
	{
		SceneManager::Get().FixedUpdate();
		while( BTime::FixedTime() + BTime::FixedDeltaTime() < BTime::Time() )
		{
			m_World.Step( BTime::FixedDeltaTime(), 8, 3 );
			BTime::Get().UpdateFT();
		}
	}
}

Balbino::PhysicsWorld::~PhysicsWorld()
{
}

void Balbino::PhysicsWorld::Update()
{
	Get().IUpdate();
}

b2Body* Balbino::PhysicsWorld::AddBody( const b2BodyDef* body )
{
	return Get().m_World.CreateBody( body );
}

b2World& Balbino::PhysicsWorld::GetWorld()
{
	return Get().m_World;
}

void Balbino::PhysicsWorld::RemoveBody( b2Body* body )
{
	Get().m_World.DestroyBody( body );
}

bool Balbino::PhysicsWorld::Raycast2D( const Balbino::Vector2& origine, const Balbino::Vector2& direction, Balbino::RaycastHit& hitInfo, float maxDistance )
{
	RaysCastCallback callback{};
	Get().m_World.RayCast( &callback, { origine.x / 8.f, origine.y / 8.f }, { origine.x / 8.f + direction.x * maxDistance, origine.y / 8.f + direction.y * maxDistance } );
	if( !callback.GetFixture() )
	{
		return false;
	}
	b2Fixture* fixture = callback.GetFixture();
	Collider2D* colliderComponent = (Collider2D*) fixture->GetUserData();

	hitInfo.collider = colliderComponent;
	hitInfo.distance = callback.GetFraction();
	hitInfo.point = Balbino::Vector3{ callback.GetPoint().x , callback.GetPoint().y, 0.f };
	hitInfo.rigidbody = colliderComponent->GetComponent<Rigidbody2D>();
	hitInfo.transform = colliderComponent->GetComponent<Transform>();

	return ( hitInfo.distance >= 0.f );
}

void Balbino::PhysicsWorld::CollisionCallback::BeginContact( b2Contact* contact )
{
	b2Fixture* fix1 = contact->GetFixtureA();
	b2Fixture* fix2 = contact->GetFixtureB();
	GameObject* gameObject1 = ( (Collider2D*) fix1->GetUserData() )->GetGameObject();
	GameObject* gameObject2 = ( (Collider2D*) fix2->GetUserData() )->GetGameObject();
	if( fix1->IsSensor() || fix2->IsSensor() )
	{
		gameObject1->OnTriggerEnter( gameObject2 );
		gameObject2->OnTriggerEnter( gameObject1 );
	}
	else
	{
		gameObject1->OnCollisionEnter( gameObject2 );
		gameObject2->OnCollisionEnter( gameObject1 );

	}
}

void Balbino::PhysicsWorld::CollisionCallback::EndContact( b2Contact* contact )
{
	contact->SetEnabled( true );
	b2Fixture* fix1 = contact->GetFixtureA();
	b2Fixture* fix2 = contact->GetFixtureB();
	Collider2D* collider1 = (Collider2D*) fix1->GetUserData();
	Collider2D* collider2 = (Collider2D*) fix2->GetUserData();
	GameObject* gameObject1 = nullptr;
	GameObject* gameObject2 = nullptr;
	if( collider1 )
		gameObject1 = collider1->GetGameObject();
	if( collider2 )
		gameObject2 = collider2->GetGameObject();

	if( fix1->IsSensor() || fix2->IsSensor() )
	{
		if( gameObject1 && gameObject2 )
		{
			gameObject1->OnTriggerExit( gameObject2 );
			gameObject2->OnTriggerExit( gameObject1 );
		}
	}
	else
	{
		if( gameObject1 && gameObject2 )
		{
			gameObject1->OnCollisionExit( gameObject2 );
			gameObject2->OnCollisionExit( gameObject1 );
		}
	}
}

//based on https://www.iforce2d.net/b2dtut/one-way-walls
void Balbino::PhysicsWorld::CollisionCallback::PreSolve( b2Contact* contact, const b2Manifold* oldManifold )
{
	(void*) oldManifold;
	b2Fixture* pFixtureA = contact->GetFixtureA();
	b2Fixture* pFixtureB = contact->GetFixtureB();
	GameObject* pGameObject1 = ( (Collider2D*) pFixtureA->GetUserData() )->GetGameObject();
	GameObject* pGameObject2 = ( (Collider2D*) pFixtureB->GetUserData() )->GetGameObject();

	//check if one of the fixtures is the platform
	GameObject* pCharacterObject = nullptr;
	b2Fixture* pPlatformFixture = nullptr;
	b2Fixture* pOtherFixture = nullptr;
	if( pGameObject1->GetTag() == "Platform" )
	{
		pPlatformFixture = pFixtureA;
		pOtherFixture = pFixtureB;
		pCharacterObject = pGameObject2;
	}
	else if( pGameObject2->GetTag() == "Platform" )
	{
		pPlatformFixture = pFixtureB;
		pOtherFixture = pFixtureA;
		pCharacterObject = pGameObject1;
	}

	if( !pPlatformFixture )
		return;

	b2Body* pPlatformBody = pPlatformFixture->GetBody();
	b2Body* pOtherBody = pOtherFixture->GetBody();

	int numPoints = contact->GetManifold()->pointCount;
	b2WorldManifold worldManifold;
	contact->GetWorldManifold( &worldManifold );

	//check if contact points are moving into platform
	for( int i = 0; i < numPoints; i++ )
	{

		b2Vec2 pointVelPlatform =
			pPlatformBody->GetLinearVelocityFromWorldPoint( worldManifold.points[i] );
		b2Vec2 pointVelOther =
			pOtherBody->GetLinearVelocityFromWorldPoint( worldManifold.points[i] );
		b2Vec2 relativeVel = pPlatformBody->GetLocalVector( pointVelOther - pointVelPlatform );
		if( pCharacterObject->GetTag() == "Player" )
		{
			auto component = pCharacterObject->GetComponent<CharacterController>();
			if( component && !component->WantsToDropDown() )
			{
				if( relativeVel.y < -1 ) //if moving down faster than 1 m/s, handle as before
				{
					return;//point is moving into platform, leave contact solid and exit
				}
				else if( relativeVel.y < 1 )
				{
					//if moving slower than 1 m/s
					//borderline case, moving only slightly out of platform
					b2Vec2 relativePoint = pPlatformBody->GetLocalPoint( worldManifold.points[i] );
					float platformFaceY = 0.5f;//front of platform, from fixture definition :(
					if( relativePoint.y > platformFaceY - 0.05 )
						return;//contact point is less than 5cm inside front face of platfrom
				}
				else
					;
			}
		}
		//else if( pCharacterObject->GetTag() == "Enemy" )
		//{
		//	auto component = pCharacterObject->GetComponent<Enemy>();
		//	if( component && component->WantsToDropDown() )
		//	{
		//		if( relativeVel.y > 1 ) //if moving down faster than 1 m/s, handle as before
		//		{
		//			return;//point is moving into platform, leave contact solid and exit
		//		}
		//		else if( relativeVel.y > 1 )
		//		{
		//			//if moving slower than 1 m/s
		//			//borderline case, moving only slightly out of platform
		//			b2Vec2 relativePoint = pPlatformBody->GetLocalPoint( worldManifold.points[i] );
		//			float platformFaceY = 0.5f;//front of platform, from fixture definition :(
		//			if( relativePoint.y > platformFaceY - 0.05 )
		//				return;//contact point is less than 5cm inside front face of platfrom
		//		}
		//	}
		//}
		//else
		//if( relativeVel.y < -1 ) //if moving down faster than 1 m/s, handle as before
		//{
		//	return;//point is moving into platform, leave contact solid and exit
		//}
		//else if( relativeVel.y < 1 )
		//{
		//	//if moving slower than 1 m/s
		//	//borderline case, moving only slightly out of platform
		//	b2Vec2 relativePoint = pPlatformBody->GetLocalPoint( worldManifold.points[i] );
		//	float platformFaceY = 0.5f;//front of platform, from fixture definition :(
		//	if( relativePoint.y > platformFaceY - 0.05 )
		//		return;//contact point is less than 5cm inside front face of platfrom
		//}
		//else
		//	;//moving up faster than 1 m/s
	}

	//no points are moving into platform, contact should not be solid
	contact->SetEnabled( false );
}
