#include "Scene.h"
#include <Renderer.h>
#include <Buffer.h>

#include <Mesh.h>

#include "Components/CameraComponent.h"
#include "Components/IDComponent.h"
#include "Components/LightComponent.h"
#include "Components/MeshRendererComponent.h"
#include "Components/TransformComponent.h"

#include "../Managers/ResourceManager.h"
#include "../Managers/System.h"

Balbino::CScene::CScene()
        : m_registry{}
        , m_camera{}
        , m_entityMap{}
        , m_pSystem{}
        , m_lightObject{}
        , m_modelUbo{}
        , m_pModelBuffer{ nullptr }
        , m_pShadingBuffer{ nullptr }
        , m_pInstanceBuffer{ nullptr }
        , m_pRenderer{ nullptr }
{
}

Balbino::CScene::~CScene()
{
    for ( std::pair<const unsigned, CEntity>& entityMap : m_entityMap )
    {
        m_registry.destroy( entityMap.second.GetEntity());
    }
    m_allDrawableObjects.clear();
    m_instanceData.clear();
    m_entityMap.clear();
}

template<typename Component>
static void CopyComponentIfExists( IEntity* dst, IEntity* src )
{
    if ( src->HasComponent<Component>())
    {
        dst->CreateOrReplaceComponent<Component>( *src->GetComponent<Component>());
    }
}

void Balbino::CScene::Initialize( ISystem* pSystem )
{
    m_pSystem = pSystem;
    CreateEntity()->AddComponent<CCameraComponent>( m_pSystem );
    m_pModelBuffer    = m_pRenderer->CreateBuffer(
            sizeof( SModelObject ),
            FawnVision::EBufferUsageFlagBits::UniformBufferBit,
            FawnVision::EMemoryPropertyFlagBits::Enum(
                    FawnVision::EMemoryPropertyFlagBits::HostVisibleBit | FawnVision::EMemoryPropertyFlagBits::HostCoherentBit
            ));
    m_pShadingBuffer  = m_pRenderer->CreateBuffer(
            sizeof( SLightObject ),
            FawnVision::EBufferUsageFlagBits::UniformBufferBit,
            FawnVision::EMemoryPropertyFlagBits::Enum(
                    FawnVision::EMemoryPropertyFlagBits::HostVisibleBit | FawnVision::EMemoryPropertyFlagBits::HostCoherentBit
            ));
    m_pInstanceBuffer = m_pRenderer->CreateBuffer(
            MAX_INSTANCE_COUNT * sizeof( FawnVision::InstanceBatch ), FawnVision::EBufferUsageFlagBits::Enum(
                    FawnVision::EBufferUsageFlagBits::TransferDstBit | FawnVision::EBufferUsageFlagBits::VertexBufferBit
            ), FawnVision::EMemoryPropertyFlagBits::Enum(
                    FawnVision::EMemoryPropertyFlagBits::HostVisibleBit | FawnVision::EMemoryPropertyFlagBits::DeviceLocalBit
            ));
}

void Balbino::CScene::Cleanup()
{
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
    const auto view          = m_registry.view<CCameraComponent>();
    const auto cameraManager = m_pSystem->GetCameraManager();
    
    for ( const auto entity : view )
    {
        const CCameraComponent& camera = view.get<CCameraComponent>( entity );
        if ( cameraManager->IsThisCameraActive( &camera ))
        {
            return &m_entityMap.at( static_cast<uint32_t>(static_cast<entt::entity>(entity)));
        }
    }
    return nullptr;
}

const entt::registry& Balbino::CScene::GetRegistry() const
{
    return m_registry;
}

entt::registry& Balbino::CScene::GetRegistry()
{
    return m_registry;
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

FawnVision::CBuffer* Balbino::CScene::GetModelBuffer() const
{
    return m_pModelBuffer;
}

FawnVision::CBuffer* Balbino::CScene::GetShadingBuffer() const
{
    return m_pShadingBuffer;
}

void Balbino::CScene::PrepareDraw()
{
    {
        const auto& view = m_registry.view<CTransformComponent, CCameraComponent>();
        const auto cameraManager = m_pSystem->GetCameraManager();
        
        for ( const auto entity : view )
        {
            const auto& [ transform, camera ] = view.get( entity );
            
            if ( cameraManager->IsThisCameraActive( &camera ))
            {
                m_camera.UpdateFrameBuffer(
                        m_pSystem->GetWindowWidth(),
                        m_pSystem->GetWindowHeight(),
                        camera.GetFov(),
                        camera.GetNearClip(),
                        camera.GetFarClip());
                m_camera.UpdateMatrices( transform.GetTranslation(), transform.GetRotation());
                break;
            }
        }
    }
    {
        m_modelUbo.view               = m_camera.GetView();
        m_modelUbo.proj               = m_camera.GetProjection();
        m_modelUbo.viewPos            = m_camera.GetViewPosition();
        m_modelUbo.displayDebugTarget = 0;      //todo change with dropdown
        m_pModelBuffer->UpdateData( &m_modelUbo, sizeof( SModelObject ));
        
        const auto& lightGroup = m_registry.group<CTransformComponent>( entt::get<CLightComponent> );
        uint32_t id{};
        for ( const auto& entity : lightGroup )
        {
            auto [ transform, light ] = lightGroup.get<CTransformComponent, CLightComponent>( entity );
            m_lightObject.lights[id].type      = (int) light.GetType();
            m_lightObject.lights[id].strength  = light.GetStrength();
            m_lightObject.lights[id].position  = transform.GetTranslation();
            m_lightObject.lights[id].direction = glm::rotate( transform.GetRotation(), { 0, 0, 1 } );
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
                    -1, -1, { -1, -1, -1 }, { -1, -1, -1 }, { -1, -1, -1 }, { -1, -1, -1 }};
        }
        m_pShadingBuffer->UpdateData( &m_lightObject, sizeof( SLightObject ) * id );
        
        // TODO CPU voxel Frustum culling
        // TODO GPU Frustum/Occlusion culling
        {
            m_allDrawableObjects.clear();
            m_instanceData.clear();
            auto       objectGroup = m_registry.view<CTransformComponent, CMeshRenderComponent>();
            uint32_t   count{};
            for ( auto entity : objectGroup )
            {
                auto [ transform, mesh ] = objectGroup.get<CTransformComponent, CMeshRenderComponent>( entity );
                const IMesh* pModel    = dynamic_cast<IMesh*>( m_pSystem->GetResourceManager()->GetModel(
                        mesh.GetMeshId()));
                const auto & materials = mesh.GetMaterials();
                const auto & mat       = pModel->GetMetaData();
                const CUuid meshId = mesh.GetMeshId();
                
                const uint32_t size{ (uint32_t) materials.size() };
                for ( uint32_t i{}; i < size; i++ )
                {
                    //compare the mesh and material with the end of the vector of draws
                    const CUuid currentMaterial = materials[i];
                    auto        it              = std::find_if(
                            m_allDrawableObjects.cbegin(),
                            m_allDrawableObjects.cend(),
                            [ meshId, currentMaterial, mat, i ]( const IndirectBatch& obj )
                            {
                                return meshId == obj.mesh && currentMaterial == obj.material && mat[i].firstIndex == obj.firstIndex;
                            }
                    );
                    
                    if ( it != m_allDrawableObjects.cend())
                    {
                        //all matches, add count
                        const uint32_t index{ static_cast<uint32_t>(it - m_allDrawableObjects.cbegin()) };
                        m_allDrawableObjects[index].instanceCount++;
                        m_instanceData[index].emplace_back( transform.GetTransform());
                    }
                    else
                    {
                        //add new draw
                        IndirectBatch newDraw{
                                .mesh = meshId, .material = currentMaterial, .firstInstance = 0, .instanceCount = 1, .firstIndex = mat[i].firstIndex, .indexCount = mat[i].indexCount
                        };
                        m_allDrawableObjects.push_back( newDraw );
                        m_instanceData.push_back( { FawnVision::InstanceBatch{ transform.GetTransform() }} );
                    }
                    ++count;
                }
            }
        }
    }
}

void Balbino::CScene::DepthPass()
{
    //if ( m_pMainCamera )
    //{
    //    // TODO enable vertexShader only
    //}
}

void Balbino::CScene::Draw()
{
    for ( uint32_t i{}; i < m_allDrawableObjects.size(); ++i )
    {
        m_pInstanceBuffer->UpdateData(
                m_instanceData[i].data(),
                ( m_instanceData[i].size() < MAX_INSTANCE_COUNT
                  ? m_instanceData[i].size()
                  : MAX_INSTANCE_COUNT ) * sizeof( FawnVision::InstanceBatch ));
        m_pSystem->GetResourceManager()->GetModel( m_allDrawableObjects[i].mesh )->Bind();
        m_pInstanceBuffer->Bind( false, true );
        if ( m_pSystem->GetResourceManager()->BindMaterial((uint64_t) m_allDrawableObjects[i].material ))
        {
            FawnVision::DrawMesh(
                    m_pRenderer->GetCommandPool(),
                    m_allDrawableObjects[i].indexCount,
                    m_allDrawableObjects[i].firstIndex,
                    m_allDrawableObjects[i].firstInstance,
                    m_allDrawableObjects[i].instanceCount
            );
        }
    }
}

FawnVision::CBuffer* Balbino::CScene::GetInstanceBuffer() const
{
    return m_pInstanceBuffer;
}
void Balbino::CScene::SetRenderer( FawnVision::CRenderer* renderer )
{
    m_pRenderer = renderer;
    if(!m_pShadingBuffer)
        return;
    m_pShadingBuffer->Release();
    m_pModelBuffer->Release();
    m_pInstanceBuffer->Release();
    
    m_pModelBuffer    = m_pRenderer->CreateBuffer(
            sizeof( SModelObject ),
            FawnVision::EBufferUsageFlagBits::UniformBufferBit,
            FawnVision::EMemoryPropertyFlagBits::Enum(
                    FawnVision::EMemoryPropertyFlagBits::HostVisibleBit | FawnVision::EMemoryPropertyFlagBits::HostCoherentBit
            ));
    m_pShadingBuffer  = m_pRenderer->CreateBuffer(
            sizeof( SLightObject ),
            FawnVision::EBufferUsageFlagBits::UniformBufferBit,
            FawnVision::EMemoryPropertyFlagBits::Enum(
                    FawnVision::EMemoryPropertyFlagBits::HostVisibleBit | FawnVision::EMemoryPropertyFlagBits::HostCoherentBit
            ));
    m_pInstanceBuffer = m_pRenderer->CreateBuffer(
            MAX_INSTANCE_COUNT * sizeof( FawnVision::InstanceBatch ), FawnVision::EBufferUsageFlagBits::Enum(
                    FawnVision::EBufferUsageFlagBits::TransferDstBit | FawnVision::EBufferUsageFlagBits::VertexBufferBit
            ), FawnVision::EMemoryPropertyFlagBits::Enum(
                    FawnVision::EMemoryPropertyFlagBits::HostVisibleBit | FawnVision::EMemoryPropertyFlagBits::DeviceLocalBit
            ));
}
