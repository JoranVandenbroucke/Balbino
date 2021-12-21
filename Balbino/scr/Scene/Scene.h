#pragma once
//#include "UUID.h"
#include "IScene.h"
#include "Entity.h"

#include <entt.hpp>


namespace Balbino
{
	class CScene final : public IScene
	{
	public:
		CScene() = default;
		virtual ~CScene() override = default;
		CScene(const CScene&) = delete;
		CScene(CScene&&) = delete;
		CScene& operator=(const CScene&) = delete;
		CScene& operator=(CScene&&) = delete;

		void Initialize() override;
		void Cleanup() override;

		void PhysicsUpdate( const float deltaTime ) override;
		void Update( const float deltaTime ) override;
		void Draw()const override;
		void OnViewportResize( uint32_t w, uint32_t h ) override;

		IEntity* CreateEntity() override;
		IEntity* CreateEntityWithUUID( CUuid uuid ) override;
		void DestroyEntity( IEntity* entity ) override;

		void DuplicateEntity( IEntity* entity )override;

		IEntity* GetPrimaryCameraEntity() override;

		template<typename... Components>
		auto GetAllEntitiesWith() const
		{
			return GetRegistry().view<Components...>();
		}

		const entt::registry& GetRegistry()const override;
		entt::registry& GetRegistry() override;
		template<typename T>
		void OnComponentAdded( CEntity entity, T& component );

		uint32_t GetViewportWidth()const;
		uint32_t GetViewportHeight()const;
		std::vector<IEntity*> GetAllEntities() override;
	private:
		entt::registry m_registry;
		uint32_t m_viewportWidth;
		uint32_t m_viewportHeight;

		std::unordered_map<uint32_t, CEntity> m_entityMap;
	};
}