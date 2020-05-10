#include "BalbinoPCH.h"
#include "Rigidbody2D.h"
#include "Collider2D.h"
#include "Transform.h"
#include "../PhysicsWorld.h"

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
	, m_BodyDef{}
{
	m_BodyDef.type = b2_dynamicBody;

	m_BodyDef.linearDamping = 0.0f;
	m_BodyDef.angularDamping = 0.05f;
	m_BodyDef.gravityScale = ( m_UseGravity ? 1.f : 0.f );
	m_BodyDef.allowSleep = true;
	m_BodyDef.awake = true;
	m_BodyDef.fixedRotation = m_FreezRotation;
}

void Balbino::Rigidbody2D::Create()
{
	this->Component::Create();
	m_pBody = PhysicsWorld::AddBoddy( &m_BodyDef );
}

void Balbino::Rigidbody2D::FixedUpdate()
{
	auto positoin = m_pTransform->GetPosition();
	auto rotation = m_pTransform->GetRotation();
	m_BodyDef.position.Set( positoin.x, positoin.y );
	m_BodyDef.angle = rotation.z;
}

void Balbino::Rigidbody2D::Update()
{
}

void Balbino::Rigidbody2D::LateUpdate()
{

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
