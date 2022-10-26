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
    BalEditor::EditorGUI::MaxNextWindow();
    BalEditor::EditorGUI::Begin( "MainWindow", open, flags );
    if ( BalEditor::EditorGUI::BeginMenuBar())
    {
        if ( BalEditor::EditorGUI::BeginMenu( "File" ))
        {
            if ( BalEditor::EditorGUI::MenuItem( "New" ))
            {
                m_saved = false;
                //todo: load new level
            }
            if ( BalEditor::EditorGUI::MenuItem( "Open" ))
            {
                //todo: load level
            }
            if ( BalEditor::EditorGUI::MenuItem( "Save Scene" ))
            {
                //todo: save scene
            }
            if ( BalEditor::EditorGUI::MenuItem( "Save Scene As" ))
            {
                //todo: save scene
            }
            BalEditor::EditorGUI::EndMenu();
        }
        if ( BalEditor::EditorGUI::BeginMenu( "GameObject" ))
        {
            BalEditor::EditorGUI::DrawText( "Add GameObject" );
            if ( BalEditor::EditorGUI::MenuItem( "Empty" ))
            {
                m_pContext->CreateEntity();
                
                std::cout << "add empty object\n";
            }
            if ( BalEditor::EditorGUI::MenuItem( "Light" ))
            {
                IEntity* entity = m_pContext->CreateEntity();
                entity->AddComponent<CLightComponent>();
            }
            BalEditor::EditorGUI::EndMenu();
        }
        if ( BalEditor::EditorGUI::BeginMenu( "View" ))
        {
            if ( BalEditor::EditorGUI::MenuItem( "Hierarchy" ))
            {
                m_pSceneHierarchy->ShowWindow();
            }
            if ( BalEditor::EditorGUI::MenuItem( "Asset Browser" ))
            {
                m_pAssetBrowser->ShowWindow();
            }
            if ( BalEditor::EditorGUI::MenuItem( "Shader Graph" ))
            {
                m_pShaderGraph->ShowWindow();
            }
            if ( BalEditor::EditorGUI::MenuItem( "Properties..." ))
            {
                m_pPropertyPanel->ShowWindow();
            }
            BalEditor::EditorGUI::EndMenu();
        }
        BalEditor::EditorGUI::EndMenuBar();
        static constexpr int dockspaceFlags = 1 << 6;
        dockspaceID = BalEditor::EditorGUI::GetID( "MainWindowDockspace" );
        BalEditor::EditorGUI::DockSpace( dockspaceID, { 0.0f, 0.0f }, dockspaceFlags );
    }
    BalEditor::EditorGUI::End();
    BalEditor::EditorGUI::SetNextWindowDockID( dockspaceID, 1 << 2 );
}

void BalEditor::CMainScreen::SetContext( IScene* pScene, CAssetBrowser* pAssetBrowser, CSceneHierarchy* pHierarchy, CShaderGraph* pGraph, CPropertyPanel* pPropertyPanel )
{
    m_pContext        = pScene;
    m_pAssetBrowser   = pAssetBrowser;
    m_pSceneHierarchy = pHierarchy;
    m_pShaderGraph    = pGraph;
    m_pPropertyPanel  = pPropertyPanel;
}
