#include <scr/Renderer/Mesh.h>
#include "Scene.h"
#include "Buffer.h"

#include "../Managers/Manager.h"
#include "../Managers/ResourceManager.h"
#include "Camera.h"

#include "Components/CameraComponent.h"
#include "Components/IDComponent.h"
#include "Components/LightComponent.h"
#include "Components/MeshRendererComponent.h"
#include "Components/TransformComponent.h"

template<>
void IScene::OnComponentAdded<CIDComponent>( IEntity* entity, CIDComponent& component )
{
    (void) entity;
    (void) component;
}

template<>
void IScene::OnComponentAdded<CTransformComponent>( IEntity* entity, CTransformComponent& component )
{
    (void) entity;
    (void) component;
}

template<>
void IScene::OnComponentAdded<CCameraComponent>( IEntity* entity, CCameraComponent& component )
{
    const auto pScene = reinterpret_cast<const Balbino::CScene*>(entity->GetScene());
    (void) entity;
    if ( pScene->GetViewportWidth() > 0 && pScene->GetViewportHeight() > 0 )
    {
        component.GetCamera().Initialize( pScene->GetViewportWidth(), pScene->GetViewportHeight());
    }
}

template<typename Component>
static void CopyComponent( entt::registry& dst, entt::registry& src, const std::unordered_map<CUuid, entt::entity>& enttMap )
{
    auto       view = src.view<Component>();
    for ( auto e : view )
    {
        CUuid uuid = src.get<CIDComponent>( e ).GetUUID();
        assert( enttMap.find( uuid ) != enttMap.end());
        entt::entity dstEnttID = enttMap.at( uuid );

        auto& component = src.get<Component>( e );
        dst.emplace_or_replace<Component>( dstEnttID, component );
    }
}

template<typename Component>
static void CopyComponentIfExists( IEntity* dst, IEntity* src )
{
    if ( src->HasComponent<Component>())
    {
        dst->CreateOrReplaceComponent<Component>( *src->GetComponent<Component>());
    }
}

Balbino::CScene::CScene()
        : m_registry{},
          m_viewportWidth{},
          m_viewportHeight{},
          m_pMainCamera{},
          m_lightObject{},
          m_modelUbo{},
          m_entityMap{},
          m_pModelBuffer{},
          m_pShadingBuffer{},
          m_pCommandPool{},
          m_pDevice{},
          m_pQueue{},
          m_pSystem{},
          m_meshChangeDirty{}
{
}

Balbino::CScene::~CScene()
{
    for ( std::pair<const unsigned, CEntity>& entityMap : m_entityMap )
    {
        m_registry.destroy( entityMap.second.GetEntity());
    }
    m_entityMap.clear();
}

void Balbino::CScene::Initialize( ISystem* pSystem, uint32_t width, uint32_t height )
{
    m_viewportWidth  = width;
    m_viewportHeight = height;
    m_pSystem        = pSystem;
    CreateEntity()->AddComponent<CCameraComponent>();
    m_pModelBuffer    = BalVulkan::CBuffer::CreateNew( m_pDevice, m_pCommandPool, m_pQueue );
    m_pShadingBuffer  = BalVulkan::CBuffer::CreateNew( m_pDevice, m_pCommandPool, m_pQueue );
    m_pInstanceBuffer = BalVulkan::CBuffer::CreateNew( m_pDevice, m_pCommandPool, m_pQueue );

    m_pModelBuffer->Initialize( sizeof( SModelObject ), BalVulkan::EBufferUsageFlagBits::UniformBufferBit,
                                BalVulkan::EMemoryPropertyFlagBits::HostVisibleBit | BalVulkan::EMemoryPropertyFlagBits::HostCoherentBit );
    m_pShadingBuffer->Initialize( sizeof( SLightObject ), BalVulkan::EBufferUsageFlagBits::UniformBufferBit,
                                  BalVulkan::EMemoryPropertyFlagBits::HostVisibleBit | BalVulkan::EMemoryPropertyFlagBits::HostCoherentBit );
    m_pInstanceBuffer->Initialize( MAX_INSTANCE_COUNT * sizeof( BalVulkan::InstanceBatch ),
                                   BalVulkan::EBufferUsageFlagBits::TransferDstBit | BalVulkan::EBufferUsageFlagBits::VertexBufferBit,
                                   BalVulkan::EMemoryPropertyFlagBits::DeviceLocalBit );

    // todo: PhysX bodies
}

void Balbino::CScene::Cleanup()
{
    // todo: Destroy/Stop PhysX

    m_pShadingBuffer->Release();
    m_pModelBuffer->Release();
    m_pInstanceBuffer->Release();
}

void Balbino::CScene::PhysicsUpdate( const float deltaTime )
{
    (void) deltaTime;
    // todo: Update PhysX
}

void Balbino::CScene::Update( const float deltaTime )
{
    (void) deltaTime;
}

void Balbino::CScene::OnViewportResize( uint32_t w, uint32_t h )
{
    m_viewportWidth  = w;
    m_viewportHeight = h;

    // Resize our non-FixedAspectRatio cameras
    const auto       view = m_registry.view<CCameraComponent>();
    for ( const auto entity : view )
    {
        auto& cameraComponent = view.get<CCameraComponent>( entity );
        cameraComponent.GetCamera().Initialize( w, h );
    }
}

IEntity* Balbino::CScene::CreateEntity()
{
    return CreateEntityWithUUID( {} );
}

IEntity* Balbino::CScene::CreateEntityWithUUID( CUuid uuid )
{
    const entt::entity ent    = m_registry.create();
    auto               entity = CEntity{ ent, this };
    entity.AddComponent<CIDComponent>( uuid );
    entity.AddComponent<CTransformComponent>( glm::vec3{} );
    m_meshChangeDirty = true;
    return &( m_entityMap[static_cast<uint32_t>(ent)] = entity );
}

void Balbino::CScene::DestroyEntity( IEntity* entity )
{
    const uint32_t entId{ static_cast<uint32_t>(entity->GetEntity()) };
    m_registry.destroy( entity->GetEntity());
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
    const auto       view = m_registry.view<CCameraComponent>();
    for ( const auto entity : view )
    {
        const CCameraComponent& camera = view.get<CCameraComponent>( entity );
        if ( camera.IsPrimary())
        {
            return &m_entityMap.at( static_cast<uint32_t>(static_cast<entt::entity>(entity)));
        }
    }
    return nullptr;
}

template<typename T>
void Balbino::CScene::OnComponentAdded( CEntity entity, T& component )
{
    (void) entity;
    (void) component;
}

template<>
void Balbino::CScene::OnComponentAdded<CCameraComponent>( CEntity entity, CCameraComponent& component )
{
    (void) entity;
    if ( m_viewportWidth > 0 && m_viewportHeight > 0 )
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
    int                   i{};
    for ( auto& [ fst, snd ] : m_entityMap )
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

void Balbino::CScene::RecreateBuffers( BalVulkan::CCommandPool* commandPool, BalVulkan::CQueue* queue )
{
    m_pCommandPool = commandPool;
    m_pQueue       = queue;
    m_pShadingBuffer->Release();
    m_pModelBuffer->Release();
    m_pInstanceBuffer->Release();

    m_pModelBuffer    = BalVulkan::CBuffer::CreateNew( m_pDevice, m_pCommandPool, m_pQueue );
    m_pShadingBuffer  = BalVulkan::CBuffer::CreateNew( m_pDevice, m_pCommandPool, m_pQueue );
    m_pInstanceBuffer = BalVulkan::CBuffer::CreateNew( m_pDevice, m_pCommandPool, m_pQueue );

    m_pModelBuffer->Initialize( sizeof( SModelObject ), BalVulkan::EBufferUsageFlagBits::UniformBufferBit,
                                BalVulkan::EMemoryPropertyFlagBits::HostVisibleBit | BalVulkan::EMemoryPropertyFlagBits::HostCoherentBit );
    m_pShadingBuffer->Initialize( MAX_INSTANCE_COUNT * sizeof( SLightObject ),
                                  BalVulkan::EBufferUsageFlagBits::UniformBufferBit,
                                  BalVulkan::EMemoryPropertyFlagBits::HostVisibleBit | BalVulkan::EMemoryPropertyFlagBits::HostCoherentBit );
    m_pInstanceBuffer->Initialize( MAX_INSTANCE_COUNT * sizeof( BalVulkan::InstanceBatch ),
                                   BalVulkan::EBufferUsageFlagBits::TransferDstBit | BalVulkan::EBufferUsageFlagBits::VertexBufferBit,
                                   BalVulkan::EMemoryPropertyFlagBits::DeviceLocalBit );
}

void Balbino::CScene::PrepareDraw()
{
    m_pMainCamera = nullptr;
    glm::mat4 cameraTransform;
    {
        const auto& view = m_registry.view<CTransformComponent, CCameraComponent>();
        for ( const auto entity : view )
        {
            const auto& [ transform, camera ] = view.get( entity );

            if ( camera.IsPrimary())
            {
                m_pMainCamera   = &camera;
                cameraTransform = transform.GetTransform();
                (void) cameraTransform;
                break;
            }
        }
    }
    if ( m_pMainCamera )
    {
        m_modelUbo.view               = glm::inverse( cameraTransform );
        m_modelUbo.proj               = m_pMainCamera->GetProjection();
        m_modelUbo.viewPos            = cameraTransform[0];
        m_modelUbo.displayDebugTarget = 0;
        m_pModelBuffer->UpdateData( &m_modelUbo, sizeof( SModelObject ));

        const auto& lightGroup = m_registry.group<CTransformComponent>( entt::get<CLightComponent> );
        uint32_t id{};
        for ( const auto& entity : lightGroup )
        {
            auto [ transform, light ] = lightGroup.get<CTransformComponent, CLightComponent>( entity );
            m_lightObject.lights[id].type      = light.GetType();
            m_lightObject.lights[id].strength  = light.GetStrength();
            m_lightObject.lights[id].position  = transform.GetTranslation();
            m_lightObject.lights[id].direction = glm::vec3( 1, 0, 0 ) * transform.GetRotation();
            m_lightObject.lights[id].color     = light.GetColor();
            m_lightObject.lights[id].size      = light.GetSize();
            ++id;
            if ( id == LIGHT_COUNT )
            {
                break;
            }
        }
        if ( id < LIGHT_COUNT - 1 )
        {
            m_lightObject.lights[id] = SLight{
                    Balbino::ELightType::Directional, -1, { -1, -1, -1 }, { -1, -1, -1 }, { -1, -1, -1 }};
        }
        m_pShadingBuffer->UpdateData( &m_lightObject, sizeof( SLightObject ));

        // TODO CPU voxel Frustum culling
        // TODO GPU Frustum/Occlusion culling
//        if ( m_meshChangeDirty )
        {
            m_allDrawableObjects.clear();
            m_instanceData.clear();
            auto       objectGroup = m_registry.view<CTransformComponent, CMeshRenderComponent>();
            uint32_t   count{};
            for ( auto entity : objectGroup )
            {
                auto [ transform, mesh ] = objectGroup.get<CTransformComponent, CMeshRenderComponent>( entity );
                const CMesh* pModel    = dynamic_cast<CMesh*>( m_pSystem->GetResourceManager()->GetModel(
                        mesh.GetMeshId()));
                const auto & materials = mesh.GetMaterials();
                const auto & mat       = pModel->GetMetaData();
                const CUuid meshID = mesh.GetMeshId();

//                IndirectBatch firstDraw{
//                        .mesh = mesh.GetMeshId(),
//                        .material = materials[0],
//                        .firstInstance = count,
//                        .instanceCount = 1,
//                        .firstIndex = mat->firstIndex,
//                        .indexCount = mat->indexCount
//                };
//                m_allDrawableObjects.push_back( firstDraw );
//                m_instanceData.push_back( {{ transform.GetTransform() }} );
                const uint32_t size{ (uint32_t) materials.size() };
                for ( uint32_t i{}; i < size; i++ )
                {
                    //compare the mesh and material with the end of the vector of draws
                    const CUuid currentMaterial = materials[i];
                    auto        it              = std::find_if( m_allDrawableObjects.cbegin(),
                                                                m_allDrawableObjects.cend(),
                                                                [ meshID, currentMaterial, mat ]( const IndirectBatch& obj )
                                                                {
                                                                    return meshID == obj.mesh || currentMaterial == obj.material || mat->firstIndex == obj.material;
                                                                } );

                    if ( it != m_allDrawableObjects.cend())
                    {
                        //all matches, add count
                        m_allDrawableObjects.back().instanceCount++;
                        const uint32_t index{ static_cast<uint32_t>(it - m_allDrawableObjects.cbegin()) };
                        m_instanceData[index].emplace_back( transform.GetTransform());
                    }
                    else
                    {
                        //add new draw
                        IndirectBatch newDraw{
                                .mesh = meshID,
                                .material = currentMaterial,
                                .firstInstance = count,
                                .instanceCount = 1,
                                .firstIndex = mat->firstIndex,
                                .indexCount = mat->indexCount
                        };
                        m_allDrawableObjects.push_back( newDraw );
                        m_instanceData.push_back( {{ transform.GetTransform() }} );
                    }
                    ++count;
                }
            }
//            m_meshChangeDirty = false;
        }
    }
}

void Balbino::CScene::DepthPass()
{
    //if ( m_pMainCamera )
    //{
    //    // TODO enable vertexshader only
    //}
}

void Balbino::CScene::Draw()
{
    if ( m_pMainCamera )
    {
        for ( uint32_t i{}; i < m_allDrawableObjects.size(); ++i )
        {
            m_pInstanceBuffer->UpdateData( m_instanceData[i].data(), ( m_instanceData[i].size() < MAX_INSTANCE_COUNT
                                                                       ? m_instanceData[i].size()
                                                                       : MAX_INSTANCE_COUNT ) * sizeof( BalVulkan::InstanceBatch ));
            m_pSystem->GetResourceManager()->GetModel( m_allDrawableObjects[i].mesh )->Bind();
            m_pInstanceBuffer->Bind( false, true );
            if ( m_pSystem->GetResourceManager()->BindMaterial((uint64_t) m_allDrawableObjects[i].material ))
            {
                BalVulkan::DrawMesh( m_pCommandPool, m_allDrawableObjects[i].indexCount,
                                     m_allDrawableObjects[i].firstIndex, m_allDrawableObjects[i].firstInstance,
                                     m_allDrawableObjects[i].instanceCount );
            }
        }
    }
}

BalVulkan::CBuffer* Balbino::CScene::GetInstanceBuffer() const
{
    return m_pInstanceBuffer;
}
