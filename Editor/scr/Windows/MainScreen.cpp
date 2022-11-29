#include "MainScreen.h"

#include <IScene.h>
#include <IEntity.h>

#include <iostream>

#include "AssetBrowser.h"
#include "SceneHierarchy.h"
#include "ShaderGraph.h"
#include "PropertyPanel.h"
#include "Components/LightComponent.h"
#include "../EditorGUI/EditorGui.h"

void BalEditor::CMainScreen::Draw()
{
    const int           flags       = 1 << 0 | 1 << 1 | 1 << 2 | 1 << 5 | 1 << 7 | 1 << 10 | 1 << 13;
    bool                open{ true };
    static unsigned int dockspaceID = 0;
    GUI::MaxNextWindow();
    GUI::Begin( "MainWindow", open, flags );
    if ( GUI::BeginMenuBar())
    {
        if ( GUI::BeginMenu( "File" ))
        {
            if ( GUI::MenuItem( "New" ))
            {
                m_saved = false;
                //todo: load new level
            }
            if ( GUI::MenuItem( "Open" ))
            {
                //todo: load level
            }
            if ( GUI::MenuItem( "Save Scene" ))
            {
                //todo: save scene
            }
            if ( GUI::MenuItem( "Save Scene As" ))
            {
                //todo: save scene
            }
            GUI::EndMenu();
        }
        if ( GUI::BeginMenu( "GameObject" ))
        {
            GUI::DrawText( "Add GameObject" );
            if ( GUI::MenuItem( "Empty" ))
            {
                m_pContext->CreateEntity();
                
                std::cout << "add empty object\n";
            }
            if ( GUI::MenuItem( "Light" ))
            {
                IEntity* entity = m_pContext->CreateEntity();
                entity->AddComponent<CLightComponent>();
            }
            GUI::EndMenu();
        }
        if ( GUI::BeginMenu( "View" ))
        {
            if ( GUI::MenuItem( "Hierarchy" ))
            {
                m_pSceneHierarchy->ShowWindow();
            }
            if ( GUI::MenuItem( "Asset Browser" ))
            {
                m_pAssetBrowser->ShowWindow();
            }
            if ( GUI::MenuItem( "Shader Graph" ))
            {
                m_pShaderGraph->ShowWindow();
            }
            if ( GUI::MenuItem( "Properties..." ))
            {
                m_pPropertyPanel->ShowWindow();
            }
            GUI::EndMenu();
        }
        GUI::EndMenuBar();
        static constexpr int dockspaceFlags = 1 << 6;
        dockspaceID = GUI::GetID( "MainWindowDockspace" );
        GUI::DockSpace( dockspaceID, { 0.0f, 0.0f }, dockspaceFlags );
    }
    GUI::End();
    GUI::SetNextWindowDockID( dockspaceID, 1 << 2 );
}

void BalEditor::CMainScreen::SetContext( IScene* pScene, CAssetBrowser* pAssetBrowser, CSceneHierarchy* pHierarchy, CShaderGraph* pGraph, CPropertyPanel* pPropertyPanel )
{
    m_pContext        = pScene;
    m_pAssetBrowser   = pAssetBrowser;
    m_pSceneHierarchy = pHierarchy;
    m_pShaderGraph    = pGraph;
    m_pPropertyPanel  = pPropertyPanel;
}
