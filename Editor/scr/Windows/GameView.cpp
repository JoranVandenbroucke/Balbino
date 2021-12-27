#include "pch.h"
#include "GameView.h"

#include <gtc/type_ptr.hpp>

#include <IEntity.h>
#include <Components/CameraComponent.h>
#include <Components/TransformComponent.h>

#include "SceneHierarchy.h"

#include <imgui.h>
#include <ImGuizmo.h>

#include "BalMath.h"


void BalEditor::CGameView::Draw()
{
	ImGui::Begin( "GameView" );
	if ( const IEntity* selected = m_pSceneHierarchy->GetSelectedEntity() )
	{
		ImGuizmo::SetOrthographic( false );
		ImGuizmo::SetDrawlist();
		ImGuizmo::SetRect( ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, ImGui::GetWindowWidth(), ImGui::GetWindowHeight() );

		const auto camera = m_pContext->GetPrimaryCameraEntity();
		const auto cameraComponent = camera->GetComponent<CCameraComponent>();
		glm::mat4 view = glm::inverse( camera->GetComponent<CTransformComponent>()->GetTransform() );
		const glm::mat4& projection = cameraComponent->GetCamera().GetProjection();

		const auto tc = selected->GetComponent<CTransformComponent>();
		glm::mat4 transform = tc->GetTransform();

		// Snapping
		float snapValue = 0.1f; // Snap to 0.5m for translation/scale
		// Snap to 45 degrees for rotation
		if ( m_gizmoType == ImGuizmo::OPERATION::ROTATE )
			snapValue = 5.0f;

		const float snapValues[3] = { snapValue, snapValue, snapValue };

		ImGuizmo::Manipulate( glm::value_ptr( view ), glm::value_ptr( projection ),
							  static_cast< ImGuizmo::OPERATION >( m_gizmoType ), ImGuizmo::LOCAL, glm::value_ptr( transform ),
							  nullptr, m_snap ? snapValues : nullptr );

		if ( ImGuizmo::IsUsing() )
		{
			glm::vec3 translation, rotation, scale;
			Balbino::BalMath::DecomposeTransform( transform, translation, rotation, scale );

			const glm::vec3 rotEuler = glm::eulerAngles( tc->GetRotation() );
			const glm::vec3 deltaRotation = rotation - rotEuler;
			tc->SetTranslation( translation );
			tc->SetRotation( glm::quat( rotEuler + deltaRotation ) );
			tc->SetScale( scale );
		}
	}
	ImGui::End();
}

void BalEditor::CGameView::SetContext( IScene* pContext, CSceneHierarchy* pSceneHierarchy )
{
	m_pSceneHierarchy = pSceneHierarchy;
	m_pContext = pContext;
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
