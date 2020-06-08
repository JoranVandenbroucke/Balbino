#include "BalbinoPCH.h"
#include "Scene.h"
#include "Components/All.h"
#include "BinaryReaderWrider.h"
#include "Editor/Debug.h"
#include "Editor/AssetBrouser.h"
#include "Editor/SpriteEditor.h"
#include "Application.h"
#include "Time.h"

#include <windows.h>
#include <commdlg.h>
#include <string.h>
#include <iostream>
#include <regex>

using namespace Balbino;

unsigned int Scene::m_IdCounter = 0;

#ifdef _DEBUG
Scene::Scene( const std::string& name )
	: m_Name{ name }
	, m_Saved{}
{
	GameObject* gameObject = DBG_NEW GameObject{};

	gameObject->AddComponent<Camera>( 640.f / 480.f, 640.f );
	//gameObject->Create();
	gameObject->SetName( "Maint Camera" );
	Add( gameObject );
}

void Scene::Draw()
{
	//start ImGui
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame( Application::GetWindow() );
	ImGui::NewFrame();

	auto allCameras = Camera::GetAllCameras();
	if( allCameras.size() != 0 )
	{
		for( auto& camera : allCameras )
		{
			auto currentCam = camera;
			if( currentCam )
			{
				Color clearColor = currentCam->GetClearColor();
				glBindFramebuffer( GL_FRAMEBUFFER, currentCam->GetCameraIndex() );
				glViewport( 0, 0, (int) 640, (int) 480 );
				glClearColor( clearColor.r, clearColor.g, clearColor.b, 1.f );
				glClear( GL_COLOR_BUFFER_BIT );
				glEnable( GL_DEPTH_TEST );
				Shader::SetCamera( *currentCam );
				for( const auto& object : m_pGameObjects )
					object->Draw();

			}
		}
	}

	auto mainCamera = Camera::GetMainCamera();
	glBindFramebuffer( GL_FRAMEBUFFER, 0 );
	glViewport( 0, 0, (int) 1920, (int) 1080 );
	glClearColor( 0.f, 0.f, 0.f, 1.f );
	glClear( GL_COLOR_BUFFER_BIT );

	ImGuiIO& io = ImGui::GetIO(); (void) io;

	SceneManager::Get().DrawEngine();
	Debug::Get().Draw();
	AssetBrouser::Get().Draw();
	SpriteEditor::Get().Draw();

	ImGui::Begin( "GameView" );
	ImGui::BeginChild( "game", ImVec2{ 640,480 } );
	//ImGui::Image( (ImTextureID) (intptr_t) m_RenderedTexture, ImVec2{ 640,480 } );
	if( mainCamera )
	{
		ImGui::GetWindowDrawList()->AddImage( (void*) (uint64_t) mainCamera->GetTargetTexture(),
			ImVec2{ ImGui::GetCursorScreenPos() },
			ImVec2{ ImGui::GetCursorScreenPos().x + 640, ImGui::GetCursorScreenPos().y + 480 } );
	}
	BTime::Get().SetTS( 1.f );
	ImGui::EndChild();
	if( !ImGui::IsWindowHovered() )
	{
		BTime::Get().SetTS( 0.f );
	}
	ImGui::End();
	// Rendering Game
	ImGui::Render();

	ImGui_ImplOpenGL3_RenderDrawData( ImGui::GetDrawData() );

	// Update and Render additional Platform Windows
	// (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
	//  For this specific demo app we could also call SDL_GL_MakeCurrent(window, gl_context) directly)
	if( io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable )
	{
		SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
		SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		SDL_GL_MakeCurrent( backup_current_window, backup_current_context );
	}
	SDL_GL_SwapWindow( Application::GetWindow() );
}

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
				m_Name = "DBG_NEW Scene";
				m_pGameObjects.clear();
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
					m_pGameObjects.clear();
					int size{};
					BinaryReadWrite::Read( saveFile, size );
					for( int i = 0; i < size; i++ )
					{
						GameObject* gameObject = DBG_NEW GameObject{};

						gameObject->Load( saveFile );
						gameObject->Create();
						Add( gameObject );
					}
					m_pGameObjects.reverse();
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
					BinaryReadWrite::Write( saveFile, int( m_pGameObjects.size() ) );
					for( GameObject* gameObject : m_pGameObjects )

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
				GameObject* newObject{ DBG_NEW GameObject{} };

				newObject->Create();
				newObject->SetName( "Empty" );
				Add( newObject );
			}
			if( ImGui::MenuItem( "Image" ) )
			{
				GameObject* newObject{ DBG_NEW GameObject{} };

				newObject->AddComponent<Sprite>();
				newObject->Create();
				newObject->SetName( "Image" );
				Add( newObject );
			}
			if( ImGui::MenuItem( "Text" ) )
			{
				GameObject* newObject{ DBG_NEW GameObject{} };

				newObject->AddComponent<Text>();
				newObject->Create();
				newObject->SetName( "Text" );
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
		GameObject* newObject{ DBG_NEW GameObject{} };

		newObject->Create();
		newObject->SetName( "Game Object" );
		Add( newObject );
	}
	ImGui::SameLine();
	if( ImGui::Button( "-" ) )
	{
		if( m_pGameObjects.size() != 0 && selected >= 0 )
		{
			std::list<GameObject*>::iterator it{ m_pGameObjects.begin() };

			for( int i = 0; i < selected; ++i ) ++it;
			m_pGameObjects.erase( it );
			--selected;
			if( selected == -1 ) selected = 0;
		}
	}
	ImGui::SameLine();
	if( ImGui::Button( ">>" ) )
	{
		if( m_pGameObjects.size() != 0 && selected > 0 )
		{
			bool worked{ true };
			std::list<GameObject*>::iterator it{ m_pGameObjects.begin() };

			for( int i = 0; i < selected; i++ ) ++it;
			std::list<GameObject*>::iterator it2{ it };

			{
				--it2;
			}
			if( worked )
			{
				( *it )->GetComponent<Transform>()->SetParrent( ( *--it2 )->GetComponent<Transform>() );
			}
		}
	}
	ImGui::SameLine();
	if( ImGui::Button( "<<" ) )
	{
		if( m_pGameObjects.size() != 0 && selected != 0 )
		{
			std::list<GameObject*>::iterator it{ m_pGameObjects.begin() };

			for( int i = 0; i < selected; i++ ) ++it;
			( *it )->GetComponent<Transform>()->SetParrent( nullptr );
		}
	}

	ImGui::SetNextItemOpen( true );
	int selection_mask = ( 1 << selected ); // Dumb representation of what may be user-side selection state. You may carry selection state inside or outside your objects in whatever format you see fit.
	static ImGuiTreeNodeFlags baseFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_FramePadding;
	bool open{};
	bool isSelected{};
	bool isDropBegined{};
	bool isDropEnded{};
	if( ImGui::TreeNode( m_Name.c_str() ) )
	{
		int i{};
		std::vector<int> endChildPos;
		if( m_pGameObjects.size() != 0 )
		{
			std::list<GameObject*>::iterator objIt{ m_pGameObjects.begin() };

			while( objIt != m_pGameObjects.end() )
			{

				const bool isCurrentSelected = ( selection_mask & ( 1 << i ) ) != 0;
				ImGuiTreeNodeFlags nodeFlags = baseFlags;
				int nrOfChilderen = ( *objIt )->GetComponent<Transform>()->GetNumberOfChilderen();
				if( isCurrentSelected )
					nodeFlags |= ImGuiTreeNodeFlags_Selected;
				nodeFlags |= ( nrOfChilderen == 0 ? ImGuiTreeNodeFlags_Leaf : 0 ); // ImGuiTreeNodeFlags_Bullet

				open = ImGui::TreeNodeEx( (void*) (intptr_t) i, nodeFlags, ( *objIt )->GetName() );

				isSelected = ImGui::IsItemClicked();

				isDropBegined = ImGui::BeginDragDropSource( ImGuiDragDropFlags_None );
				isDropEnded = ImGui::BeginDragDropTarget();

				if( isSelected )
					selected = i;
				if( isDropBegined )
				{
					ImGui::SetDragDropPayload( "GameObject", &i, sizeof( int ) );
					ImGui::Text( "Swap %s", ( *objIt )->GetName() );
					ImGui::EndDragDropSource();
				}
				if( isDropEnded )
				{
					if( const ImGuiPayload* payload = ImGui::AcceptDragDropPayload( "GameObject" ) )
					{
						int end = *(const int*) payload->Data;
						if( end >= 0 && end < int( m_pGameObjects.size() ) )
						{
							std::list<GameObject*>::iterator it3{ m_pGameObjects.begin() };

							for( int j = 0; j < end; j++ )++it3;
							m_pGameObjects.splice( objIt, m_pGameObjects, it3, std::next( it3 ) );
						}
					}
					ImGui::EndDragDropTarget();
				}
				if( !open )
				{
					std::advance( objIt, nrOfChilderen );
					i += nrOfChilderen;
				}
				else
				{
					endChildPos.push_back( nrOfChilderen );
					for( int& j : endChildPos )
					{
						--j;
					}
					while( endChildPos.size() && endChildPos.back() < 0 )
					{
						ImGui::TreePop();
						endChildPos.pop_back();
					}
				}
				++objIt;
				++i;
			}
		}
		ImGui::TreePop();
	}
	ImGui::End();

	if( m_pGameObjects.size() != 0 )
	{
		ImGui::Begin( "Inspector" );

		std::list<GameObject*>::iterator it{ m_pGameObjects.begin() };


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
			for( int i = 0; i < 11; i++ )
			{
				if( ImGui::Selectable( m_pComponentsString[i] ) )
				{
					switch( ComponentList( i ) )
					{
					case Balbino::ComponentList::Audio:
						( *it )->AddComponent<ConsoleAudio>()->Create();
						break;
					case Balbino::ComponentList::LoggedAudio:
						( *it )->AddComponent<LoggedAudio>()->Create();
						break;
					case Balbino::ComponentList::Avatar:
						( *it )->AddComponent<Avatar>()->Create();
						break;
					case Balbino::ComponentList::Camera:
						( *it )->AddComponent<Camera>( 640.f / 480.f, 640.f )->Create();
						break;
					case Balbino::ComponentList::FPSScript:
						( *it )->AddComponent<FPSScript>()->Create();
						break;
					case Balbino::ComponentList::Text:
						( *it )->AddComponent<Text>()->Create();
						break;
					case Balbino::ComponentList::Sprite:
						( *it )->AddComponent<Sprite>()->Create();
						break;
					case Balbino::ComponentList::Transform:
						( *it )->AddComponent<Transform>()->Create();
						break;
					case Balbino::ComponentList::LevelLoader:
						( *it )->AddComponent<LevelLoader>()->Create();

					case Balbino::ComponentList::BoxCollider2D:
						( *it )->AddComponent<BoxCollider2D>()->Create();
						break;
					case Balbino::ComponentList::Rigidbody2D:
						( *it )->AddComponent<Rigidbody2D>()->Create();
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
#else
Scene::Scene( const std::string& name )
	: m_Name{ name }
{
}
void Scene::Draw() const
{

	auto allCameras = Camera::GetAllCameras();
	if( allCameras.size() != 0 )
	{
		for( auto& camera : allCameras )
		{
			auto currentCam = camera;
			if( currentCam )
			{
				Color clearColor = currentCam->GetClearColor();
				glBindFramebuffer( GL_FRAMEBUFFER, currentCam->GetCameraIndex() );
				glViewport( 0, 0, (int) 640, (int) 480 );
				glClearColor( clearColor.r, clearColor.g, clearColor.b, 1.f );
				glClear( GL_COLOR_BUFFER_BIT );
				glEnable( GL_DEPTH_TEST );
				Shader::SetCamera( *currentCam );
				for( const auto& object : m_pGameObjects )
					object->Draw();

			}
		}
	}

	auto mainCamera = Camera::GetMainCamera();
	glBindFramebuffer( GL_FRAMEBUFFER, 0 );
	glViewport( 0, 0, (int) 1920, (int) 1080 );
	glClearColor( 0.f, 0.f, 0.f, 1.f );
	glClear( GL_COLOR_BUFFER_BIT );

	SDL_GL_SwapWindow( Application::GetWindow() );
}
#endif // _DEBUG

Scene::~Scene()
{
	for( GameObject* pGameObject : m_pGameObjects )
	{
		delete pGameObject;
	}
	m_pGameObjects.clear();
}

void Balbino::Scene::Add( GameObject* pObject, int pos )
{
	if( pos != -1 )
	{
		std::list<GameObject*>::iterator it{ m_pGameObjects.begin() };

		for( int i = 0; i < pos; i++ ) ++it;

		m_pGameObjects.insert( it, pObject );
	}
	else
	{
		m_pGameObjects.push_back( pObject );
	}
}

void Balbino::Scene::FixedUpdate()
{
	if( BTime::TimeScale() )
		for( auto& object : m_pGameObjects )
		object->FixedUpdate();
}

void Scene::Update()
{
	if( BTime::TimeScale() )
		for( auto& object : m_pGameObjects )
			object->Update();
}

void Balbino::Scene::LateUpdate()
{
	if( BTime::TimeScale() )
		for( auto& object : m_pGameObjects )
		object->LateUpdate();

	m_pGameObjects.sort( []( const GameObject* obj, const GameObject* obj2 )
	{
		return obj->IsDestroy() < obj2->IsDestroy();
	} );

	std::list<GameObject*>::reverse_iterator it{ m_pGameObjects.rbegin() };

	for( it; it != m_pGameObjects.rend(); ++it )
	{
		if( !( *it )->IsDestroy() )
		{
			break;
		}
		else
		{
			delete* it;
		}
	}
	m_pGameObjects.erase( it.base(), m_pGameObjects.end() );
}

void Balbino::Scene::Load()
{
	for( auto& object : m_pGameObjects )
		object->Create();
}

void Balbino::Scene::Unload()
{
	for( auto& object : m_pGameObjects )
		object->Destroy();
}