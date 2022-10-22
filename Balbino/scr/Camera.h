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
                : m_projMatrix{ 1 },
                  m_viewMatrix{ 1 },
                  m_viewPosition{},
                  m_width{ 0 },
                  m_height{ 0 },
                  m_fov{ 0 }
        {
        }
        
        ~CCamera() = default;
        CCamera( const CCamera& other ) = default;
        
        CCamera( CCamera&& ) = default;
        CCamera& operator=( const CCamera& ) = default;
        CCamera& operator=( CCamera&& ) = default;
        
        void UpdateFrameBuffer( float width, float height, float fov = glm::radians(
                45.0f ), float nearPlane = 0.001f, float farPlane = 1000 )
        {
            m_fov = fov;
            
            m_width  = width;
            m_height = height;
            
            float m_aspectRatio = width / height;
            m_projMatrix = glm::perspective( fov, m_aspectRatio, nearPlane, farPlane );
        }
        void UpdateMatrices( const glm::vec3& position, const glm::vec3& rotation )
        {
            m_viewMatrix   = glm::lookAt( position, rotation, { 0, 1, 0 } );
            m_viewPosition = glm::vec4{ position, 1 };
        }
        [[nodiscard]] const glm::mat4& GetProjection() const
        {
            return m_projMatrix;
        }
        [[nodiscard]] const glm::mat4& GetView() const
        {
            return m_viewMatrix;
        }
        [[nodiscard]] const glm::vec4& GetViewPosition() const
        {
            return m_viewPosition;
        }
        [[nodiscard]] float GetFov() const
        {
            return m_fov;
        }
        [[nodiscard]] float GetWidth() const
        {
            return m_width;
        }
        [[nodiscard]] float GetHeight() const
        {
            return m_height;
        }
    
    private:
        glm::mat4 m_projMatrix;
        glm::mat4 m_viewMatrix;
        glm::vec4 m_viewPosition;
        
        float m_fov;
        float m_width;
        float m_height;
    };
}
