#pragma once
#include <entt.hpp>

#include "UUID.h"

struct IEntity;

struct IScene
{
	virtual ~IScene() = default;
	virtual void Initialize( uint32_t width, uint32_t height ) = 0;
	virtual void Cleanup() = 0;

	virtual void PhysicsUpdate( const float deltaTime ) = 0;
	virtual void Update( const float deltaTime ) = 0;
	virtual void Draw()const = 0;
	virtual void OnViewportResize( uint32_t w, uint32_t h ) = 0;

	virtual IEntity* CreateEntity() = 0;
	virtual IEntity* CreateEntityWithUUID( CUuid uuid ) = 0;
	virtual void DestroyEntity( IEntity* entity ) = 0;

	virtual void DuplicateEntity( IEntity* entity ) = 0;

	virtual IEntity* GetPrimaryCameraEntity() = 0;

	[[nodiscard]] virtual const entt::registry& GetRegistry()const = 0;
	virtual entt::registry& GetRegistry() = 0;
	[[nodiscard]] virtual std::vector<IEntity*> GetAllEntities() = 0;

	template<typename T>
	static void OnComponentAdded( IEntity* entity, T& component );
};

template <typename T>
void IScene::OnComponentAdded( IEntity* entity, T& component )
{
	( void ) entity; ( void ) component;
	//static_assert( false );
}