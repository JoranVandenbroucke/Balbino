#include "Scene.h"
#include "Buffer.h"

#include "Camera.h"
#include "Common.h"
#include "Device.h"
#include "IManager.h"
#include "IResourceManager.h"
#include "../Renderer/Mesh.h"
#include "Components/CameraComponent.h"
#include "Components/IDComponent.h"
#include "Components/MeshRendererComponent.h"
#include "Components/TransformComponent.h"
#include "../Random.h"

#include "Entity.h"

template<>
void IScene::OnComponentAdded<CIDComponent>(IEntity* entity, CIDComponent& component)
{
    (void) entity;
    (void) component;
}

template<>
void IScene::OnComponentAdded<CTransformComponent>(IEntity* entity, CTransformComponent& component)
{
    (void) entity;
    (void) component;
}

template<>
void IScene::OnComponentAdded<CCameraComponent>(IEntity* entity, CCameraComponent& component)
{
    const auto pScene = reinterpret_cast< const Balbino::CScene* >( entity->GetScene());
    (void) entity;
    if( pScene->GetViewportWidth() > 0 && pScene->GetViewportHeight() > 0 )
    {
        component.GetCamera().Initialize( pScene->GetViewportWidth(), pScene->GetViewportHeight());
    }
}

template<typename Component>
static void CopyComponent(entt::registry& dst, entt::registry& src, const std::unordered_map<CUuid, entt::entity>& enttMap)
{
    auto view = src.view<Component>();
    for( auto e : view )
    {
        CUuid uuid = src.get<CIDComponent>( e ).GetUUID();
        assert( enttMap.find( uuid ) != enttMap.end());
        entt::entity dstEnttID = enttMap.at( uuid );

        auto& component = src.get<Component>( e );
        dst.emplace_or_replace<Component>( dstEnttID, component );
    }
}

template<typename Component>
static void CopyComponentIfExists(IEntity* dst, IEntity* src)
{
    if( src->HasComponent<Component>())
    {
        dst->CreateOrReplaceComponent<Component>( *src->GetComponent<Component>());
    }
}

Balbino::CScene::CScene()
        : m_registry()
          , m_viewportWidth()
          , m_viewportHeight()
          , m_sLightObject()
          , m_entityMap()
          , m_instanceBuffer()
          , m_pModelBuffer()
          , m_pShadingBuffer()
          , m_pCommandPool()
          , m_pDevice()
          , m_pQueue()
          , m_timer()
          , m_pSystem(){}

Balbino::CScene::~CScene()
{
    for( std::pair<const unsigned, CEntity>& entityMap : m_entityMap )
    {
        m_registry.destroy( entityMap.second.GetEntity());
    }
    m_entityMap.clear();
}

void Balbino::CScene::Initialize(ISystem* pSystem, uint32_t width, uint32_t height)
{
    m_viewportWidth = width;
    m_viewportHeight = height;
    m_pSystem = pSystem;
    CreateEntity()->CreateComponent<CCameraComponent>();
    m_pModelBuffer = BalVulkan::CBuffer::CreateNew( m_pDevice, m_pCommandPool, m_pQueue );
    m_pShadingBuffer = BalVulkan::CBuffer::CreateNew( m_pDevice, m_pCommandPool, m_pQueue );

    m_pModelBuffer->Initialize( sizeof( SModelObject ), BalVulkan::EBufferUsageFlagBits::UniformBufferBit, BalVulkan::EMemoryPropertyFlagBits::HostVisibleBit | BalVulkan::EMemoryPropertyFlagBits::HostCoherentBit );
    m_pShadingBuffer->Initialize( sizeof( SLightObject ), BalVulkan::EBufferUsageFlagBits::UniformBufferBit, BalVulkan::EMemoryPropertyFlagBits::HostVisibleBit | BalVulkan::EMemoryPropertyFlagBits::HostCoherentBit );
    CRandom rng{};
    for( int i{}; i < LIGHT_COUNT; ++i )
    {
        SLight light{};
        light.position = glm::vec4{ rng.Value() * 10 - 5.0f, rng.Value() * 10 - 5.0f, rng.Value() * 10 - 5.0f, 1.0f };
        light.color = glm::vec4{ rng.Value(), rng.Value(), rng.Value(), rng.Value() * 100.f};
        light.radius = rng.Value() * 100;
        m_sLightObject.lights[i] = light;
    }

    //todo: PhysX bodies
}

void Balbino::CScene::Cleanup()
{
    //todo: Destroy/Stop PhysX

    m_pShadingBuffer->Release();
    m_pModelBuffer->Release();
}

void Balbino::CScene::PhysicsUpdate(const float deltaTime)
{
    (void) deltaTime;
    //todo: Update PhysX
}

void Balbino::CScene::Update(const float deltaTime)
{
    (void) deltaTime;
    //m_timer += deltaTime;
}

void Balbino::CScene::Draw()
{
    const CCamera* mainCamera = nullptr;
    glm::mat4 cameraTransform;
    {
        const auto& view = m_registry.view<CTransformComponent, CCameraComponent>();
        for( const auto entity : view )
        {
            const auto& [transform, camera] = view.get( entity );

            if( camera.IsPrimary())
            {
                mainCamera = &camera.GetCamera();
                cameraTransform = transform.GetTransform();
                (void) cameraTransform;
                break;
            }
        }
    }
    if( mainCamera )
    {
        // Draw Meshed
        {
            SModelObject ubo{ .view = mainCamera->GetView(), .proj = mainCamera->GetProjection(), };
            m_sLightObject.viewPos = mainCamera->GetView()[3];
            m_sLightObject.displayDebugTarget = int(m_timer) % 4;
            m_pShadingBuffer->UpdateData(&m_sLightObject, sizeof(SLightObject) );

            const auto& group = m_registry.group<CTransformComponent>( entt::get<CMeshRenderComponent> );
            for( const auto& entity : group )
            {
                auto [transform, model] = group.get<CTransformComponent, CMeshRenderComponent>( entity );
                ubo.model = transform.GetTransform();

                m_pModelBuffer->UpdateData( &ubo, sizeof(ubo) );

                const CMesh* pModel = dynamic_cast<CMesh*>( m_pSystem->GetResourceManager()->GetModel( model.GetMeshId()));
                const auto& mat = model.GetMaterials();
                pModel->Bind();
                const auto& metaData = pModel->GetMetaData();
                const auto size = pModel->GetMaterialCount();
                for( uint64_t i{}; i < size; ++i )
                {
                    if( m_pSystem->GetResourceManager()->BindMaterial( static_cast<uint64_t>( mat[i] )))
                    {
                        BalVulkan::DrawMesh( m_pCommandPool, metaData[i].indexCount, metaData[i].indexStart );
                    }
                }
            }
        }
    }
}

void Balbino::CScene::OnViewportResize(uint32_t w, uint32_t h)
{
    m_viewportWidth = w;
    m_viewportHeight = h;

    // Resize our non-FixedAspectRatio cameras
    const auto view = m_registry.view<CCameraComponent>();
    for( const auto entity : view )
    {
        auto& cameraComponent = view.get<CCameraComponent>( entity );
        cameraComponent.GetCamera().Initialize( w, h );
    }
}

IEntity* Balbino::CScene::CreateEntity()
{
    return CreateEntityWithUUID( {} );
}

IEntity* Balbino::CScene::CreateEntityWithUUID(CUuid uuid)
{
    const entt::entity ent = m_registry.create();
    auto entity = CEntity{ ent, this };
    entity.CreateComponent<CIDComponent>( uuid );
    entity.CreateComponent<CTransformComponent>( glm::vec3{} );
    return &( m_entityMap[static_cast< uint32_t >( ent )] = entity );
}

void Balbino::CScene::DestroyEntity(IEntity* entity)
{
    const uint32_t entId{ static_cast< uint32_t >( entity->GetEntity()) };
    m_registry.destroy( entity->GetEntity());
    m_entityMap.erase( entId );
}

void Balbino::CScene::DuplicateEntity(IEntity* entity)
{
    IEntity* newEntity = CreateEntity();

    CopyComponentIfExists<CTransformComponent>( newEntity, entity );
    CopyComponentIfExists<CCameraComponent>( newEntity, entity );
}

IEntity* Balbino::CScene::GetPrimaryCameraEntity()
{
    const auto view = m_registry.view<CCameraComponent>();
    for( const auto entity : view )
    {
        const CCameraComponent& camera = view.get<CCameraComponent>( entity );
        if( camera.IsPrimary())
        {
            return &m_entityMap.at( static_cast< uint32_t >( static_cast< entt::entity >( entity )));
        }
    }
    return nullptr;
}

template<typename T>
void Balbino::CScene::OnComponentAdded(CEntity entity, T& component)
{
    (void) entity;
    (void) component;
}

template<>
void Balbino::CScene::OnComponentAdded<CCameraComponent>(CEntity entity, CCameraComponent& component)
{
    (void) entity;
    if( m_viewportWidth > 0 && m_viewportHeight > 0 )
    {
        component.GetCamera().Initialize( m_viewportWidth, m_viewportHeight );
    }
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
    std::vector<IEntity*> entities( m_entityMap.size(), nullptr );
    int i{};
    for( auto& [fst, snd] : m_entityMap )
    {
        entities[i++] = &snd;
    }
    return entities;
}

ISystem* Balbino::CScene::GetSystem() const
{
    return m_pSystem;
}

BalVulkan::CBuffer* Balbino::CScene::GetModelBuffer() const
{
    return m_pModelBuffer;
}

BalVulkan::CBuffer* Balbino::CScene::GetShadingBuffer() const
{
    return m_pShadingBuffer;
}

void Balbino::CScene::RecreateBuffers(BalVulkan::CCommandPool* commandPool, BalVulkan::CQueue* queue)
{
    m_pCommandPool = commandPool;
    m_pQueue = queue;
    m_pShadingBuffer->Release();
    m_pModelBuffer->Release();

    m_pModelBuffer = BalVulkan::CBuffer::CreateNew( m_pDevice, m_pCommandPool, m_pQueue );
    m_pShadingBuffer = BalVulkan::CBuffer::CreateNew( m_pDevice, m_pCommandPool, m_pQueue );

    m_pModelBuffer->Initialize( sizeof( SModelObject ), BalVulkan::EBufferUsageFlagBits::UniformBufferBit, BalVulkan::EMemoryPropertyFlagBits::HostVisibleBit | BalVulkan::EMemoryPropertyFlagBits::HostCoherentBit );
    m_pShadingBuffer->Initialize( sizeof( SLightObject ), BalVulkan::EBufferUsageFlagBits::UniformBufferBit, BalVulkan::EMemoryPropertyFlagBits::HostVisibleBit | BalVulkan::EMemoryPropertyFlagBits::HostCoherentBit );
}

void Balbino::CScene::PrepareDraw()
{
    //const CCamera* mainCamera = nullptr;
    //glm::mat4 cameraTransform;
    //{
    //	const auto view = m_registry.view<CTransformComponent, CCameraComponent>();
    //	for ( const auto entity : view )
    //	{
    //		auto [transform, camera] = view.get<CTransformComponent, CCameraComponent>( entity );

    //		if ( camera.IsPrimary() )
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
    //		std::vector<glm::mat4> instanceData;
    //		std::set<CUuid> instanceModelId;
    //		const auto group = m_registry.group_if_exists<CTransformComponent>( entt::get<CMeshRenderComponent> );
    //		for ( const auto& entity : group )
    //		{
    //			auto [transform, model] = group.get<CTransformComponent, CMeshRenderComponent>( entity );
    //			uint64_t currentSize{}instance;
    //			instanceModelId.emplace( model.GetMeshId());

    //		}
    //		m_instanceBuffer->Update();
    //	}
    //}
}
