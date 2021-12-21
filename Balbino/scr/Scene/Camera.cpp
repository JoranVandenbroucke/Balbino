#include "pch.h"
#include "Camera.h"

#include <ext/matrix_clip_space.hpp>
#include <ext/matrix_transform.hpp>
#include <gtx/rotate_vector.hpp>

Balbino::CCamera::CCamera()
	: m_view{1}
	, m_projection{1}
	, m_index{}
	, m_width{ 0 }
	, m_height{ 0 }
	, m_fov{ 0 }
	, m_aspectRatio{ 0 }
{
}

bool Balbino::CCamera::operator==( const CCamera& other ) const
{
	return this->m_index == other.m_index &&
		fabs( this->m_aspectRatio - other.m_aspectRatio ) < 0.001f &&
		fabs( this->m_fov - other.m_fov ) < 0.001f;
}

void Balbino::CCamera::Initialize( uint32_t width, uint32_t height, float fov )
{
	m_width = width;
	m_height = height;
	m_fov = fov;
	m_index = 0;
	m_aspectRatio = ( float ) width / ( float ) height;
	m_projection = glm::perspective( fov, m_aspectRatio, 0.01f, 1000.0f );
	m_projection[1][1] *= -1;

	m_view = lookAt( glm::vec3{ 0,0,0 }, glm::vec3{ 0,0,1 }, glm::vec3{ 0,1,0 } );
}

const glm::mat4& Balbino::CCamera::GetView() const
{
	return m_view;
}

const glm::mat4& Balbino::CCamera::GetProjection() const
{
	return m_projection;
}

void Balbino::CCamera::SetDrawIndex( const int index )
{
	m_index = index;
}

int Balbino::CCamera::GetDrawIndex() const
{
	return m_index;
}
