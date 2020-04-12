#include "BalbinoPCH.h"
#include "Scene.h"
#include "SceneObject.h"
#include "imgui-1.75/imgui.h"

using namespace Balbino;

unsigned int Scene::m_IdCounter = 0;

Scene::Scene( const std::string& name ) : m_Name( name.c_str() )
{
}

Scene::~Scene() = default;

void Scene::Add( const std::shared_ptr<SceneObject> & object )
{
	m_Objects.push_back( object );
}

void Scene::Update()
{
	for( auto& object : m_Objects )
	{
		object->Update();
	}
}

void Scene::Draw() const
{
	for( const auto& object : m_Objects )
	{
		object->Draw();
	}
}

#ifdef _DEBUG
void Balbino::Scene::DrawEditor()
{
	static int selected{ 0 };
	ImGui::Begin( "Hierarchy" );
	if( ImGui::TreeNode( m_Name.c_str() ) )
	{
		static int selection_mask = ( 1 << selected ); // Dumb representation of what may be user-side selection state. You may carry selection state inside or outside your objects in whatever format you see fit.
		static ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
		for( int i{}; i < m_Objects.size(); ++i )
		{
			ImGuiTreeNodeFlags node_flags = base_flags;
			const bool is_selected = ( selection_mask & ( 1 << i ) ) != 0;
			if( is_selected )
				node_flags |= ImGuiTreeNodeFlags_Selected;
			node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen; // ImGuiTreeNodeFlags_Bullet
			ImGui::TreeNodeEx( (void*) (intptr_t) i, node_flags, "GameObject %d", i );
			if( ImGui::IsItemClicked() )
				selected = i;
		}
		ImGui::TreePop();
	}
	ImGui::End();

	if( m_Objects.size() != 0 )
	{
		ImGui::Begin( "Inspector" );
		m_Objects[selected]->DrawInspector();
		ImGui::End();
	}
}
#endif // _DEBUG


void Balbino::Scene::Load()
{
	for( auto& object : m_Objects )
	{
		object->Create();
	}
}

void Balbino::Scene::Unload()
{
	for( auto& object : m_Objects )
	{
		object->Destroy();
	}
}