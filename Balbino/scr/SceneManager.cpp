#include "BalbinoPCH.h"
#include "SceneManager.h"
#include "Scene.h"
#include "SceneObject.h"
#include "GameObject/GameObject.h"

void Balbino::SceneManager::SetScene( const unsigned int sceneNr )
{
	if( sceneNr < Get().m_Scenes.size() )
	{
		Scene* current = Get().m_CurrentScenes;

		if( current )
		{
			current->Unload();
		}
		current = Get().m_Scenes[sceneNr];
		current->Load();
		Get().m_CurrentScenes = current;
	}
}
Balbino::GameObject* Balbino::SceneManager::AddGameObjectToScene( GameObject* gameObject )
{
	if( !Get().m_CurrentScenes )
	{
		return nullptr;
	}

	if( gameObject == nullptr )
	{
		gameObject = new GameObject;
	}
	Get().m_CurrentScenes->Add( gameObject );
	return gameObject;
}

void Balbino::SceneManager::FixedUpdate()
{
	auto currentScene = m_CurrentScenes;
	if( currentScene )
	{
		currentScene->FixedUpdate();
	}
}
void Balbino::SceneManager::Update()
{
	auto currentScene = m_CurrentScenes;
	if( currentScene )
	{
		currentScene->Update();
	}
}

void Balbino::SceneManager::LateUpdate()
{
	auto currentScene = m_CurrentScenes;
	if( currentScene )
	{
		currentScene->LateUpdate();
	}
}

void Balbino::SceneManager::Draw()
{
	auto currentScene = m_CurrentScenes;
	if( currentScene )
	{
		currentScene->Draw();
	}
}

Balbino::Scene* Balbino::SceneManager::GetCurrentScene()

{
	return m_CurrentScenes;
}
#ifdef _DEBUG
void Balbino::SceneManager::DrawEngine()
{
	auto currentScene = m_CurrentScenes;
	if( currentScene )
	{
		currentScene->DrawEditor();
	}
}
#endif //_DEBUG

Balbino::Scene& Balbino::SceneManager::CreateScene( const std::string& name )
{
	Scene* scene = new Scene{ name };

	m_Scenes.push_back( scene );
	return *scene;
}