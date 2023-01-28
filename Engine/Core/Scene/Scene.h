#pragma once
#define MAX_INSTANCE_COUNT 1024

#include <vulkan/vulkan.h>
#include <entt/entt.hpp>
#include <Components/TransformComponent.h>
#include <Vertex.h>
#include "UUID.h"
#include "Entity.h"
#include "IScene.h"
#include "Camera.h"
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
        
        CScene& operator=( const CScene& ) = delete;
        
        CScene& operator=( CScene&& ) = delete;
        
        void Initialize( ISystem* pSystem ) override;
        
        void Cleanup() override;
        
        void PhysicsUpdate( float deltaTime ) override;
        
        void Update( float deltaTime ) override;
        
        void PrepareDraw() override;
        
        void DepthPass() override;
        
        void Draw() override;
        
        void SetRenderSettings( const FawnVision::CDevice* pDevice, const FawnVision::CCommandPool* pCommandPool, const FawnVision::CQueue* pQueue )
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
        
        [[nodiscard]] std::vector<IEntity*> GetAllEntities() override;
        
        [[nodiscard]] ISystem* GetSystem() const override;
        
        [[nodiscard]] FawnVision::CBuffer* GetModelBuffer() const override;
        
        [[nodiscard]] FawnVision::CBuffer* GetShadingBuffer() const override;
        
        [[nodiscard]] FawnVision::CBuffer* GetInstanceBuffer() const override;
        
        void RecreateBuffers( FawnVision::CCommandPool* commandPool, FawnVision::CQueue* queue );
    
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
        
        Balbino::CCamera m_camera;
        
        std::unordered_map<uint32_t, CEntity> m_entityMap;
        ISystem* m_pSystem;
        
        SLightObject m_lightObject;
        SModelObject m_modelUbo;
        
        FawnVision::CBuffer* m_pModelBuffer;
        FawnVision::CBuffer* m_pShadingBuffer;
        FawnVision::CBuffer* m_pInstanceBuffer;
        
        const FawnVision::CCommandPool* m_pCommandPool;
        const FawnVision::CDevice     * m_pDevice;
        const FawnVision::CQueue      * m_pQueue;
        
        std::vector<IndirectBatch>                         m_allDrawableObjects;
        std::vector<std::vector<FawnVision::InstanceBatch>> m_instanceData;
    };
}
