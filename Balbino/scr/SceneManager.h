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
	class BALBINO_API SceneManager final: public Singleton<SceneManager>
	{
	public:
		Scene& CreateScene( const std::string& name );

		void SetScene(const unsigned int sceneNr );
		void Update();
		void Draw();
		std::weak_ptr<Scene> GetCurrentScene();
#ifdef _DEBUG
		void DrawEngine();
#endif
	private:
		friend class Singleton<SceneManager>;
		SceneManager() = default;
		std::vector<std::shared_ptr<Scene>> m_Scenes;
		std::weak_ptr<Scene> m_CurrentScenes;
	};
}
#pragma warning(pop)