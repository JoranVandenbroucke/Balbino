#pragma once

#include "../Light.h"
#include "../IEntity.h"
#include "../../3rdParty/GLM/glm/vec3.hpp"

class CLightComponent final
{
public:
    explicit CLightComponent( IEntity* pEntity )
            : m_pEntity{ pEntity }
            , m_color{1,1,1}
            , m_size{100}
            , m_strength{100}
            ,m_type{Balbino::ELightType::Point}
    {
    }

    ~CLightComponent() = default;
    CLightComponent( const CLightComponent& ) = default;
    CLightComponent( CLightComponent&& ) = default;
    CLightComponent& operator=( const CLightComponent& ) = default;
    CLightComponent& operator=( CLightComponent&& ) = default;

    [[nodiscard]] const glm::vec3& GetColor() const
    {
        return m_color;
    }

    [[nodiscard]] const glm::vec3& GetSize() const
    {
        return m_size;
    }

    [[nodiscard]] float GetStrength() const
    {
        return m_strength;
    }

    [[nodiscard]] Balbino::ELightType GetType() const
    {
        return m_type;
    }

    [[nodiscard]] IEntity* GetEntity() const
    {
        return m_pEntity;
    }

    void SetColor( const glm::vec3& color )
    {
        m_color = color;
    }

    void SetSize( const glm::vec3& size )
    {
        m_size = size;
    }

    void SetStrength( float strength )
    {
        m_strength = strength;
    }

    void SetType( Balbino::ELightType type )
    {
        m_type = type;
    }

private:
    IEntity* m_pEntity;
    glm::vec3 m_color;
    glm::vec3 m_size;
    float m_strength;
    Balbino::ELightType m_type;
};
