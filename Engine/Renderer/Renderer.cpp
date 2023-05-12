#include "Renderer.h"

#include "ISystem.h"

#include <IResourceManager.h>
#include <IScene.h>

#include <CommandPool.h>
#include <Fence.h>
#include <FrameBuffer.h>
#include <ImageResource.h>
#include <ImageView.h>
#include <Instance.h>
#include <Queue.h>
#include <RenderPass.h>
#include <Semaphore.h>
#include <Swapchain.h>

#include <SDL3/SDL_vulkan.h>

FawnVision::CRenderer::CRenderer()
        : m_pInstance{ nullptr }
          , m_pDevice{ nullptr }
          , m_pSwapchain{ nullptr }
          , m_pFrameBuffer{ nullptr }
          , m_pQueue{ nullptr }
          , m_pCommandPool{ nullptr }
          , m_pSignalingSemaphore{ nullptr }
          , m_pWaitingSemaphore{ nullptr }
          ,
        #ifdef BALBINO_EDITOR
          m_pInterface{ nullptr }
          ,
        #endif // BALBINO_EDITOR
          m_width{ 0 }
          , m_height{ 0 }
          , m_imageIndex{ 0 }
          , m_aspectRation{ 0 }
          , m_pWindow{ nullptr }
          , m_pScene{ nullptr }
{
}


#ifdef BALBINO_EDITOR
#include "src/Interface.h"
void FawnVision::CRenderer::Setup( SDL_Window* pWindow, const char** extensions, uint32_t extensionsCount, FawnForge::CInterface* pInterface, ISystem* pSystem )
{
    Setup( pWindow, extensions, extensionsCount );
    pInterface->Initialize( pWindow, m_width, m_height, this, pSystem );
    m_pInterface = pInterface;
}
#endif

void FawnVision::CRenderer::Setup( SDL_Window* pWindow, const char** extensions, uint32_t extensionsCount )
{
    m_pWindow = pWindow;
    VkSurfaceKHR surface;
    
    m_pInstance = new FawnVision::CInstance{};
    m_pInstance->Initialize( extensions, extensionsCount );
    
    SDL_Vulkan_CreateSurface( pWindow, m_pInstance->GetHandle(), &surface );
    SDL_GetWindowSize( pWindow, &m_width, &m_height );
    m_pInstance->SetSurface( surface );
    m_pInstance->InitializeDevice(m_pDevice,m_pInstance->FindBestPhysicalDeviceIndex( surface ));
    
    m_aspectRation = static_cast<float>( m_height ) / static_cast<float>( m_width );
    m_pSwapchain   = new FawnVision::CSwapchain{ m_pDevice, surface };
    m_pQueue       = new FawnVision::CQueue{ m_pDevice };
    m_pCommandPool = new FawnVision::CCommandPool{ m_pDevice };
    m_pRenderPass  = new FawnVision::CRenderPass{ m_pDevice };
    m_pFrameBuffer = new FawnVision::CFrameBuffer{ m_pDevice };
    m_pQueue->Initialize();
    m_pSwapchain->Initialize( m_width, m_height );
    m_pCommandPool->Initialize( m_pQueue->GetQueFamily(), m_pSwapchain );
    
    uint32_t                            imageResourceSize;
    std::vector<FawnVision::EFormat::Enum> formats;
    m_pSwapchain->GetImages( m_swapchainResources, imageResourceSize );
    m_swapchainViews.reserve( imageResourceSize );
    for ( uint32_t i = 0; i < imageResourceSize; ++i )
    {
        m_swapchainViews.push_back(new FawnVision::CImageView{ *m_swapchainResources[i], FawnVision::EImageViewType::View2D });
    }
    formats.push_back((FawnVision::EFormat::Enum) m_swapchainResources[0]->GetFormat());
    formats.push_back((FawnVision::EFormat::Enum) m_pDevice->GetPhysicalDeviceInfo().GetDepthFormat());
    m_pRenderPass->Initialize( formats, 0, (uint32_t) m_swapchainViews.size());
    
    m_pDepthImage = new FawnVision::CImageResource{ m_pDevice };
    m_pDepthImage->Initialize(
            FawnVision::EImageViewType::View2D,
            formats.back(),
            m_pSwapchain->GetExtend().width,
            m_pSwapchain->GetExtend().height,
            1,
            1,
            1,
            0,
            FawnVision::EImageUsageFlagBits::DepthStencilAttachmentBit,
            FawnVision::EImageLayout::Undefined
    );
    m_pDepthImageView = new FawnVision::CImageView{ *m_pDepthImage, FawnVision::EImageViewType::View2D, 0, 1, 0, 1};
    
    m_pFrameBuffer->Initialize( m_pRenderPass, m_width, m_height, m_swapchainViews, m_pDepthImageView );
    
    m_pSignalingSemaphore = new FawnVision::CSemaphore{ m_pDevice };
    m_pWaitingSemaphore   = new FawnVision::CSemaphore{ m_pDevice };
    m_pSignalingSemaphore->Initialize();
    m_pWaitingSemaphore->Initialize();
    const uint32_t size = m_pCommandPool->GetCommandBufferCount();
    m_pFences.reserve( size );
    for ( uint32_t i = 0; i < size; ++i )
    {
        m_pFences.push_back( new FawnVision::CFence{ m_pDevice });
        m_pFences.back()->Initialize();
        m_pInFlightFences.push_back( nullptr );
    }
}

void FawnVision::CRenderer::Cleanup()
{
    m_pDevice->WaitIdle();
    if ( m_pSignalingSemaphore )
    {
        m_pSignalingSemaphore->Release();
        m_pSignalingSemaphore = nullptr;
    }
    if ( m_pWaitingSemaphore )
    {
        m_pWaitingSemaphore->Release();
        m_pWaitingSemaphore = nullptr;
    }
    if ( m_pCommandPool )
    {
        const uint64_t size = m_pCommandPool->GetCommandBufferCount();
        for ( uint64_t i    = 0; i < size; ++i )
        {
            m_pFences[i]->Release();
            m_pInFlightFences[i] = nullptr;
        }
    }
    if ( !m_pFences.empty())
    {
        m_pFences.clear();
    }
    
    m_pRenderPass->Release();
    for ( auto& image : m_swapchainViews )
    {
        image->Release();
    }
    for ( auto& image : m_swapchainResources )
    {
        image->Release();
    }
    m_pDepthImage->Release();
    m_pDepthImageView->Release();
    m_swapchainViews.clear();
    m_swapchainResources.clear();
    if ( m_pFrameBuffer )
    {
        m_pFrameBuffer->Release();
        m_pFrameBuffer = nullptr;
    }
    if ( m_pCommandPool )
    {
        m_pCommandPool->Release();
        m_pCommandPool = nullptr;
    }
    if ( m_pQueue )
    {
        m_pQueue->Release();
        m_pQueue = nullptr;
    }
    if ( m_pSwapchain )
    {
        m_pSwapchain->Release();
        m_pSwapchain = nullptr;
    }
    if ( m_pDevice )
    {
        m_pDevice->Release();
        m_pDevice = nullptr;
    }
    
    if ( m_pInstance )
    {
        delete m_pInstance;
        m_pInstance = nullptr;
    }
}
void FawnVision::CRenderer::RecreateSwapChain()
{
    SDL_GetWindowSizeInPixels( m_pWindow, &m_width, &m_height );
    while ( m_width == 0 || m_height == 0 )
    {
        SDL_GetWindowSize( m_pWindow, &m_width, &m_height );
    }
    
    m_pDevice->WaitIdle();
    
    m_pRenderPass->Release();
    
    for ( auto& image : m_swapchainViews )
    {
        image->Release();
    }
    for ( auto& image : m_swapchainResources )
    {
        image->Release();
    }
    m_pDepthImage->Release();
    m_pDepthImageView->Release();
    m_swapchainViews.clear();
    m_swapchainResources.clear();
    
    m_pFrameBuffer->Release();
    m_pCommandPool->Release();
    m_pQueue->Release();
    m_pSwapchain->Initialize( m_width, m_height );
    
    m_pQueue       = new FawnVision::CQueue{ m_pDevice };
    m_pCommandPool = new FawnVision::CCommandPool{ m_pDevice };
    m_pRenderPass  = new FawnVision::CRenderPass{ m_pDevice };
    m_pFrameBuffer = new FawnVision::CFrameBuffer{ m_pDevice };
    
    m_pQueue->Initialize();
    m_pCommandPool->Initialize( m_pQueue->GetQueFamily(), m_pSwapchain );
    
    uint32_t                            imageResourceSize;
    std::vector<FawnVision::EFormat::Enum> formats;
    m_pSwapchain->GetImages( m_swapchainResources, imageResourceSize );
    m_swapchainViews.reserve( imageResourceSize );
    for ( uint32_t i = 0; i < imageResourceSize; ++i )
    {
        m_swapchainViews.push_back(
                new FawnVision::CImageView{ *m_swapchainResources[i], FawnVision::EImageViewType::View2D });
    }
    formats.push_back((FawnVision::EFormat::Enum) m_swapchainResources[0]->GetFormat());
    formats.push_back((FawnVision::EFormat::Enum) m_pDevice->GetPhysicalDeviceInfo().GetDepthFormat());
    m_pRenderPass->Initialize( formats, 0u, (uint32_t) m_swapchainViews.size());
    
    m_pDepthImage = new FawnVision::CImageResource{ m_pDevice };
    m_pDepthImage->Initialize(
            FawnVision::EImageViewType::View2D,
            formats.back(),
            m_pSwapchain->GetExtend().width,
            m_pSwapchain->GetExtend().height,
            1,
            1,
            1,
            0,
            FawnVision::EImageUsageFlagBits::DepthStencilAttachmentBit,
            FawnVision::EImageLayout::Undefined
    );
    m_pDepthImageView = new FawnVision::CImageView{
            *m_pDepthImage, FawnVision::EImageViewType::View2D, 0, 1, 0, 1
    };
    
    m_pFrameBuffer->Initialize( m_pRenderPass, m_width, m_height, m_swapchainViews, m_pDepthImageView );
    
    #ifdef BALBINO_EDITOR
    m_pInterface->Resize( m_width, m_height );
    #endif
    
    m_pScene->SetRenderer( this );
    m_pScene->GetSystem()->GetResourceManager()->ReloadAll( this );
}


bool FawnVision::CRenderer::StartDraw()
{
    m_pFences[m_pCommandPool->GetCurrentIndex()]->Wait();
    if ( m_pSwapchain->AcquireNextImage( m_pSignalingSemaphore, m_imageIndex ))
    {
        RecreateSwapChain();
        return true;
    }
    m_pCommandPool->BeginRender( m_pRenderPass, m_pFrameBuffer, m_pSwapchain );
    return false;
}

bool FawnVision::CRenderer::EndDraw()
{
    #ifdef BALBINO_EDITOR
    m_pInterface->Draw( m_pCommandPool );
    #endif // BALBINO_EDITOR
    
    if ( m_pInFlightFences[m_imageIndex] != nullptr )
    {
        m_pInFlightFences[m_imageIndex]->Wait();
    }
    m_pInFlightFences[m_imageIndex] = m_pFences[m_pCommandPool->GetCurrentIndex()];
    m_pFences[m_pCommandPool->GetCurrentIndex()]->Reset();
    m_pCommandPool->EndRender();
    m_pQueue->SubmitPass(
            m_pWaitingSemaphore, m_pSignalingSemaphore, m_pCommandPool, m_pFences[m_pCommandPool->GetCurrentIndex()]
    );
    if ( m_pQueue->PresentToScreen( m_pSwapchain, m_pWaitingSemaphore, m_imageIndex ))
    {
        RecreateSwapChain();
        return true;
    }
    m_pQueue->WaitIdle();
    m_pCommandPool->UpdateFrameIndex();
    return false;
}

float FawnVision::CRenderer::GetAspectRatio() const
{
    return m_aspectRation;
}

void FawnVision::CRenderer::GiveSceneRenderData( IScene* pScene )
{
    m_pScene = pScene;
    pScene->SetRenderer( this );
}
bool FawnVision::CRenderer::NextSubPass()
{
    m_pCommandPool->NextSubPass();
    return true;
}




