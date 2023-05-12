#include "MainScreen.h"

#include <IScene.h>
#include <IEntity.h>

#include <iostream>

#include "AssetBrowser.h"
#include "SceneHierarchy.h"
#include "ShaderEditor.h"
#include "PropertyPanel.h"
#include "Components/LightComponent.h"
#include "../Renderer/EditorGui.h"

void FawnForge::CMainScreen::Draw()
{
    const int           flags       = 1 << 0 | 1 << 1 | 1 << 2 | 1 << 5 | 1 << 7 | 1 << 10 | 1 << 13;
    bool                open{ true };
    static unsigned int dockspaceID = 0;
    Gui::MaxNextWindow();
    Gui::Begin( "MainWindow", open, flags );
    if ( Gui::BeginMenuBar())
    {
        if ( Gui::BeginMenu( "File" ))
        {
            if ( Gui::MenuItem( "New" ))
            {
                m_saved = false;
                //todo: load new level
            }
            if ( Gui::MenuItem( "Open" ))
            {
                //todo: load level
            }
            if ( Gui::MenuItem( "Save Scene" ))
            {
                //todo: save scene
            }
            if ( Gui::MenuItem( "Save Scene As" ))
            {
                //todo: save scene
            }
            Gui::EndMenu();
        }
        if ( Gui::BeginMenu( "GameObject" ))
        {
            Gui::DrawText( "Add GameObject" );
            if ( Gui::MenuItem( "Empty" ))
            {
                m_pContext->CreateEntity();
                
                std::cout << "add empty object\n";
            }
            if ( Gui::MenuItem( "Light" ))
            {
                IEntity* entity = m_pContext->CreateEntity();
                entity->AddComponent<CLightComponent>();
            }
            Gui::EndMenu();
        }
        if ( Gui::BeginMenu( "View" ))
        {
            if ( Gui::MenuItem( "Hierarchy" ))
            {
                m_pSceneHierarchy->ShowWindow();
            }
            if ( Gui::MenuItem( "Asset Browser" ))
            {
                m_pAssetBrowser->ShowWindow();
            }
            if ( Gui::MenuItem( "Shader Graph" ))
            {
                m_pShaderGraph->ShowWindow();
            }
            if ( Gui::MenuItem( "Properties..." ))
            {
                m_pPropertyPanel->ShowWindow();
            }
            Gui::EndMenu();
        }
        Gui::EndMenuBar();
        static constexpr int dockspaceFlags = 1 << 6;
        dockspaceID = Gui::GetId( "MainWindowDockspace" );
        Gui::DockSpace( dockspaceID, { 0.0f, 0.0f }, dockspaceFlags );
    }
    Gui::End();
    Gui::SetNextWindowDockId( dockspaceID, 1 << 2 );
}

void FawnForge::CMainScreen::SetContext( IScene* pScene, CAssetBrowser* pAssetBrowser, CSceneHierarchy* pHierarchy, CShaderEditor* pGraph, CPropertyPanel* pPropertyPanel )
{
    m_pContext        = pScene;
    m_pAssetBrowser   = pAssetBrowser;
    m_pSceneHierarchy = pHierarchy;
    m_pShaderGraph    = pGraph;
    m_pPropertyPanel  = pPropertyPanel;
}
