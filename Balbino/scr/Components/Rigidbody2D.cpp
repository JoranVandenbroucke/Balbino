#include "BalbinoPCH.h"
#include "Rigidbody2D.h"
#include "Collider2D.h"
#include "BoxCollider2D.h"
#include "Transform.h"
#include "../PhysicsWorld.h"
#include "../BinaryReaderWrider.h"
#include <Box2D.h>

Balbino::Rigidbody2D::Rigidbody2D( const GameObject* const origine )

	:Component{ origine }
	, m_IsKinematic{ false }
	, m_FreezRotation{ false }
	, m_UseGravity{ true }
	, m_AngularDrag{ 0.05f }
	, m_Drag{ 0.0f }
	, m_Hits{}
	, m_Mass{ 1.f }
	, m_Velocity{}
	, m_pBody{ nullptr }
{

}

void Balbino::Rigidbody2D::Create()
{
	if( m_Created ) return; this->Component::Create();
	Collider2D* pCollider = GetComponent<Collider2D>();
	if( !pCollider )
	{
		pCollider = AddComponent<BoxCollider2D>();
	}
	pCollider->Create();

	m_pBody = pCollider->m_pBody;
	m_pBody->SetType( m_IsKinematic ? b2BodyType::b2_kinematicBody : b2BodyType::b2_dynamicBody );
	auto fixture = m_pBody->GetFixtureList();
	fixture->SetDensity( m_Mass );
	fixture->SetFriction( 0.3f );
	fixture->SetUserData( this );
	m_pBody->ResetMassData();
	m_pBody->SetLinearDamping( m_Drag );
	m_pBody->SetAngularDamping( m_AngularDrag );
	m_pBody->SetGravityScale( m_UseGravity ? 1.f : 0.f );

	m_pBody->SetSleepingAllowed( true );
	m_pBody->SetAwake( true );
	m_pBody->SetFixedRotation( true );
	m_pBody->SetEnabled( true );
}

void Balbino::Rigidbody2D::FixedUpdate()
{
}

void Balbino::Rigidbody2D::Update()
{
}

void Balbino::Rigidbody2D::LateUpdate()
{
	auto velocity = m_pBody->GetLinearVelocity();
	m_Velocity.x = velocity.x;
	m_Velocity.y = velocity.y;
}

void Balbino::Rigidbody2D::Draw() const
{
}

void Balbino::Rigidbody2D::Save( std::ostream& file )
{
	(void) file;
	BinaryReadWrite::Write( file, int( ComponentList::Rigidbody2D ) );
	BinaryReadWrite::Write( file, m_IsKinematic );
	BinaryReadWrite::Write( file, m_FreezRotation );
	BinaryReadWrite::Write( file, m_UseGravity );
	BinaryReadWrite::Write( file, m_AngularDrag );
	BinaryReadWrite::Write( file, m_Drag );
	BinaryReadWrite::Write( file, m_Mass );
}

void Balbino::Rigidbody2D::Load( std::istream& file )
{
	(void) file;
	BinaryReadWrite::Read( file, m_IsKinematic );
	BinaryReadWrite::Read( file, m_FreezRotation );
	BinaryReadWrite::Read( file, m_UseGravity );
	BinaryReadWrite::Read( file, m_AngularDrag );
	BinaryReadWrite::Read( file, m_Drag );
	BinaryReadWrite::Read( file, m_Mass );
}

bool Balbino::Rigidbody2D::SweepTest( const Balbino::Vector2& direction, Balbino::RaycastHit& hitInfo, float maxDistance )
{
	(void) direction;
	(void) hitInfo;
	(void) maxDistance;
	return true;
}

Balbino::RaycastHit& Balbino::Rigidbody2D::SweepTestAll( const Balbino::Vector3& direction, int& hitInfoSize, float maxDistance )
{
	(void) direction;
	(void) hitInfoSize;
	(void) maxDistance;
	return m_Hits[0];
}

void Balbino::Rigidbody2D::AddForce( const Balbino::Vector2& force )
{
	m_pBody->ApplyForceToCenter( { force.x, force.y }, true );
}

void Balbino::Rigidbody2D::AddForceAtPosition( const Balbino::Vector2& force, const Balbino::Vector2& position )
{
	m_pBody->ApplyForce( { force.x, force.y }, { position.x, position.y }, true );
}

Balbino::Vector2 Balbino::Rigidbody2D::GetVelocity() const
{
	return m_Velocity;
}

void Balbino::Rigidbody2D::SetVelocity( Balbino::Vector2& velocity ) const
{
	m_pBody->SetLinearVelocity( { velocity.x, velocity.y } );
}

void Balbino::Rigidbody2D::UseGravity( bool useGravity )
{
	if( m_pBody )
	{
		m_pBody->SetGravityScale( useGravity ? 1.f : 0.f );
	}
	m_UseGravity = useGravity;
}

#ifdef BALBINO_DEBUG
void Balbino::Rigidbody2D::DrawInpector()
{
	ImGui::BeginChild( "RigidBody2D Component", ImVec2{ -1, 128 }, true );
	ImGui::Text( "RigidBody2D" );
	ImGui::EndChild();
}
#endif // BALBINO_DEBUG

