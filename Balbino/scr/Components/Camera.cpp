#include "BalbinoPCH.h"
#include "Camera.h"
#include "../GameObject/GameObject.h"

Balbino::Camera::Camera( const std::weak_ptr<GameObject> origine, float fov, float w, float h )
	:Component( origine )
	, m_Projection{ glm::perspective( fov / 2.f, w / h, 0.1f, 100.f ) }
	, m_View{ glm::mat4{ 1.f } }
	, m_ViewProjection{}
{
	Update();
}
const glm::mat4& Balbino::Camera::GetViewProjection() const
{
	return m_ViewProjection;
}

void Balbino::Camera::Create()
{
	this->Component::Create();
}

void Balbino::Camera::Update()
{
	m_ViewProjection = m_View * m_Projection;
}

void Balbino::Camera::Draw() const
{
}

void Balbino::Camera::Translate( const vec3& v )
{
	m_View = glm::translate( m_View, glm::vec3{ v.x, v.y, v.z }*-1.f );
}
#ifdef _DEBUG
void Balbino::Camera::DrawInpector()
{
}
#endif // _DEBUG

