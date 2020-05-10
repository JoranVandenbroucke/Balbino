#include "BalbinoPCH.h"
#include "PhysicsWorld.h"
#include "Time.h"

Balbino::PhysicsWorld::PhysicsWorld()
	:m_World{ { 0.f, 9.80665f } }
{
	m_World.SetAllowSleeping( true );
	m_World.SetWarmStarting( true );
	m_World.SetContinuousPhysics( true );
	m_World.SetSubStepping( false );
}

void Balbino::PhysicsWorld::Initialize()
{
	Get().IUpdate();
}

void Balbino::PhysicsWorld::IUpdate()
{
	while( BTime::FixedTime() + BTime::FixedDeltaTime() > BTime::Time()  )
	{
		m_World.Step( BTime::FixedDeltaTime(), 8, 3 );
		BTime::Get().UpdateFT();
	}
}

Balbino::PhysicsWorld::~PhysicsWorld()
{
}

void Balbino::PhysicsWorld::Update()
{
	Get().IUpdate();
}

b2Body* Balbino::PhysicsWorld::AddBoddy( const b2BodyDef* body )
{
	return Get().m_World.CreateBody( body );
}

b2World& Balbino::PhysicsWorld::GetWorld()
{
	return Get().m_World;
}
