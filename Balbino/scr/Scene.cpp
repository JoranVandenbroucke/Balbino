#include "BalbinoPCH.h"
#include "Scene.h"
#include "Components/All.h"
#include "GameObject/GameObject.h"
#include "imgui-1.75/imgui.h"
#include "BinaryReaderWrider.h"
#include "Debug.h"

#include <windows.h>
#include <commdlg.h>
#include <string.h>
#include <iostream>
#include <regex>

using namespace Balbino;

unsigned int Scene::m_IdCounter = 0;

#ifdef _DEBUG
Scene::Scene( const std::string& name )
	: m_GameObjects{}
	, m_Name{ name }
	, m_Saved{}
{
}
#else
Scene::Scene( const std::string& name )
	: m_GameObjects{}
	, m_Name{ name }
{
}
#endif // _DEBUG

Scene::~Scene() = default;

void Balbino::Scene::Add( const std::shared_ptr<GameObject> & object, int pos )
{
	std::list<std::shared_ptr<GameObject>>::iterator it{ m_GameObjects.begin() };
	if( pos != -1 )
		for( int i = 0; i < pos; i++ ) ++it;

	m_GameObjects.insert( it, object );
}

void Scene::Update()
{
	for( auto& object : m_GameObjects )
		object->Update();
}
void Scene::Draw() const
{
	for( const auto& object : m_GameObjects )
		object->Draw();
}

#ifdef _DEBUG
void Balbino::Scene::DrawEditor()
{

	// a lot of the ImGui Code comes from the demo file (imgui_demo.cpp)
	ImGuiIO& io = ImGui::GetIO(); (void) io;
	ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoBringToFrontOnFocus /*| ImGuiWindowFlags_NoDocking*/;
	ImGui::SetNextWindowSize( io.DisplaySize );
	ImGui::SetNextWindowPos( { 0,0 } );
	static ImGuiID dockspaceID = 0;
	ImGui::Begin( "MainWindow", 0, flags );
	if( ImGui::BeginMenuBar() )
	{
		if( ImGui::BeginMenu( "File" ) )
		{
			if( ImGui::MenuItem( "New" ) )
			{
				m_Saved = false;
				m_Name = "New Scene";
				m_GameObjects.clear();
			}
			if( ImGui::MenuItem( "Open" ) )
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
				if( GetOpenFileName( &ofn ) )
				{
					m_SavePosition = std::filesystem::relative( fileName, currentPath ).u8string();
				}
				std::filesystem::current_path( currentPath );
				std::regex sceneName{ R"(^.*\\(.*)\.Balbino$)" };
				std::smatch match;
				if( std::regex_match( m_SavePosition, match, sceneName ) )
					m_Name = match[1].str();

				std::ifstream saveFile{};
				saveFile.open( m_SavePosition, std::ios::in | std::ios::binary );
				if( saveFile.is_open() )
				{
					m_GameObjects.clear();
					int size{};
					BinaryReadWrite::Read( saveFile, size );
					for( int i = 0; i < size; i++ )
					{
						std::shared_ptr<GameObject> gameObject = std::make_shared<GameObject>();
						gameObject->Load( saveFile );
						gameObject->Create();
						Add( gameObject );
						m_IndentPosition.push_back( 0 );
					}
					m_GameObjects.reverse();
					m_Saved = true;
				}
				saveFile.close();
			}
			if( ImGui::MenuItem( "Save Scene" ) )
			{
				std::string currentPath = std::filesystem::current_path().u8string();
				wchar_t fileName[MAX_PATH] = L"";
				if( !m_Saved )
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
					if( GetSaveFileName( &ofn ) )
					{
						m_SavePosition = std::filesystem::relative( fileName, currentPath ).u8string();
					}
					std::filesystem::current_path( currentPath );
					std::regex sceneName{ R"(^.*\\(.*)\.Balbino$)" };
					std::smatch match;
					if( std::regex_match( m_SavePosition, match, sceneName ) )
						m_Name = match[1].str();
				}

				std::ofstream saveFile{};
				saveFile.open( m_SavePosition, std::ios::out | std::ios::binary );
				if( saveFile.is_open() )
				{
					BinaryReadWrite::Write( saveFile, int( m_GameObjects.size() ) );
					for( std::shared_ptr<GameObject> gameObject : m_GameObjects )
					{
						gameObject->Save( saveFile );
					}
					m_Saved = true;
				}
				saveFile.close();
			}
			ImGui::EndMenu();
		}
		if( ImGui::BeginMenu( "GameObject" ) )
		{
			ImGui::Text( "Add GameObject" );
			if( ImGui::MenuItem( "Empty" ) )
			{
				std::shared_ptr<GameObject> newObject{ std::make_shared<GameObject>() };
				newObject->Create();
				newObject->SetName( "Empty" );
				std::list<int>::iterator it{ m_IndentPosition.begin() };
				m_IndentPosition.insert( it, 0 );
				Add( newObject );
			}
			if( ImGui::MenuItem( "Image" ) )
			{
				std::shared_ptr<GameObject> newObject{ std::make_shared<GameObject>() };
				newObject->AddComponent<Texture2D>();
				newObject->Create();
				newObject->SetName( "Image" );
				std::list<int>::iterator it{ m_IndentPosition.begin() };
				m_IndentPosition.insert( it, 0 );
				Add( newObject );
			}
			if( ImGui::MenuItem( "Text" ) )
			{
				std::shared_ptr<GameObject> newObject{ std::make_shared<GameObject>() };
				newObject->AddComponent<Text>();
				newObject->Create();
				newObject->SetName( "Text" );
				std::list<int>::iterator it{ m_IndentPosition.begin() };
				m_IndentPosition.insert( it, 0 );
				Add( newObject );
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
	static int selected{ 0 };
	ImGui::Begin( "Hierarchy" );
	if( ImGui::Button( "+" ) )
	{
		std::shared_ptr<GameObject> newObject{ std::make_shared<GameObject>() };
		//newObject->AddComponent<Transform>();
		newObject->Create();
		newObject->SetName( "Game Object" );
		//m_IndentPosition.push_back( 0 );

		std::list<int>::iterator it{ m_IndentPosition.begin() };
		if( selected != -1 )
			for( int i = 0; i < selected; i++ ) ++it;

		m_IndentPosition.insert( it, 0 );
		Add( newObject );
	}
	ImGui::SameLine();
	if( ImGui::Button( "-" ) )
	{
		if( m_GameObjects.size() != 0 && selected >= 0 )
		{
			std::list<std::shared_ptr<GameObject>>::iterator it{ m_GameObjects.begin() };
			std::list<int>::iterator it2{ m_IndentPosition.begin() };
			for( int i = 1; i < selected; i++ ) ++it;
			for( int i = 1; i < selected; i++ ) ++it2;
			m_GameObjects.erase( it );
			m_IndentPosition.erase( it2 );
			--selected;
		}
	}
	ImGui::SameLine();
	if( ImGui::Button( ">>" ) )
	{
		if( m_GameObjects.size() != 0 && selected > 0 )
		{
			bool worked{ true };
			std::list<std::shared_ptr<GameObject>>::iterator it{ m_GameObjects.begin() };
			std::list<int>::iterator it3{ m_IndentPosition.begin() };
			for( int i = 0; i < selected; i++ ) ++it;
			for( int i = 0; i < selected; i++ ) ++it3;
			std::list<std::shared_ptr<GameObject>>::iterator it2{ it };
			for( std::list<int>::iterator it4{ it3 }; *--it4 < *it3; )
			{
				if( it4 == m_IndentPosition.begin() )
				{
					worked = false;
					break;
				}
				--it2;
			}
			if( worked )
			{
				++( *it3 );
				( *it )->GetComponent<Transform>()->SetParrent( ( *--it2 )->GetComponent<Transform>() );
			}
		}
	}
	ImGui::SameLine();
	if( ImGui::Button( "<<" ) )
	{
		if( m_GameObjects.size() != 0 && selected != 0 )
		{
			std::list<std::shared_ptr<GameObject>>::iterator it{ m_GameObjects.begin() };
			std::list<int>::iterator it2{ m_IndentPosition.begin() };
			for( int i = 0; i < selected; i++ ) ++it;
			for( int i = 0; i < selected; i++ ) ++it2;
			( *it2 ) = 0;
			( *it )->GetComponent<Transform>()->SetParrent( nullptr );
		}
	}



	ImGui::SetNextItemOpen( true );
	int selection_mask = ( 1 << selected ); // Dumb representation of what may be user-side selection state. You may carry selection state inside or outside your objects in whatever format you see fit.
	static ImGuiTreeNodeFlags baseFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_FramePadding;
	bool open{ true };
	if( ImGui::TreeNode( m_Name.c_str() ) )
	{
		int i{};
		int iOld{};
		if( m_GameObjects.size() != 0 )
		{
			do
			{
				std::list<std::shared_ptr<GameObject>>::iterator it1{ m_GameObjects.begin() };
				std::list<int>::iterator it2{ m_IndentPosition.begin() };
				for( int j = 0; j < i; j++ )++it1, ++it2;

				const bool isSelected = ( selection_mask & ( 1 << i ) ) != 0;
				ImGuiTreeNodeFlags nodeFlags = baseFlags;
				int nrOfChilderen = ( *it1 )->GetComponent<Transform>()->GetNumberOfChilderen();
				if( isSelected )
					nodeFlags |= ImGuiTreeNodeFlags_Selected;
				nodeFlags |= ( nrOfChilderen == 0 ? ImGuiTreeNodeFlags_Leaf : 0 ); // ImGuiTreeNodeFlags_Bullet

				int indent = *it2;

				open = ImGui::TreeNodeEx( (void*) (intptr_t) i, nodeFlags,
					( *it1 )->GetName() );
				//open = ImGui::TreeNodeEx( (void*) (intptr_t) i,
				//	ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_FramePadding | ( isSelected ? ImGuiTreeNodeFlags_Selected : 0 ) | ( nrOfChilderen == 0 ? ImGuiTreeNodeFlags_Leaf : 0 ),
				//	( *it1 )->GetName() );

				ImGui::PushID( (void*) &i );
				if( ImGui::BeginPopupContextItem() )
				{
					ImGui::EndPopup();
				}
				ImGui::PopID();

				if( ImGui::IsItemClicked() )
					selected = i;

				if( ImGui::BeginDragDropSource( ImGuiDragDropFlags_None ) )
				{
					ImGui::SetDragDropPayload( "GameObject", &i, sizeof( int ) );
					ImGui::Text( "Swap %s", ( *it1 )->GetName() );
					ImGui::EndDragDropSource();
				}

				if( ImGui::BeginDragDropTarget() )
				{
					if( const ImGuiPayload* payload = ImGui::AcceptDragDropPayload( "GameObject" ) )
					{
						int end = *(const int*) payload->Data;
						if( end >= 0 && end < int( m_GameObjects.size() ) )
						{
							std::list<std::shared_ptr<GameObject>>::iterator it3{ m_GameObjects.begin() };
							for( int j = 0; j < end; j++ )++it3;
							m_GameObjects.splice( it1, m_GameObjects, it3, std::next( it3 ) );
						}
					}
					ImGui::EndDragDropTarget();
				}

				if( open )
				{
					if( ( i + 1 != int( m_IndentPosition.size() ) && *( ++it2 ) <= indent ) )
					{
						ImGui::TreePop();
					}
					if( i + 1 == int( m_IndentPosition.size() ) )
					{
						for( int j = 0; j < indent + 1; j++ )
						{
							ImGui::TreePop();
						}
					}
					++i;
				}
				else
				{
					if( m_IndentPosition.size() > 1 )
					{
						if( i + 1 == int( m_IndentPosition.size() ) )
						{
							for( int j = 0; j < indent; j++ )
							{
								ImGui::TreePop();
							}
						}

						while( i + 1 < int( m_IndentPosition.size() )
							&& *( ++it2 ) > indent )++i;
						if( i + 1 == int( m_IndentPosition.size() )
							&& *it2 > indent )
						{
							break;
						}
					}
					if( iOld == i )++i;
				}
				iOld = i;
			}
			while( i < int( m_GameObjects.size() ) /*|| ( i == 0 && m_GameObjects.size() == 1 )*/ );
		}
		ImGui::TreePop();
	}
	ImGui::End();

	if( m_GameObjects.size() != 0 )
	{
		ImGui::Begin( "Inspector" );

		std::list<std::shared_ptr<GameObject>>::iterator it{ m_GameObjects.begin() };

		for( int i = 0; i < selected; i++ )++it;
		std::string actualName{ ( *it )->GetName() };
		char name[32]{};
		int size{ std::min( int( actualName.size() ), 32 ) };

		for( int i = 0; i < size; i++ )
		{
			name[i] = actualName[i];
		}

		ImGui::InputText( "Name:", name, 32 );
		( *it )->SetName( name );
		( *it )->DrawInspector();

		if( ImGui::Button( "Add Component" ) )
		{
			ImGui::OpenPopup( "Select Components" );
		}
		if( ImGui::BeginPopup( "Select Components" ) )
		{
			for( int i = 0; i < 8; i++ )
			{
				if( ImGui::Selectable( m_ComponentsString[i] ) )
				{
					switch( ComponentList( i ) )
					{
					case Balbino::ComponentList::Audio:
						( *it )->AddComponent<ConsoleAudio>();
						( *it )->GetComponent<ConsoleAudio>()->Create();
						break;
					case Balbino::ComponentList::LoggedAudio:
						( *it )->AddComponent<LoggedAudio>();
						( *it )->GetComponent<LoggedAudio>()->Create();
						break;
					case Balbino::ComponentList::Avatar:
						( *it )->AddComponent<Avatar>();
						( *it )->GetComponent<Avatar>()->Create();
						break;
					case Balbino::ComponentList::Camera:
						//( *it )->AddComponent<Camera>();
						break;
					case Balbino::ComponentList::FPSScript:
						( *it )->AddComponent<FPSScript>();
						( *it )->GetComponent<FPSScript>()->Create();
						break;
					case Balbino::ComponentList::Text:
						( *it )->AddComponent<Text>();
						( *it )->GetComponent<Text>()->Create();
						break;
					case Balbino::ComponentList::Texture2D:
						( *it )->AddComponent<Texture2D>();
						( *it )->GetComponent<Texture2D>()->Create();
						break;
					case Balbino::ComponentList::Transform:
						( *it )->AddComponent<Transform>();
						( *it )->GetComponent<Transform>()->Create();
						break;
					default:
						break;
					}
				}
			}
			ImGui::EndPopup();
		}
		ImGui::End();
	}
}
#endif // _DEBUG


void Balbino::Scene::Load()
{
	for( auto& object : m_GameObjects )
		object->Create();
}

void Balbino::Scene::Unload()
{
	for( auto& object : m_GameObjects )
		object->Destroy();
}