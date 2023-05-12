#include "SceneHierarchy.h"

#include <IEntity.h>

#include "FileParcer.h"

#include "Components/IDComponent.h"
#include "Components/TransformComponent.h"
#include "Components/MeshRendererComponent.h"
#include "Components/CameraComponent.h"
#include "Components/LightComponent.h"

#include "IResourceManager.h"
#include "Material.h"

#include "../Renderer/EditorGui.h"

FawnForge::CSceneHierarchy::CSceneHierarchy()
        : m_pContext{ nullptr }
        , m_pSelectionContext{ nullptr }
        , m_pSystem{ nullptr }
        , m_isVisible{ true }
{
}

void FawnForge::CSceneHierarchy::Draw()
{
    if ( Gui::Begin( "Scene Hierarchy", m_isVisible, 0 ))
    {
        if ( m_pContext )
        {
            const auto entities = m_pContext->GetAllEntities();
            for ( const auto& entity : entities )
            {
                DrawEntityNode( entity );
            }
            
            if ( Gui::IsMouseDown( 0 ) && Gui::IsWindowHovered())
            {
                m_pSelectionContext = {};
            }
            
            // Right-click on blank space
            uint64_t selectedOption{ Gui::DrawPopupContextWindow( "Right Click", { "Create Empty" } ) };
            if ( selectedOption == 0 )
            {
                m_pContext->CreateEntity();
            }
        }
        if ( Gui::Begin( "Properties", m_isVisible, 0 ))
        {
            if ( m_pSelectionContext )
            {
                DrawComponents( m_pSelectionContext );
                if ( Gui::BeginPopup( "AddComponent" ))
                {
                    DisplayAddComponentEntry<CCameraComponent>( "Camera", m_pSystem );
                    DisplayAddComponentEntry<CLightComponent>( "Light" );
                    //DisplayAddComponentEntry<CMeshRenderComponent>( "MeshRender" );
                    
                    Gui::EndPopup();
                }
            }
            Gui::End();
        }
        Gui::End();
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
    Gui::SetWindowFocus( "Scene Hierarchy" );
    Gui::SetWindowFocus( "Properties" );
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
        const glm::vec2 contentRegionAvailable = FawnForge::Gui::GetContentRegionAvail();
        
        FawnForge::Gui::PushStyleVar( 5, glm::vec2{ 4, 4 } );
        const float lineHeight = FawnForge::Gui::GetLineHeight();
        FawnForge::Gui::Separator();
        const bool open = FawnForge::Gui::TreeNodeEx((uint64_t) typeid( T ).hash_code(), treeNodeFlags, name );
        FawnForge::Gui::PopStyleVar();
        FawnForge::Gui::SameLine( contentRegionAvailable.x - lineHeight * 0.5f );
        if ( FawnForge::Gui::DrawButton( "+", glm::vec2{ lineHeight, lineHeight } ))
        {
            FawnForge::Gui::StartPopup( "ComponentSettings", false );
        }
        
        bool removeComponent = false;
        if ( FawnForge::Gui::BeginPopup( "ComponentSettings" ))
        {
            if ( FawnForge::Gui::MenuItem( "Remove component" ))
            {
                removeComponent = true;
            }
            
            FawnForge::Gui::EndPopup();
        }
        
        if ( open )
        {
            uiFunction( component );
            FawnForge::Gui::TreePop();
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
    const int         flags  = ( m_pSelectionContext == pEntity ? 1 << 0 : 0 ) | 1 << 7 | 1 << 11 | ( !childCount
                                                                                                      ? 1 << 8
                                                                                                      : 0 );
    const bool        opened = Gui::TreeNodeEx( static_cast<int>( *pEntity ), flags, tag );
    if ( Gui::IsItemClicked())
    {
        m_pSelectionContext = pEntity;
    }
    
    bool entityDeleted = false;
    if ( Gui::BeginPopupContextItem())
    {
        if ( Gui::MenuItem( "Delete Entity" ))
        {
            entityDeleted = true;
        }
        
        Gui::EndPopup();
    }
    
    if ( opened )
    {
        if ( childCount )
        {
            constexpr int treeFlags = 1 << 7 | 1 << 11;
            if ( Gui::TreeNodeEx( 9817239, treeFlags, tag ))
            {
                Gui::TreePop();
            }
        }
        Gui::TreePop();
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
                
                Gui::DrawFloat3( "Translation", tra, 0.01f );
                Gui::DrawFloat3( "Rotation", rot, 0.01f );
                Gui::DrawFloat3( "Scale", sca, 0.01f, 1 );
                
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
                changed |= Gui::DrawFloat( "Vertical FOV", fov, 0.1f );
                changed |= Gui::DrawFloat( "Near", near, 0.01f );
                changed |= Gui::DrawFloat( "Far", far, 0.01f );
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
                const CUuid meshId = component->GetMeshId();
                const std::vector<CUuid>            & materials       = component->GetMaterials();
                const std::map<uint64_t, CMaterial*>& loadedMaterials = m_pContext->GetSystem()->GetResourceManager()->GetAllLoadedMaterials();
                Gui::DrawText( "mesh ID: " + std::to_string((uint64_t) meshId ));
                for ( int i{}; i < materials.size(); ++i )
                {
                    Gui::DrawText( "material ID: " + std::to_string((uint64_t) materials[i] ));
                    if ( const void* payload = Gui::ReceiveDragDrop( ToString( file_type::file_type_material )))
                    {
                        component->SetMaterial( i, ((SFile*) payload )->uuid );
                    }
                }
                for ( auto& loadedMaterial : loadedMaterials )
                {
                    Gui::Separator();
                    const auto                                    & mat       = loadedMaterial.second;
                    const std::vector<FawnVision::SShaderResource>& resources = mat->GetShaderResourcesVector();
                    for ( const auto                              & resource : resources )
                    {
                        Gui::DrawText( resource.name );
                    }
                }
            }
    );
    
    DrawComponent<CLightComponent>(
            "Light Component", pEntity, [ & ]( auto component )
            {
                float     strength{ component->GetStrength() };
                glm::vec3 size{ component->GetSize() };
                glm::vec3 color{ component->GetColor() };
                
                float    colorFloat[3]{ color.r, color.g, color.b };
                uint64_t type{ uint64_t( component->GetType()) };
                uint64_t oldType{ type };
                
                bool hasChanged{};
                hasChanged |= Gui::DrawFloat3( "Colour", colorFloat, 0.01f );
                hasChanged |= Gui::DrawFloat( "Strength", strength, 0.01f );
                hasChanged |= Gui::DrawComboBox(
                        "Light GetType",
                        type,
                        { "Directional", "Point", "Spot", "Area" }
                );
                
                switch ( component->GetType())
                {
                    case light_type_directional:
                    case light_type_point:
                        hasChanged |= Gui::DrawFloat( "Size", size.r, 0.01f );
                        break;
                    case light_type_spot:
                        hasChanged |= Gui::DrawFloat( "Size", size.r, 0.01f );
                        hasChanged |= Gui::DrawFloat( "Near Radius", size.g, 0.01f );
                        hasChanged |= Gui::DrawFloat( "Far Radius", size.b, 0.01f );
                        break;
                    case light_type_area:
                        hasChanged |= Gui::DrawFloat( "Size", size.r, 0.01f );
                        hasChanged |= Gui::DrawFloat( "Width", size.g, 0.01f );
                        hasChanged |= Gui::DrawFloat( "Height", size.b, 0.01f );
                        break;
                    case light_type_max:
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
                    
                    if ( type != oldType )
                    {
                        type = light_type( type );
                        switch ( type )
                        {
                            case light_type_directional:
                            case light_type_point:
                                size.g = size.b = 0;
                                break;
                            case light_type_spot:
                                size.g = 0.01f;
                                size.b = 10.0f;
                                break;
                            case light_type_area:
                                size.g = 1.0f;
                                break;
                            case light_type_max:
                                size.b = 1.0f;
                                break;
                        }
                        component->SetSize( size );
                        component->SetType( light_type(type) );
                    }
                }
            }
    );
    
    if ( Gui::DrawButton( "Add Component", { -1, 0 } ))
    {
        Gui::StartPopup( "AddComponent", false );
    }
}

template<typename T, typename ... Args>
void FawnForge::CSceneHierarchy::DisplayAddComponentEntry( const std::string& entryName, Args&& ... args )
{
    if ( !m_pSelectionContext->HasComponent<T>())
    {
        if ( Gui::MenuItem( entryName ))
        {
            m_pSelectionContext->AddComponent<T>( std::forward<Args>( args )... );
            Gui::CloseCurrentPopup();
        }
    }
}
