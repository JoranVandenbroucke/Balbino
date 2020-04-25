#include "BalbinoPCH.h"
#include "SceneManager.h"
#include "Scene.h"
#include "SceneObject.h"
void Balbino::SceneManager::SetScene( const unsigned int sceneNr )
{
	if( sceneNr < Get().m_Scenes.size() )
	{
		std::shared_ptr<Scene> current = Get().m_CurrentScenes.lock();
		if( current )
		{
			current->Unload();
		}
		current = Get().m_Scenes[sceneNr];
		current->Load();
		Get().m_CurrentScenes = current;
	}
}
void Balbino::SceneManager::Update()
{
	auto currentScene = m_CurrentScenes.lock();
	if( currentScene )
	{
		currentScene->Update();
	}
}

void Balbino::SceneManager::Draw()
{
	auto currentScene = m_CurrentScenes.lock();
	if( currentScene )
	{
		currentScene->Draw();
	}
}

std::weak_ptr<Balbino::Scene> Balbino::SceneManager::GetCurrentScene()
{
	return m_CurrentScenes;
}
#ifdef _DEBUG
void Balbino::SceneManager::DrawEngine()
{
	auto currentScene = m_CurrentScenes.lock();
	if( currentScene )
	{
		currentScene->DrawEditor();
	}
}
#endif //_DEBUG

Balbino::Scene& Balbino::SceneManager::CreateScene( const std::string& name )
{
	const auto scene = std::shared_ptr<Scene>( new Scene( name ) );
	m_Scenes.push_back( scene );
	return *scene;
}