#pragma once
#include <Core.h>
#include <entt/entt.hpp>

#include "Entity.h"
#include "IDComponent.h"
#include "TransformComponent.h"

namespace BalbinoScene
{
    class CScene final
    {
    public:
        constexpr CScene()                           = default;
        ~CScene()                                    = default;
        constexpr CScene( const CScene& )            = delete;
        constexpr CScene( CScene&& )                 = delete;
        constexpr CScene& operator=( const CScene& ) = delete;
        constexpr CScene& operator=( CScene&& )      = delete;

        void Initialize();
        void Cleanup();
        void Update( float deltatTime );

        [[nodiscard]] constexpr const entt::registry& GetRegistry() const
        {
            return m_registry;
        }
        constexpr entt::registry& GetRegistry()
        {
            return m_registry;
        }
        inline CEntity CreateEntity()
        {
            CEntity entity { m_registry.create(), m_registry };
            entity.AddComponent<BalbinoComponent::CIDComponent>( CUuid {} );
            entity.AddComponent<BalbinoComponent::CTransformComponent>( BambiMath::Vector3 {} );
            return entity;
        }
        inline void DestroyEntity( const CEntity& entity )
        {
            m_registry.destroy( entity.GetEntity() );
        };
        [[nodiscard]] constexpr std::vector<CEntity> GetAllEntities()
        {
            std::vector<BalbinoScene::CEntity> entities;
            entities.reserve( m_registry.storage<entt::entity>().size() );

            std::for_each( m_registry.storage<entt::entity>().begin(), m_registry.storage<entt::entity>().end(), [ &entities, &reg = m_registry ]( auto entity ) {
              entities.emplace_back( entity, reg );
            } );

            return entities;
        }

    private:
        entt::registry m_registry {};
    };
}// namespace BalbinoScene
