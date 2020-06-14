#include "BalbinoPCH.h"
#include "Scene.h"
#include "Components/All.h"
#include "BinaryReaderWrider.h"
#include "Editor/Debug.h"
#include "Editor/AssetBrouser.h"
#include "Editor/SpriteEditor.h"
#include "Application.h"
#include "Time.h"
#include "InputManager.h"

#include <windows.h>
#include <commdlg.h>
#include <string.h>
#include <iostream>
#include <regex>

using namespace Balbino;

unsigned int Scene::m_IdCounter = 0;

#ifdef BALBINO_DEBUG
Scene::Scene( const std::string& name )
	: m_Name{}
	, m_Saved{}
	, m_CanPlay{}
	, m_SavePosition{ name }
{
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
	InputManager::DrawInspector();

	ImGui::Begin( "GameView" );
	ImGui::BeginChild( "game", ImVec2{ 640, 500 }, true );
	//ImGui::Image( (ImTextureID) (intptr_t) m_RenderedTexture, ImVec2{ 640,480 } );
	if( mainCamera )
	{
		/*	ImGui::GetWindowDrawList()->AddImage( (void*) (uint64_t) mainCamera->GetTargetTexture(),
				ImVec2{ ImGui::GetCursorScreenPos() },
				ImVec2{ ImGui::GetCursorScreenPos().x + 640, ImGui::GetCursorScreenPos().y + 480 } );*/
		ImGui::Image( (void*) (uint64_t) mainCamera->GetTargetTexture(), { 640.f,480.f } );
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
				Load();
				//std::regex sceneName{ R"(^.*\\(.*)\.Balbino$)" };
				//std::smatch match;
				//if( std::regex_match( m_SavePosition, match, sceneName ) )
				//	m_Name = match[1].str();

				//std::ifstream saveFile{};
				//saveFile.open( m_SavePosition, std::ios::in | std::ios::binary );
				//if( saveFile.is_open() )
				//{
				//	for( GameObject* pObject: m_pGameObjects)
				//	{
				//		delete pObject;
				//	}
				//	m_pGameObjects.clear();
				//	int size{};
				//	BinaryReadWrite::Read( saveFile, size );
				//	for( int i = 0; i < size; i++ )
				//	{
				//		GameObject* gameObject = DBG_NEW GameObject{};

				//		gameObject->Load( saveFile );
				//		//gameObject->Create();
				//		Add( gameObject );
				//	}
				//	Load();
				//	//m_pGameObjects.reverse();
				//	m_Saved = true;
				//}
				//saveFile.close();
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

				SaveToFile( m_SavePosition );
			}
			if( ImGui::MenuItem( "Save Scene As" ) )
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
				if( GetSaveFileName( &ofn ) )
				{
					m_SavePosition = std::filesystem::relative( fileName, currentPath ).u8string();
				}
				std::filesystem::current_path( currentPath );
				std::regex sceneName{ R"(^.*\\(.*)\.Balbino$)" };
				std::smatch match;
				if( std::regex_match( m_SavePosition, match, sceneName ) )
					m_Name = match[1].str();
				SaveToFile( m_SavePosition );
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

		ImGui::InputText( "Name", name, 32 );
		( *it )->SetName( name );
		( *it )->DrawInspector();

		if( ImGui::Button( "Add Component" ) )
		{
			ImGui::OpenPopup( "Select Components" );
		}
		if( ImGui::BeginPopup( "Select Components" ) )
		{
			for( int i = 0; i < m_ConponentAmount; i++ )
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
						( *it )->AddComponent<CharacterController>()->Create();
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
						break;
					case Balbino::ComponentList::BoxCollider2D:
						( *it )->AddComponent<BoxCollider2D>()->Create();
						break;
					case Balbino::ComponentList::Rigidbody2D:
						( *it )->AddComponent<Rigidbody2D>()->Create();
						break;
					case Balbino::ComponentList::Animation:
						( *it )->AddComponent<Animation>()->Create();
						break;
					case Balbino::ComponentList::Enemy:
						( *it )->AddComponent<Enemy>()->Create();
						break;
					case Balbino::ComponentList::Bubble:
						( *it )->AddComponent<Bubble>()->Create();
						break;
					case Balbino::ComponentList::BubbleManager:
						( *it )->AddComponent<BubbleManager>()->Create();
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
	: m_Name{}
	, m_VertexBuff{ (void*) m_Vert, 4 }
	, m_IndexBuff{ (void*) m_Index, 6, sizeof( m_Index[0] ) }
	, m_ScreenShader{ "Shaders/Screen.vert", "Shaders/Screen.frag" }
	, m_SavePosition{ name }
{
}
void Scene::Draw() const
{
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
				glClearColor( clearColor.r, clearColor.g, clearColor.b, 1.f );
				glClear( GL_COLOR_BUFFER_BIT );
				Shader::SetCamera( *currentCam );
				for( const auto& object : m_pGameObjects )
					object->Draw();

			}
		}
	}
	auto mainCamera = Camera::GetMainCamera();
	glBindFramebuffer( GL_FRAMEBUFFER, 0 );
	glClearColor( 1.0f, 1.0f, 1.0f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT );
	if( mainCamera )
	{
		m_ScreenShader.Bind();
		m_VertexBuff.Bind();
		m_IndexBuff.Bind();
		glActiveTexture( GL_TEXTURE0 );
		glBindTexture( GL_TEXTURE_2D, mainCamera->GetTargetTexture() );
		glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0 );
		//Shader::SetCamera( *mainCamera );
		//for( const auto& object : m_pGameObjects )
		//	object->Draw();
		//glBindTexture( GL_TEXTURE_2D, mainCamera->GetTargetTexture() );	// use the color attachment texture as the texture of the quad plane
		//glDrawArrays( GL_TRIANGLES, 0, 6 );
	}
	InputManager::DrawInspector();
	ImGui::Begin( "GameOptions" );
	GameObject* pBub = GetGameObject( "Bub" );
	GameObject* pBob = GetGameObject( "Bob" );
	if( pBub && pBob )
	{
		const auto allDevices = InputManager::GetAllInputDevices();
		bool playBub = pBub->ActiveInHierarchy();
		bool playBob = pBob->ActiveInHierarchy();
		std::string currentDeviceBub = pBub->GetComponent<CharacterController>()->GetCurrentDevice();
		std::string currentDeviceBob = pBub->GetComponent<CharacterController>()->GetCurrentDevice();
		ImGui::Text( "Character Settings:" );
		ImGui::PushID( 1 );
		ImGui::Text( "Bub:" );
		if( ImGui::Checkbox( "Enable", &playBub ) )
		{
			pBub->SetActive( playBub );
		}
		if( ImGui::BeginCombo( "CurrentDevice", currentDeviceBub.c_str() ) )
		{
			for( const std::string& device : allDevices )
			{
				bool isSelected = device == currentDeviceBub;
				if( ImGui::Selectable( device.c_str() ) )
				{
					std::regex numberRemover{ R"(^\d: (.+)$)" };
					std::smatch match;
					if( std::regex_match( device, match, numberRemover ) )
					{
						pBub->GetComponent<CharacterController>()->SetCurrentDevice( match[1] );
					}
				}
				if( isSelected )
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}
		ImGui::PopID();

		ImGui::PushID( 1 );
		ImGui::Text( "Bob:" );
		if( ImGui::Checkbox( "Enable", &playBob ) )
		{
			pBub->SetActive( playBob );
		}
		if( ImGui::BeginCombo( "CurrentDevice", currentDeviceBob.c_str() ) )
		{
			for( const std::string& device : allDevices )
			{
				bool isSelected = device == currentDeviceBob;
				if( ImGui::Selectable( device.c_str() ) )
				{
					std::regex numberRemover{ R"(^\d: (.+)$)" };
					std::smatch match;
					if( std::regex_match( device, match, numberRemover ) )
					{
						pBob->GetComponent<CharacterController>()->SetCurrentDevice( match[1] );
					}
				}
				if( isSelected )
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}
		ImGui::PopID();
		ImGui::End();
		ImGui::Render();
	}
	ImGui_ImplOpenGL3_RenderDrawData( ImGui::GetDrawData() );
	ImGuiIO& io = ImGui::GetIO();
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
#endif // BALBINO_DEBUG

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

GameObject* Balbino::Scene::GetGameObject( const std::string name )const
{
	auto it = std::find_if( m_pGameObjects.cbegin(), m_pGameObjects.cend(), [name]( const GameObject* pGameObject )
	{
		return pGameObject->GetName() == name;
	} );
	if( it == m_pGameObjects.cend() )
		return nullptr;
	return ( *it );
}

void Balbino::Scene::FixedUpdate()
{
	if( BTime::TimeScale() )
		for( auto& object : m_pGameObjects )
			object->FixedUpdate();
}

void Balbino::Scene::Update()
{
	for( auto& object : m_pGameObjects )
		object->Update();
}

void Balbino::Scene::LateUpdate()
{
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
	LoadFromFile( m_SavePosition );
	for( auto& object : m_pGameObjects )
		object->Create();
}

void Balbino::Scene::Unload()
{
	for( auto& object : m_pGameObjects )
		delete object;
	m_pGameObjects.clear();
}

void Balbino::Scene::LoadFromFile( const std::string file )
{
	std::regex sceneName{ R"(^.*\\(.*)\.Balbino$)" };
	std::smatch match;
	if( std::regex_match( file, match, sceneName ) )
		m_Name = match[1].str();

	std::ifstream saveFile{};
	saveFile.open( file, std::ios::in | std::ios::binary );
	if( saveFile.is_open() )
	{
		for( GameObject* pObject : m_pGameObjects )
		{
			delete pObject;
		}
		m_pGameObjects.clear();
		int size{};
		BinaryReadWrite::Read( saveFile, size );
		for( int i = 0; i < size; i++ )
		{
			GameObject* gameObject = DBG_NEW GameObject{};

			gameObject->Load( saveFile );
			Add( gameObject );
		}
		m_SavePosition = file;
#ifdef BALBINO_DEBUG
		m_Saved = true;
#endif // BALBINO_DEBUG

	}
	else
	{
		m_Name = "Default Scene";
		GameObject* gameObject = DBG_NEW GameObject{};

		gameObject->AddComponent<Camera>( 640.f / 480.f, 640.f );
		gameObject->SetName( "Maint Camera" );
		Add( gameObject );
	}
	saveFile.close();
}

void Balbino::Scene::SaveToFile( const std::string file )
{
	std::ofstream saveFile{};
	saveFile.open( file, std::ios::out | std::ios::binary );
	if( saveFile.is_open() )
	{
		m_SavePosition = file;
		BinaryReadWrite::Write( saveFile, int( m_pGameObjects.size() ) );
		for( GameObject* gameObject : m_pGameObjects )

		{
			gameObject->Save( saveFile );
		}
#ifdef BALBINO_DEBUG
		m_Saved = true;
#endif // BALBINO_DEBUG

	}
	saveFile.close();
}
