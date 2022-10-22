#pragma once

#include <entt/entt.hpp>

#include "UUID.h"

struct IEntity;
struct ISystem;
namespace BalVulkan
{
    class CBuffer;
}
struct IScene
{
    virtual ~IScene() = default;
    
    virtual void Initialize( ISystem* pSystem ) = 0;
    
    virtual void Cleanup() = 0;
    
    virtual void PhysicsUpdate( float deltaTime ) = 0;
    
    virtual void Update( float deltaTime ) = 0;
    
    virtual void PrepareDraw() = 0;
    
    virtual void DepthPass() = 0;
    
    virtual void Draw() = 0;
    
    virtual IEntity* CreateEntity() = 0;
    
    virtual IEntity* CreateEntityWithUUID( CUuid uuid ) = 0;
    
    virtual void DestroyEntity( IEntity* entity ) = 0;
    
    virtual void DuplicateEntity( IEntity* entity ) = 0;
    
    virtual IEntity* GetPrimaryCameraEntity() = 0;
    
    [[nodiscard]] virtual const entt::registry& GetRegistry() const = 0;
    
    virtual entt::registry& GetRegistry() = 0;
    
    [[nodiscard]] virtual std::vector<IEntity*> GetAllEntities() = 0;
    
    [[nodiscard]] virtual ISystem* GetSystem() const = 0;
    
    [[nodiscard]] virtual BalVulkan::CBuffer* GetModelBuffer() const = 0;
    
    [[nodiscard]] virtual BalVulkan::CBuffer* GetShadingBuffer() const = 0;
    
    [[nodiscard]] virtual BalVulkan::CBuffer* GetInstanceBuffer() const = 0;
};