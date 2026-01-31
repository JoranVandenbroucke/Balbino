#include "Interface.h"

#include <ImGuizmo.h>
#include <SDL3/SDL.h>
#define IMGUI_DEFINE_MATH_OPERATORS
#include <backends/imgui_impl_sdl3.h>
#include <backends/imgui_impl_vulkan.h>
#include <imgui.h>

#include "Tools/FilesSystem/Inporter/MeshFileImporter.h"
#include "Tools/FilesSystem/Inporter/TextureFileImporter.h"
#include "Windows/AssetBrowser.h"
#include "Windows/GameView.h"
#include "Windows/MainScreen.h"
#include "Windows/MaterialEditor.h"
#include "Windows/PropertyPanel.h"
#include "Windows/SceneHierarchy.h"
#include "Windows/ShaderEditor.h"

#include <ISystem.h>

void FawnForge::CInterface::Initialize( SDL_Window* pWindow, int32_t w, int32_t h, FawnVision::CRenderer* pRenderer, ISystem* pSystem )
{
    m_pWindow   = pWindow;
    m_pRenderer = pRenderer;
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGui_ImplSDL3_InitForVulkan( pWindow );
    pRenderer->SetupEditor();
    ImNodes::CreateContext();

    ImNodes::GetIO().LinkDetachWithModifierClick.Modifier = &ImGui::GetIO().KeyCtrl;
    ImNodesStyle& style { ImNodes::GetStyle() };

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;// Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;    // Enable Docking
    io.DisplaySize             = ImVec2( static_cast<float>( w ), static_cast<float>( h ) );
    io.DisplayFramebufferScale = ImVec2( 1.0f, 1.0f );

    // Setup Dear ImGui style
    SetImGuiStyle( style );

    // Upload Fonts
    // Use any command queue
    pRenderer->SetupEditorFont();

    m_pMain            = new CMainScreen {};
    m_pGameView        = new CGameView {};
    m_pAssetBrowser    = new CAssetBrowser {};
    m_pSceneHierarchy  = new CSceneHierarchy {};
    m_pShaderGraph     = new CShaderEditor {};
    m_pMaterialEditor  = new CMaterialEditor {};
    m_pMeshImporter    = new CMeshFileImporter {};
    m_pTextureImporter = new CTextureFileImporter {};
    m_pPropertyPanel   = new CPropertyPanel {};

    m_pAssetBrowser->Initialize( pSystem );
    m_pMaterialEditor->Initialize( pSystem, m_pPropertyPanel );
}
void FawnForge::CInterface::Draw()
{

    if ( m_queueNextResource && !m_pendingResources.empty() )
    {
        ImportFile( m_pendingResources.back().c_str(), m_pAssetBrowser->GetCurrentDirectory(), m_pMeshImporter, m_pTextureImporter );
        m_pendingResources.pop_back();
        m_queueNextResource = false;
    }

    ImGui_ImplVulkan_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();
    ImGuizmo::BeginFrame();

    m_pMain->Draw();
    m_pGameView->Draw();
    m_pAssetBrowser->Draw();
    m_pSceneHierarchy->Draw();
    m_pShaderGraph->Draw();
    m_pMaterialEditor->Draw();
    m_pPropertyPanel->Draw();

    if ( m_pMeshImporter->IsVisible() )
    {
        m_queueNextResource = m_pMeshImporter->DrawImportSettings();
    }
    if ( m_pTextureImporter->IsVisible() )
    {
        m_queueNextResource = m_pTextureImporter->DrawImportSettings();
    }
    ImGui::Render();
    m_pRenderer->DrawEditor();
}
void FawnForge::CInterface::Cleanup()
{
    m_pAssetBrowser->Cleanup();
    m_pRenderer->CleanupEditor();
    ImNodes::DestroyContext();
    ImGui_ImplVulkan_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();

    delete m_pMain;
    delete m_pGameView;
    delete m_pAssetBrowser;
    delete m_pSceneHierarchy;
    delete m_pShaderGraph;
    delete m_pMaterialEditor;
    delete m_pMeshImporter;
    delete m_pTextureImporter;
    delete m_pPropertyPanel;
}
void FawnForge::CInterface::ProcessEvent( SDL_Event e ) noexcept
{
    ImGui_ImplSDL3_ProcessEvent( &e );
    switch ( e.type )
    {
        case SDL_EVENT_DROP_FILE: {
            if ( m_pendingResources.empty() )
            {
                m_queueNextResource = true;
            }
            m_pendingResources.emplace_back( e.drop.file );
            SDL_free( e.drop.file );// Free dropped_filedir memory
            break;
        }
        case SDL_EVENT_KEY_DOWN: {
            int key { m_pGameView->GetGuizmoType() };
            bool snap {};
            switch ( e.key.keysym.sym )
            {
                case SDLK_g: key = ImGuizmo::OPERATION::TRANSLATE; break;
                case SDLK_s: key = ImGuizmo::OPERATION::SCALE; break;
                case SDLK_r:
                    if ( key == ImGuizmo::OPERATION::ROTATE )
                    {
                        key = ImGuizmo::OPERATION::ROTATE_SCREEN;
                    }
                    else
                    {
                        key = ImGuizmo::OPERATION::ROTATE;
                    }
                    break;
                case SDLK_x:
                    if ( key & ImGuizmo::OPERATION::TRANSLATE || key & ImGuizmo::OPERATION::TRANSLATE_Y || key & ImGuizmo::OPERATION::TRANSLATE_Z )
                    {
                        key = ImGuizmo::OPERATION::TRANSLATE_X;
                    }
                    else if ( key == ImGuizmo::OPERATION::TRANSLATE_X )
                    {
                        key = ImGuizmo::OPERATION::TRANSLATE;
                    }

                    else if ( key & ImGuizmo::OPERATION::ROTATE || key & ImGuizmo::OPERATION::ROTATE_Y || key & ImGuizmo::OPERATION::ROTATE_Z )
                    {
                        key = ImGuizmo::OPERATION::ROTATE_X;
                    }
                    else if ( key == ImGuizmo::OPERATION::ROTATE_X )
                    {
                        key = ImGuizmo::OPERATION::ROTATE;
                    }
                    else if ( key & ImGuizmo::OPERATION::SCALE || key & ImGuizmo::OPERATION::SCALE_Y || key & ImGuizmo::OPERATION::SCALE_Z )
                    {
                        key = ImGuizmo::OPERATION::SCALE_X;
                    }
                    else if ( key == ImGuizmo::OPERATION::SCALE_X )
                    {
                        key = ImGuizmo::OPERATION::SCALE;
                    }
                    break;
                case SDLK_y:
                    if ( key & ImGuizmo::OPERATION::TRANSLATE || key & ImGuizmo::OPERATION::TRANSLATE_X || key & ImGuizmo::OPERATION::TRANSLATE_Z )
                    {
                        key = ImGuizmo::OPERATION::TRANSLATE_Y;
                    }
                    else if ( key == ImGuizmo::OPERATION::TRANSLATE_Y )
                    {
                        key = ImGuizmo::OPERATION::TRANSLATE;
                    }

                    else if ( key & ImGuizmo::OPERATION::ROTATE || key & ImGuizmo::OPERATION::ROTATE_X || key & ImGuizmo::OPERATION::ROTATE_Z )
                    {
                        key = ImGuizmo::OPERATION::ROTATE_Y;
                    }
                    else if ( key == ImGuizmo::OPERATION::ROTATE_Y )
                    {
                        key = ImGuizmo::OPERATION::ROTATE;
                    }

                    else if ( key & ImGuizmo::OPERATION::SCALE || key & ImGuizmo::OPERATION::SCALE_X || key & ImGuizmo::OPERATION::SCALE_Z )
                    {
                        key = ImGuizmo::OPERATION::SCALE_Y;
                    }
                    else if ( key == ImGuizmo::OPERATION::SCALE_Y )
                    {
                        key = ImGuizmo::OPERATION::SCALE;
                    }
                    break;
                case SDLK_z:
                    if ( key & ImGuizmo::OPERATION::TRANSLATE || key & ImGuizmo::OPERATION::TRANSLATE_X || key & ImGuizmo::OPERATION::TRANSLATE_Y )
                    {
                        key = ImGuizmo::OPERATION::TRANSLATE_Z;
                    }
                    else if ( key == ImGuizmo::OPERATION::TRANSLATE_Z )
                    {
                        key = ImGuizmo::OPERATION::TRANSLATE;
                    }

                    else if ( key & ImGuizmo::OPERATION::ROTATE || key & ImGuizmo::OPERATION::ROTATE_X || key & ImGuizmo::OPERATION::ROTATE_Y )
                    {
                        key = ImGuizmo::OPERATION::ROTATE_Z;
                    }
                    else if ( key == ImGuizmo::OPERATION::ROTATE_Z )
                    {
                        key = ImGuizmo::OPERATION::ROTATE;
                    }

                    else if ( key & ImGuizmo::OPERATION::SCALE || key & ImGuizmo::OPERATION::SCALE_X || key & ImGuizmo::OPERATION::SCALE_Y )
                    {
                        key = ImGuizmo::OPERATION::SCALE_Z;
                    }
                    else if ( key == ImGuizmo::OPERATION::SCALE_Z )
                    {
                        key = ImGuizmo::OPERATION::SCALE;
                    }
                    break;
                case SDLK_LCTRL:
                case SDLK_RCTRL: snap = true;
                default:;
            }
            m_pGameView->SetGuizmo( key );
            m_pGameView->SetSnap( snap );
            break;
        }
        case SDL_EVENT_KEY_UP:
            if ( e.key.keysym.sym == SDLK_LCTRL || e.key.keysym.sym == SDLK_RCTRL )
            {
                m_pGameView->SetSnap( false );
            }
        default:;
    }
}
void FawnForge::CInterface::SetContext( ISystem* pSystem ) noexcept
{
    m_pMain->SetContext( pSystem->GetCurrentActiveScene(), m_pAssetBrowser, m_pSceneHierarchy, m_pShaderGraph, m_pPropertyPanel );
    m_pSceneHierarchy->SetContext( pSystem );
    m_pGameView->SetContext( pSystem, m_pSceneHierarchy );
    m_pAssetBrowser->SetShaderGraphReference( m_pShaderGraph, m_pMaterialEditor, m_pPropertyPanel );
}
void FawnForge::CInterface::SetImGuiStyle( ImNodesStyle& imNodesStyle ) noexcept
{
    ImGuiStyle* style                        = &ImGui::GetStyle();
    ImVec4* colors                           = style->Colors;
    colors[ ImGuiCol_Text ]                  = ImVec4( 0.00f, 0.00f, 0.00f, 1.00f );
    colors[ ImGuiCol_TextDisabled ]          = ImVec4( 0.60f, 0.60f, 0.60f, 1.00f );
    colors[ ImGuiCol_WindowBg ]              = ImVec4( 0.94f, 0.94f, 0.94f, 1.00f );
    colors[ ImGuiCol_ChildBg ]               = ImVec4( 0.00f, 0.00f, 0.00f, 0.00f );
    colors[ ImGuiCol_PopupBg ]               = ImVec4( 0.90f, 0.90f, 0.90f, 1.00f );
    colors[ ImGuiCol_Border ]                = ImVec4( 0.00f, 0.00f, 0.00f, 0.39f );
    colors[ ImGuiCol_BorderShadow ]          = ImVec4( 1.00f, 1.00f, 1.00f, 0.10f );
    colors[ ImGuiCol_FrameBg ]               = ImVec4( 1.00f, 1.00f, 1.00f, 1.00f );
    colors[ ImGuiCol_FrameBgHovered ]        = ImVec4( 0.59f, 0.98f, 0.26f, 0.40f );
    colors[ ImGuiCol_FrameBgActive ]         = ImVec4( 0.59f, 0.98f, 0.26f, 0.67f );
    colors[ ImGuiCol_TitleBg ]               = ImVec4( 0.96f, 0.96f, 0.96f, 1.00f );
    colors[ ImGuiCol_TitleBgActive ]         = ImVec4( 0.82f, 0.82f, 0.82f, 1.00f );
    colors[ ImGuiCol_TitleBgCollapsed ]      = ImVec4( 1.00f, 1.00f, 1.00f, 0.51f );
    colors[ ImGuiCol_MenuBarBg ]             = ImVec4( 0.86f, 0.86f, 0.86f, 1.00f );
    colors[ ImGuiCol_ScrollbarBg ]           = ImVec4( 0.98f, 0.98f, 0.98f, 0.53f );
    colors[ ImGuiCol_ScrollbarGrab ]         = ImVec4( 0.69f, 0.69f, 0.69f, 0.80f );
    colors[ ImGuiCol_ScrollbarGrabHovered ]  = ImVec4( 0.49f, 0.49f, 0.49f, 0.80f );
    colors[ ImGuiCol_ScrollbarGrabActive ]   = ImVec4( 0.49f, 0.49f, 0.49f, 1.00f );
    colors[ ImGuiCol_CheckMark ]             = ImVec4( 0.16f, 0.89f, 0.20f, 1.00f );
    colors[ ImGuiCol_SliderGrab ]            = ImVec4( 0.59f, 0.98f, 0.26f, 0.78f );
    colors[ ImGuiCol_SliderGrabActive ]      = ImVec4( 0.59f, 0.98f, 0.26f, 1.00f );
    colors[ ImGuiCol_Button ]                = ImVec4( 0.59f, 0.98f, 0.26f, 0.40f );
    colors[ ImGuiCol_ButtonHovered ]         = ImVec4( 0.59f, 0.98f, 0.26f, 1.00f );
    colors[ ImGuiCol_ButtonActive ]          = ImVec4( 0.53f, 0.98f, 0.06f, 1.00f );
    colors[ ImGuiCol_Header ]                = ImVec4( 0.59f, 0.98f, 0.26f, 0.31f );
    colors[ ImGuiCol_HeaderHovered ]         = ImVec4( 0.59f, 0.98f, 0.26f, 0.80f );
    colors[ ImGuiCol_HeaderActive ]          = ImVec4( 0.59f, 0.98f, 0.26f, 1.00f );
    colors[ ImGuiCol_Separator ]             = ImVec4( 0.39f, 0.39f, 0.39f, 1.00f );
    colors[ ImGuiCol_SeparatorHovered ]      = ImVec4( 0.59f, 0.98f, 0.26f, 0.78f );
    colors[ ImGuiCol_SeparatorActive ]       = ImVec4( 0.59f, 0.98f, 0.26f, 1.00f );
    colors[ ImGuiCol_ResizeGrip ]            = ImVec4( 0.59f, 0.98f, 0.26f, 0.00f );
    colors[ ImGuiCol_ResizeGripHovered ]     = ImVec4( 0.59f, 0.98f, 0.26f, 0.67f );
    colors[ ImGuiCol_ResizeGripActive ]      = ImVec4( 0.59f, 0.98f, 0.26f, 0.95f );
    colors[ ImGuiCol_Tab ]                   = ImVec4( 0.59f, 0.59f, 0.59f, 0.50f );
    colors[ ImGuiCol_TabHovered ]            = ImVec4( 0.56f, 0.98f, 0.36f, 1.00f );
    colors[ ImGuiCol_TabActive ]             = ImVec4( 0.57f, 0.98f, 0.36f, 1.00f );
    colors[ ImGuiCol_TabUnfocused ]          = ImVec4( 0.56f, 0.98f, 0.36f, 1.00f );
    colors[ ImGuiCol_TabUnfocusedActive ]    = ImVec4( 0.56f, 0.98f, 0.36f, 1.00f );
    colors[ ImGuiCol_DockingPreview ]        = ImVec4( 0.59f, 0.98f, 0.26f, 0.78f );
    colors[ ImGuiCol_DockingEmptyBg ]        = ImVec4( 0.59f, 0.98f, 0.26f, 1.00f );
    colors[ ImGuiCol_PlotLines ]             = ImVec4( 0.39f, 0.39f, 0.39f, 1.00f );
    colors[ ImGuiCol_PlotLinesHovered ]      = ImVec4( 1.00f, 0.43f, 0.35f, 1.00f );
    colors[ ImGuiCol_PlotHistogram ]         = ImVec4( 0.90f, 0.70f, 0.00f, 1.00f );
    colors[ ImGuiCol_PlotHistogramHovered ]  = ImVec4( 1.00f, 0.60f, 0.00f, 1.00f );
    colors[ ImGuiCol_TableHeaderBg ]         = ImVec4( 0.19f, 0.19f, 0.20f, 1.00f );
    colors[ ImGuiCol_TableBorderStrong ]     = ImVec4( 0.31f, 0.31f, 0.35f, 1.00f );
    colors[ ImGuiCol_TableBorderLight ]      = ImVec4( 0.23f, 0.23f, 0.25f, 1.00f );
    colors[ ImGuiCol_TableRowBg ]            = ImVec4( 0.00f, 0.00f, 0.00f, 0.00f );
    colors[ ImGuiCol_TableRowBgAlt ]         = ImVec4( 1.00f, 1.00f, 1.00f, 0.06f );
    colors[ ImGuiCol_TextSelectedBg ]        = ImVec4( 0.59f, 0.98f, 0.26f, 0.35f );
    colors[ ImGuiCol_DragDropTarget ]        = ImVec4( 0.43f, 0.90f, 0.18f, 1.00f );
    colors[ ImGuiCol_NavHighlight ]          = ImVec4( 0.43f, 0.90f, 0.18f, 1.00f );
    colors[ ImGuiCol_NavWindowingHighlight ] = ImVec4( 0.43f, 0.90f, 0.18f, 1.00f );
    colors[ ImGuiCol_NavWindowingDimBg ]     = ImVec4( 0.00f, 0.00f, 0.00f, 0.58f );
    colors[ ImGuiCol_ModalWindowDimBg ]      = ImVec4( 0.20f, 0.20f, 0.20f, 0.35f );

    style->WindowPadding     = ImVec2( 2.0f, 2.0f );
    style->FramePadding      = ImVec2( 2.0f, 2.0f );
    style->CellPadding       = ImVec2( 2.0f, 2.0f );
    style->ItemSpacing       = ImVec2( 4.0f, 4.0f );
    style->ItemInnerSpacing  = ImVec2( 4.0f, 4.0f );
    style->TouchExtraPadding = ImVec2( 0.0f, 0.0f );
    style->IndentSpacing     = 16.0f;
    style->ScrollbarSize     = 12.0f;
    style->GrabMinSize       = 12.0f;

    style->WindowBorderSize = 0.0f;
    style->ChildBorderSize  = 1.0f;
    style->PopupBorderSize  = 1.0f;
    style->FrameBorderSize  = 1.0f;
    style->TabBorderSize    = 1.0f;

    style->WindowRounding    = 0.0f;
    style->ChildRounding     = 0.0f;
    style->FrameRounding     = 0.0f;
    style->PopupRounding     = 0.0f;
    style->GrabRounding      = 0.0f;
    style->TabRounding       = 0.0f;
    style->LogSliderDeadzone = 1.0f;

    imNodesStyle.Colors[ ImNodesCol_NodeBackground ]         = IM_COL32( 240, 240, 240, 255 );
    imNodesStyle.Colors[ ImNodesCol_NodeBackgroundHovered ]  = IM_COL32( 240, 240, 240, 255 );
    imNodesStyle.Colors[ ImNodesCol_NodeBackgroundSelected ] = IM_COL32( 240, 240, 240, 255 );
    imNodesStyle.Colors[ ImNodesCol_NodeOutline ]            = IM_COL32( 100, 100, 100, 255 );
    imNodesStyle.Colors[ ImNodesCol_TitleBar ]               = IM_COL32( 248, 248, 248, 255 );
    imNodesStyle.Colors[ ImNodesCol_TitleBarHovered ]        = IM_COL32( 209, 209, 209, 255 );
    imNodesStyle.Colors[ ImNodesCol_TitleBarSelected ]       = IM_COL32( 209, 209, 209, 255 );

    imNodesStyle.Colors[ ImNodesCol_Link ] = IM_COL32( 150, 250, 66, 100 );

    imNodesStyle.Colors[ ImNodesCol_LinkHovered ]  = IM_COL32( 150, 250, 66, 242 );
    imNodesStyle.Colors[ ImNodesCol_LinkSelected ] = IM_COL32( 150, 250, 66, 242 );

    imNodesStyle.Colors[ ImNodesCol_Pin ]                = IM_COL32( 150, 250, 66, 160 );
    imNodesStyle.Colors[ ImNodesCol_PinHovered ]         = IM_COL32( 150, 250, 66, 255 );
    imNodesStyle.Colors[ ImNodesCol_BoxSelector ]        = IM_COL32( 170, 250, 70, 30 );
    imNodesStyle.Colors[ ImNodesCol_BoxSelectorOutline ] = IM_COL32( 170, 250, 70, 150 );
    imNodesStyle.Colors[ ImNodesCol_GridBackground ]     = IM_COL32( 225, 225, 225, 255 );
    imNodesStyle.Colors[ ImNodesCol_GridLine ]           = IM_COL32( 180, 180, 180, 100 );
    imNodesStyle.Colors[ ImNodesCol_GridLinePrimary ]    = IM_COL32( 120, 120, 120, 100 );

    imNodesStyle.Colors[ ImNodesCol_MiniMapBackground ]        = IM_COL32( 25, 25, 25, 100 );
    imNodesStyle.Colors[ ImNodesCol_MiniMapBackgroundHovered ] = IM_COL32( 25, 25, 25, 200 );
    imNodesStyle.Colors[ ImNodesCol_MiniMapOutline ]           = IM_COL32( 150, 150, 150, 100 );
    imNodesStyle.Colors[ ImNodesCol_MiniMapOutlineHovered ]    = IM_COL32( 150, 150, 150, 200 );
}
void FawnForge::CInterface::Resize( const int32_t w, const int32_t h ) noexcept
{
    ImGuiIO& io    = ImGui::GetIO();
    io.DisplaySize = ImVec2( (float)w, (float)h );
}
