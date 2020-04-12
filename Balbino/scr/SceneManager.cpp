#include "BalbinoPCH.h"
#include "SceneManager.h"
#include "Scene.h"
#include "SceneObject.h"
void Balbino::SceneManager::SetScene( const unsigned int sceneNr )
{
	if( sceneNr < m_Scenes.size() )
	{
		std::shared_ptr<Scene> current = m_CurrentScenes.lock();
		if( current )
		{
			current->Unload();
		}
		current = m_Scenes[sceneNr];
		current->Load();
		m_CurrentScenes = current;
	}
}
void Balbino::SceneManager::Update()
{
	m_CurrentScenes.lock()->Update();
}

void Balbino::SceneManager::Draw()
{
	m_CurrentScenes.lock()->Draw();
}

void Balbino::SceneManager::DrawEngine()
{
	m_CurrentScenes.lock()->DrawEditor();
}

Balbino::Scene& Balbino::SceneManager::CreateScene( const std::string& name )
{
	const auto scene = std::shared_ptr<Scene>( new Scene( name ) );
	m_Scenes.push_back( scene );
	return *scene;
	//return Get().ICreateScene( name );
}