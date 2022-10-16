#include "Interface.h"

#include "Windows/AssetBrowser.h"
#include "Windows/GameView.h"
#include "Windows/MainScreen.h"
#include "Windows/SceneHierarchy.h"
#include "Windows/ShaderGraph.h"
#include "Windows/MaterialEditor.h"
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

BalEditor::CInterface::CInterface()
        : m_pMain{ nullptr },
          m_pGameView{ nullptr },
          m_pAssetBrowser{ nullptr },
          m_pSceneHierarchy{ nullptr },
          m_pShaderGraph{ nullptr },
          m_pMaterialEditor{ nullptr },
          m_descriptorPool{ nullptr },
          m_pWindow{ nullptr },
          m_queueNextResource{ false },
          m_pDevice{ nullptr }
{
}

void BalEditor::CInterface::Initialize( SDL_Window* pWindow, const int32_t w, const int32_t h, const BalVulkan::CDevice* pDevice, const BalVulkan::CQueue* pQueue, const BalVulkan::CCommandPool* pCommandPool, const BalVulkan::CRenderPass* pRenderPass, const BalVulkan::CSwapchain* pSwapchain, ISystem* pSystem )
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
    ImNodes::StyleColorsDark();
    
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
    SetImGuiStyle();
    
    ImNodes::GetIO().link_detach_with_modifier_click.modifier = &ImGui::GetIO().KeyCtrl;
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
    
    m_pAssetBrowser->Initialize( pSystem );
    m_pMaterialEditor->Initialize( pSystem );
}

void BalEditor::CInterface::Draw( BalVulkan::CCommandPool* pCommandPool )
{
    if ( m_queueNextResource && m_pendingResources.size())
    {
        ImportFile( m_pendingResources.back().c_str(), m_pAssetBrowser->GetCurrentDirectory(), m_pMeshImporter,
                    m_pTextureImporter );
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

void BalEditor::CInterface::Cleanup() const
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
}

void BalEditor::CInterface::ProcessEvent( SDL_Event e )
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

void BalEditor::CInterface::SetContext( IScene* pScene )
{
    m_pMain->SetContext( pScene, m_pAssetBrowser, m_pSceneHierarchy, m_pShaderGraph );
    m_pSceneHierarchy->SetContext( pScene );
    m_pGameView->SetContext( pScene->GetSystem(), pScene, m_pSceneHierarchy );
    m_pAssetBrowser->SetShaderGraphReference( m_pShaderGraph, m_pMaterialEditor );
}

void BalEditor::CInterface::SetImGuiStyle()
{
    ImGuiStyle* style  = &ImGui::GetStyle();
    ImVec4    * colors = style->Colors;
    
    colors[ImGuiCol_Text]                  = ImVec4( 1.000f, 1.000f, 1.000f, 1.000f );
    colors[ImGuiCol_TextDisabled]          = ImVec4( 0.500f, 0.500f, 0.500f, 1.000f );
    colors[ImGuiCol_WindowBg]              = ImVec4( 0.180f, 0.180f, 0.180f, 1.000f );
    colors[ImGuiCol_ChildBg]               = ImVec4( 0.280f, 0.280f, 0.280f, 0.000f );
    colors[ImGuiCol_PopupBg]               = ImVec4( 0.313f, 0.313f, 0.313f, 1.000f );
    colors[ImGuiCol_Border]                = ImVec4( 0.266f, 0.266f, 0.266f, 1.000f );
    colors[ImGuiCol_BorderShadow]          = ImVec4( 0.000f, 0.000f, 0.000f, 0.000f );
    colors[ImGuiCol_FrameBg]               = ImVec4( 0.160f, 0.160f, 0.160f, 1.000f );
    colors[ImGuiCol_FrameBgHovered]        = ImVec4( 0.200f, 0.200f, 0.200f, 1.000f );
    colors[ImGuiCol_FrameBgActive]         = ImVec4( 0.280f, 0.280f, 0.280f, 1.000f );
    colors[ImGuiCol_TitleBg]               = ImVec4( 0.148f, 0.148f, 0.148f, 1.000f );
    colors[ImGuiCol_TitleBgActive]         = ImVec4( 0.148f, 0.148f, 0.148f, 1.000f );
    colors[ImGuiCol_TitleBgCollapsed]      = ImVec4( 0.148f, 0.148f, 0.148f, 1.000f );
    colors[ImGuiCol_MenuBarBg]             = ImVec4( 0.195f, 0.195f, 0.195f, 1.000f );
    colors[ImGuiCol_ScrollbarBg]           = ImVec4( 0.160f, 0.160f, 0.160f, 1.000f );
    colors[ImGuiCol_ScrollbarGrab]         = ImVec4( 0.277f, 0.277f, 0.277f, 1.000f );
    colors[ImGuiCol_ScrollbarGrabHovered]  = ImVec4( 0.300f, 0.300f, 0.300f, 1.000f );
    colors[ImGuiCol_ScrollbarGrabActive]   = ImVec4( 0.038f, 0.420f, 0.000f, 1.000f );
    colors[ImGuiCol_CheckMark]             = ImVec4( 1.000f, 1.000f, 1.000f, 1.000f );
    colors[ImGuiCol_SliderGrab]            = ImVec4( 0.391f, 0.391f, 0.391f, 1.000f );
    colors[ImGuiCol_SliderGrabActive]      = ImVec4( 0.038f, 0.420f, 0.000f, 1.000f );
    colors[ImGuiCol_Button]                = ImVec4( 1.000f, 1.000f, 1.000f, 0.000f );
    colors[ImGuiCol_ButtonHovered]         = ImVec4( 1.000f, 1.000f, 1.000f, 0.156f );
    colors[ImGuiCol_ButtonActive]          = ImVec4( 1.000f, 1.000f, 1.000f, 0.391f );
    colors[ImGuiCol_Header]                = ImVec4( 0.313f, 0.313f, 0.313f, 1.000f );
    colors[ImGuiCol_HeaderHovered]         = ImVec4( 0.469f, 0.469f, 0.469f, 1.000f );
    colors[ImGuiCol_HeaderActive]          = ImVec4( 0.469f, 0.469f, 0.469f, 1.000f );
    colors[ImGuiCol_Separator]             = colors[ImGuiCol_Border];
    colors[ImGuiCol_SeparatorHovered]      = ImVec4( 0.391f, 0.391f, 0.391f, 1.000f );
    colors[ImGuiCol_SeparatorActive]       = ImVec4( 0.038f, 0.420f, 0.000f, 1.000f );
    colors[ImGuiCol_ResizeGrip]            = ImVec4( 1.000f, 1.000f, 1.000f, 0.250f );
    colors[ImGuiCol_ResizeGripHovered]     = ImVec4( 1.000f, 1.000f, 1.000f, 0.670f );
    colors[ImGuiCol_ResizeGripActive]      = ImVec4( 0.038f, 0.420f, 0.000f, 1.000f );
    colors[ImGuiCol_Tab]                   = ImVec4( 0.098f, 0.098f, 0.098f, 1.000f );
    colors[ImGuiCol_TabHovered]            = ImVec4( 0.352f, 0.352f, 0.352f, 1.000f );
    colors[ImGuiCol_TabActive]             = ImVec4( 0.195f, 0.195f, 0.195f, 1.000f );
    colors[ImGuiCol_TabUnfocused]          = ImVec4( 0.098f, 0.098f, 0.098f, 1.000f );
    colors[ImGuiCol_TabUnfocusedActive]    = ImVec4( 0.195f, 0.195f, 0.195f, 1.000f );
    colors[ImGuiCol_DockingPreview]        = ImVec4( 0.038f, 0.420f, 0.000f, 0.781f );
    colors[ImGuiCol_DockingEmptyBg]        = ImVec4( 0.180f, 0.180f, 0.180f, 1.000f );
    colors[ImGuiCol_PlotLines]             = ImVec4( 0.469f, 0.469f, 0.469f, 1.000f );
    colors[ImGuiCol_PlotLinesHovered]      = ImVec4( 0.038f, 0.420f, 0.000f, 1.000f );
    colors[ImGuiCol_PlotHistogram]         = ImVec4( 0.586f, 0.586f, 0.586f, 1.000f );
    colors[ImGuiCol_PlotHistogramHovered]  = ImVec4( 0.038f, 0.420f, 0.000f, 1.000f );
    colors[ImGuiCol_TextSelectedBg]        = ImVec4( 1.000f, 1.000f, 1.000f, 0.156f );
    colors[ImGuiCol_DragDropTarget]        = ImVec4( 0.038f, 0.420f, 0.000f, 1.000f );
    colors[ImGuiCol_NavHighlight]          = ImVec4( 0.038f, 0.420f, 0.000f, 1.000f );
    colors[ImGuiCol_NavWindowingHighlight] = ImVec4( 0.038f, 0.420f, 0.000f, 1.000f );
    colors[ImGuiCol_NavWindowingDimBg]     = ImVec4( 0.000f, 0.000f, 0.000f, 0.586f );
    colors[ImGuiCol_ModalWindowDimBg]      = ImVec4( 0.000f, 0.000f, 0.000f, 0.586f );
    
    style->WindowPadding     = ImVec2( 2.0f, 2.0f );
    style->FramePadding      = ImVec2( 2.0f, 2.0f );
    style->CellPadding       = ImVec2( 2.0f, 2.0f );
    style->ItemSpacing       = ImVec2( 4.0f, 4.0f );
    style->ItemInnerSpacing  = ImVec2( 4.0f, 4.0f );
    style->TouchExtraPadding = ImVec2( 0.0f, 0.0f );
    style->IndentSpacing     = 22.0f;
    style->ScrollbarSize     = 12.0f;
    style->GrabMinSize       = 12.0f;
    
    style->WindowBorderSize = 0.0f;
    style->ChildBorderSize  = 1.0f;
    style->PopupBorderSize  = 1.0f;
    style->FrameBorderSize  = 1.0f;
    style->TabBorderSize    = 1.0f;
    
    style->WindowRounding    = 0.0f;
    style->ChildRounding     = 4.0f;
    style->FrameRounding     = 4.0f;
    style->PopupRounding     = 4.0f;
    style->ScrollbarSize     = 4.0f;
    style->GrabRounding      = 4.0f;
    style->LogSliderDeadzone = 4.0f;
    style->TabRounding       = 4.0f;
}
