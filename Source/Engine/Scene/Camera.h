#pragma once
#include <BambiMath.h>

namespace BalbinoScene
{
    class CCamera final
    {
    public:
        CCamera() = default;

        ~CCamera()                      = default;
        CCamera( const CCamera& other ) = default;

        CCamera( CCamera&& )                 = default;
        CCamera& operator=( const CCamera& ) = default;
        CCamera& operator=( CCamera&& )      = default;

        void UpdateFrameBuffer( float width, float height, float fov = BambiMath::degreeToRadiance * 45.0f, float nearPlane = 0.001f, float farPlane = 1000 ) noexcept;
        void UpdateMatrices( const BambiMath::Vector3& position, const BambiMath::Quaternion& rotation ) noexcept;

        [[nodiscard]] constexpr const BambiMath::Matrix4& GetProjection() const noexcept{return m_projMatrix;}
        [[nodiscard]] constexpr const BambiMath::Matrix4& GetView() const noexcept{return m_viewMatrix;}
        [[nodiscard]] constexpr const BambiMath::Vector4& GetViewPosition() const noexcept{return m_viewPosition;}
        [[nodiscard]] constexpr float GetFov() const noexcept{return m_fov;}
        [[nodiscard]] constexpr float GetWidth() const noexcept{return m_width;}
        [[nodiscard]] constexpr float GetHeight() const noexcept{return m_height;}

    private:
        BambiMath::Matrix4 m_projMatrix { 1 };
        BambiMath::Matrix4 m_viewMatrix { 1 };
        BambiMath::Vector4 m_viewPosition {};

        float m_fov {};
        float m_width {};
        float m_height {};
    };
}// namespace Balbino
