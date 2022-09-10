#pragma once
#define MAX_INSTANCE_COUNT 1024

#include <vulkan/vulkan.h>
#include <Components/TransformComponent.h>
#include <Vertex.h>
#include "entt/entt.hpp"
#include "UUID.h"
#include "Entity.h"
#include "IScene.h"
#include "../Renderer/VertexBuffer.h"
#include "../Renderer/UBOStructs.h"

struct ISystem;

class CCameraComponent;

class CCameraComponent;

class CMeshRenderComponent;

namespace Balbino
{
    class CScene final : public IScene
    {
    public:
        CScene();

        ~CScene() override;

        CScene( const CScene& ) = delete;

        CScene( CScene&& ) = delete;

        CScene& operator =( const CScene& ) = delete;

        CScene& operator =( CScene&& ) = delete;

        void Initialize( ISystem* pSystem, uint32_t width, uint32_t height ) override;

        void Cleanup() override;

        void PhysicsUpdate( float deltaTime ) override;

        void Update( float deltaTime ) override;

        void PrepareDraw() override;

        void DepthPass() override;

        void Draw() override;

        void OnViewportResize( uint32_t w, uint32_t h ) override;

        void SetRenderSettings( const BalVulkan::CDevice* pDevice, const BalVulkan::CCommandPool* pCommandPool, const BalVulkan::CQueue* pQueue )
        {
            m_pDevice      = pDevice;
            m_pCommandPool = pCommandPool;
            m_pQueue       = pQueue;
        }

        IEntity* CreateEntity() override;

        IEntity* CreateEntityWithUUID( CUuid uuid ) override;

        void DestroyEntity( IEntity* entity ) override;

        void DuplicateEntity( IEntity* entity ) override;

        IEntity* GetPrimaryCameraEntity() override;

        template<typename... Components>
        auto GetAllEntitiesWith() const
        {
            return GetRegistry().view<Components...>();
        }

        [[nodiscard]] const entt::registry& GetRegistry() const override;

        entt::registry& GetRegistry() override;

        template<typename T>
        void OnComponentAdded( CEntity entity, T& component );

        [[nodiscard]] uint32_t GetViewportWidth() const;

        [[nodiscard]] uint32_t GetViewportHeight() const;

        [[nodiscard]] std::vector<IEntity*> GetAllEntities() override;

        [[nodiscard]] ISystem* GetSystem() const override;

        [[nodiscard]] BalVulkan::CBuffer* GetModelBuffer() const override;

        [[nodiscard]] BalVulkan::CBuffer* GetShadingBuffer() const override;

        [[nodiscard]] BalVulkan::CBuffer* GetInstanceBuffer() const override;

        void RecreateBuffers( BalVulkan::CCommandPool* commandPool, BalVulkan::CQueue* queue );

    private:
        struct IndirectBatch
        {
            CUuid    mesh;
            CUuid    material;
            uint32_t firstInstance;
            uint32_t instanceCount;
            uint32_t firstIndex;
            uint32_t indexCount;
        };

        entt::registry m_registry;
        uint32_t       m_viewportWidth;
        uint32_t       m_viewportHeight;

        CCameraComponent* m_pMainCamera;

        SLightObject m_lightObject;
        SModelObject m_modelUbo;

        std::unordered_map<uint32_t, CEntity> m_entityMap;

        BalVulkan::CBuffer* m_pModelBuffer;
        BalVulkan::CBuffer* m_pShadingBuffer;
        BalVulkan::CBuffer* m_pInstanceBuffer;

        const BalVulkan::CCommandPool* m_pCommandPool;
        const BalVulkan::CDevice     * m_pDevice;
        const BalVulkan::CQueue      * m_pQueue;
        ISystem                      * m_pSystem;

        bool m_meshChangeDirty;

        std::vector<IndirectBatch>                         m_allDrawableObjects;
        std::vector<std::vector<BalVulkan::InstanceBatch>> m_instanceData;
    };
}
