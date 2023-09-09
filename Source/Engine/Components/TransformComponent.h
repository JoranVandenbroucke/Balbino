#pragma once
#include <Core.h>
#include <BambiMath.h>

namespace BalbinoScene
{
    class CEntity;
}

namespace BalbinoComponent
{
    class CTransformComponent final
    {
    public:
        CTransformComponent() = default;
        explicit CTransformComponent( const BambiMath::Vector3& translation )
            : m_position { translation }
        {
        }

        constexpr ~CTransformComponent()                             = default;
        constexpr CTransformComponent( const CTransformComponent& )  = default;
        constexpr CTransformComponent( CTransformComponent&& )       = default;
        CTransformComponent& operator=( const CTransformComponent& ) = default;
        CTransformComponent& operator=( CTransformComponent&& )      = default;

        bool operator==( const CTransformComponent& rhs ) const
        {
            return m_position == rhs.m_position && m_scale == rhs.m_scale && m_rotation == rhs.m_rotation && m_pParent == rhs.m_pParent && m_children == rhs.m_children;
        }
        bool operator!=( const CTransformComponent& rhs ) const
        {
            return !( rhs == *this );
        }

        [[nodiscard]] constexpr BambiMath::Vector3 GetWorldTranslation() const noexcept
        {
            if ( m_pParent )
                return m_pParent->GetWorldTranslation() + BambiMath::Rotate( m_pParent->GetWorldRotation(), m_position );
            return m_position;
        }
        [[nodiscard]] constexpr BambiMath::Quaternion GetWorldRotation() const noexcept
        {
            if ( m_pParent )
                return m_pParent->GetWorldRotation() * m_rotation;
            return m_rotation;
        }
        [[nodiscard]] constexpr BambiMath::Vector3 GetWorldScale() const noexcept
        {
            if ( m_pParent )
                return m_pParent->GetWorldScale() * m_scale;
            return m_scale;
        }

        [[nodiscard]] constexpr BambiMath::Vector3 GetTranslation() const noexcept
        {
            return m_position;
        }
        [[nodiscard]] constexpr BambiMath::Vector3 GetScale() const noexcept
        {
            return m_scale;
        }
        [[nodiscard]] constexpr BambiMath::Quaternion GetRotation() const noexcept
        {
            return m_rotation;
        }

        inline void SetTranslation( const BambiMath::Vector3& position ) noexcept
        {
            m_position = position;
        }
        inline void SetRotation( const BambiMath::Quaternion& rotation ) noexcept
        {
            m_rotation = rotation;
        }
        inline void SetScale( const BambiMath::Vector3& scale ) noexcept
        {
            m_scale = scale;
        }

        [[nodiscard]] constexpr const CTransformComponent& GetParent() const noexcept
        {
            if ( !m_pParent )
                return {};
            return *m_pParent;
        }
        [[nodiscard]] constexpr const CTransformComponent& GetChild( int index ) const noexcept
        {
            return *m_children[ index ];
        }
        [[nodiscard]] constexpr uint32_t GetChildCount() const noexcept
        {
            return static_cast<uint32_t>( m_children.size() );
        }

        inline void AddChild( const CTransformComponent& entity ) noexcept
        {
            m_children.push_back( &entity );
        }
        inline void RemoveChild( const CTransformComponent& transformComponent ) noexcept
        {
            (void)std::ranges::remove( m_children, &transformComponent );
        }
        inline void RemoveAllChildren() noexcept
        {
            m_children.clear();
        }
        inline void ClearParent() noexcept
        {
            m_pParent->RemoveChild( *this );
            m_pParent = nullptr;
        }

    private:
        BambiMath::Vector3 m_position { 0, 0, 0 };
        BambiMath::Vector3 m_scale { 1, 1, 1 };
        BambiMath::Quaternion m_rotation { 0, 0, 0, 1 };

        CTransformComponent* m_pParent { BALBINO_NULL };
        std::vector<const CTransformComponent*> m_children;
    };
}// namespace BalbinoComponent