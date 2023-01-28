//
// Created by joran on 04/11/2022.
//
#include "Camera.h"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

#pragma warning(push)
#pragma warning(disable:4201)
#include <glm/gtx/quaternion.hpp>

#pragma warning(pop)

namespace Balbino
{
    CCamera::CCamera()
            : m_projMatrix{ 1 }
              , m_viewMatrix{ 1 }
              , m_viewPosition{}
              , m_fov{ 0 }
              , m_width{ 0 }
              , m_height{ 0 }
    {
    }
    void CCamera::UpdateFrameBuffer( float width, float height, float fov, float nearPlane, float farPlane )
    {
        m_fov = fov;
        
        m_width  = width;
        m_height = height;
        
        float m_aspectRatio = width / height;
        m_projMatrix = glm::perspective( fov, m_aspectRatio, nearPlane, farPlane );
    }
    void CCamera::UpdateMatrices( const glm::vec3& position, const glm::quat& rotation )
    {
        m_viewMatrix   = glm::lookAt( position, position + glm::rotate( rotation, { 0, 0, 1 } ), { 0, 1, 0 } );
        m_viewPosition = glm::vec4{ position, 1 };
    }
    const glm::mat4& CCamera::GetProjection() const
    {
        return m_projMatrix;
    }
    const glm::mat4& CCamera::GetView() const
    {
        return m_viewMatrix;
    }
    const glm::vec4& CCamera::GetViewPosition() const
    {
        return m_viewPosition;
    }
    float CCamera::GetFov() const
    {
        return m_fov;
    }
    float CCamera::GetWidth() const
    {
        return m_width;
    }
    float CCamera::GetHeight() const
    {
        return m_height;
    }
}