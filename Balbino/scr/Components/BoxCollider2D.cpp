#include "BalbinoPCH.h"
#include "BoxCollider2D.h"
#include "Transform.h"
#include "Rigidbody2D.h"
#include "../GameObject/GameObject.h"
#include "../Math.h"

#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>

Balbino::BoxCollider::BoxCollider( const Balbino::GameObject* const origine )
	: Collider2D{ origine }
	, m_Center{}
	, m_Size{}
{
}

void Balbino::BoxCollider::Create()
{
	this->Component::Create();
	m_Collider.SetAsBox( m_Size.x, m_Size.y, { m_Center.x, m_Center.y }, m_pTransform->GetRotation().z );
}

void Balbino::BoxCollider::Update()
{
}

void Balbino::BoxCollider::Draw() const
{
}

void Balbino::BoxCollider::Save( std::ostream& file )
{
	(void) file;
}

void Balbino::BoxCollider::Load( std::istream& file )
{
	(void) file;
}

void Balbino::BoxCollider::DrawInpector()
{
}

const Balbino::Vector3& Balbino::BoxCollider::GetCenter() const
{
	return m_Center;
}

const Balbino::Vector3& Balbino::BoxCollider::GetSize() const
{
	return m_Size;
}

void Balbino::BoxCollider::SetCenter( const Balbino::Vector3& newCenter )
{
	m_Center = newCenter;
	m_Collider.SetAsBox( m_Size.x, m_Size.y, { m_Center.x, m_Center.y }, m_pTransform->GetRotation().z );
}

void Balbino::BoxCollider::SetSize( const Balbino::Vector3& newSize )
{
	m_Size = newSize;
	m_Collider.SetAsBox( m_Size.x, m_Size.y, { m_Center.x, m_Center.y }, m_pTransform->GetRotation().z );
}

//https://web.archive.org/web/20141121091114/http://www.cs.utah.edu/~awilliam/box/box.pdf
//https://noonat.github.io/intersect/
// Optimizedmethod
bool Balbino::BoxCollider::RayCast( const Balbino::Ray& ray, Balbino::RaycastHit& hitInfo, float size ) const
{
	b2Transform transform;
	transform.SetIdentity();

	b2RayCastInput input;
	input.p1.Set( ray.origine.x, ray.origine.y );
	input.p2.Set( ray.origine.x + ray.direction.x * size, ray.origine.y + ray.direction.y * size );
	input.maxFraction = 1.0f;
	int32 childIndex = 0;

	b2RayCastOutput output;
	bool hit = m_Collider.RayCast( &output, input, transform, childIndex );

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
