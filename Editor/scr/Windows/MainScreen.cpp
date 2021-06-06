#include "pch.h"
#include "MainScreen.h"

#include <imgui.h>
#include <windows.h>
#include <commdlg.h>
#include <filesystem>
#include <iostream>
#include <regex>

void MainScreen::Draw()
{
	ImGuiIO& io = ImGui::GetIO(); (void) io;
	ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoBringToFrontOnFocus /*| ImGuiWindowFlags_NoDocking*/;
	ImGui::SetNextWindowSize( io.DisplaySize );
	ImGui::SetNextWindowPos( { 0,0 } );
	static ImGuiID dockspaceID = 0;
	ImGui::Begin( "MainWindow", 0, flags );
	if ( ImGui::BeginMenuBar() )
	{
		if ( ImGui::BeginMenu( "File" ) )
		{
			if ( ImGui::MenuItem( "New" ) )
			{
				m_Saved = false;
			}
			if ( ImGui::MenuItem( "Open" ) )
			{
				std::string currentPath = std::filesystem::current_path().u8string();
				wchar_t fileName[MAX_PATH] = L"";
				OPENFILENAME ofn;
				ZeroMemory( &ofn, sizeof( ofn ) );
				ofn.lStructSize = sizeof( OPENFILENAME );
				ofn.hwndOwner = nullptr;
				ofn.lpstrFilter = L"Balbino Files (.Balbino)\0*.Balbino\0Any File\0*.*\0";
				ofn.lpstrFile = fileName;
				ofn.nMaxFile = MAX_PATH;
				ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
				ofn.lpstrDefExt = L"";
				if ( GetOpenFileName( &ofn ) )
				{
					m_SavePosition = std::filesystem::relative( fileName, currentPath ).u8string();
					//todo: load scene
				}
				std::filesystem::current_path( currentPath );
			}
			if ( ImGui::MenuItem( "Save Scene" ) )
			{
				std::string currentPath = std::filesystem::current_path().u8string();
				wchar_t fileName[MAX_PATH] = L"";
				if ( !m_Saved )
				{
					OPENFILENAME ofn;
					ZeroMemory( &ofn, sizeof( ofn ) );
					ofn.lStructSize = sizeof( OPENFILENAME );
					ofn.hwndOwner = nullptr;
					ofn.lpstrFilter = L"Balbino Files (.Balbino)\0*.Balbino\0Any File\0*.*\0";
					ofn.lpstrFile = fileName;
					ofn.nMaxFile = MAX_PATH;
					ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
					ofn.lpstrDefExt = L"";
					if ( GetSaveFileName( &ofn ) )
					{
						m_SavePosition = std::filesystem::relative( fileName, currentPath ).u8string();
					}
					std::filesystem::current_path( currentPath );
					std::regex sceneName{ R"(^.*\\(.*)\.Balbino$)" };
					std::smatch match;
					if ( std::regex_match( m_SavePosition, match, sceneName ) )
					//todo: save scene
						std::cout << "saving the scen\n";
				}
			}
			if ( ImGui::MenuItem( "Save Scene As" ) )
			{
				std::string currentPath = std::filesystem::current_path().u8string();
				wchar_t fileName[MAX_PATH] = L"";

				OPENFILENAME ofn;
				ZeroMemory( &ofn, sizeof( ofn ) );
				ofn.lStructSize = sizeof( OPENFILENAME );
				ofn.hwndOwner = nullptr;
				ofn.lpstrFilter = L"Balbino Files (.Balbino)\0*.Balbino\0Any File\0*.*\0";
				ofn.lpstrFile = fileName;
				ofn.nMaxFile = MAX_PATH;
				ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
				ofn.lpstrDefExt = L"";
				if ( GetSaveFileName( &ofn ) )
				{
					m_SavePosition = std::filesystem::relative( fileName, currentPath ).u8string();
				}
				std::filesystem::current_path( currentPath );
				std::regex sceneName{ R"(^.*\\(.*)\.Balbino$)" };
				std::smatch match;
				//todo: save as
				if ( std::regex_match( m_SavePosition, match, sceneName ) )
					std::cout << match[1].str() << '\n';
					//m_Name = match[1].str();
				//SaveToFile( m_SavePosition );
			}
			ImGui::EndMenu();
		}
		if ( ImGui::BeginMenu( "GameObject" ) )
		{
			ImGui::Text( "Add GameObject" );
			if ( ImGui::MenuItem( "Empty" ) )
			{
				//todo: add empty object;
				//GameObject* newObject{ DBG_NEW GameObject{} };

				//newObject->Create();
				//newObject->SetName( "Empty" );
				//Add( newObject );
				std::cout << "add empty object\n";
			}
			if ( ImGui::MenuItem( "Image" ) )
			{
				//todo: add Image object;
				//GameObject* newObject{ DBG_NEW GameObject{} };

				//newObject->AddComponent<Sprite>();
				//newObject->Create();
				//newObject->SetName( "Image" );
				//Add( newObject );
				std::cout << "add image object\n";
			}
			if ( ImGui::MenuItem( "Text" ) )
			{
				//todo: add text object;
				//GameObject* newObject{ DBG_NEW GameObject{} };

				//newObject->AddComponent<ImGui::Text>();
				//newObject->Create();
				//newObject->SetName( "Text" );
				//Add( newObject );
				std::cout << "add Text object\n";
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
		static const ImGuiDockNodeFlags dockspaceFlags = ImGuiDockNodeFlags_None;
		dockspaceID = ImGui::GetID( "MainWindowDockspace" );
		ImGui::DockSpace( dockspaceID, ImVec2( 0.0f, 0.0f ), dockspaceFlags );
	}
	ImGui::End();

	ImGui::SetNextWindowDockID( dockspaceID, ImGuiCond_FirstUseEver );
}
