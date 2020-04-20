#include "BalbinoPCH.h"
#include "Camera.h"
#include "../GameObject/GameObject.h"
#include "../BinaryReaderWrider.h"

Balbino::Camera::Camera( const std::weak_ptr<GameObject> origine, float fov, float w )
	:Component( origine )
	, m_IsOrtographic{ false }
	, m_OrthographicSize{ w }
	, m_FieldOfView{ fov }
	, m_Depth{ -1 }
{
	Update();
}

void Balbino::Camera::Create()
{
	this->Component::Create();
}

void Balbino::Camera::Update()
{
	//m_ViewProjection = m_View * m_Projection;
}

void Balbino::Camera::Draw() const
{
}

void Balbino::Camera::Save( std::ostream& file )
{
	BinaryReadWrite::Write( file, int( ComponentList::Camera ) );
	BinaryReadWrite::Write( file, m_IsOrtographic );
	BinaryReadWrite::Write( file, m_OrthographicSize );
	BinaryReadWrite::Write( file, m_FieldOfView );
	BinaryReadWrite::Write( file, m_Depth );

}

void Balbino::Camera::Load( std::istream& file )
{
	BinaryReadWrite::Read( file, m_IsOrtographic );
	BinaryReadWrite::Read( file, m_OrthographicSize );
	BinaryReadWrite::Read( file, m_FieldOfView );
	BinaryReadWrite::Read( file, m_Depth );
}

#ifdef _DEBUG
void Balbino::Camera::DrawInpector()
{
}
#endif // _DEBUG

