#include "BalbinoPCH.h"
#include "BoxCollider2D.h"
#include "Transform.h"
#include "Rigidbody2D.h"
#include "../GameObject/GameObject.h"
#include "../Math.h"
#include "Sprite.h"
#include "../PhysicsWorld.h"
#include "../Editor/Debug.h"
#include "../BinaryReaderWrider.h"
#include <Box2D.h>
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>

Balbino::BoxCollider2D::BoxCollider2D( const Balbino::GameObject* const origine )
	: Collider2D{ origine }
	, m_Center{}
	, m_Size{ 1.f, 1.f }
	, m_BodyDef{}
	, m_Colliser{}
	, m_pFixture{ nullptr }
	, m_IsTrigger{}
{
}

Balbino::BoxCollider2D::~BoxCollider2D()
{
	m_pFixture->SetUserData( nullptr );
	PhysicsWorld::RemoveBody( m_pBody );
}

void Balbino::BoxCollider2D::Create()
{
	if( m_Created ) return;
	this->Component::Create();
	Reset();
}

void Balbino::BoxCollider2D::FixedUpdate()
{
	auto positoin = m_pTransform->GetPosition();
	auto rotation = m_pTransform->GetRotation();
	auto scale = m_pTransform->GetScale();
	if( m_OldTransformPosition != positoin || m_OldTransformRotation != rotation )
	{
		m_pBody->SetTransform( { positoin.x / m_ppm, positoin.y / m_ppm }, rotation.z );
		m_pBody->SetLinearVelocity( { 0, 0 } );
		m_OldTransformPosition = positoin;
		m_OldTransformRotation = rotation;
		m_pBody->SetAwake( true );
	}
	if( m_OldTransformScale != scale )
	{
		m_Colliser.SetAsBox( m_Size.x * scale.x, m_Size.y * scale.y, { m_Center.x, m_Center.y }, rotation.z );
		m_OldTransformScale = scale;
		m_pBody->SetAwake( true );
	}
}

void Balbino::BoxCollider2D::Update()
{
}

void Balbino::BoxCollider2D::LateUpdate()
{
	auto positoin = m_pTransform->GetPosition();
	auto rotation = m_pTransform->GetRotation();
	//std::cout << m_pBody->GetLinearVelocity().x << '\t' << m_pBody->GetLinearVelocity().y << '\t' << std::boolalpha << m_pBody->IsAwake() << '\n';
	if( m_OldTransformPosition == positoin )
	{
		auto bodyPos = m_pBody->GetPosition();
		float bodyAngle = m_pBody->GetAngle();
		m_pTransform->SetPosition( bodyPos.x * m_ppm, bodyPos.y * m_ppm, positoin.z );
		m_pTransform->SetRotation( rotation.x, rotation.y, bodyAngle );
		m_OldTransformPosition = m_pTransform->GetPosition();
	}
}

void Balbino::BoxCollider2D::Draw() const
{
	auto positoin = m_pTransform->GetPosition();
	auto scale = m_pTransform->GetScale();
	const Vector3 point[4] =
	{
		{positoin.x + m_Center.x - m_Size.x * m_ppm * scale.x,-positoin.y + m_Center.y - m_Size.y * m_ppm * scale.y, 0.f},
		{positoin.x + m_Center.x - m_Size.x * m_ppm * scale.x,-positoin.y + m_Center.y + m_Size.y * m_ppm * scale.y, 0.f},
		{positoin.x + m_Center.x + m_Size.x * m_ppm * scale.x,-positoin.y + m_Center.y + m_Size.y * m_ppm * scale.y, 0.f},
		{positoin.x + m_Center.x + m_Size.x * m_ppm * scale.x,-positoin.y + m_Center.y - m_Size.y * m_ppm * scale.y, 0.f}
	};
	Debug::DrawLine( point[0], point[1], Color{ 0xC7EA46 } );
	Debug::DrawLine( point[1], point[2], Color{ 0xC7EA46 } );
	Debug::DrawLine( point[2], point[3], Color{ 0xC7EA46 } );
	Debug::DrawLine( point[3], point[0], Color{ 0xC7EA46 } );
}

void Balbino::BoxCollider2D::Save( std::ostream& file )
{
	(void) file;
	BinaryReadWrite::Write( file, int( ComponentList::BoxCollider2D ) );
	BinaryReadWrite::Write( file, m_Center );
	BinaryReadWrite::Write( file, m_Size );
	BinaryReadWrite::Write( file, m_IsTrigger );
}

void Balbino::BoxCollider2D::Load( std::istream& file )
{
	(void) file;
	BinaryReadWrite::Read( file, m_Center );
	BinaryReadWrite::Read( file, m_Size );
	BinaryReadWrite::Read( file, m_IsTrigger );
}

#ifdef BALBINO_DEBUG
void Balbino::BoxCollider2D::DrawInpector()
{
	ImGui::BeginChild( "BoxCollider2D", ImVec2{ -1, 128 }, true );
	ImGui::Text( "Box Collider 2D" );
	float size[3]{ m_Size.x, m_Size.y, m_Size.z }, center[3]{ m_Center.x , m_Center.y, m_Center.z };
	bool chaingedTrigger = ImGui::Checkbox( "isTrigger", &m_IsTrigger );
	bool chaingedCenter = ImGui::DragFloat3( "Center", center );
	bool chaingedSize = ImGui::DragFloat3( "Size", size );

	if( chaingedCenter )
		SetCenter( Balbino::Vector3{ center[0], center[1], center[2] } );
	if( chaingedSize )
		SetSize( Balbino::Vector3{ size[0], size[1], size[2] } );
	if( chaingedTrigger )
		SetTrigger( m_IsTrigger );
	ImGui::EndChild();
}
#endif //BALBINO_DEBUG

const Balbino::Vector3& Balbino::BoxCollider2D::GetCenter() const
{
	return m_Center;
}

const Balbino::Vector3& Balbino::BoxCollider2D::GetSize() const
{
	return m_Size;
}

void Balbino::BoxCollider2D::SetCenter( const Balbino::Vector3& newCenter )
{
	m_Center = newCenter;
	auto fixture = m_pBody->GetFixtureList();
	auto colider = fixture->GetShape();
	( (b2PolygonShape*) colider )->SetAsBox( m_Size.x, m_Size.y, { m_Center.x, m_Center.y }, m_pTransform->GetRotation().z );
}

void Balbino::BoxCollider2D::SetSize( const Balbino::Vector3& newSize )
{
	m_Size = newSize;
	auto fixture = m_pBody->GetFixtureList();
	auto colider = fixture->GetShape();
	( (b2PolygonShape*) colider )->SetAsBox( m_Size.x, m_Size.y, { m_Center.x, m_Center.y }, m_pTransform->GetRotation().z );
}

void Balbino::BoxCollider2D::SetTrigger( bool isTrigger )
{
	m_IsTrigger = isTrigger;
	m_pFixture->SetSensor( isTrigger );
}

void Balbino::BoxCollider2D::Reset()
{
	m_Size = { 1.f, 1.f, 1.f };
	auto sprite = GetComponent<Sprite>();
	if( sprite )
		m_Size = { sprite->GetWidth() / ( m_ppm * 2.f), sprite->GetHeight() / ( m_ppm * 2.f ) };
	if( m_Size.x < 0.001f )
	{
		m_Size.x = 1.f;
	}
	if( m_Size.y < 0.001f )
	{
		m_Size.y = 1.f;
	}
	m_Colliser.SetAsBox( m_Size.x, m_Size.y, { m_Center.x, m_Center.y }, m_pTransform->GetRotation().z );

	m_BodyDef.type = b2_staticBody;
	if( m_pBody )
	{
		PhysicsWorld::RemoveBody( m_pBody );
	}
	m_pBody = PhysicsWorld::AddBody( &m_BodyDef );

	m_pFixture = m_pBody->CreateFixture( &m_Colliser, 0 );
	m_pFixture->SetSensor( m_IsTrigger );
	m_pFixture->SetUserData( this );
}

//https://web.archive.org/web/20141121091114/http://www.cs.utah.edu/~awilliam/box/box.pdf
//https://noonat.github.io/intersect/
// Optimizedmethod
bool Balbino::BoxCollider2D::RayCast( const Balbino::Ray& ray, Balbino::RaycastHit& hitInfo, float size ) const
{
	b2Transform transform;
	transform.SetIdentity();

	b2RayCastInput input;
	input.p1.Set( ray.origine.x, ray.origine.y );
	input.p2.Set( ray.origine.x + ray.direction.x * size, ray.origine.y + ray.direction.y * size );
	input.maxFraction = 1.0f;
	int32 childIndex = 0;

	b2RayCastOutput output;
	auto fixture = m_pBody->GetFixtureList();
	auto colider = fixture->GetShape();
	bool hit = colider->RayCast( &output, input, transform, childIndex );

	if( hit )
	{
		hitInfo.collider = this;
		b2Vec2 hitPoint = input.p1 + output.fraction * ( input.p2 - input.p1 );
		hitInfo.distance = input.p2.Length() - input.p1.Length();
		hitInfo.point = { hitPoint.x, hitPoint.y };
		hitInfo.rigidbody = GetComponent<Rigidbody2D>();
		hitInfo.transform = m_pTransform;
	}
	return hit;
}