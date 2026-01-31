#pragma once
#include <Core.h>
#include <string>

namespace BalbinoScene
{
    class CScene;
}

namespace FawnForge
{
    class CAssetBrowser;
    class CSceneHierarchy;
    class CShaderEditor;
    class CPropertyPanel;
    class CMainScreen final
    {
    public:
        CMainScreen()                                = default;
        ~CMainScreen()                               = default;
        CMainScreen( const CMainScreen& )            = delete;
        CMainScreen( CMainScreen&& )                 = delete;
        CMainScreen& operator=( const CMainScreen& ) = delete;
        CMainScreen& operator=( CMainScreen&& )      = delete;

        void Draw() noexcept;
        void SetContext( BalbinoScene::CScene* pScene, CAssetBrowser* pAssetBrowser, CSceneHierarchy* pHierarchy, CShaderEditor* pGraph, CPropertyPanel* pPropertyPanel ) noexcept;

    private:
        std::string m_savePosition {};

        CAssetBrowser* m_pAssetBrowser {BALBINO_NULL};
        CSceneHierarchy* m_pSceneHierarchy {BALBINO_NULL};
        CShaderEditor* m_pShaderGraph {BALBINO_NULL};
        CPropertyPanel* m_pPropertyPanel {BALBINO_NULL};
        BalbinoScene::CScene* m_pScene{BALBINO_NULL};
    };
}// namespace FawnForge
