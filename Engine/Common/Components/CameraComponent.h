#pragma once

#include "../IEntity.h"
#include "../ISystem.h"

class CCameraComponent final
{
public:
    CCameraComponent()
            : CCameraComponent{ nullptr, nullptr }
    {
    }
    explicit CCameraComponent( IEntity* pEntity, const ISystem* pSystem )
            : m_pEntity{ pEntity }
    {
        if(pSystem)
            pSystem->GetCameraManager()->AddCamera( this );
    }
    
    CCameraComponent( const CCameraComponent& ) = default;
    CCameraComponent( CCameraComponent&& ) = default;
    CCameraComponent& operator=( const CCameraComponent& ) = default;
    CCameraComponent& operator=( CCameraComponent&& ) = default;
    
    [[nodiscard]] inline IEntity* GetEntity() const
    {
        return m_pEntity;
    }
    [[nodiscard]] inline int GetRenderIndex() const
    {
        return m_renderIndex;
    }
    inline void SetRenderIndex( int renderIndex )
    {
        m_renderIndex = renderIndex;
    }
    [[nodiscard]] inline float GetFov() const
    {
        return m_fov;
    }
    inline void SetFov( float fov )
    {
        m_fov = fov;
    }
    [[nodiscard]] inline float GetNearClip() const
    {
        return m_nearClip;
    }
    inline void SetNearClip( float nearClip )
    {
        m_nearClip = nearClip;
    }
    [[nodiscard]] inline float GetFarClip() const
    {
        return m_farClip;
    }
    inline void SetFarClip( float farClip )
    {
        m_farClip = farClip;
    }
    
private:
    int   m_renderIndex{};
    float m_fov{1.309f};
    float m_nearClip{0.01f};
    float m_farClip{1e3f};
    IEntity* m_pEntity;
};
