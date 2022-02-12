#include "pch.h"
#include "GameView.h"

#include <gtc/type_ptr.hpp>

#include <IEntity.h>
#include <Components/CameraComponent.h>
#include <Components/TransformComponent.h>
#include <Components/MeshRendererComponent.h>

#include "SceneHierarchy.h"

#include <imgui.h>
#include <ImGuizmo.h>

#include "BalMath.h"
#include "FileParcer.h"
#include "IManager.h"
#include "IResourceManager.h"
#include "IMesh.h"


void BalEditor::CGameView::Draw()
{
	ImGui::Begin( "GameView" );
	ImGui::BeginChild( "GameViewChild" );
	if ( const IEntity* selected = m_pSceneHierarchy->GetSelectedEntity() )
	{
		ImGuizmo::SetOrthographic( false );
		ImGuizmo::SetDrawlist();
		ImGuizmo::SetRect( ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, ImGui::GetWindowWidth(), ImGui::GetWindowHeight() );

		const auto camera = m_pContext->GetPrimaryCameraEntity();
		const auto cameraComponent = camera->GetComponent<CCameraComponent>();
		glm::mat4 view = inverse( camera->GetComponent<CTransformComponent>()->GetTransform() );
		const glm::mat4& projection = cameraComponent->GetCamera().GetProjection();

		const auto tc = selected->GetComponent<CTransformComponent>();
		glm::mat4 transform = tc->GetTransform();

		// Snapping
		float snapValue = 0.1f; // Snap to 0.5m for translation/scale
		// Snap to 45 degrees for rotation
		if ( m_gizmoType == ImGuizmo::OPERATION::ROTATE )
			snapValue = 5.0f;

		const float snapValues[3] = { snapValue, snapValue, snapValue };

		Manipulate( value_ptr( view ), value_ptr( projection ),
		            static_cast<ImGuizmo::OPERATION>( m_gizmoType ), ImGuizmo::LOCAL, value_ptr( transform ),
		            nullptr, m_snap ? snapValues : nullptr );

		if ( ImGuizmo::IsUsing() )
		{
			glm::vec3 translation, rotation, scale;
			Balbino::BalMath::DecomposeTransform( transform, translation, rotation, scale );

			const glm::vec3 rotEuler = eulerAngles( tc->GetRotation() );
			const glm::vec3 deltaRotation = rotation - rotEuler;
			tc->SetTranslation( translation );
			tc->SetRotation( glm::quat( rotEuler + deltaRotation ) );
			tc->SetScale( scale );
		}
	}
	ImGui::EndChild();
	if ( ImGui::BeginDragDropTarget() )
	{
		if ( const ImGuiPayload* payload = ImGui::AcceptDragDropPayload( ToString( EFileTypes::Model ) ) )
		{
			std::cout << "loading model: ";
			std::cout << payload->DataSize;
			std::cout << "\n";
			//todo: make model
			const Balbino::IMesh* pModel = m_pSystem->GetResourceManager()->LoadModel(static_cast<SFile*>( payload->Data )->path.generic_string());
			IEntity* pEnt = m_pContext->CreateEntity();
			pEnt->CreateComponent<CMeshRenderComponent>( pModel->GetUuid() )->SetMaterialCount( pModel->GetMaterialCount() );
		}
		ImGui::EndDragDropTarget();
	}
	ImGui::End();
}

void BalEditor::CGameView::SetContext( ISystem* pSystem, IScene* pContext, CSceneHierarchy* pSceneHierarchy )
{
	m_pSceneHierarchy = pSceneHierarchy;
	m_pContext = pContext;
	m_pSystem = pSystem;
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
