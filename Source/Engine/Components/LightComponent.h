#pragma once
#include "Types.h"
#include <Core.h>

namespace BalbinoScene
{
    class CEntity;
}
namespace BalbinoComponent
{
    enum class light_type : uint8_t
    {
        directional,
        point,
        spot,
        area,
        max
    };

    class CLightComponent final
    {
    public:
        constexpr CLightComponent()                          = default;
        constexpr ~CLightComponent()                         = default;
        constexpr CLightComponent( const CLightComponent& )  = default;
        constexpr CLightComponent( CLightComponent&& )       = default;
        CLightComponent& operator=( const CLightComponent& ) = default;
        CLightComponent& operator=( CLightComponent&& )      = default;

        constexpr bool operator==( const CLightComponent& rhs ) const
        {
            return m_color == rhs.m_color && m_size == rhs.m_size && m_strength == rhs.m_strength && m_type == rhs.m_type;
        }
        constexpr bool operator!=( const CLightComponent& rhs ) const
        {
            return !( rhs == *this );
        }

        [[nodiscard]] constexpr const BambiMath::Vector3& GetColor() const noexcept
        {
            return m_color;
        }
        [[nodiscard]] constexpr const BambiMath::Vector3& GetSize() const noexcept
        {
            return m_size;
        }
        [[nodiscard]] constexpr float GetStrength() const noexcept
        {
            return m_strength;
        }
        [[nodiscard]] constexpr light_type GetType() const noexcept
        {
            return m_type;
        }

        inline void SetColor( const BambiMath::Vector3& color ) noexcept
        {
            m_color = color;
        }
        inline void SetSize( const BambiMath::Vector3& size ) noexcept
        {
            m_size = size;
        }
        inline void SetStrength( float strength ) noexcept
        {
            m_strength = strength;
        }
        inline void SetType( light_type type ) noexcept
        {
            m_type = type;
        }

    private:
        BambiMath::Vector3 m_color { 1, 1, 1 };
        BambiMath::Vector3 m_size { 1 };
        float m_strength { 1 };
        light_type m_type { light_type::point };
    };
}// namespace BalbinoComponent
