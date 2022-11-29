#include "GameView.h"

#pragma warning(push)
#pragma warning(disable:4201)
#include <glm/gtc/type_ptr.hpp>

#pragma warning(pop)

#include <IEntity.h>
#include <Components/CameraComponent.h>
#include <Components/TransformComponent.h>
#include <Components/MeshRendererComponent.h>
#include "SceneHierarchy.h"

#include <ImGuizmo.h>

#include "BalMath.h"
#include "FileParcer.h"
#include "ISystem.h"
#include "IResourceManager.h"
#include "IMesh.h"
#include "../EditorGUI/EditorGui.h"


void BalEditor::CGameView::Draw()
{
    int  flags = 1 << 5 | 43 | 1 << 7 | 1 << 0;
    bool open{ true };
    GUI::Begin( "GameView", open, flags );
    GUI::BeginChild( "GameViewChild", { -1, -1 }, false, flags );
    if ( const IEntity* selected = m_pSceneHierarchy->GetSelectedEntity())
    {
        ImGuizmo::SetOrthographic( false );
        ImGuizmo::SetDrawlist();
        ImGuizmo::SetRect( 0, 0, m_pSystem->GetWindowWidth(), m_pSystem->GetWindowHeight());
        
        const auto cameraObject = m_pContext->GetPrimaryCameraEntity();
        if ( cameraObject )
        {
            const auto& cameraCameraComponent    = cameraObject->GetComponent<CCameraComponent>();
            const auto& cameraTransformComponent = cameraObject->GetComponent<CTransformComponent>();
            
            const auto& cameraView       = glm::inverse( cameraTransformComponent->GetTransform());
            const auto& cameraProjection = glm::perspective( cameraCameraComponent->GetFov(),
                                                             m_pSystem->GetWindowWidth() / m_pSystem->GetWindowHeight(),
                                                             cameraCameraComponent->GetNearClip(),
                                                             cameraCameraComponent->GetFarClip());
            
            const auto transformComponentSelected = selected->GetComponent<CTransformComponent>();
            glm::mat4  transformSelected          = transformComponentSelected->GetTransform();
            
            // Snapping
            float snapValue = 0.1f;
            if ( m_gizmoType == ImGuizmo::OPERATION::ROTATE )
            {
                snapValue = 5.0f;
            }
            
            const float snapValues[3] = { snapValue, snapValue, snapValue };
            
            Manipulate( value_ptr( cameraView ), value_ptr( cameraProjection ),
                        static_cast<ImGuizmo::OPERATION>( m_gizmoType ), ImGuizmo::LOCAL,
                        value_ptr( transformSelected ), nullptr, m_snap ? snapValues : nullptr );
            
            if ( ImGuizmo::IsUsing())
            {
                glm::vec3 translation, rotation, scale;
                Balbino::BalMath::DecomposeTransform( transformSelected, translation, rotation, scale );
                
                const glm::vec3 rotEuler      = eulerAngles( transformComponentSelected->GetRotation());
                const glm::vec3 deltaRotation = rotation - rotEuler;
                transformComponentSelected->SetTranslation( translation );
                transformComponentSelected->SetRotation( glm::quat( rotEuler + deltaRotation ));
                transformComponentSelected->SetScale( scale );
            }
        }
    }
    GUI::EndChild();
    if ( void* pData = GUI::ReceiveDragDrop( ToString( EFileTypes::Model )))
    {
        const Balbino::IMesh* pModel = m_pSystem->GetResourceManager()->LoadModel( static_cast<SFile*>( pData )->path );
        IEntity             * pEnt   = m_pContext->CreateEntity();
        pEnt->AddComponent<CMeshRenderComponent>( pModel->GetUuid())->SetMaterialCount( pModel->GetMaterialCount());
    }
    GUI::End();
}

void BalEditor::CGameView::SetContext( ISystem* pSystem, IScene* pContext, CSceneHierarchy* pSceneHierarchy )
{
    m_pSceneHierarchy = pSceneHierarchy;
    m_pContext        = pContext;
    m_pSystem         = pSystem;
}

void BalEditor::CGameView::SetSnap( bool snap )
{
    m_snap = snap;
}

void BalEditor::CGameView::SetGuizmo( int key )
{
    m_gizmoType = key;
}

int BalEditor::CGameView::GetGuizmoType() const
{
    return m_gizmoType;
}