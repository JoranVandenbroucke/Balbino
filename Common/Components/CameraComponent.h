#pragma once
#include "../../Balbino/scr/Camera.h"

#include "../IEntity.h"
#include "../ISystem.h"

class CCameraComponent final
{
public:
    CCameraComponent() = default;
    explicit CCameraComponent( IEntity* pEntity, ISystem* pSystem )
            : m_pEntity{ pEntity },
              m_renderIndex{},
              m_fov{ 1.309f },
              m_nearClip{ 0.01f },
              m_farClip{ 1000.f }
    {
        pSystem->GetCameraManager()->AddCamera( this );
    }
    
    ~CCameraComponent() = default;
    CCameraComponent( const CCameraComponent& ) = default;
    CCameraComponent( CCameraComponent&& ) = default;
    CCameraComponent& operator=( const CCameraComponent& ) = default;
    CCameraComponent& operator=( CCameraComponent&& ) = default;
    
    [[nodiscard]] IEntity* GetEntity() const
    {
        return m_pEntity;
    }
    int GetRenderIndex() const
    {
        return m_renderIndex;
    }
    void SetRenderIndex( int renderIndex )
    {
        m_renderIndex = renderIndex;
    }
    float GetFov() const
    {
        return m_fov;
    }
    void SetFov( float fov )
    {
        m_fov = fov;
    }
    float GetNearClip() const
    {
        return m_nearClip;
    }
    void SetNearClip( float nearClip )
    {
        m_nearClip = nearClip;
    }
    float GetFarClip() const
    {
        return m_farClip;
    }
    void SetFarClip( float farClip )
    {
        m_farClip = farClip;
    }
private:
    int   m_renderIndex;
    float m_fov;
    float m_nearClip;
    float m_farClip;
    IEntity* m_pEntity;
};
