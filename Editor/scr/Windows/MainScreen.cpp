#include "MainScreen.h"

#include <IScene.h>
#include <IEntity.h>

#include <iostream>

#include "AssetBrowser.h"
#include "SceneHierarchy.h"
#include "ShaderGraph.h"
#include "Components/LightComponent.h"
#include "../EditorGUI/EditorGui.h"

void BalEditor::CMainScreen::Draw()
{
    const int flags = 1 << 0 | 1 << 1 | 1 << 2 | 1 << 5 | 1 << 7 | 1 << 10 | 1 << 13;
    bool      open{ true };
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
            BalEditor::EditorGUI::EndMenu();
        }
        BalEditor::EditorGUI::EndMenuBar();
    }
    BalEditor::EditorGUI::End();
}

void BalEditor::CMainScreen::SetContext( IScene* pScene, CAssetBrowser* pAssetBrowser, CSceneHierarchy* pHierarchy, CShaderGraph* pGraph )
{
    m_pContext        = pScene;
    m_pAssetBrowser   = pAssetBrowser;
    m_pSceneHierarchy = pHierarchy;
    m_pShaderGraph    = pGraph;
}
