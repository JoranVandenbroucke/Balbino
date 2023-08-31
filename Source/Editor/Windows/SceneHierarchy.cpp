#include "SceneHierarchy.h"

#include "../Renderer/EditorGui.h"
#include <Scene.h>
#include <ISystem.h>
#include <FawnVision.h>
#include <CameraComponent.h>
#include <IDComponent.h>
#include <LightComponent.h>
#include <MeshRendererComponent.h>
#include <TransformComponent.h>

void FawnForge::CSceneHierarchy::Draw() noexcept
{
    if ( Gui::Begin( "Scene Hierarchy", m_isVisible, 0 ) )
    {
        if ( m_pScene )
        {
            for ( const auto entities = m_pScene->GetAllEntities(); const auto& entity : entities )

            {
                DrawEntityNode( entity );
            }

            if ( Gui::IsMouseDown( 0 ) && Gui::IsWindowHovered() )
            {
                m_selectedEntity = {};
            }

            // Right-click on blank space
            uint64_t selectedOption { Gui::PopupContextWindow( "Right Click", { "Create Empty" } ) };
            if ( selectedOption == 0 )
            {
                m_pScene->CreateEntity();
            }
        }
        if ( Gui::Begin( "Properties", m_isVisible, 0 ) )
        {
            if ( m_selectedEntity.IsValid() )
            {
                DrawComponents( m_selectedEntity );
                if ( Gui::BeginPopup( "AddComponent" ) )
                {
                    DisplayAddComponentEntry<BalbinoComponent::CCameraComponent>( "Camera" );
                    DisplayAddComponentEntry<BalbinoComponent::CLightComponent>( "Light" );
                    Gui::EndPopup();
                }
            }
            Gui::End();
        }
        Gui::End();
    }
}

void FawnForge::CSceneHierarchy::SetContext( const ISystem* pSystem ) noexcept
{
    m_pSystem  = pSystem;
    m_pScene = pSystem->GetCurrentActiveScene();
}

void FawnForge::CSceneHierarchy::ShowWindow() noexcept
{
    m_isVisible = true;
    Gui::SetWindowFocus( "Scene Hierarchy" );
    Gui::SetWindowFocus( "Properties" );
}

const BalbinoScene::CEntity& FawnForge::CSceneHierarchy::GetSelectedEntity() const
{
    return m_selectedEntity;
}
BalbinoScene::CEntity& FawnForge::CSceneHierarchy::GetSelectedEntity()
{
    return m_selectedEntity;
}

template<typename T, typename UIFunction>
constexpr static void DrawComponent( const std::string& name, BalbinoScene::CEntity& entity, UIFunction uiFunction )
{
    constexpr int treeNodeFlags = 1 << 1 | 1 << 2 | 1 << 5 | 1 << 10 | 1 << 11;
    if ( entity.HasComponent<T>() )
    {
        auto component                         = entity.GetComponent<T>();
        const BambiMath::Vector2 contentRegionAvailable = FawnForge::Gui::GetContentRegionAvail();

        FawnForge::Gui::PushStyleVar( 5, BambiMath::Vector2 { 4, 4 } );
        const float lineHeight = FawnForge::Gui::GetLineHeight();
        FawnForge::Gui::Separator();
        const bool open = FawnForge::Gui::TreeNodeEx( ( uint64_t ) typeid( T ).hash_code(), treeNodeFlags, name );
        FawnForge::Gui::PopStyleVar();
        FawnForge::Gui::SameLine( contentRegionAvailable.x - lineHeight * 0.5f );
        if ( FawnForge::Gui::Button( "+", BambiMath::Vector2 { lineHeight, lineHeight } ) )
        {
            FawnForge::Gui::StartPopup( "ComponentSettings", false );
        }

        bool removeComponent = false;
        if ( FawnForge::Gui::BeginPopup( "ComponentSettings" ) )
        {
            if ( FawnForge::Gui::MenuItem( "Remove component" ) )
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
            entity.RemoveComponent<T>();
        }
    }
}

void FawnForge::CSceneHierarchy::DrawEntityNode( const BalbinoScene::CEntity& entity ) noexcept
{
    const BalbinoComponent::CIDComponent& idComponent { entity.GetComponent<BalbinoComponent::CIDComponent>() };

    const std::string tag = idComponent.GetUUID().ToString();
    //todo:if children but closed, skip
    const uint32_t childCount { entity.GetComponent<BalbinoComponent::CTransformComponent>().GetChildCount() };
    const int flags   = ( m_selectedEntity == entity ? 1 << 0 : 0 ) | 1 << 7 | 1 << 11 | ( !childCount ? 1 << 8 : 0 );
    const bool opened = Gui::TreeNodeEx( std::hash<CUuid>()(idComponent.GetUUID()), flags, tag );
    if ( Gui::IsItemClicked() )
    {
        m_selectedEntity = entity;
    }

    bool entityDeleted = false;
    if ( Gui::BeginPopupContextItem() )
    {
        if ( Gui::MenuItem( "Delete Entity" ) )
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
            if ( Gui::TreeNodeEx( 9817239, treeFlags, tag ) )
            {
                Gui::TreePop();
            }
        }
        Gui::TreePop();
    }

    if ( entityDeleted )
    {
        m_pScene->DestroyEntity( entity );
        if ( m_selectedEntity == entity )
        {
            m_selectedEntity = {};
        }
    }
}

void FawnForge::CSceneHierarchy::DrawComponents( BalbinoScene::CEntity& pEntity )
{
    (void)pEntity;
    DrawComponent<BalbinoComponent::CTransformComponent>( "Transform", pEntity, []( auto component ) {
        BambiMath::Vector3 translation = component.GetTranslation();
        BambiMath::Vector3 rotation    = BambiMath::radianceToDegree * BambiMath::EulerAngles( component.GetRotation() );
        BambiMath::Vector3 scale       = component.GetScale();

        float tra[ 3 ] { translation.x, translation.y, translation.z };
        float rot[ 3 ] { rotation.x, rotation.y, rotation.z };
        float sca[ 3 ] { scale.x, scale.y, scale.z };

        Gui::Float3( "Translation", tra, 0.01f );
        Gui::Float3( "Rotation", rot, 0.01f );
        Gui::Float3( "Scale", sca, 0.01f, 1 );

        component.SetScale( BambiMath::Vector3 { sca[ 0 ], sca[ 1 ], sca[ 2 ] } );
        component.SetRotation( BambiMath::Quaternion ( radians( BambiMath::Vector3 { rot[ 0 ], rot[ 1 ], rot[ 2 ] } ) ) );
        component.SetTranslation( BambiMath::Vector3 { tra[ 0 ], tra[ 1 ], tra[ 2 ] } );
    } );

    DrawComponent<BalbinoComponent::CCameraComponent>( "Camera", pEntity, []( auto& component ) {
        float fov  = BambiMath::radianceToDegree * component.GetFov();
        float nearClip = component.GetNearClip();
        float farClip  = component.GetFarClip();
        bool changed {};
        changed |= Gui::Float( "Vertical FOV", fov, 0.1f );
        changed |= Gui::Float( "Near", nearClip, 0.01f );
        changed |= Gui::Float( "Far", farClip, 0.01f );
        if ( changed )
        {
            component.SetFov( BambiMath::degreeToRadiance * fov );
            component.SetNearClip( nearClip );
            component.SetFarClip( farClip );
        }
    } );

    DrawComponent<BalbinoComponent::CMeshRenderComponent>( "Mesh Render Component", pEntity, [ & ]( auto& component ) {
        const CUuid meshId                                                = component.GetMeshId();
        const std::vector<CUuid>& materials                               = component.GetMaterials();
        Gui::Text( std::format( "Mesh ID: {}", meshId.ToString() ) );
        for ( int i {}; i < materials.size(); ++i )
        {
            Gui::Text( std::format( "Material ID: {}", materials[ i ].ToString() ) );
            if ( const void* payload = Gui::ReceiveDragDrop( ToString( file_type::material ) ) )
            {
                component.SetMaterial( i, ( (const SFile*)( payload ) )->uuid );
            }
        }
    } );

    DrawComponent<BalbinoComponent::CLightComponent>( "Light Component", pEntity, []( auto& component ) {
        float strength { component.GetStrength() };
        BambiMath::Vector3 size { component.GetSize() };
        BambiMath::Vector3 color { component.GetColor() };

        float colorFloat[ 3 ] { color.r, color.g, color.b };
        uint64_t type { uint64_t( component.GetType() ) };
        const uint64_t oldType { type };

        bool hasChanged {};
        hasChanged |= Gui::Float3( "Colour", colorFloat, 0.01f );
        hasChanged |= Gui::Float( "Strength", strength, 0.01f );
        hasChanged |= Gui::ComboBox( "Light GetType", type, { "Directional", "Point", "Spot", "Area" } );

        switch ( component.GetType() )
        {
            case BalbinoComponent::light_type::directional:
            case BalbinoComponent::light_type::point: hasChanged |= Gui::Float( "Size", size.r, 0.01f ); break;
            case BalbinoComponent::light_type::spot:
                hasChanged |= Gui::Float( "Size", size.r, 0.01f );
                hasChanged |= Gui::Float( "Near Radius", size.g, 0.01f );
                hasChanged |= Gui::Float( "Far Radius", size.b, 0.01f );
                break;
            case BalbinoComponent::light_type::area:
                hasChanged |= Gui::Float( "Size", size.r, 0.01f );
                hasChanged |= Gui::Float( "Width", size.g, 0.01f );
                hasChanged |= Gui::Float( "Height", size.b, 0.01f );
                break;
            default: break;
        }

        if ( hasChanged )
        {
            if ( size != component.GetSize() )
            {
                component.SetSize( size );
            }
            if ( strength != component.GetStrength() )
            {
                component.SetStrength( strength );
            }
            if ( color.r != colorFloat[ 0 ] || color.b != colorFloat[ 1 ] || color.b != colorFloat[ 2 ] )
            {
                component.SetColor( BambiMath::Vector3 { colorFloat[ 0 ], colorFloat[ 1 ], colorFloat[ 2 ] } );
            }

            if ( type != oldType )
            {
                BalbinoComponent::light_type newType = BalbinoComponent::light_type( type );
                switch ( newType )
                {
                    case BalbinoComponent::light_type::directional:
                    case BalbinoComponent::light_type::point: size.g = size.b = 0; break;
                    case BalbinoComponent::light_type::spot:
                        size.g = 0.01f;
                        size.b = 10.0f;
                        break;
                    case BalbinoComponent::light_type::area: size.g = 1.0f; break;
                    case BalbinoComponent::light_type::max: size.b = 1.0f; break;
                    default: break;
                }
                component.SetSize( size );
                component.SetType( newType );
            }
        }
    } );

    if ( Gui::Button( "Add Component", { -1, 0 } ) )
    {
        Gui::StartPopup( "AddComponent", false );
    }
}

template<typename T, typename... Args>
void FawnForge::CSceneHierarchy::DisplayAddComponentEntry( const std::string& entryName, Args&&... args )
{
    if ( !m_selectedEntity.HasComponent<T>() && Gui::MenuItem( entryName ) )
    {
        m_selectedEntity.AddComponent<T>( std::forward<Args>( args )... );
        Gui::CloseCurrentPopup();
    }
}
