#pragma once
#include "SceneManager.h"
#include "GameObject/GameObject.h"
#include "Core.h"
#include <list>
#pragma warning(push)
#pragma warning(disable:4251)

namespace Balbino
{
	class BALBINO_API Scene
	{
		friend Scene& SceneManager::CreateScene( const std::string& name );
	public:
		void Add( GameObject* pObject, int pos = -1 );


		void FixedUpdate();
		void Update();
		void LateUpdate();
		void Draw() const;
		void Load();
		void Unload();

		~Scene();
		Scene( const Scene& other ) = delete;
		Scene( Scene&& other ) = delete;
		Scene& operator=( const Scene& other ) = delete;
		Scene& operator=( Scene&& other ) = delete;


	private:
		explicit Scene( const std::string& name );
		std::list<GameObject*> m_pGameObjects;

		std::string m_Name;
		static unsigned int m_IdCounter;
		const char* const m_ComponentsString[9]
		{
			"Audio",
			"LoggedAudio",
			"Avatar",
			"Camera",
			"FPSScript",
			"LevelLoader",
			"Text",
			"Sprite",
			"Transform"
		};
#ifdef _DEBUG
	public:
		void DrawEditor();
	private:
		bool m_Saved;
		std::string m_SavePosition;
#endif // _DEBUG
	};
}
#pragma warning(pop)