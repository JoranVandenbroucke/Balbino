#pragma once
#include <string>

struct IScene;

namespace BalEditor
{
	class CShaderGraph;
	class CSceneHierarchy;
	class CAssetBrowser;

	class CMainScreen
	{
	public:
		CMainScreen() = default;
		~CMainScreen() = default;
		CMainScreen( const CMainScreen& ) = delete;
		CMainScreen( CMainScreen&& ) = delete;
		CMainScreen& operator=( const CMainScreen& ) = delete;
		CMainScreen& operator=( CMainScreen&& ) = delete;

		void Draw();
		void SetContext( IScene* pScene, CAssetBrowser* pAssetBrowser, CSceneHierarchy* pHierarchy, CShaderGraph* pGraph );

	private:
		std::string m_savePosition;
		bool m_saved = false;
		CAssetBrowser* m_pAssetBrowser;
		CSceneHierarchy* m_pSceneHierarchy;
		CShaderGraph* m_pShaderGraph;
		IScene* m_pContext;
	};
}
