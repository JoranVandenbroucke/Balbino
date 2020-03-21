#pragma once
#include "SceneManager.h"
#include "Core.h"
#pragma warning(push)
#pragma warning(disable:4251)
namespace Balbino
{
	class SceneObject;
	class BALBINO_API Scene
	{
		friend Scene& SceneManager::CreateScene( const std::string& name );
	public:
		void Add( const std::shared_ptr<SceneObject>& object );

		void Update();
		void Draw() const;

		~Scene();
		Scene( const Scene& other ) = delete;
		Scene( Scene&& other ) = delete;
		Scene& operator=( const Scene& other ) = delete;
		Scene& operator=( Scene&& other ) = delete;

	private:
		explicit Scene( const std::string& name );

		std::string m_Name;
		std::vector <std::shared_ptr<SceneObject>> m_Objects{};

		static unsigned int m_IdCounter;
	};
}
#pragma warning(pop)