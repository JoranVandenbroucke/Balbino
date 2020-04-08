#include "BalbinoPCH.h"
#include "Transform.h"
#include "../GameObject/GameObject.h"

Balbino::Transform::Transform( const std::weak_ptr<GameObject> origine )
	: Component{ origine }
	, m_Position{ 0.f, 0.f, 0.f }
{
}

Balbino::Transform::~Transform()
{
}

void Balbino::Transform::Create()
{
	this->Component::Create();
}

void Balbino::Transform::Update()
{
}

void Balbino::Transform::Draw() const
{
}
#ifdef _DEBUG
#include "../imgui-1.75/imgui.h"
void Balbino::Transform::DrawInpector() const
{
	ImGui::BeginChild( "Transform" );
	ImGui::PushItemWidth(80);
	ImGui::DragFloat("z: ", &m_Position.x);
	ImGui::DragFloat("y: ", &m_Position.x);
	ImGui::DragFloat("z: ", &m_Position.x);
	ImGui::PopItemWidth();
	ImGui::EndChild();
}
#endif
void Balbino::Transform::SetPosition( const float x, const float y, const float z )
{
	m_Position.x = x;
	m_Position.y = y;
	m_Position.z = z;
}