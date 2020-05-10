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

		static void SetScene(const unsigned int sceneNr );
		static GameObject* AddGameObjectToScene( GameObject* gameObject = nullptr);

		void FixedUpdate();
		void Update();
		void LateUpdate();
		void Draw();
		Scene* GetCurrentScene();

#ifdef _DEBUG
		void DrawEngine();
#endif
	private:
		friend class Singleton<SceneManager>;
		SceneManager() = default;
		std::vector<Scene*> m_Scenes;

		Scene* m_CurrentScenes;
	};
}
#pragma warning(pop)