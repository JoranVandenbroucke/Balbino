#include "BalbinoPCH.h"
#include "Rigidbody2D.h"
#include "Collider2D.h"
#include "BoxCollider2D.h"
#include "Transform.h"
#include "../PhysicsWorld.h"
#include <Box2D.h>

Balbino::Rigidbody2D::Rigidbody2D( const GameObject* const origine )

	:Component{ origine }
	, m_IsKinematic{ false }
	, m_FreezRotation{ false }
	, m_UseGravity{ true }
	, m_AngularDrag{}
	, m_MaxAngularVelocity{ 7 }
	, m_AngularVelocity{}
	, m_CenterOfMass{}
	, m_Velocity{}
	, m_Hits{}
{

}

void Balbino::Rigidbody2D::Create()
{
	this->Component::Create();
	Collider2D* pCollider = GetComponent<Collider2D>();
	if( !pCollider )
	{
		pCollider = AddComponent<BoxCollider2D>();
		pCollider->Create();
	}

	m_pBody =  pCollider->m_pBody;
	m_pBody->SetType( b2BodyType::b2_dynamicBody );
	auto fixture = m_pBody->GetFixtureList();
	fixture->SetDensity( 1.f );
	fixture->SetFriction( 0.3f );
	fixture->SetUserData( this );
	m_pBody->ApplyForce( { 5.5f,1.f }, { 5.5f,5.f }, true );
	m_pBody->ResetMassData();
	m_pBody->SetLinearDamping( 0.0f );
	m_pBody->SetAngularDamping( 0.01f );
	m_pBody->SetGravityScale( 1.f );

	m_pBody->SetSleepingAllowed( true );
	m_pBody->SetAwake( true );
	m_pBody->SetFixedRotation( true );
	m_pBody->SetEnabled( true );
}

void Balbino::Rigidbody2D::FixedUpdate()
{
	auto positoin = m_pTransform->GetPosition();
	auto rotation = m_pTransform->GetRotation();
	if( m_OldTransformPosition != positoin )
	{
		m_pBody->SetTransform( { positoin.x / m_ppm, positoin.y / m_ppm }, rotation.z );
		m_pBody->SetLinearVelocity( { 0, 0 } );
		m_OldTransformPosition = positoin;
	}
}

void Balbino::Rigidbody2D::Update()
{
}

void Balbino::Rigidbody2D::LateUpdate()
{
	auto positoin = m_pTransform->GetPosition();
	auto rotation = m_pTransform->GetRotation();
	std::cout << m_pBody->GetLinearVelocity().x << '\t' << m_pBody->GetLinearVelocity().y << '\t' << std::boolalpha << m_pBody->IsAwake()<< '\n';
	if( m_OldTransformPosition == positoin )
	{
		auto bodyPos = m_pBody->GetPosition();
		float bodyAngle = m_pBody->GetAngle();
		m_pTransform->SetPosition( bodyPos.x * m_ppm, bodyPos.y * m_ppm, positoin.z );
		m_pTransform->SetRotation( rotation.x, rotation.y, bodyAngle );
		m_OldTransformPosition = m_pTransform->GetPosition();
	}
}

void Balbino::Rigidbody2D::Draw() const
{
}

void Balbino::Rigidbody2D::Save( std::ostream& file )
{
	(void) file;
}

void Balbino::Rigidbody2D::Load( std::istream& file )
{
	(void) file;
}

bool Balbino::Rigidbody2D::SweepTest( const Balbino::Vector3& direction, Balbino::RaycastHit& hitInfo, float maxDistance )
{
	hitInfo.point.x = m_CenterOfMass.x + direction.x * maxDistance;
	hitInfo.point.y = m_CenterOfMass.y + direction.y * maxDistance;

	return true;
}

Balbino::RaycastHit& Balbino::Rigidbody2D::SweepTestAll( const Balbino::Vector3& direction, int& hitInfoSize, float maxDistance )
{
	(void) direction;
	(void) hitInfoSize;
	(void) maxDistance;
	return m_Hits[0];
}

void Balbino::Rigidbody2D::DrawInpector()
{
	ImGui::BeginChild( "RigidBody2D Component", ImVec2{ -1, 128 }, true );
	ImGui::Text( "RigidBody2D" );
	ImGui::EndChild();
}
