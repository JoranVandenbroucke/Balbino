#pragma once
#include "../../Balbino/scr/Scene/Camera.h"

#include "../IEntity.h"
#include "../ISystem.h"

class CCameraComponent final
{
    enum class EClearMode : uint8_t
    {
        Inheret,
        None,
        Color,
        Shader,
        Image
    };
    union UClear
    {
        float color[4];
        CUuid MaterialId;
        CUuid TextureId;
    };

public:
    CCameraComponent()
            : CCameraComponent{ nullptr, nullptr }
    {
    }
    explicit CCameraComponent( IEntity* pEntity, ISystem* pSystem )
            : m_renderIndex{}
              , m_fov{ 1.309f }
              , m_nearClip{ 0.01f }
              , m_farClip{ 1000.f }
              , m_pEntity{ pEntity }
              , m_clearMode{ EClearMode::Color }
              , m_clear{{ 248.f / 255.f, 181.f / 255.f, 203.f / 255.f, 1.0f }}
    {
        pSystem->GetCameraManager()->AddCamera( this );
    }
    
    CCameraComponent( const CCameraComponent& ) = default;
    CCameraComponent( CCameraComponent&& ) = default;
    CCameraComponent& operator=( const CCameraComponent& ) = default;
    CCameraComponent& operator=( CCameraComponent&& ) = default;
    
    inline IEntity* GetEntity() const
    {
        return m_pEntity;
    }
    inline int GetRenderIndex() const
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
    [[nodiscard]] inline EClearMode GetClearMode() const
    {
        return m_clearMode;
    }
    inline void SetClearMode( EClearMode clearMode )
    {
        m_clearMode = clearMode;
    }
    [[nodiscard]] inline const UClear& GetClear() const
    {
        return m_clear;
    }
    inline void SetClear( const UClear& clear )
    {
        m_clear = clear;
    }
private:
    int   m_renderIndex;
    float m_fov;
    float m_nearClip;
    float m_farClip;
    IEntity* m_pEntity;
    
    EClearMode m_clearMode;
    UClear     m_clear;
};
