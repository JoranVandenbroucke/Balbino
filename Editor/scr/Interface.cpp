#include "Interface.h"

#include "Windows/AssetBrowser.h"
#include "Windows/GameView.h"
#include "Windows/MainScreen.h"
#include "Windows/SceneHierarchy.h"
#include "Windows/ShaderGraph.h"
#include "Windows/MaterialEditor.h"
#include "Windows/PropertyPanel.h"
#include "Tools/FilesSystem/Inporter/MeshFileImporter.h"
#include "Tools/FilesSystem/Inporter/TextureFileImporter.h"

#include <RenderPass.h>
#include <Queue.h>

#include <imgui.h>
#include <ImGuizmo.h>
#include <SDL_vulkan.h>

#include <backends/imgui_impl_sdl.h>
#include <backends/imgui_impl_vulkan.h>

#include "imnodes.h"
#include "Instance.h"

FawnForge::CInterface::CInterface()
        : m_queueNextResource{ false }
          , m_pMain{ nullptr }
          , m_pGameView{ nullptr }
          , m_pAssetBrowser{ nullptr }
          , m_pSceneHierarchy{ nullptr }
          , m_pShaderGraph{ nullptr }
          , m_pMaterialEditor{ nullptr }
          , m_pMeshImporter{ nullptr }
          , m_pTextureImporter{ nullptr }
          , m_pPropertyPanel{ nullptr }
          , m_descriptorPool{ nullptr }
          , m_pWindow{ nullptr }
          , m_pDevice{ nullptr }
{
}

void FawnForge::CInterface::Initialize( SDL_Window* pWindow, const int32_t w, const int32_t h, const FawnVision::CDevice* pDevice, const FawnVision::CQueue* pQueue, const FawnVision::CCommandPool* pCommandPool, const FawnVision::CRenderPass* pRenderPass, const FawnVision::CSwapchain* pSwapchain, ISystem* pSystem )
{
    (void) pSystem;
    m_pWindow = pWindow;
    m_pDevice = pDevice;
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    
    ImGui_ImplSDL2_InitForVulkan( pWindow );
    // CreateNew Descriptor Pool
    {
        constexpr VkDescriptorPoolSize poolSizes[] = {{ VK_DESCRIPTOR_TYPE_SAMPLER,                1000 },
                                                      { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
                                                      { VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE,          1000 },
                                                      { VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,          1000 },
                                                      { VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER,   1000 },
                                                      { VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER,   1000 },
                                                      { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,         1000 },
                                                      { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,         1000 },
                                                      { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
                                                      { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
                                                      { VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT,       1000 }};
        VkDescriptorPoolCreateInfo     pool_info   = {};
        pool_info.sType         = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        pool_info.flags         = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
        pool_info.maxSets       = 1000 * IM_ARRAYSIZE( poolSizes );
        pool_info.poolSizeCount = static_cast<uint32_t>(IM_ARRAYSIZE( poolSizes ));
        pool_info.pPoolSizes    = poolSizes;
        vkCreateDescriptorPool( pDevice->GetVkDevice(), &pool_info, nullptr, &m_descriptorPool );
    }
    
    ImGui_ImplVulkan_InitInfo info{};
    info.Instance        = nullptr; //todo: maybe?
    info.PhysicalDevice  = pDevice->GetPhysicalDeviceInfo()->device;
    info.Device          = pDevice->GetVkDevice();
    info.QueueFamily     = pQueue->GetQueFamily();
    info.CheckVkResultFn = nullptr;
    info.Queue           = pQueue->GetQueue();
    info.Allocator       = nullptr;
    info.PipelineCache   = nullptr;
    info.DescriptorPool  = m_descriptorPool;
    info.MinImageCount   = pSwapchain->GetMinImage();
    info.ImageCount      = pSwapchain->GetImageCount();
    info.Subpass         = 0;
    
    ImGui_ImplVulkan_Init( &info, pRenderPass->GetRenderPass());
    ImNodes::CreateContext();
    
    ImNodes::GetIO().LinkDetachWithModifierClick.Modifier = &ImGui::GetIO().KeyCtrl;
    ImNodesStyle& style{ ImNodes::GetStyle() };
    
    ImGuiIO& io                = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; // Enable Docking
    //io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
    //io.ConfigViewportsNoAutoMerge = true;
    //io.ConfigViewportsNoTaskBarIcon = true;
    io.DisplaySize             = ImVec2( static_cast<float>( w ), static_cast<float>( h ));
    io.DisplayFramebufferScale = ImVec2( 1.0f, 1.0f );
    
    // Setup Dear ImGui style
    SetImGuiStyle( style );
    //ImNodes::PushAttributeFlag( ImNodes::AttributeFlags_EnableLinkDetachWithDragClick );
    
    // Upload Fonts
    {
        // Use any command queue
        const VkCommandPool   commandPool   = pCommandPool->GetCommandPool();
        const VkCommandBuffer commandBuffer = pCommandPool->GetCommandBuffer();
        
        vkResetCommandPool( pDevice->GetVkDevice(), commandPool, 0 );
        VkCommandBufferBeginInfo beginInfo = {};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
        vkBeginCommandBuffer( commandBuffer, &beginInfo );
        
        ImGui_ImplVulkan_CreateFontsTexture( commandBuffer );
        
        VkSubmitInfo endInfo = {};
        endInfo.sType              = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        endInfo.commandBufferCount = 1;
        endInfo.pCommandBuffers    = &commandBuffer;
        vkEndCommandBuffer( commandBuffer );
        vkQueueSubmit( pQueue->GetQueue(), 1, &endInfo, VK_NULL_HANDLE );
        
        pDevice->WaitIdle();
        ImGui_ImplVulkan_DestroyFontUploadObjects();
    }
    //m_pDevice = pDevice;
    m_pMain            = new CMainScreen{};
    m_pGameView        = new CGameView{};
    m_pAssetBrowser    = new CAssetBrowser{};
    m_pSceneHierarchy  = new CSceneHierarchy{};
    m_pShaderGraph     = new CShaderGraph{};
    m_pMaterialEditor  = new CMaterialEditor{};
    m_pMeshImporter    = new CMeshFileImporter{};
    m_pTextureImporter = new CTextureFileImporter{};
    m_pPropertyPanel   = new CPropertyPanel{};
    
    m_pAssetBrowser->Initialize( pSystem );
    m_pMaterialEditor->Initialize( pSystem, m_pPropertyPanel );
}

void FawnForge::CInterface::Draw( FawnVision::CCommandPool* pCommandPool )
{
    if ( m_queueNextResource && !m_pendingResources.empty())
    {
        ImportFile(
                m_pendingResources.back().c_str(),
                m_pAssetBrowser->GetCurrentDirectory(),
                m_pMeshImporter,
                m_pTextureImporter
        );
        m_pendingResources.pop_back();
        m_queueNextResource = false;
    }
    
    ImGui_ImplVulkan_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
    ImGuizmo::BeginFrame();
    
    m_pMain->Draw();
    m_pGameView->Draw();
    m_pAssetBrowser->Draw();
    m_pSceneHierarchy->Draw();
    m_pShaderGraph->Draw();
    m_pMaterialEditor->Draw();
    m_pPropertyPanel->Draw();
    
    if ( m_pMeshImporter->IsVisible())
    {
        m_queueNextResource = m_pMeshImporter->DrawImportSettings();
    }
    if ( m_pTextureImporter->IsVisible())
    {
        m_queueNextResource = m_pTextureImporter->DrawImportSettings();
    }
    ImGui::Render();
    ImGui_ImplVulkan_RenderDrawData( ImGui::GetDrawData(), pCommandPool->GetCommandBuffer());
}

void FawnForge::CInterface::Cleanup() const
{
    //ImNodes::PopAttributeFlag();
    m_pAssetBrowser->Cleanup();
    vkDestroyDescriptorPool( m_pDevice->GetVkDevice(), m_descriptorPool, nullptr );
    ImNodes::DestroyContext();
    ImGui_ImplVulkan_Shutdown();
    ImGui_ImplSDL2_Shutdown();
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

void FawnForge::CInterface::ProcessEvent( SDL_Event e )
{
    ImGui_ImplSDL2_ProcessEvent( &e );
    switch ( e.type )
    {
        case SDL_DROPFILE:
        {
            if ( m_pendingResources.empty())
            {
                m_queueNextResource = true;
            }
            m_pendingResources.push_back( std::string( e.drop.file ));
            SDL_free( e.drop.file ); // Free dropped_filedir memory
            break;
        }
        case SDL_KEYDOWN:
        {
            int  key{ m_pGameView->GetGuizmoType() };
            bool snap{};
            switch ( e.key.keysym.sym )
            {
                case SDLK_g:
                    key = ImGuizmo::OPERATION::TRANSLATE;
                    break;
                case SDLK_s:
                    key = ImGuizmo::OPERATION::SCALE;
                    break;
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
                case SDLK_RCTRL:
                    snap = true;
                default:;
            }
            m_pGameView->SetGuizmo( key );
            m_pGameView->SetSnap( snap );
            break;
        }
        case SDL_KEYUP:
            if ( e.key.keysym.sym == SDLK_LCTRL || e.key.keysym.sym == SDLK_RCTRL )
            {
                m_pGameView->SetSnap( false );
            }
        default:;
    }
}

void FawnForge::CInterface::SetContext( IScene* pScene, ISystem* pSystem )
{
    m_pMain->SetContext( pScene, m_pAssetBrowser, m_pSceneHierarchy, m_pShaderGraph, m_pPropertyPanel );
    m_pSceneHierarchy->SetContext( pScene, pSystem );
    m_pGameView->SetContext( pScene->GetSystem(), pScene, m_pSceneHierarchy );
    m_pAssetBrowser->SetShaderGraphReference( m_pShaderGraph, m_pMaterialEditor, m_pPropertyPanel );
}

void FawnForge::CInterface::SetImGuiStyle( ImNodesStyle& imNodesStyle )
{
    ImGuiStyle* style  = &ImGui::GetStyle();
    ImVec4    * colors = style->Colors;
    colors[ImGuiCol_Text]                  = ImVec4( 0.00f, 0.00f, 0.00f, 1.00f );
    colors[ImGuiCol_TextDisabled]          = ImVec4( 0.60f, 0.60f, 0.60f, 1.00f );
    colors[ImGuiCol_WindowBg]              = ImVec4( 0.94f, 0.94f, 0.94f, 1.00f );
    colors[ImGuiCol_ChildBg]               = ImVec4( 0.00f, 0.00f, 0.00f, 0.00f );
    colors[ImGuiCol_PopupBg]               = ImVec4( 0.90f, 0.90f, 0.90f, 1.00f );
    colors[ImGuiCol_Border]                = ImVec4( 0.00f, 0.00f, 0.00f, 0.39f );
    colors[ImGuiCol_BorderShadow]          = ImVec4( 1.00f, 1.00f, 1.00f, 0.10f );
    colors[ImGuiCol_FrameBg]               = ImVec4( 1.00f, 1.00f, 1.00f, 1.00f );
    colors[ImGuiCol_FrameBgHovered]        = ImVec4( 0.59f, 0.98f, 0.26f, 0.40f );
    colors[ImGuiCol_FrameBgActive]         = ImVec4( 0.59f, 0.98f, 0.26f, 0.67f );
    colors[ImGuiCol_TitleBg]               = ImVec4( 0.96f, 0.96f, 0.96f, 1.00f );
    colors[ImGuiCol_TitleBgActive]         = ImVec4( 0.82f, 0.82f, 0.82f, 1.00f );
    colors[ImGuiCol_TitleBgCollapsed]      = ImVec4( 1.00f, 1.00f, 1.00f, 0.51f );
    colors[ImGuiCol_MenuBarBg]             = ImVec4( 0.86f, 0.86f, 0.86f, 1.00f );
    colors[ImGuiCol_ScrollbarBg]           = ImVec4( 0.98f, 0.98f, 0.98f, 0.53f );
    colors[ImGuiCol_ScrollbarGrab]         = ImVec4( 0.69f, 0.69f, 0.69f, 0.80f );
    colors[ImGuiCol_ScrollbarGrabHovered]  = ImVec4( 0.49f, 0.49f, 0.49f, 0.80f );
    colors[ImGuiCol_ScrollbarGrabActive]   = ImVec4( 0.49f, 0.49f, 0.49f, 1.00f );
    colors[ImGuiCol_CheckMark]             = ImVec4( 0.16f, 0.89f, 0.20f, 1.00f );
    colors[ImGuiCol_SliderGrab]            = ImVec4( 0.59f, 0.98f, 0.26f, 0.78f );
    colors[ImGuiCol_SliderGrabActive]      = ImVec4( 0.59f, 0.98f, 0.26f, 1.00f );
    colors[ImGuiCol_Button]                = ImVec4( 0.59f, 0.98f, 0.26f, 0.40f );
    colors[ImGuiCol_ButtonHovered]         = ImVec4( 0.59f, 0.98f, 0.26f, 1.00f );
    colors[ImGuiCol_ButtonActive]          = ImVec4( 0.53f, 0.98f, 0.06f, 1.00f );
    colors[ImGuiCol_Header]                = ImVec4( 0.59f, 0.98f, 0.26f, 0.31f );
    colors[ImGuiCol_HeaderHovered]         = ImVec4( 0.59f, 0.98f, 0.26f, 0.80f );
    colors[ImGuiCol_HeaderActive]          = ImVec4( 0.59f, 0.98f, 0.26f, 1.00f );
    colors[ImGuiCol_Separator]             = ImVec4( 0.39f, 0.39f, 0.39f, 1.00f );
    colors[ImGuiCol_SeparatorHovered]      = ImVec4( 0.59f, 0.98f, 0.26f, 0.78f );
    colors[ImGuiCol_SeparatorActive]       = ImVec4( 0.59f, 0.98f, 0.26f, 1.00f );
    colors[ImGuiCol_ResizeGrip]            = ImVec4( 0.59f, 0.98f, 0.26f, 0.00f );
    colors[ImGuiCol_ResizeGripHovered]     = ImVec4( 0.59f, 0.98f, 0.26f, 0.67f );
    colors[ImGuiCol_ResizeGripActive]      = ImVec4( 0.59f, 0.98f, 0.26f, 0.95f );
    colors[ImGuiCol_Tab]                   = ImVec4( 0.59f, 0.59f, 0.59f, 0.50f );
    colors[ImGuiCol_TabHovered]            = ImVec4( 0.56f, 0.98f, 0.36f, 1.00f );
    colors[ImGuiCol_TabActive]             = ImVec4( 0.57f, 0.98f, 0.36f, 1.00f );
    colors[ImGuiCol_TabUnfocused]          = ImVec4( 0.56f, 0.98f, 0.36f, 1.00f );
    colors[ImGuiCol_TabUnfocusedActive]    = ImVec4( 0.56f, 0.98f, 0.36f, 1.00f );
    colors[ImGuiCol_DockingPreview]        = ImVec4( 0.59f, 0.98f, 0.26f, 0.78f );
    colors[ImGuiCol_DockingEmptyBg]        = ImVec4( 0.59f, 0.98f, 0.26f, 1.00f );
    colors[ImGuiCol_PlotLines]             = ImVec4( 0.39f, 0.39f, 0.39f, 1.00f );
    colors[ImGuiCol_PlotLinesHovered]      = ImVec4( 1.00f, 0.43f, 0.35f, 1.00f );
    colors[ImGuiCol_PlotHistogram]         = ImVec4( 0.90f, 0.70f, 0.00f, 1.00f );
    colors[ImGuiCol_PlotHistogramHovered]  = ImVec4( 1.00f, 0.60f, 0.00f, 1.00f );
    colors[ImGuiCol_TableHeaderBg]         = ImVec4( 0.19f, 0.19f, 0.20f, 1.00f );
    colors[ImGuiCol_TableBorderStrong]     = ImVec4( 0.31f, 0.31f, 0.35f, 1.00f );
    colors[ImGuiCol_TableBorderLight]      = ImVec4( 0.23f, 0.23f, 0.25f, 1.00f );
    colors[ImGuiCol_TableRowBg]            = ImVec4( 0.00f, 0.00f, 0.00f, 0.00f );
    colors[ImGuiCol_TableRowBgAlt]         = ImVec4( 1.00f, 1.00f, 1.00f, 0.06f );
    colors[ImGuiCol_TextSelectedBg]        = ImVec4( 0.59f, 0.98f, 0.26f, 0.35f );
    colors[ImGuiCol_DragDropTarget]        = ImVec4( 0.43f, 0.90f, 0.18f, 1.00f );
    colors[ImGuiCol_NavHighlight]          = ImVec4( 0.43f, 0.90f, 0.18f, 1.00f );
    colors[ImGuiCol_NavWindowingHighlight] = ImVec4( 0.43f, 0.90f, 0.18f, 1.00f );
    colors[ImGuiCol_NavWindowingDimBg]     = ImVec4( 0.00f, 0.00f, 0.00f, 0.58f );
    colors[ImGuiCol_ModalWindowDimBg]      = ImVec4( 0.20f, 0.20f, 0.20f, 0.35f );
    
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
    
    imNodesStyle.Colors[ImNodesCol_NodeBackground]         = IM_COL32( 240, 240, 240, 255 );
    imNodesStyle.Colors[ImNodesCol_NodeBackgroundHovered]  = IM_COL32( 240, 240, 240, 255 );
    imNodesStyle.Colors[ImNodesCol_NodeBackgroundSelected] = IM_COL32( 240, 240, 240, 255 );
    imNodesStyle.Colors[ImNodesCol_NodeOutline]            = IM_COL32( 100, 100, 100, 255 );
    imNodesStyle.Colors[ImNodesCol_TitleBar]               = IM_COL32( 248, 248, 248, 255 );
    imNodesStyle.Colors[ImNodesCol_TitleBarHovered]        = IM_COL32( 209, 209, 209, 255 );
    imNodesStyle.Colors[ImNodesCol_TitleBarSelected]       = IM_COL32( 209, 209, 209, 255 );
    
    imNodesStyle.Colors[ImNodesCol_Link] = IM_COL32( 150, 250, 66, 100 );
    
    imNodesStyle.Colors[ImNodesCol_LinkHovered]  = IM_COL32( 150, 250, 66, 242 );
    imNodesStyle.Colors[ImNodesCol_LinkSelected] = IM_COL32( 150, 250, 66, 242 );
    
    imNodesStyle.Colors[ImNodesCol_Pin]                = IM_COL32( 150, 250, 66, 160 );
    imNodesStyle.Colors[ImNodesCol_PinHovered]         = IM_COL32( 150, 250, 66, 255 );
    imNodesStyle.Colors[ImNodesCol_BoxSelector]        = IM_COL32( 170, 250, 70, 30 );
    imNodesStyle.Colors[ImNodesCol_BoxSelectorOutline] = IM_COL32( 170, 250, 70, 150 );
    imNodesStyle.Colors[ImNodesCol_GridBackground]     = IM_COL32( 225, 225, 225, 255 );
    imNodesStyle.Colors[ImNodesCol_GridLine]           = IM_COL32( 180, 180, 180, 100 );
    imNodesStyle.Colors[ImNodesCol_GridLinePrimary]    = IM_COL32( 120, 120, 120, 100 );
    
    imNodesStyle.Colors[ImNodesCol_MiniMapBackground]        = IM_COL32( 25, 25, 25, 100 );
    imNodesStyle.Colors[ImNodesCol_MiniMapBackgroundHovered] = IM_COL32( 25, 25, 25, 200 );
    imNodesStyle.Colors[ImNodesCol_MiniMapOutline]           = IM_COL32( 150, 150, 150, 100 );
    imNodesStyle.Colors[ImNodesCol_MiniMapOutlineHovered]    = IM_COL32( 150, 150, 150, 200 );
    
    imNodesStyle.Colors[ImNodesCol_MiniMapOutlineHovered] = IM_COL32( 150, 150, 150, 200 );
}
void FawnForge::CInterface::Resize( const int32_t w, const int32_t h )
{
    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize = ImVec2((float) w, (float) h );
}
