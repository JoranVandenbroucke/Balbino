#pragma once
#include "SceneManager.h"
#include "GameObject/GameObject.h"
#include "Core.h"
#include "Renderer/Renderer.h"
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
		void LoadFromFile( const std::string file );
		void SaveToFile( const std::string file );

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
		static const int m_ConponentAmount{ 15 };
		const char* const m_pComponentsString[m_ConponentAmount]
		{
			"Audio",
			"LoggedAudio",
			"Avatar",
			"Animation",
			"Bubble",
			"BubbleManager",
			"BoxCollider2D",
			"Camera",
			"Enemy",
			"FPSScript",
			"LevelLoader",
			"Rigidbody2D",
			"Sprite",
			"Text",
			"Transform"
		};

		std::string m_SavePosition;
#ifdef BALBINO_DEBUG
	public:
		void DrawEditor();
		void Draw();
	private:
		bool m_Saved;
		bool m_CanPlay;
#else
	public:
		void Draw()const;
	private:
		Balbino::vertex m_Vert[4]
		{
			{-1.0f, 1.0f,0.0f, 0.f,0.f, 1.f,0.f,0.f,1.f},
			{-1.0f, -1.0f,0.0f, 0.f,1.f, 0.f,0.f,1.f,1.f},
			{ 1.0f, 1.0f,0.0f, 1.f,0.f, 0.f,1.f,0.f,1.f},
			{ 1.0f, -1.0f,0.0f, 1.f,1.f, 1.f,1.f,1.f,1.f}
		};
		const UINT32 m_Index[6]
		{
			0,1,2,
			1,2,3
		};
		VertexBuffer m_VertexBuff;
		IndexBuffer m_IndexBuff;
		Shader m_ScreenShader;
#endif // BALBINO_DEBUG
	};
}
#pragma warning(pop)