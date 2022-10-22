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

BalEditor::CSceneHierarchy::CSceneHierarchy()
        : m_pContext{ nullptr },
          m_pSelectionContext{ nullptr },
          m_pSystem{ nullptr },
          m_isVisible{ true }
{
}

void BalEditor::CSceneHierarchy::Draw()
{
    if ( BalEditor::EditorGUI::Begin( "Scene Hierarchy", m_isVisible, 0 ))
    {
        if ( m_pContext )
        {
            const auto entities = m_pContext->GetAllEntities();
            for ( const auto& entity : entities )
            {
                DrawEntityNode( entity );
            }
            
            if ( BalEditor::EditorGUI::IsMouseDown( 0 ) && BalEditor::EditorGUI::IsWindowHovered())
            {
                m_pSelectionContext = {};
            }
            
            // Right-click on blank space
            int selectedOption{
                    BalEditor::EditorGUI::DrawPopupContextWindow( "Right Click", { "Create Empty" } )
            };
            if ( selectedOption == 0 )
            {
                m_pContext->CreateEntity();
            }
        }
        if ( BalEditor::EditorGUI::Begin( "Properties", m_isVisible, 0 ))
        {
            if ( m_pSelectionContext )
            {
                DrawComponents( m_pSelectionContext );
                if ( BalEditor::EditorGUI::BeginPopup( "AddComponent" ))
                {
                    DisplayAddComponentEntry<CCameraComponent>( "Camera", m_pSystem );
                    DisplayAddComponentEntry<CLightComponent>( "Light" );
                    //DisplayAddComponentEntry<CMeshRenderComponent>( "MeshRender" );
                    
                    BalEditor::EditorGUI::EndPopup();
                }
            }
            BalEditor::EditorGUI::End();
        }
        BalEditor::EditorGUI::End();
    }
}

void BalEditor::CSceneHierarchy::SetContext( IScene* pScene, ISystem* pSystem )
{
    m_pContext = pScene;
    m_pSystem  = pSystem;
}

void BalEditor::CSceneHierarchy::ShowWindow()
{
    m_isVisible = true;
    BalEditor::EditorGUI::SetWindowFocus( "Scene Hierarchy" );
    BalEditor::EditorGUI::SetWindowFocus( "Properties" );
}

IEntity* BalEditor::CSceneHierarchy::GetSelectedEntity() const
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
        const glm::vec2 contentRegionAvailable = BalEditor::EditorGUI::GetContentRegionAvail();
        
        BalEditor::EditorGUI::PushStyleVar( 5, glm::vec2{ 4, 4 } );
        const float lineHeight = BalEditor::EditorGUI::GetLineHeight();
        BalEditor::EditorGUI::Separator();
        const bool open = BalEditor::EditorGUI::TreeNodeEx((uint64_t) typeid( T ).hash_code(), treeNodeFlags,
                                                           name.c_str());
        BalEditor::EditorGUI::PopStyleVar();
        BalEditor::EditorGUI::SameLine( contentRegionAvailable.x - lineHeight * 0.5f );
        if ( BalEditor::EditorGUI::DrawButton( "+", glm::vec2{ lineHeight, lineHeight } ))
        {
            BalEditor::EditorGUI::StartPopup( "ComponentSettings", false );
        }
        
        bool removeComponent = false;
        if ( BalEditor::EditorGUI::BeginPopup( "ComponentSettings" ))
        {
            if ( BalEditor::EditorGUI::MenuItem( "Remove component" ))
            {
                removeComponent = true;
            }
            
            BalEditor::EditorGUI::EndPopup();
        }
        
        if ( open )
        {
            uiFunction( component );
            BalEditor::EditorGUI::TreePop();
        }
        
        if ( removeComponent )
        {
            entity->RemoveComponent<T>();
        }
    }
}

void BalEditor::CSceneHierarchy::DrawEntityNode( IEntity* pEntity )
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
    const int         flags  = ( m_pSelectionContext == pEntity ? 1 << 0 : 0 ) |
                               1 << 7 | 1 << 11 | ( !childCount ? 1 << 8 : 0 );
    const bool        opened = BalEditor::EditorGUI::TreeNodeEx( static_cast<int>( *pEntity ), flags, tag.c_str());
    if ( BalEditor::EditorGUI::IsItemClicked())
    {
        m_pSelectionContext = pEntity;
    }
    
    bool entityDeleted = false;
    if ( BalEditor::EditorGUI::BeginPopupContextItem())
    {
        if ( BalEditor::EditorGUI::MenuItem( "Delete Entity" ))
        {
            entityDeleted = true;
        }
        
        BalEditor::EditorGUI::EndPopup();
    }
    
    if ( opened )
    {
        if ( childCount )
        {
            constexpr int treeFlags = 1 << 7 | 1 << 11;
            if ( BalEditor::EditorGUI::TreeNodeEx( 9817239, treeFlags, tag.c_str()))
            {
                BalEditor::EditorGUI::TreePop();
            }
        }
        BalEditor::EditorGUI::TreePop();
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

void BalEditor::CSceneHierarchy::DrawComponents( IEntity* pEntity ) const
{
    (void) pEntity;
    DrawComponent<CTransformComponent>( "Transform", pEntity, []( auto component )
    {
        glm::vec3 translation = component->GetTranslation();
        glm::vec3 rotation    = glm::degrees( glm::eulerAngles( component->GetRotation()));
        glm::vec3 scale       = component->GetScale();
        
        float tra[3]{ translation.x, translation.y, translation.z };
        float rot[3]{ rotation.x, rotation.y, rotation.z };
        float sca[3]{ scale.x, scale.y, scale.z };
        
        BalEditor::EditorGUI::DrawFloat3( "Translation", tra, 0.01f );
        BalEditor::EditorGUI::DrawFloat3( "Rotation", rot, 0.01f );
        BalEditor::EditorGUI::DrawFloat3( "Scale", sca, 0.01f, 1 );
        
        component->SetScale( glm::vec3{ sca[0], sca[1], sca[2] } );
        component->SetRotation( glm::quat( radians( glm::vec3{ rot[0], rot[1], rot[2] } )));
        component->SetTranslation( glm::vec3{ tra[0], tra[1], tra[2] } );
    } );
    
    DrawComponent<CCameraComponent>( "Camera", pEntity, []( auto component )
    {
        float fov  = glm::degrees( component->GetFov());
        float near = component->GetNearClip();
        float far  = component->GetFarClip();
        bool  changed{};
        changed |= BalEditor::EditorGUI::DrawFloat( "Vertical FOV", fov, 0.1f );
        changed |= BalEditor::EditorGUI::DrawFloat( "Near", near, 0.01f );
        changed |= BalEditor::EditorGUI::DrawFloat( "Far", far, 0.01f );
        if ( changed )
        {
            component->SetFov( glm::radians( fov ));
            component->SetNearClip( near );
            component->SetFarClip( far );
        }
    } );
    
    DrawComponent<CMeshRenderComponent>( "Mesh Render Component", pEntity, [ & ]( auto component )
    {
        const CUuid meshID = component->GetMeshId();
        const std::vector<CUuid>                     & materials       = component->GetMaterials();
        const std::map<uint64_t, Balbino::CMaterial*>& loadedMaterials = m_pContext->GetSystem()->GetResourceManager()->GetAllLoadedMaterials();
        BalEditor::EditorGUI::DrawText(( "mesh ID: " + std::to_string((uint64_t) meshID )).c_str());
        for ( int i{}; i < materials.size(); ++i )
        {
            BalEditor::EditorGUI::DrawText(( "material ID: " + std::to_string((uint64_t) materials[i] )).c_str());
            if ( const void* payload = BalEditor::EditorGUI::ReceiveDragDrop( ToString( EFileTypes::Material )))
            {
                component->SetMaterial( i, ((SFile*) payload )->uuid );
            }
        }
        for ( auto& loadedMaterial : loadedMaterials )
        {
            BalEditor::EditorGUI::Separator();
            const auto                                                       & mat       = loadedMaterial.second;
            const std::unordered_map<std::string, BalVulkan::SShaderResource>& resources = mat->GetShaderResourcesMap();
            for ( const auto                                                 & resource : resources )
            {
                BalEditor::EditorGUI::DrawText( resource.first.c_str());
            }
        }
    } );
    
    DrawComponent<CLightComponent>( "Light Component", pEntity, [ & ]( auto component )
    {
        float               strength{ component->GetStrength() };
        glm::vec3           size{ component->GetSize() };
        glm::vec3           color{ component->GetColor() };
        Balbino::ELightType type{ component->GetType() };
        
        float       colorFloat[3]{ color.r, color.g, color.b };
        std::string typeName{};
        std::string typeNameDup{};
        switch ( type )
        {
            case Balbino::ELightType::Directional:
                typeName = "Directional";
                break;
            case Balbino::ELightType::Point:
                typeName = "Point";
                break;
            case Balbino::ELightType::Spot:
                typeName = "Spot";
                break;
            case Balbino::ELightType::Area:
                typeName = "Area";
                break;
            case Balbino::ELightType::Max:
                break;
        }
        typeNameDup = typeName;
        
        bool hasChanged{};
        hasChanged |= BalEditor::EditorGUI::DrawFloat3( "Color", colorFloat, 0.01f );
        hasChanged |= BalEditor::EditorGUI::DrawFloat( "Strength", strength, 0.01f );
        hasChanged |= BalEditor::EditorGUI::DrawComboBox( "Light Type", typeName,
                                                          { "Directional", "Point", "Spot", "Area" } );
        
        switch ( type )
        {
            case Balbino::ELightType::Directional:
            case Balbino::ELightType::Point:
                hasChanged |= BalEditor::EditorGUI::DrawFloat( "Size", size.r, 0.01f );
                break;
            case Balbino::ELightType::Spot:
                hasChanged |= BalEditor::EditorGUI::DrawFloat( "Size", size.r, 0.01f );
                hasChanged |= BalEditor::EditorGUI::DrawFloat( "Near Radius", size.g, 0.01f );
                hasChanged |= BalEditor::EditorGUI::DrawFloat( "Far Radius", size.b, 0.01f );
                break;
            case Balbino::ELightType::Area:
                hasChanged |= BalEditor::EditorGUI::DrawFloat( "Size", size.r, 0.01f );
                hasChanged |= BalEditor::EditorGUI::DrawFloat( "Width", size.g, 0.01f );
                hasChanged |= BalEditor::EditorGUI::DrawFloat( "Height", size.b, 0.01f );
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
                if ( typeName == "Directional" )
                {
                    type = Balbino::ELightType::Directional;
                    size.g = size.b = 0;
                }
                else if ( typeName == "Point" )
                {
                    type = Balbino::ELightType::Point;
                    size.g = size.b = 0;
                }
                else if ( typeName == "Spot" )
                {
                    type = Balbino::ELightType::Spot;
                    size.g = 0.01f;
                    size.b = 10.0f;
                }
                
                else if ( typeName == "Area" )
                {
                    type = Balbino::ELightType::Area;
                    size.g = 1.0f;
                    size.b = 1.0f;
                }
                component->SetSize( size );
                component->SetType( type );
            }
        }
    } );
    
    if ( BalEditor::EditorGUI::DrawButton( "Add Component", { -1, 0 } ))
    {
        BalEditor::EditorGUI::StartPopup( "AddComponent", false );
    }
}

template<typename T, typename ... Args>
void BalEditor::CSceneHierarchy::DisplayAddComponentEntry( const std::string& entryName, Args&& ... args )
{
    if ( !m_pSelectionContext->HasComponent<T>())
    {
        if ( BalEditor::EditorGUI::MenuItem( entryName.c_str()))
        {
            m_pSelectionContext->AddComponent<T>( std::forward<Args>( args )... );
            BalEditor::EditorGUI::CloseCurrentPopup();
        }
    }
}
