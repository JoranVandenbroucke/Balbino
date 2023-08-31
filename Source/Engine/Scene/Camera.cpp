//
// Created by joran on 04/11/2022.
//
#include "Camera.h"

namespace BalbinoScene
{
    void CCamera::UpdateFrameBuffer( float width, float height, float fov, float nearPlane, float farPlane ) noexcept
    {
        m_fov = fov;

        m_width  = width;
        m_height = height;

        float aspectRatio = width / height;
        m_projMatrix      = BambiMath::PerspectiveMatrix( fov, aspectRatio, nearPlane, farPlane );
    }
    void CCamera::UpdateMatrices( const BambiMath::Vector3& position, const BambiMath::Quaternion& rotation ) noexcept
    {
        m_viewMatrix   = BambiMath::LookAt( position, position + BambiMath::Rotate(rotation, { 0, 0, 1 } ), { 0, 1, 0 } );
        m_viewPosition = BambiMath::Vector4 { position, 1 };
    }
}// namespace Balbino