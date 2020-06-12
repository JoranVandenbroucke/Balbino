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
	public:
		void Add( GameObject* pObject, int pos = -1 );
		GameObject* GetGameObject( const std::string name );

		void FixedUpdate();
		void Update();
		void LateUpdate();
		void Load();
		void Unload();

		~Scene();
		Scene( const Scene& other ) = delete;
		Scene( Scene&& other ) = delete;
		Scene& operator=( const Scene& other ) = delete;
		Scene& operator=( Scene&& other ) = delete;

	private:
		explicit Scene( const std::string& name );
		friend Scene& SceneManager::CreateScene( const std::string& name );
		std::list<GameObject*> m_pGameObjects;

		std::string m_Name;
		static unsigned int m_IdCounter;
		const char* const m_pComponentsString[12]
		{
			"Audio",
			"LoggedAudio",
			"Avatar",
			"Animation",
			"BoxCollider2D",
			"Camera",
			"FPSScript",
			"LevelLoader",
			"Rigidbody2D",
			"Sprite",
			"Text",
			"Transform"
		};

#ifdef _DEBUG
	public:
		void DrawEditor();
		void Draw();
	private:
		bool m_Saved;
		bool m_CanPlay;
		std::string m_SavePosition;
#else
	public:
		void Draw()const;

#endif // _DEBUG
	};
}
#pragma warning(pop)