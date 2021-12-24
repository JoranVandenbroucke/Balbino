#include "pch.h"
#include "Scene.h"

#include "Camera.h"
#include "glm.hpp"
#include "Components/CameraComponent.h"
#include "Components/IDComponent.h"
#include "Components/TransformComponent.h"

template<>
void IScene::OnComponentAdded<CIDComponent>( IEntity* entity, CIDComponent& component )
{
	( void ) entity; ( void ) component;
}

template<>
void IScene::OnComponentAdded<CTransformComponent>( IEntity* entity, CTransformComponent& component )
{
	( void ) entity; ( void ) component;
}

template<>
void IScene::OnComponentAdded<CCameraComponent>( IEntity* entity, CCameraComponent& component )
{
	const Balbino::CScene* pScene = reinterpret_cast< const Balbino::CScene* >( entity->GetScene() );
	( void ) entity;
	if ( pScene->GetViewportWidth() > 0 && pScene->GetViewportHeight() > 0 )
		component.GetCamera().Initialize( pScene->GetViewportWidth(), pScene->GetViewportHeight() );
}

template<typename Component>
static void CopyComponent( entt::registry& dst, entt::registry& src, const std::unordered_map<CUuid, entt::entity>& enttMap )
{
	auto view = src.view<Component>();
	for ( auto e : view )
	{
		CUuid uuid = src.get<CIDComponent>( e ).GetUUID();
		assert( enttMap.find( uuid ) != enttMap.end() );
		entt::entity dstEnttID = enttMap.at( uuid );

		auto& component = src.get<Component>( e );
		dst.emplace_or_replace<Component>( dstEnttID, component );
	}
}

template<typename Component>
static void CopyComponentIfExists( IEntity* dst, IEntity* src )
{
	if ( src->HasComponent<Component>() )
		dst->CreateOrReplaceComponent<Component>( *src->GetComponent<Component>() );
}

Balbino::CScene::~CScene()
{
	for ( std::pair<const unsigned, CEntity>& entityMap : m_entityMap )
	{
		m_registry.destroy( entityMap.second.GetEntity() );
	}
	m_entityMap.clear();
}

void Balbino::CScene::Initialize( uint32_t width, uint32_t height )
{
	m_viewportWidth = width;
	m_viewportHeight = height;
	CreateEntity()->CreateComponent<CCameraComponent>();
	//todo: PhysX bodies
	//m_registry.view<NativeScriptComponent>().each( [=]( auto entity, auto& nsc )
	//{
	//	if ( !nsc.Instance )
	//	{
	//		nsc.Instance = nsc.InstantiateScript();
	//		nsc.Instance->m_Entity = Entity{ entity, this };
	//		nsc.Instance->OnCreate();
	//	}
	//
	//	nsc.Instance->OnUpdate( ts );
	//} );
}

void Balbino::CScene::Cleanup()
{
	//todo: Destroy/Stop PhysX
	//const int32_t velocityIterations = 6;
	//const int32_t positionIterations = 2;
	//m_PhysicsWorld->Step( ts, velocityIterations, positionIterations );

	//// Retrieve transform from Box2D
	//auto view = m_registry.view<Rigidbody2DComponent>();
	//for ( auto e : view )
	//{
	//	Entity entity = { e, this };
	//	auto& transform = entity.GetComponent<TransformComponent>();
	//	auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();
	//
	//	b2Body* body = ( b2Body* ) rb2d.RuntimeBody;
	//	const auto& position = body->GetPosition();
	//	transform.Translation.x = position.x;
	//	transform.Translation.y = position.y;
	//	transform.Rotation.z = body->GetAngle();
	//}
}

void Balbino::CScene::PhysicsUpdate( const float deltaTime )
{
	( void ) deltaTime;
	//todo: Update PhysX
}

void Balbino::CScene::Update( const float deltaTime )
{
	( void ) deltaTime;
}

void Balbino::CScene::Draw() const
{
	//const CCamera* mainCamera = nullptr;
	//glm::mat4 cameraTransform;
	//{
	//	const auto view = m_registry.view<CTransformComponent, CCameraComponent>();
	//	for ( const auto entity : view )
	//	{
	//		auto [transform, camera] = view.get<CTransformComponent, CCameraComponent>( entity );

	//		if ( camera.GetIsPrimary() )
	//		{
	//			mainCamera = &camera.GetCamera();
	//			cameraTransform = transform.GetTransform();
	//			break;
	//		}
	//	}
	//}
	//if ( mainCamera )
	//{
	//	// Draw Meshed
	//	{
	//		//auto group = m_registry.group<CTransformComponent>( entt::get<CMeshRendererComponent> );
	//		//for ( auto entity : group )
	//		//{
	//		//	auto [transform, sprite] = group.get<CTransformComponent, CMeshRendererComponent>( entity );

	//		//}
	//	}

	//	// Draw UI
	//	{
	//		//Todo: draw UI
	//	}

	//}
}

void Balbino::CScene::OnViewportResize( uint32_t w, uint32_t h )
{
	m_viewportWidth = w;
	m_viewportHeight = h;

	// Resize our non-FixedAspectRatio cameras
	const auto view = m_registry.view<CCameraComponent>();
	for ( const auto entity : view )
	{
		auto& cameraComponent = view.get<CCameraComponent>( entity );
			cameraComponent.GetCamera().Initialize( w, h);
	}
}

IEntity* Balbino::CScene::CreateEntity()
{
	return CreateEntityWithUUID( {} );
}

IEntity* Balbino::CScene::CreateEntityWithUUID( CUuid uuid )
{
	const entt::entity ent = m_registry.create();
	CEntity entity = CEntity{ ent, this };
	entity.CreateComponent<CIDComponent>( uuid );
	entity.CreateComponent<CTransformComponent>( glm::vec3{} );
	return &(m_entityMap[static_cast<uint32_t>( ent )] = entity);
}

void Balbino::CScene::DestroyEntity( IEntity* entity )
{
	const uint32_t entId { (uint32_t)entity->GetEntity() };
	m_registry.destroy( entity->GetEntity() );
	m_entityMap.erase( entId );
}

void Balbino::CScene::DuplicateEntity( IEntity* entity )
{
	IEntity* newEntity = CreateEntity();

	CopyComponentIfExists<CTransformComponent>( newEntity, entity );
	CopyComponentIfExists<CCameraComponent>( newEntity, entity );
}

IEntity* Balbino::CScene::GetPrimaryCameraEntity()
{
	const auto view = m_registry.view<CCameraComponent>();
	for ( const auto entity : view )
	{
		const CCameraComponent& camera = view.get<CCameraComponent>( entity );
		if ( camera.IsPrimary())
			return &m_entityMap.at(static_cast<uint32_t>( static_cast<entt::entity>( entity ) ));
	}
	return nullptr;
}

template <typename T>
void Balbino::CScene::OnComponentAdded( CEntity entity, T& component )
{
	( void ) entity;
	( void ) component;
}

template <>
void Balbino::CScene::OnComponentAdded<CCameraComponent>( CEntity entity, CCameraComponent& component )
{
	( void ) entity;
	if ( m_viewportWidth > 0 && m_viewportHeight > 0 )
		component.GetCamera().Initialize( m_viewportWidth, m_viewportHeight );
}

const entt::registry& Balbino::CScene::GetRegistry() const
{
	return m_registry;
}

entt::registry& Balbino::CScene::GetRegistry()
{
	return m_registry;
}

uint32_t Balbino::CScene::GetViewportWidth() const
{
	return m_viewportWidth;
}

uint32_t Balbino::CScene::GetViewportHeight() const
{
	return m_viewportHeight;
}

std::vector<IEntity*> Balbino::CScene::GetAllEntities()
{
	std::vector<IEntity*> entities( m_entityMap.size(), nullptr);
	int i{};
	for ( auto& [fst, snd] : m_entityMap)
	{
		entities[i++] = &snd;
	}
	return entities;
}