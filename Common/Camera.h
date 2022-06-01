#pragma once
#include <glm/glm.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include <cmath>

namespace Balbino
{
	class CCamera
	{
	public:
		CCamera()
			: m_view{ 1 }
			, m_projection{ 1 }
			, m_index{}
			, m_width{ 0 }
			, m_height{ 0 }
			, m_fov{ 0 }
			, m_aspectRatio{ 0 }
			, m_nearClip{ 0.001f }
			, m_farClip{ 1000.0f }
		{
		}

		~CCamera() = default;
		CCamera( const CCamera& other ) = default;

		CCamera( CCamera&& ) = default;
		CCamera& operator=( const CCamera& ) = default;
		CCamera& operator=( CCamera&& ) = default;

		bool operator==( const CCamera& other ) const
		{
			return this->m_index == other.m_index &&
				fabs( this->m_aspectRatio - other.m_aspectRatio ) < 0.001f &&
				fabs( this->m_fov - other.m_fov ) < 0.001f;
		}

		void Initialize( uint32_t width, uint32_t height, float fov = glm::radians( 45.0f ), float nearPlane = 0.001f, float farPlane = 1000 )
		{
			m_width = width;
			m_height = height;
			m_fov = fov;
			m_index = 0;
			m_aspectRatio = static_cast<float>( width ) / static_cast<float>( height );
			m_nearClip = nearPlane;
			m_farClip = farPlane;
			m_projection = glm::perspective( fov, m_aspectRatio, nearPlane, farPlane );
			m_projection[1][1] *= -1;

			m_view = lookAt( glm::vec3{ 0, 0, 0 }, glm::vec3{ 0, 0, 1 }, glm::vec3{ 0, 1, 0 } );
		}

		void UpdateProjectionMatrix( float fov = glm::radians( 45.0f ), float nearPlane = 0.001f, float farPlane = 1000 )
		{
			m_fov = fov;
			m_nearClip = std::max( nearPlane, 0.0001f );
			m_farClip = farPlane;
			m_projection = glm::perspective( fov, m_aspectRatio, nearPlane, farPlane );
			m_projection[1][1] *= -1;
		}

		void SetDrawIndex( int index )
		{
			m_index = index;
		}

		[[nodiscard]] const glm::mat4& GetView() const
		{
			return m_view;
		}

		[[nodiscard]] const glm::mat4& GetProjection() const
		{
			return m_projection;
		}

		[[nodiscard]] int GetDrawIndex() const
		{
			return m_index;
		}

		[[nodiscard]] float GetFov() const
		{
			return m_fov;
		}

		[[nodiscard]] float GetNearClip() const
		{
			return m_nearClip;
		}

		[[nodiscard]] float GetFarClip() const
		{
			return m_farClip;
		}

	private:
		glm::mat4 m_view;
		glm::mat4 m_projection;

		int m_index;
		uint32_t m_width;
		uint32_t m_height;
		float m_fov;
		float m_aspectRatio;
		float m_nearClip;
		float m_farClip;
	};
}
