#include "pch.h"
#include "MainScreen.h"

#include <IScene.h>

#include <imgui.h>

#include <iostream>


void BalEditor::CMainScreen::Draw()
{
	const ImGuiIO& io = ImGui::GetIO();
	( void ) io;
	const ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBackground |
		ImGuiWindowFlags_NoBringToFrontOnFocus /*| ImGuiWindowFlags_NoDocking*/;
	ImGui::SetNextWindowSize( io.DisplaySize );
	ImGui::SetNextWindowPos( { 0, 0 } );
	static ImGuiID dockspaceID = 0;
	ImGui::Begin( "MainWindow", nullptr, flags );
	if ( ImGui::BeginMenuBar() )
	{
		if ( ImGui::BeginMenu( "File" ) )
		{
			if ( ImGui::MenuItem( "New" ) )
			{
				m_saved = false;
			}
			if ( ImGui::MenuItem( "Open" ) )
			{
				//todo: load level
			}
			if ( ImGui::MenuItem( "Save Scene" ) )
			{
				//todo: save scene
			}
			if ( ImGui::MenuItem( "Save Scene As" ) )
			{
				//todo: save scene
			}
			ImGui::EndMenu();
		}
		if ( ImGui::BeginMenu( "GameObject" ) )
		{
			ImGui::Text( "Add GameObject" );
			if ( ImGui::MenuItem( "Empty" ) )
			{
				m_pContext->CreateEntity();
				//todo: add empty object;
				
				std::cout << "add empty object\n";
			}
			if ( ImGui::MenuItem( "Image" ) )
			{
				m_pContext->CreateEntity();
				//todo: add Image object;
				std::cout << "add image object\n";
			}
			if ( ImGui::MenuItem( "Text" ) )
			{
				m_pContext->CreateEntity();
				//todo: add text object;
				std::cout << "add Text object\n";
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
		static constexpr ImGuiDockNodeFlags dockspaceFlags = ImGuiDockNodeFlags_None;
		dockspaceID = ImGui::GetID( "MainWindowDockspace" );
		ImGui::DockSpace( dockspaceID, ImVec2( 0.0f, 0.0f ), dockspaceFlags );
	}
	ImGui::End();

	ImGui::SetNextWindowDockID( dockspaceID, ImGuiCond_FirstUseEver );
}

void BalEditor::CMainScreen::SetContext( IScene* pScene )
{
	m_pContext = pScene;
}
