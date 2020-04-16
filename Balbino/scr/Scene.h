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
		void Add( const std::shared_ptr<GameObject>& object, int pos = -1 );

		void Update();
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
		std::list<std::shared_ptr<GameObject>> m_GameObjects;
		std::string m_Name;
		static unsigned int m_IdCounter;
		enum class ComponentList
		{
			Audio,
			LoggedAudio,
			Avatar,
			Camera,
			FPSScript,
			Text,
			Texture2D,
			Transform
		} m_ComponentsEnum;
		const char* const m_ComponentsString[8]
		{
			"Audio",
			"LoggedAudio",
			"Avatar",
			"Camera",
			"FPSScript",
			"Text",
			"Texture2D",
			"Transform"
		};
#ifdef _DEBUG
	public:
		void DrawEditor();
	private:
		//std::vector<int> m_Indent;
		std::list<int> m_IndentPosition;
#endif // _DEBUG
	};
}
#pragma warning(pop)