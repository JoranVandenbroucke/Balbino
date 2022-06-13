#include "SceneHierarchy.h"

#include <string>

#include <IEntity.h>
#include <IScene.h>

#include "FileParcer.h"

#include "Components/IDComponent.h"
#include "Components/TransformComponent.h"
#include "Components/MeshRendererComponent.h"
#include "Components/CameraComponent.h"

#include "IResourceManager.h"
#include "Material.h"

#include "../EditorGUI/EditorGui.h"
#include <imgui.h>
#include <imgui_internal.h>

BalEditor::CSceneHierarchy::CSceneHierarchy()
        : m_pContext{ nullptr }
          , m_pSelectionContext{ nullptr }
          , m_isVisible{ true }
{
}

void BalEditor::CSceneHierarchy::Draw()
{
    if ( m_isVisible )
    {
        if ( ImGui::Begin( "Scene Hierarchy", &m_isVisible ))
        {
            if ( m_pContext )
            {
                const auto entities = m_pContext->GetAllEntities();
                for ( const auto& entity: entities )
                {
                    DrawEntityNode( entity );
                }

                if ( ImGui::IsMouseDown( 0 ) && ImGui::IsWindowHovered())
                {
                    m_pSelectionContext = {};
                }

                // Right-click on blank space
                if ( ImGui::BeginPopupContextWindow( nullptr, 1, false ))
                {
                    if ( ImGui::MenuItem( "CreateNew Empty Entity" ))
                    {
                        m_pContext->CreateEntity();
                    }

                    ImGui::EndPopup();
                }
            }
        }
        ImGui::End();
        if ( ImGui::Begin( "Properties", &m_isVisible ))
        {
            if ( m_pSelectionContext )
            {
                DrawComponents( m_pSelectionContext );
            }
        }
        ImGui::End();
    }
}

void BalEditor::CSceneHierarchy::SetContext( IScene* pScene )
{
    m_pContext = pScene;
}

void BalEditor::CSceneHierarchy::ShowWindow()
{
    m_isVisible = true;
    ImGui::SetWindowFocus( "Scene Hierarchy" );
    ImGui::SetWindowFocus( "Properties" );
}

IEntity* BalEditor::CSceneHierarchy::GetSelectedEntity() const
{
    return m_pSelectionContext;
}

template<typename T, typename UIFunction>
static void DrawComponent( const std::string& name, IEntity* entity, UIFunction uiFunction )
{
    constexpr ImGuiTreeNodeFlags treeNodeFlags =
            ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth |
            ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
    if ( entity->HasComponent<T>())
    {
        auto component = entity->GetComponent<T>();
        const ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

        ImGui::PushStyleVar( ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 } );
        const float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
        ImGui::Separator();
        const bool open = ImGui::TreeNodeEx(( void* ) typeid( T ).hash_code(), treeNodeFlags, "%s", name.c_str());
        ImGui::PopStyleVar();
        ImGui::SameLine( contentRegionAvailable.x - lineHeight * 0.5f );
        if ( ImGui::Button( "+", ImVec2{ lineHeight, lineHeight } ))
        {
            ImGui::OpenPopup( "ComponentSettings" );
        }

        bool removeComponent = false;
        if ( ImGui::BeginPopup( "ComponentSettings" ))
        {
            if ( ImGui::MenuItem( "Remove component" ))
            {
                removeComponent = true;
            }

            ImGui::EndPopup();
        }

        if ( open )
        {
            uiFunction( component );
            ImGui::TreePop();
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

    const std::string tag = std::to_string( static_cast<uint64_t>( pIdComponent->GetUUID()));
    //todo if children but closed, skip
    const uint32_t childCount{ pEntity->GetComponent<CTransformComponent>()->GetChildCount() };
    const ImGuiTreeNodeFlags flags =
            ( m_pSelectionContext == pEntity ? ImGuiTreeNodeFlags_Selected : 0 ) | ImGuiTreeNodeFlags_OpenOnArrow |
            ImGuiTreeNodeFlags_SpanAvailWidth | ( !childCount ? ImGuiTreeNodeFlags_Leaf : 0 );
    const bool opened = ImGui::TreeNodeEx(( void* ) static_cast<uint64_t>( static_cast<uint32_t>( *pEntity )), flags, "%s", tag.c_str());
    if ( ImGui::IsItemClicked())
    {
        m_pSelectionContext = pEntity;
    }

    bool entityDeleted = false;
    if ( ImGui::BeginPopupContextItem())
    {
        if ( ImGui::MenuItem( "Delete Entity" ))
        {
            entityDeleted = true;
        }

        ImGui::EndPopup();
    }

    if ( opened )
    {
        if ( childCount )
        {
            constexpr ImGuiTreeNodeFlags treeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
            if ( ImGui::TreeNodeEx( reinterpret_cast<void*>( 9817239 ), treeFlags, "%s", tag.c_str()))
            {
                ImGui::TreePop();
            }
        }
        ImGui::TreePop();
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
    ( void ) pEntity;
    DrawComponent<CTransformComponent>( "Transform", pEntity, []( auto component )
    {
        glm::vec3 translation = component->GetTranslation();
        glm::vec3 rotation = glm::degrees( glm::eulerAngles( component->GetRotation()));
        glm::vec3 scale = component->GetScale();

        float tra[3]{ translation.x, translation.y, translation.z };
        float rot[3]{ rotation.x, rotation.y, rotation.z };
        float sca[3]{ scale.x, scale.y, scale.z };

        BalEditor::EditorGUI::DrawFloat3( "Translation", tra, 0.01f );
        BalEditor::EditorGUI::DrawFloat3( "Rotation", rot, 0.01f );
        BalEditor::EditorGUI::DrawFloat3( "Scale", sca, 0.01f, 1 );

        component->SetScale( glm::vec3{ sca[ 0 ], sca[ 1 ], sca[ 2 ] } );
        component->SetRotation( glm::quat( radians( glm::vec3{ rot[ 0 ], rot[ 1 ], rot[ 2 ] } )));
        component->SetTranslation( glm::vec3{ tra[ 0 ], tra[ 1 ], tra[ 2 ] } );
    } );

    DrawComponent<CCameraComponent>( "Camera", pEntity, []( auto component )
    {
        auto& camera = component->GetCamera();
        bool isPrimary = component->IsPrimary();
        if (BalEditor::EditorGUI::DrawToggle("Primary", isPrimary))
        {
            component->SetIsPrimary( isPrimary );
        }

        float perspectiveVerticalFov = glm::degrees( camera.GetFov());
        float perspectiveNear = camera.GetNearClip();
        float perspectiveFar = camera.GetFarClip();
        bool changed = BalEditor::EditorGUI::DrawFloat( "Vertical FOV", perspectiveVerticalFov, 0.1f );
        changed |= BalEditor::EditorGUI::DrawFloat( "Near", perspectiveNear, 0.01f );
        changed |= BalEditor::EditorGUI::DrawFloat( "Far", perspectiveFar, 0.01f );
        if ( changed )
        {
            camera.UpdateProjectionMatrix( glm::radians( perspectiveVerticalFov ), perspectiveNear, perspectiveFar );
        }
    } );

    DrawComponent<CMeshRenderComponent>( "Mesh Render Component", pEntity, [ & ]( auto component )
    {
        const CUuid meshID = component->GetMeshId();
        const std::vector<CUuid>& materials = component->GetMaterials();
        const std::map<uint64_t, Balbino::CMaterial*>& loadedMaterials = m_pContext->GetSystem()->GetResourceManager()->GetAllLoadedMaterials();
        ImGui::Text( "%s", ( "mesh ID: " + std::to_string(( uint64_t ) meshID )).c_str());
//		for(auto& material : materials)
        for ( int i{}; i < materials.size(); ++i )
        {
            ImGui::Text( "%s", ( "material ID: " + std::to_string(( uint64_t ) materials[ i ] )).c_str());
            if ( ImGui::BeginDragDropTarget())
            {
                if ( const ImGuiPayload* payload = ImGui::AcceptDragDropPayload( ToString( EFileTypes::Material )))
                {
                    component->SetMaterial( i, (( SFile* ) payload->Data )->uuid );
                }
                ImGui::EndDragDropTarget();
            }
        }
        for ( auto& loadedMaterial: loadedMaterials )
        {
            ImGui::Separator();
            const auto& mat = loadedMaterial.second;
            const std::unordered_map<std::string, BalVulkan::SShaderResource>& resources = mat->GetShaderResourcesMap();
            for ( const auto& resource: resources )
            {
                ImGui::Text( "%s", resource.first.c_str());
            }
        }
    } );
}
