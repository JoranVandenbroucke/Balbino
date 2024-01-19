#include "MainScreen.h"
#include <CameraComponent.h>
#include <LightComponent.h>
#include <Scene.h>

#include "../Renderer/EditorGui.h"

#include "AssetBrowser.h"
#include "MaterialEditor.h"
#include "PropertyPanel.h"
#include "SceneHierarchy.h"
#include "ShaderEditor.h"

void FawnForge::CMainScreen::Draw() noexcept
{
    const int flags = 1 << 0 | 1 << 1 | 1 << 2 | 1 << 5 | 1 << 7 | 1 << 10 | 1 << 13;
    bool open { true };
    static unsigned int dockspaceId = 0;
    Gui::MaxNextWindow();
    Gui::Begin( "MainWindow", open, flags );
    if ( Gui::BeginMenuBar() )
    {
        if ( Gui::BeginMenu( "File" ) )
        {
            if ( Gui::MenuItem( "New" ) )
            {
                //todo: Load new level
            }
            if ( Gui::MenuItem( "Open" ) )
            {
                //todo: Load level
            }
            if ( Gui::MenuItem( "Save Scene" ) )
            {
                //todo: save scene
            }
            if ( Gui::MenuItem( "Save Scene As" ) )
            {
                //todo: save scene
            }
            Gui::EndMenu();
        }
        if ( Gui::BeginMenu( "GameObject" ) )
        {
            Gui::Text( "Add GameObject" );
            if ( Gui::MenuItem( "Empty" ) )
            {
                m_pScene->CreateEntity();

                std::cout << "mix_mode_add empty object\n";
            }
            if ( Gui::MenuItem( "Light" ) )
            {
                BalbinoScene::CEntity entity {m_pScene->CreateEntity()};
                entity.AddComponent<CervidaeComponent::CLightComponent>();
            }
            Gui::EndMenu();
        }
        if ( Gui::BeginMenu( "View" ) )
        {
            if ( Gui::MenuItem( "Hierarchy" ) )
            {
                m_pSceneHierarchy->ShowWindow();
            }
            if ( Gui::MenuItem( "Asset Browser" ) )
            {
                m_pAssetBrowser->ShowWindow();
            }
            if ( Gui::MenuItem( "Shader Graph" ) )
            {
                m_pShaderGraph->ShowWindow( SFile { std::string( m_pAssetBrowser->GetCurrentDirectory() ), "" } );
            }
            if ( Gui::MenuItem( "Properties..." ) )
            {
                m_pPropertyPanel->ShowWindow();
            }
            Gui::EndMenu();
        }
        Gui::EndMenuBar();
        static constexpr int dockspaceFlags = 1 << 6;
        dockspaceId                         = Gui::GetId( "MainWindowDockspace" );
        Gui::DockSpace( dockspaceId, { 0.0f, 0.0f }, dockspaceFlags );
    }
    Gui::End();
    Gui::SetNextWindowDockId( dockspaceId, 1 << 2 );
}

void FawnForge::CMainScreen::SetContext( BalbinoScene::CScene* pScene, CAssetBrowser* pAssetBrowser, CSceneHierarchy* pHierarchy, CShaderEditor* pGraph, CPropertyPanel* pPropertyPanel ) noexcept
{
    m_pScene          = pScene;
    m_pAssetBrowser   = pAssetBrowser;
    m_pSceneHierarchy = pHierarchy;
    m_pShaderGraph    = pGraph;
    m_pPropertyPanel  = pPropertyPanel;
}
