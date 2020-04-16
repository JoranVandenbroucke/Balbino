#include "BalbinoPCH.h"
#include "Scene.h"
#include "Components/All.h"
#include "GameObject/GameObject.h"
#include "imgui-1.75/imgui.h"

using namespace Balbino;

unsigned int Scene::m_IdCounter = 0;

Scene::Scene( const std::string& name )
	: m_GameObjects{}
	, m_Name{ name }
{
}

Scene::~Scene() = default;

void Balbino::Scene::Add( const std::shared_ptr<GameObject> & object, int pos )
{
	std::list<std::shared_ptr<GameObject>>::iterator it{ m_GameObjects.begin() };
	if( pos != -1 )
		for( size_t i = 0; i < pos; i++ ) ++it;

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
			}
			if( ImGui::MenuItem( "Open" ) )
			{
			}
			if( ImGui::MenuItem( "Save Scene" ) )
			{
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
			for( size_t i = 0; i < selected; i++ ) ++it;

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
			for( size_t i = 1; i < selected; i++ ) ++it;
			for( size_t i = 1; i < selected; i++ ) ++it2;
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
			for( size_t i = 0; i < selected; i++ ) ++it;
			for( size_t i = 0; i < selected; i++ ) ++it3;
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
			for( size_t i = 0; i < selected; i++ ) ++it;
			for( size_t i = 0; i < selected; i++ ) ++it2;
			( *it2 ) = 0;
			( *it )->GetComponent<Transform>()->SetParrent( nullptr );
		}
	}



	ImGui::SetNextItemOpen( true );
	int selection_mask = ( 1 << selected ); // Dumb representation of what may be user-side selection state. You may carry selection state inside or outside your objects in whatever format you see fit.
	static ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
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
				const bool is_selected = ( selection_mask & ( 1 << i ) ) != 0;
				ImGuiTreeNodeFlags node_flags = base_flags;
				if( is_selected )
					node_flags |= ImGuiTreeNodeFlags_Selected;
				node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen; // ImGuiTreeNodeFlags_Bullet

				int nrOfChilderen = ( *it1 )->GetComponent<Transform>()->GetNumberOfChilderen();

				int indent = *it2;
				//ImGui::PushStyleVar( ImGuiStyleVar_FramePadding, ImVec2( 5, 5 ) );
				open = ImGui::TreeNodeEx( (void*) (intptr_t) i,
					ImGuiTreeNodeFlags_FramePadding | ImGuiTreeNodeFlags_NoTreePushOnOpen | ( is_selected ? ImGuiTreeNodeFlags_Selected : 0 ) || ( nrOfChilderen == 0 ? ImGuiTreeNodeFlags_Leaf : 0 ),
					( *it1 )->GetName() );
				//ImGui::PopStyleVar();


				ImGui::PushID( (void*) &i );
				if( ImGui::BeginPopupContextItem() )
				{
					// Some processing...
					ImGui::EndPopup();
				}
				ImGui::PopID();

				if( ImGui::IsItemClicked() )
					selected = i;

				if( ImGui::BeginDragDropSource( ImGuiDragDropFlags_None ) )
				{
					ImGui::SetDragDropPayload( "GameObject", &i, sizeof( int ) );
					ImGui::Text( "Swap %s", ( *it1 )->GetName() );
					// Some processing...
					ImGui::EndDragDropSource();
				}

				if( ImGui::BeginDragDropTarget() )
				{
					if( const ImGuiPayload* payload = ImGui::AcceptDragDropPayload( "GameObject" ) )
					{
						int end = *(const int*) payload->Data;
						if( end >= 0 && end < m_GameObjects.size() )
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
					if( ( i + 1 != m_IndentPosition.size() && *( ++it2 ) <= indent ) )
					{
						ImGui::TreePop();
						//for( int j = 0; j < m_IndentPosition[i + 1] - indent; j++ )
						//{
						//	ImGui::TreePop();
						//}
					}
					if( i + 1 == m_IndentPosition.size() )
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
						if( i + 1 == m_IndentPosition.size() )
						{
							for( int j = 0; j < indent; j++ )
							{
								ImGui::TreePop();
							}
						}

						while( i + 1 < m_IndentPosition.size()
							&& *( ++it2 ) > indent )++i;
						if( i + 1 == m_IndentPosition.size()
							&& *it2 > indent )
						{
							break;
						}
					}
					if( iOld == i )++i;
				}
				iOld = i;
			}
			while( i < m_GameObjects.size() /*|| ( i == 0 && m_GameObjects.size() == 1 )*/ );
		}
		ImGui::TreePop();
	}

	ImGui::End();
	if( m_GameObjects.size() != 0 )
	{
		ImGui::Begin( "Inspector" );

		std::list<std::shared_ptr<GameObject>>::iterator it{ m_GameObjects.begin() };

		for( int i = 0; i < selected; i++ )++it;
		char* name{ const_cast<char*>( ( *it )->GetName() ) };
		ImGui::InputText( "Name:", name, 32 );
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
					case Balbino::Scene::ComponentList::Audio:
						( *it )->AddComponent<ConsoleAudio>();
						( *it )->GetComponent<ConsoleAudio>()->Create();
						break;
					case Balbino::Scene::ComponentList::LoggedAudio:
						( *it )->AddComponent<LoggedAudio>();
						( *it )->GetComponent<LoggedAudio>()->Create();
						break;
					case Balbino::Scene::ComponentList::Avatar:
						( *it )->AddComponent<Avatar>();
						( *it )->GetComponent<Avatar>()->Create();
						break;
					case Balbino::Scene::ComponentList::Camera:
						//( *it )->AddComponent<Camera>();
						break;
					case Balbino::Scene::ComponentList::FPSScript:
						( *it )->AddComponent<FPSScript>();
						( *it )->GetComponent<FPSScript>()->Create();
						break;
					case Balbino::Scene::ComponentList::Text:
						( *it )->AddComponent<Text>();
						( *it )->GetComponent<Text>()->Create();
						break;
					case Balbino::Scene::ComponentList::Texture2D:
						( *it )->AddComponent<Texture2D>();
						( *it )->GetComponent<Texture2D>()->Create();
						break;
					case Balbino::Scene::ComponentList::Transform:
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