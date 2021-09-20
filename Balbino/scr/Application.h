#pragma once
#include "Core.h"
#include "Managers/Manager.h"

struct SDL_Window;
namespace Balbino
{
	class CRenderer;
	class CInputHandler;
	class CCameraManager;
	class CTextureManager;
	class CShaderManager;
	class CMeshManager;

#ifdef BL_EDITOR
	class CInterface;
#endif
	
	class BALBINO_API Application
	{
	public:
		Application();
		virtual ~Application();
		Application( const Application& ) = delete;
		Application( Application&& ) = delete;
		Application& operator=( const Application& ) = delete;
		Application& operator=( Application&& ) = delete;

		void Initialize();
		virtual void LoadGame() const;
		void Cleanup();
		void Run();

	private:
		SDL_Window* m_pWindow;

		CManager m_manager;
		CRenderer* m_pRenderer;
		CInputHandler* m_pInputHandler;
		CCameraManager* m_pCameraManager;
		CTextureManager* m_pTextureManager;
		CShaderManager* m_pShaderManager;
		CMeshManager* m_pMeshManager;
#ifdef BL_EDITOR
		CInterface* m_pInterface;;
#endif
	};

	//to be defined in client
	Application* CreateApplication();
}
