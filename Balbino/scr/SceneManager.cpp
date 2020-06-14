#include "BalbinoPCH.h"
#include "SceneManager.h"
#include "Scene.h"
#include "SceneObject.h"
#include "GameObject/GameObject.h"

Balbino::SceneManager::SceneManager()
	:Singleton{}
	, m_pCurrentScenes{ nullptr }
{
}

void Balbino::SceneManager::SetScene( const unsigned int sceneNr )
{
	if( sceneNr < Get().m_pScenes.size() )
	{
		Scene* current = Get().m_pCurrentScenes;

		if( current )
		{
			current->Unload();
		}
		Get().m_pCurrentScenes = Get().m_pScenes[sceneNr];
		Get().m_pCurrentScenes->Load();
	}
}
Balbino::GameObject* Balbino::SceneManager::AddGameObjectToScene( GameObject* gameObject )
{
	if( !Get().m_pCurrentScenes )
	{
		return nullptr;
	}

	if( gameObject == nullptr )
	{
		gameObject = DBG_NEW GameObject;
	}
	Get().m_pCurrentScenes->Add( gameObject );
	return gameObject;
}

Balbino::GameObject* Balbino::SceneManager::GetGameObjectByName( const std::string name )
{
	if( !Get().m_pCurrentScenes )
	{
		return nullptr;
	}

	return Get().m_pCurrentScenes->GetGameObject( name );
}

void Balbino::SceneManager::FixedUpdate()
{
	auto currentScene = m_pCurrentScenes;
	if( currentScene )
	{
		currentScene->FixedUpdate();
	}
}
void Balbino::SceneManager::Update()
{
	auto currentScene = m_pCurrentScenes;
	if( currentScene )
	{
		currentScene->Update();
	}
}

void Balbino::SceneManager::LateUpdate()
{
	auto currentScene = m_pCurrentScenes;
	if( currentScene )
	{
		currentScene->LateUpdate();
	}
}

void Balbino::SceneManager::Draw()
{
	auto currentScene = m_pCurrentScenes;
	if( currentScene )
	{
		currentScene->Draw();
	}
}

Balbino::Scene* Balbino::SceneManager::GetCurrentScene()
{
	return m_pCurrentScenes;
}
void Balbino::SceneManager::Stop()
{
	m_pCurrentScenes = nullptr;
	for( Scene* pScene : m_pScenes )
	{
		delete pScene;
		pScene = nullptr;
	}
	m_pScenes.clear();
}
#ifdef BALBINO_DEBUG
void Balbino::SceneManager::DrawEngine()
{
	auto currentScene = m_pCurrentScenes;
	if( currentScene )
	{
		currentScene->DrawEditor();
	}
}
#endif //BALBINO_DEBUG

Balbino::Scene& Balbino::SceneManager::CreateScene( const std::string& name )
{
	Scene* scene = DBG_NEW Scene{ name };

	m_pScenes.push_back( scene );
	return *scene;
}