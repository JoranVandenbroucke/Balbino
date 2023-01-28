#include "SceneHierarchy.h"

#include <IEntity.h>
#include <IScene.h>

#include "FileParcer.h"

#include "Components/IDComponent.h"
#include "Components/TransformComponent.h"
#include "Components/MeshRendererComponent.h"
#include "Components/CameraComponent.h"
#include "Components/LightComponent.h"

#include "IResourceManager.h"
#include "Material.h"

#include "../EditorGUI/EditorGui.h"

FawnForge::CSceneHierarchy::CSceneHierarchy()
        : m_pContext{ nullptr }
          , m_pSelectionContext{ nullptr }
          , m_pSystem{ nullptr }
          , m_isVisible{ true }
{
}

void FawnForge::CSceneHierarchy::Draw()
{
    if ( GUI::Begin( "Scene Hierarchy", m_isVisible, 0 ))
    {
        if ( m_pContext )
        {
            const auto entities = m_pContext->GetAllEntities();
            for ( const auto& entity : entities )
            {
                DrawEntityNode( entity );
            }
            
            if ( GUI::IsMouseDown( 0 ) && GUI::IsWindowHovered())
            {
                m_pSelectionContext = {};
            }
            
            // Right-click on blank space
            uint64_t selectedOption{ GUI::DrawPopupContextWindow( "Right Click", { "Create Empty" } ) };
            if ( selectedOption == 0 )
            {
                m_pContext->CreateEntity();
            }
        }
        if ( GUI::Begin( "Properties", m_isVisible, 0 ))
        {
            if ( m_pSelectionContext )
            {
                DrawComponents( m_pSelectionContext );
                if ( GUI::BeginPopup( "AddComponent" ))
                {
                    DisplayAddComponentEntry<CCameraComponent>( "Camera", m_pSystem );
                    DisplayAddComponentEntry<CLightComponent>( "Light" );
                    //DisplayAddComponentEntry<CMeshRenderComponent>( "MeshRender" );
                    
                    GUI::EndPopup();
                }
            }
            GUI::End();
        }
        GUI::End();
    }
}

void FawnForge::CSceneHierarchy::SetContext( IScene* pScene, ISystem* pSystem )
{
    m_pContext = pScene;
    m_pSystem  = pSystem;
}

void FawnForge::CSceneHierarchy::ShowWindow()
{
    m_isVisible = true;
    GUI::SetWindowFocus( "Scene Hierarchy" );
    GUI::SetWindowFocus( "Properties" );
}

IEntity* FawnForge::CSceneHierarchy::GetSelectedEntity() const
{
    return m_pSelectionContext;
}

template<typename T, typename UIFunction>
static void DrawComponent( const std::string& name, IEntity* entity, UIFunction uiFunction )
{
    constexpr int treeNodeFlags = 1 << 1 | 1 << 2 | 1 << 5 | 1 << 10 | 1 << 11;
    if ( entity->HasComponent<T>())
    {
        auto            component              = entity->GetComponent<T>();
        const glm::vec2 contentRegionAvailable = FawnForge::GUI::GetContentRegionAvail();
        
        FawnForge::GUI::PushStyleVar( 5, glm::vec2{ 4, 4 } );
        const float lineHeight = FawnForge::GUI::GetLineHeight();
        FawnForge::GUI::Separator();
        const bool open = FawnForge::GUI::TreeNodeEx((uint64_t) typeid( T ).hash_code(), treeNodeFlags, name.c_str());
        FawnForge::GUI::PopStyleVar();
        FawnForge::GUI::SameLine( contentRegionAvailable.x - lineHeight * 0.5f );
        if ( FawnForge::GUI::DrawButton( "+", glm::vec2{ lineHeight, lineHeight } ))
        {
            FawnForge::GUI::StartPopup( "ComponentSettings", false );
        }
        
        bool removeComponent = false;
        if ( FawnForge::GUI::BeginPopup( "ComponentSettings" ))
        {
            if ( FawnForge::GUI::MenuItem( "Remove component" ))
            {
                removeComponent = true;
            }
            
            FawnForge::GUI::EndPopup();
        }
        
        if ( open )
        {
            uiFunction( component );
            FawnForge::GUI::TreePop();
        }
        
        if ( removeComponent )
        {
            entity->RemoveComponent<T>();
        }
    }
}

void FawnForge::CSceneHierarchy::DrawEntityNode( IEntity* pEntity )
{
    CIDComponent* pIdComponent;
    pIdComponent = pEntity->GetComponent<CIDComponent>();
    if ( !pIdComponent )
    {
        return;
    }
    
    const std::string tag    = std::to_string( static_cast<uint64_t>( pIdComponent->GetUUID()));
    //todo if children but closed, skip
    const uint32_t    childCount{ pEntity->GetComponent<CTransformComponent>()->GetChildCount() };
    const int         flags  = ( m_pSelectionContext == pEntity ? 1 << 0 : 0 ) | 1 << 7 |
                               1 << 11 | ( !childCount ? 1 << 8 : 0 );
    const bool        opened = GUI::TreeNodeEx( static_cast<int>( *pEntity ), flags, tag.c_str());
    if ( GUI::IsItemClicked())
    {
        m_pSelectionContext = pEntity;
    }
    
    bool entityDeleted = false;
    if ( GUI::BeginPopupContextItem())
    {
        if ( GUI::MenuItem( "Delete Entity" ))
        {
            entityDeleted = true;
        }
        
        GUI::EndPopup();
    }
    
    if ( opened )
    {
        if ( childCount )
        {
            constexpr int treeFlags = 1 << 7 | 1 << 11;
            if ( GUI::TreeNodeEx( 9817239, treeFlags, tag.c_str()))
            {
                GUI::TreePop();
            }
        }
        GUI::TreePop();
    }
    
    if ( entityDeleted )
    {
        m_pContext->DestroyEntity( pEntity );
        if ( m_pSelectionContext == pEntity )
        {
            m_pSelectionContext = nullptr;
        }
    }
}

void FawnForge::CSceneHierarchy::DrawComponents( IEntity* pEntity ) const
{
    (void) pEntity;
    DrawComponent<CTransformComponent>(
            "Transform", pEntity, []( auto component )
            {
                glm::vec3 translation = component->GetTranslation();
                glm::vec3 rotation    = glm::degrees( glm::eulerAngles( component->GetRotation()));
                glm::vec3 scale       = component->GetScale();
                
                float tra[3]{ translation.x, translation.y, translation.z };
                float rot[3]{ rotation.x, rotation.y, rotation.z };
                float sca[3]{ scale.x, scale.y, scale.z };
                
                GUI::DrawFloat3( "Translation", tra, 0.01f );
                GUI::DrawFloat3( "Rotation", rot, 0.01f );
                GUI::DrawFloat3( "Scale", sca, 0.01f, 1 );
                
                component->SetScale( glm::vec3{ sca[0], sca[1], sca[2] } );
                component->SetRotation( glm::quat( radians( glm::vec3{ rot[0], rot[1], rot[2] } )));
                component->SetTranslation( glm::vec3{ tra[0], tra[1], tra[2] } );
            }
    );
    
    DrawComponent<CCameraComponent>(
            "Camera", pEntity, []( auto component )
            {
                float fov  = glm::degrees( component->GetFov());
                float near = component->GetNearClip();
                float far  = component->GetFarClip();
                bool  changed{};
                changed |= GUI::DrawFloat( "Vertical FOV", fov, 0.1f );
                changed |= GUI::DrawFloat( "Near", near, 0.01f );
                changed |= GUI::DrawFloat( "Far", far, 0.01f );
                if ( changed )
                {
                    component->SetFov( glm::radians( fov ));
                    component->SetNearClip( near );
                    component->SetFarClip( far );
                }
            }
    );
    
    DrawComponent<CMeshRenderComponent>(
            "Mesh Render Component", pEntity, [ & ]( auto component )
            {
                const CUuid meshID = component->GetMeshId();
                const std::vector<CUuid>                     & materials       = component->GetMaterials();
                const std::map<uint64_t, Balbino::CMaterial*>& loadedMaterials = m_pContext->GetSystem()->GetResourceManager()->GetAllLoadedMaterials();
                GUI::DrawText(( "mesh ID: " + std::to_string((uint64_t) meshID )).c_str());
                for ( int i{}; i < materials.size(); ++i )
                {
                    GUI::DrawText(( "material ID: " + std::to_string((uint64_t) materials[i] )).c_str());
                    if ( const void* payload = GUI::ReceiveDragDrop( ToString( EFileTypes::Material )))
                    {
                        component->SetMaterial( i, ((SFile*) payload )->uuid );
                    }
                }
                for ( auto& loadedMaterial : loadedMaterials )
                {
                    GUI::Separator();
                    const auto                                   & mat       = loadedMaterial.second;
                    const std::vector<FawnVision::SShaderResource>& resources = mat->GetShaderResourcesVector();
                    for ( const auto                             & resource : resources )
                    {
                        GUI::DrawText( resource.name.c_str());
                    }
                }
            }
    );
    
    DrawComponent<CLightComponent>(
            "Light Component", pEntity, [ & ]( auto component )
            {
                float                     strength{ component->GetStrength() };
                glm::vec3                 size{ component->GetSize() };
                glm::vec3                 color{ component->GetColor() };
                Balbino::ELightType::Enum type{ component->GetType() };
                
                float    colorFloat[3]{ color.r, color.g, color.b };
                uint64_t typeName{ uint64_t( type ) };
                uint64_t typeNameDup{};
                typeNameDup = typeName;
                
                bool hasChanged{};
                hasChanged |= GUI::DrawFloat3( "Color", colorFloat, 0.01f );
                hasChanged |= GUI::DrawFloat( "Strength", strength, 0.01f );
                hasChanged |= GUI::DrawComboBox( "Light Type", typeName, { "Directional", "Point", "Spot", "Area" } );
                
                switch ( type )
                {
                    case Balbino::ELightType::Directional:
                    case Balbino::ELightType::Point:
                        hasChanged |= GUI::DrawFloat( "Size", size.r, 0.01f );
                        break;
                    case Balbino::ELightType::Spot:
                        hasChanged |= GUI::DrawFloat( "Size", size.r, 0.01f );
                        hasChanged |= GUI::DrawFloat( "Near Radius", size.g, 0.01f );
                        hasChanged |= GUI::DrawFloat( "Far Radius", size.b, 0.01f );
                        break;
                    case Balbino::ELightType::Area:
                        hasChanged |= GUI::DrawFloat( "Size", size.r, 0.01f );
                        hasChanged |= GUI::DrawFloat( "Width", size.g, 0.01f );
                        hasChanged |= GUI::DrawFloat( "Height", size.b, 0.01f );
                        break;
                    case Balbino::ELightType::Max:
                        break;
                }
                
                if ( hasChanged )
                {
                    /*float size{ component->GetSize() };
                glm::vec3 strength{ component->GetStrength() };
                glm::vec3 color{ component->GetColor() };
                Balbino::ELightType type{ component->GetType() };
                     "Directional","Point","Spot","Area"*/
                    if ( size != component->GetSize())
                    {
                        component->SetSize( size );
                    }
                    if ( strength != component->GetStrength())
                    {
                        component->SetStrength( strength );
                    }
                    if ( color.r != colorFloat[0] || color.b != colorFloat[1] || color.b != colorFloat[2] )
                    {
                        component->SetColor( glm::vec3{ colorFloat[0], colorFloat[1], colorFloat[2] } );
                    }
                    
                    if ( typeName != typeNameDup )
                    {
                        type = Balbino::ELightType::Enum( typeName );
                        switch ( type )
                        {
                            case Balbino::ELightType::Directional:
                                size.g = size.b = 0;
                                break;
                            case Balbino::ELightType::Point:
                                size.g = size.b = 0;
                                break;
                            case Balbino::ELightType::Spot:
                                size.g = 0.01f;
                                size.b = 10.0f;
                                break;
                            case Balbino::ELightType::Area:
                                size.g = 1.0f;
                                break;
                            case Balbino::ELightType::Max:
                                size.b = 1.0f;
                                break;
                        }
                        component->SetSize( size );
                        component->SetType( type );
                    }
                }
            }
    );
    
    if ( GUI::DrawButton( "Add Component", { -1, 0 } ))
    {
        GUI::StartPopup( "AddComponent", false );
    }
}

template<typename T, typename ... Args>
void FawnForge::CSceneHierarchy::DisplayAddComponentEntry( const std::string& entryName, Args&& ... args )
{
    if ( !m_pSelectionContext->HasComponent<T>())
    {
        if ( GUI::MenuItem( entryName.c_str()))
        {
            m_pSelectionContext->AddComponent<T>( std::forward<Args>( args )... );
            GUI::CloseCurrentPopup();
        }
    }
}
