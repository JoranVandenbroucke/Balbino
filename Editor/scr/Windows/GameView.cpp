#include "pch.h"
#include "GameView.h"

#include <cstdint>
#include <imgui.h>

void GameView::Draw()
{
	ImGui::Begin( "GameView" );
	ImGui::BeginChild( "game", ImVec2{ 640, 500 }, true );
	//todo::Add Render Pass
	{
		//ImGui::Image( (void*) (uint64_t) mainCamera->GetTargetTexture(), { 640.f,480.f } );
	}
	ImGui::EndChild();
	ImGui::End();
}
