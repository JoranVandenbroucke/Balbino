#include "BalbinoPCH.h"
#include "Transform.h"
#include "../GameObject/GameObject.h"
#include "../BinaryReaderWrider.h"

Balbino::Transform::Transform( const std::weak_ptr<GameObject> origine )
	:Component{ origine }
	, m_Position{ 0.f, 0.f, 0.f }
	, m_Rotation{ 0.f, 0.f, 0.f }
	, m_Scale{ 1.f, 1.f, 1.f }
	, m_Parent{}
	, m_Childeren{}
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
	glm::mat4x4 translate{ {1.f, 0.f, 0.f, m_Position.x}, {0.f, 1.f, 0.f, m_Position.y}, {0.f, 0.f, 1.f, m_Position.z}, {0.f, 0.f, 0.f, 1.f} };
	glm::mat4x4 scale{ {m_Scale.x, 0.f, 0.f, 0.f}, {0.f, m_Scale.y, 0.f, 0.f}, {0.f, 0.f, m_Scale.z, 0.f}, {0.f, 0.f, 0.f, 1.f} };
	glm::mat4x4 rotateX{ {cos( m_Rotation.x ), 0.f, sin( m_Rotation.x ), 0.f},{0.f, 1.f, 0.f, 0.f}, {-sin( m_Rotation.x ), 0, cos( m_Rotation.x ), 0.f},{0.f, 0.f, 0.f, 1.f} };
	glm::mat4x4 rotateY{ {1.f, 0.f, 0.f, 0.f},{0.f, cos( m_Rotation.y ), -sin( m_Rotation.y ), 0.f}, {0.f, sin( m_Rotation.y ), cos( m_Rotation.y ), 0.f},{0.f, 0.f, 0.f, 1.f} };
	glm::mat4x4 rotateZ{ {cos( m_Rotation.z ), -sin( m_Rotation.z ), 0.f, 0.f}, {sin( m_Rotation.z ), cos( m_Rotation.z ), 0.f, 0.f},{0.f, 0.f, 1.f, 0.f},{0.f, 0.f, 0.f, 1.f} };
	glm::mat4x4 rotate{ rotateX * rotateY * rotateZ };
	TransfomationMatrix = scale * rotate * translate;

	if( m_Parent )
	{
		TransfomationMatrix *= m_Parent->TransfomationMatrix;
		//TransfomationMatrix = m_Parent->TransfomationMatrix * TransfomationMatrix;
	}
}

void Balbino::Transform::Draw() const
{
}
void Balbino::Transform::Save( std::ostream& file )
{
	BinaryReadWrite::Write( file, int( ComponentList::Transform ) );
	BinaryReadWrite::Write( file, m_Position );
	BinaryReadWrite::Write( file, m_Rotation );
	BinaryReadWrite::Write( file, m_Scale );
	//BinaryReadWrite::Write( file, int( m_Childeren.size() ) );

}
void Balbino::Transform::Load( std::istream& file )
{
	BinaryReadWrite::Read( file, m_Position );
	BinaryReadWrite::Read( file, m_Rotation );
	BinaryReadWrite::Read( file, m_Scale );
}
void Balbino::Transform::SetParrent( std::shared_ptr<Transform> parent )
{
	std::shared_ptr<Transform> thisPtr{ shared_from_this() };
	if( m_Parent )
	{
		m_Parent->m_Childeren.erase( std::remove( m_Parent->m_Childeren.begin(), m_Parent->m_Childeren.end(), thisPtr ) );
	}
	if( parent )
	{
		parent->m_Childeren.push_back( thisPtr );
	}
	m_Parent = parent;
}
int Balbino::Transform::GetNumberOfChilderen()
{
	return int( m_Childeren.size() );
}
void Balbino::Transform::RemoveChild( int index )
{
	m_Childeren.erase( m_Childeren.begin() + index );
}
#ifdef _DEBUG
#include "../imgui-1.75/imgui.h"
void Balbino::Transform::DrawInpector()
{
	ImGui::BeginChild( "Transform Component", ImVec2{ 420, 128 }, true );
	ImGui::Text( "Transformation" );
	ImGui::Separator();

	ImGui::PushItemWidth( 80 );
	ImGui::Text( "Position: " ); ImGui::SameLine();
	ImGui::Text( "X:" ); ImGui::SameLine(); ImGui::DragFloat( "##PosX", &m_Position.x, 0.1f ); ImGui::SameLine();
	ImGui::Text( "Y:" ); ImGui::SameLine(); ImGui::DragFloat( "##PosY", &m_Position.y, 0.1f ); ImGui::SameLine();
	ImGui::Text( "Z:" ); ImGui::SameLine(); ImGui::DragFloat( "##PosZ", &m_Position.z, 0.1f );
	ImGui::PopItemWidth();

	ImGui::PushItemWidth( 80 );
	ImGui::Text( "Rotation: " ); ImGui::SameLine();
	ImGui::Text( "X:" ); ImGui::SameLine(); ImGui::DragFloat( "##RotX", &m_Rotation.x, 0.1f ); ImGui::SameLine();
	ImGui::Text( "Y:" ); ImGui::SameLine(); ImGui::DragFloat( "##RotY", &m_Rotation.y, 0.1f ); ImGui::SameLine();
	ImGui::Text( "Z:" ); ImGui::SameLine(); ImGui::DragFloat( "##RotZ", &m_Rotation.z, 0.1f );
	ImGui::PopItemWidth();

	ImGui::PushItemWidth( 80 );
	ImGui::Text( "Scale:    " ); ImGui::SameLine();
	ImGui::Text( "X:" ); ImGui::SameLine(); ImGui::DragFloat( "##ScaleX", &m_Scale.x, 0.1f ); ImGui::SameLine();
	ImGui::Text( "Y:" ); ImGui::SameLine(); ImGui::DragFloat( "##ScaleY", &m_Scale.y, 0.1f ); ImGui::SameLine();
	ImGui::Text( "Z:" ); ImGui::SameLine(); ImGui::DragFloat( "##SclaeZ", &m_Scale.z, 0.1f );
	ImGui::PopItemWidth();
	ImGui::EndChild();
}
void Balbino::Transform::DrawHierarchy()
{

}
#endif
void Balbino::Transform::SetPosition( const float x, const float y, const float z )
{
	m_Position.x = x;
	m_Position.y = y;
	m_Position.z = z;
}

void Balbino::Transform::SetRotation( float x, float y, float z )
{
	m_Rotation.x = x;
	m_Rotation.y = y;
	m_Rotation.z = z;
}

void Balbino::Transform::SetScale( float x, float y, float z )
{
	m_Scale.x = x;
	m_Scale.y = y;
	m_Scale.z = z;
}
