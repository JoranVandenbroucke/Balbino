#pragma once
#include <BambiMath.h>
#include <Core.h>
#include <UUID.h>
#include <entt/entt.hpp>

namespace BalbinoScene
{
    class CScene;
    class CEntity final
    {
    public:
        constexpr CEntity() noexcept = default;
        constexpr ~CEntity() noexcept = default;
        constexpr CEntity( const CEntity& ) = default;

        constexpr explicit CEntity( entt::entity entity, entt::registry& regestry ) noexcept
            : m_entity { entity }
            , m_registry { &regestry }
        {
        }
        constexpr CEntity( CEntity&& other ) noexcept
            : m_entity { std::exchange( other.m_entity, {} ) }
            , m_registry { std::exchange( other.m_registry, BALBINO_NULL ) }
        {
        }
        constexpr CEntity& operator=( CEntity&& other ) noexcept
        {
            if ( this != &other )
            {
                m_entity = std::exchange( other.m_entity, {} );
                m_registry  = std::exchange( other.m_registry, BALBINO_NULL );
            }
            return *this;
        }
        constexpr CEntity& operator=( const CEntity& other )
        {
            if ( this != &other )
            {
                m_entity = other.m_entity;
                m_registry  = other.m_registry;
            }
            return *this;
        }

        inline bool operator==( const CEntity& other ) const noexcept
        {
            return m_entity == other.m_entity && m_registry == other.m_registry;
        };
        inline bool operator!=( const CEntity& other ) const noexcept
        {
            return !( *this == other );
        };

        [[nodiscard]] constexpr entt::entity GetEntity() const noexcept
        {
            return m_entity;
        }
        [[nodiscard]] bool IsValid() const noexcept;

        template<typename Component>
        [[nodiscard]] bool HasComponent() const{
            return m_registry->any_of<Component>( m_entity );
        }
        template<typename Component, typename... Args>
        Component AddComponent( Args&&... args ){
            if ( IsValid() )
            {
                if ( HasComponent<Component>() )
                    return GetComponent<Component>();
                return m_registry->emplace<Component>( m_entity, std::forward<Args>( args )... );
            }
            return {};
        }
        template<typename Component>
        Component GetComponent() const{
            if ( IsValid() && HasComponent<Component>() )
                return m_registry->get<Component>( m_entity );
            return {};
        }
        template<typename Component>
        void RemoveComponent(){
            if ( IsValid() && HasComponent<Component>() )
            {
                m_registry->remove<Component>(m_entity);
            }
        }

    private:
        entt::entity m_entity {};
        entt::registry* m_registry{BALBINO_NULL};
    };
}// namespace BalbinoScene
