#pragma once
#include <Core.h>

namespace BalbinoScene
{
    class CEntity;
}
namespace BalbinoComponent
{
    class CCameraComponent final
    {
    public:
        constexpr CCameraComponent()                           = default;
        constexpr ~CCameraComponent()                          = default;
        constexpr CCameraComponent( const CCameraComponent& )  = default;
        constexpr CCameraComponent( CCameraComponent&& )       = default;
        CCameraComponent& operator=( const CCameraComponent& ) = default;
        CCameraComponent& operator=( CCameraComponent&& )      = default;

        [[nodiscard]] constexpr int GetRenderIndex() const noexcept
        {
            return m_renderIndex;
        }
        [[nodiscard]] constexpr float GetFov() const noexcept
        {
            return m_fov;
        }
        [[nodiscard]] constexpr float GetNearClip() const noexcept
        {
            return m_nearClip;
        }
        [[nodiscard]] constexpr float GetFarClip() const noexcept
        {
            return m_farClip;
        }
        inline void SetRenderIndex( int renderIndex ) noexcept
        {
            m_renderIndex = renderIndex;
        }
        inline void SetFov( float fov ) noexcept
        {
            m_fov = fov;
        }
        inline void SetNearClip( float nearClip ) noexcept
        {
            m_nearClip = nearClip;
        }
        inline void SetFarClip( float farClip ) noexcept
        {
            m_farClip = farClip;
        }

    private:
        int m_renderIndex {};
        float m_fov { 1.309f };
        float m_nearClip { 0.01f };
        float m_farClip { 1e3f };
    };
}// namespace BalbinoComponent