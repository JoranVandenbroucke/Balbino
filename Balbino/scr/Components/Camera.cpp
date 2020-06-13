#include "BalbinoPCH.h"
#include "Camera.h"
//#include "../GameObject/GameObject.h"
#include "../BinaryReaderWrider.h"
#include "../Editor/Debug.h"
#include "Transform.h"

Balbino::Camera* Balbino::Camera::m_pMainCamera{};
std::vector<Balbino::Camera*> Balbino::Camera::m_pAllCameras{};


Balbino::Camera::Camera( const GameObject* const origine, float aspectRatio, float w )

	:Component( origine )
	, m_ProjectionMatrix{ glm::ortho( -w / 2.f, w / 2.f, -w / aspectRatio / 2.f, w / aspectRatio / 2.f, -1.f, 1.f ) }
	, m_ClearColor{ 0, 0, 0, 1 }
	, m_Depth{ -1 }
	, m_ViewMatrix{ glm::mat4( 1.0 ) }
	, m_IsMain{}
{
	glGenFramebuffers( 1, &m_CameraIndex );
	glBindFramebuffer( GL_FRAMEBUFFER, m_CameraIndex );
	glDrawBuffer( GL_COLOR_ATTACHMENT0 );

	glGenTextures( 1, &m_RenderTexutre );
	glBindTexture( GL_TEXTURE_2D, m_RenderTexutre );

	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, 640, 480, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL );

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

	glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_RenderTexutre, 0 );

	if( glCheckFramebufferStatus( GL_FRAMEBUFFER ) != GL_FRAMEBUFFER_COMPLETE )
		Debug::LogError( "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" );
	glBindFramebuffer( GL_FRAMEBUFFER, 0 );

	m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
}

Balbino::Camera::~Camera()
{
	if( m_pMainCamera == this )
	{
		m_pMainCamera = nullptr;
	}
	m_pAllCameras.erase( std::remove( m_pAllCameras.begin(), m_pAllCameras.end(), this ) );
}

void Balbino::Camera::Create()
{
	if( m_Created ) return; this->Component::Create();
	if( !m_pMainCamera )
	{
		m_pMainCamera = this;
		m_IsMain = true;
	}
	m_pAllCameras.push_back( this );
}

void Balbino::Camera::Update()
{
	m_ViewMatrix = glm::inverse( m_pTransform->TransfomationMatrix );
	m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
}

void Balbino::Camera::Draw() const
{
}

void Balbino::Camera::Save( std::ostream& file )
{
	(void) file;
	BinaryReadWrite::Write( file, int( ComponentList::Camera ) );
	BinaryReadWrite::Write( file, m_IsMain );
	BinaryReadWrite::Write( file, m_Depth );
	BinaryReadWrite::Write( file, m_ClearColor );
}

void Balbino::Camera::Load( std::istream& file )
{
	(void) file;
	BinaryReadWrite::Read( file, m_IsMain );
	BinaryReadWrite::Read( file, m_Depth );
	BinaryReadWrite::Read( file, m_ClearColor );
	if( m_IsMain )
	{
		m_pMainCamera = this;
	}
}

void Balbino::Camera::SetClearColor( const Color& newClearColor )
{
	m_ClearColor = newClearColor;
}

void Balbino::Camera::SetDepth( int newDepth )
{
	m_Depth = newDepth;
}

const Balbino::Color& Balbino::Camera::GetClearColor() const
{
	return m_ClearColor;
}

const glm::mat4& Balbino::Camera::GetProjectionMatrix() const
{
	return m_ProjectionMatrix;
}

const glm::mat4& Balbino::Camera::GetViewMatrix() const
{
	return m_ViewMatrix;
}

const glm::mat4& Balbino::Camera::GetViewProjectionMatrix() const
{
	return m_ViewProjectionMatrix;
}

int Balbino::Camera::GetCameraIndex() const
{
	return m_CameraIndex;
}

int Balbino::Camera::GetDepth() const
{
	return m_Depth;
}

GLuint Balbino::Camera::GetTargetTexture() const
{
	return m_RenderTexutre;
}

Balbino::Camera* Balbino::Camera::GetMainCamera()

{
	return m_pMainCamera;
}

std::vector<Balbino::Camera*>& Balbino::Camera::GetAllCameras()

{
	std::sort( m_pAllCameras.begin(), m_pAllCameras.end(), []( Balbino::Camera* c1, Balbino::Camera* c2 )
	{
		return c1->GetDepth() > c2->GetDepth();
	} );
	return m_pAllCameras;
}

#ifdef _DEBUG
void Balbino::Camera::DrawInpector()
{
	float color[3]{ m_ClearColor.r / 255.f, m_ClearColor.g / 255.f, m_ClearColor.b / 255.f };

	ImGui::BeginChild( "Camera Component", ImVec2{ -1, 128 }, true );
	ImGui::Text( "Camera" );
	ImGui::Text( "Clear Color: " ); ImGui::SameLine();
	ImGui::ColorEdit3("#Color", color );
	ImGui::Text( "Depth:       " ); ImGui::SameLine();
	ImGui::DragInt( "#Depth", &m_Depth );
	ImGui::EndChild();

	m_ClearColor.r = (unsigned char) ( color[0] * 255 );
	m_ClearColor.g = (unsigned char) ( color[1] * 255 );
	m_ClearColor.b = (unsigned char) ( color[2] * 255 );
}
#endif