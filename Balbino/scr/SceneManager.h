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

		void Update();
		void Draw();
	private:
		friend class Singleton<SceneManager>;
		SceneManager() = default;
		std::vector<std::shared_ptr<Scene>> m_Scenes;
	};
}
#pragma warning(pop)