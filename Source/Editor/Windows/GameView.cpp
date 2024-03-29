#include "GameView.h"

#include <BambiMath.h>
#include <CameraComponent.h>
#include <Entity.h>
#include <IResourceManager.h>
#include <ISystem.h>
#include <Mesh.h>
#include <MeshRendererComponent.h>
#include <Scene.h>
#include <TransformComponent.h>

#include "SceneHierarchy.h"

#include "../Renderer/EditorGui.h"
#include "ImGuizmo.h"

void FawnForge::CGameView::Draw() noexcept
{
    int flags = 1 << 5 | 43 | 1 << 7 | 1 << 0;
    bool open { true };
    Gui::Begin( "GameView", open, flags );
    Gui::BeginChild( "GameViewChild", { -1, -1 }, false, flags );
    if ( const BalbinoScene::CEntity& selected = m_pSceneHierarchy->GetSelectedEntity(); selected.IsValid() )
    {
        ImGuizmo::SetOrthographic( false );
        ImGuizmo::SetDrawlist();
        ImGuizmo::SetRect( 0, 0, (float)m_pSystem->GetWindowWidth(), (float)m_pSystem->GetWindowHeight() );
#pragma todo( "Re Add ImGuizmo" ) // todo: fix those comments
#pragma fixme( "Re Add ImGuizmo" )// fixme: fix those comments
                                  //        const auto cameraObject = m_pSystem->GetCameraManager()->GetActiveCamera();
                                  //        if ( cameraObject )
                                  //        {
                                  //            const auto& cameraCameraComponent    = cameraObject->GetComponent<CervidaeComponent::CCameraComponent>();
                                  //            const auto& cameraTransformComponent = cameraObject.GetComponent<CervidaeComponent::CTransformComponent>();
                                  //
        //            const auto& cameraView       = BambiMath::LookAt( cameraTransformComponent->GetTranslation(), cameraTransformComponent->GetTranslation() + glm::rotate( cameraTransformComponent->GetRotation(), BambiMath::Vector3 { 0, 0, 1 } ), BambiMath::Vector3 { 0, 1, 0 } );
        //            const auto& cameraProjection = BambiMath::Perspective( cameraCameraComponent->GetFov(), m_pSystem->GetWindowWidth() / m_pSystem->GetWindowHeight(), cameraCameraComponent->GetNearClip(), cameraCameraComponent->GetFarClip() );
        //
        //            const auto transformComponentSelected = selected.GetComponent<CervidaeComponent::CTransformComponent>();
        //            BambiMath::Matrix4 transformSelected           = transformComponentSelected.GetTransform();
        //
        //            // Snapping
        //            float snapValue = 0.1f;
        //            if ( m_gizmoType == ImGuizmo::OPERATION::ROTATE )
        //            {
        //                snapValue = 5.0f;
        //            }
        //
        //            const float snapValues[ 3 ] { snapValue, snapValue, snapValue };
        //
        //            Manipulate( value_ptr( cameraView ), value_ptr( cameraProjection ), static_cast<ImGuizmo::OPERATION>( m_gizmoType ), ImGuizmo::LOCAL, value_ptr( transformSelected ), nullptr, m_snap ? snapValues : nullptr );
        //
        //            if ( ImGuizmo::IsUsing() )
        //            {
        //                BambiMath::Vector3 translation;
        //                BambiMath::Vector3 rotation;
        //                BambiMath::Vector3 scale;
        //                BambiMath::DecomposeTransform( transformSelected, translation, rotation, scale );
        //
        //                const BambiMath::Vector3 rotEuler      = eulerAngles( transformComponentSelected.GetRotation() );
        //                const BambiMath::Vector3 deltaRotation = rotation - rotEuler;
        //                transformComponentSelected.SetTranslation( translation );
        //                transformComponentSelected.SetRotation( glm::quat( rotEuler + deltaRotation ) );
        //                transformComponentSelected.SetScale( scale );
        //            }
        //        }
    }
    Gui::EndChild();
    if ( void* pData = Gui::ReceiveDragDrop( ToString( file_type::model ) ) )
    {
        try
        {
            const FawnVision::CMesh* pModel = m_pSystem->GetResourceManager()->GetModel( static_cast<SFile*>( pData )->path );
            BalbinoScene::CEntity pEnt      = m_pContext->CreateEntity();
            pEnt.AddComponent<CervidaeComponent::CMeshRenderComponent>( pModel->GetUuid() ).SetMaterialCount( pModel->GetMaterialCount() );
        }
        catch ( const FawnTrace::Exception& e )
        {
            m_pSystem->Log.Error( e.what() );
        }
    }
    Gui::End();
}

void FawnForge::CGameView::SetContext( ISystem* pSystem, CSceneHierarchy* pSceneHierarchy ) noexcept
{
    m_pSceneHierarchy = pSceneHierarchy;
    m_pSystem         = pSystem;
    m_pContext        = pSystem->GetCurrentActiveScene();
}

void FawnForge::CGameView::SetSnap( bool snap ) noexcept
{
    m_snap = snap;
}

void FawnForge::CGameView::SetGuizmo( int key ) noexcept
{
    m_gizmoType = key;
}

int FawnForge::CGameView::GetGuizmoType() const
{
    return m_gizmoType;
}