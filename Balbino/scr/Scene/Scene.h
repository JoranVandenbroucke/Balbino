#pragma once
//#include "UUID.h"
#include "entt/entt.hpp"
#include "Entity.h"
#include "IScene.h"
#include "../Renderer/VertexBuffer.h"
#include "../Renderer/UBOStructs.h"

struct ISystem;

namespace Balbino
{
	class CScene final : public IScene
	{
	public:
		CScene();
		~CScene() override;
		CScene( const CScene& ) = delete;
		CScene( CScene&& ) = delete;
		CScene& operator=( const CScene& ) = delete;
		CScene& operator=( CScene&& ) = delete;

		void Initialize( ISystem* pSystem, uint32_t width, uint32_t height ) override;
		void Cleanup() override;

		void PhysicsUpdate( float deltaTime ) override;
		void Update( float deltaTime ) override;
		void PrepareDraw() override;
		void Draw() override;
		void OnViewportResize( uint32_t w, uint32_t h ) override;
		void SetRenderSettings( const BalVulkan::CDevice* pDevice, const BalVulkan::CCommandPool* pCommandPool, const BalVulkan::CQueue* pQueue)
		{
			m_pDevice = pDevice;
			m_pCommandPool = pCommandPool;
			m_pQueue = pQueue;
		}
		IEntity* CreateEntity() override;
		IEntity* CreateEntityWithUUID( CUuid uuid ) override;
		void DestroyEntity( IEntity* entity ) override;

		void DuplicateEntity( IEntity* entity ) override;

		IEntity* GetPrimaryCameraEntity() override;

		template <typename... Components>
		auto GetAllEntitiesWith() const
		{
			return GetRegistry().view<Components...>();
		}

		const entt::registry& GetRegistry() const override;
		entt::registry& GetRegistry() override;
		template <typename T>
		void OnComponentAdded( CEntity entity, T& component );

		uint32_t GetViewportWidth() const;
		uint32_t GetViewportHeight() const;
		std::vector<IEntity*> GetAllEntities() override;
		ISystem* GetSystem()const override;
		BalVulkan::CBuffer* GetModelBuffer()const override;
		BalVulkan::CBuffer* GetShadingBuffer()const override;
		void RecreateBuffers( BalVulkan::CCommandPool* commandPool, BalVulkan::CQueue* queue );
	private:
		entt::registry m_registry;
		uint32_t m_viewportWidth;
		uint32_t m_viewportHeight;
		SLightObject m_sLightObject;
		std::unordered_map<uint32_t, CEntity> m_entityMap;

        float m_timer;

		Balbino::CVertexBuffer* m_instanceBuffer;
		BalVulkan::CBuffer* m_pModelBuffer;
		BalVulkan::CBuffer* m_pShadingBuffer;

		const BalVulkan::CCommandPool* m_pCommandPool;
		const BalVulkan::CDevice* m_pDevice;
		const BalVulkan::CQueue* m_pQueue;
		ISystem* m_pSystem;
	};
}
