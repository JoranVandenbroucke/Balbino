#pragma once
#include "Singleton.h"
#include <string>
#include <vector>
#include <memory>
#include "Core.h"
#pragma warning(push)
#pragma warning(disable:4251)

namespace Balbino
{
	class Scene;
	class GameObject;
	class BALBINO_API SceneManager final: public Singleton<SceneManager>
	{
	public:
		Scene& CreateScene( const std::string& name );
		virtual ~SceneManager() = default;
		SceneManager( SceneManager& ) = delete;
		SceneManager( SceneManager&& ) = delete;
		SceneManager& operator=( SceneManager& ) = delete;
		SceneManager& operator=( SceneManager&& ) = delete;
		static void SetScene(const unsigned int sceneNr );
		static GameObject* AddGameObjectToScene( GameObject* gameObject = nullptr);
		static GameObject* GetGameObjectByName( const std::string name );
		void FixedUpdate();
		void Update();
		void LateUpdate();
		void Draw();
		Scene* GetCurrentScene();
		void Stop();
#ifdef _DEBUG
		void DrawEngine();
#endif
	private:
		friend class Singleton<SceneManager>;
		SceneManager();
		std::vector<Scene*> m_pScenes;

		Scene* m_pCurrentScenes;
	};
}
#pragma warning(pop)