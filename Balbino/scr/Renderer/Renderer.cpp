#include "Renderer.h"
#include <SDL_vulkan.h>

#include <CommandPool.h>
#include <Device.h>
#include <FrameBuffer.h>
#include <Instance.h>
#include <Queue.h>
#include <Swapchain.h>

#include "Fence.h"
#include "Semaphore.h"
#include "../Managers/ResourceManager.h"
#include "../Scene/Scene.h"
#include "IManager.h"

Balbino::CRenderer::CRenderer()
        : m_pInstance{ nullptr }
          , m_pDevice{ nullptr }
          , m_pSwapchain{ nullptr }
          , m_pFrameBuffer{ nullptr }
          , m_pQueue{ nullptr }
          , m_pCommandPool{ nullptr }
          , m_pSignalingSemaphore{ nullptr }
          , m_pWaitingSemaphore{ nullptr }
#ifdef BALBINO_EDITOR
        , m_pInterface{ nullptr }
#endif // BALBINO_EDITOR

        , m_width{ 0 }
          , m_height{ 0 }
          , m_imageIndex{ 0 }
          , m_aspectRation{ 0 }
          , m_pWindow{ nullptr }
          , m_pScene{ nullptr }
{
}

Balbino::CRenderer::~CRenderer()
{
}

#ifdef BALBINO_EDITOR

void Balbino::CRenderer::Setup(SDL_Window* pWindow, const char** extensions, uint32_t extensionsCount, BalEditor::CInterface* pInterface, ISystem* pSystem)
{
    Setup( pWindow, extensions, extensionsCount );
    g_pDevice = m_pDevice;
    g_pCommandPool = m_pCommandPool;
    g_pQueue = m_pQueue;
    g_pFrameBuffer = m_pFrameBuffer;
    g_pSwapChain = m_pSwapchain;
    pInterface->Initialize( pWindow, m_width, m_height, m_pDevice, m_pQueue, m_pCommandPool, m_pFrameBuffer, m_pSwapchain, pSystem );
    m_pInterface = pInterface;
}

#endif

void Balbino::CRenderer::RecreateSwapChain()
{
    int width = 0, height = 0;
    SDL_Vulkan_GetDrawableSize( m_pWindow, &width, &height );
    while( width == 0 || height == 0 )
    {
        SDL_GetWindowSize( m_pWindow, &width, &height );
    }

    m_pDevice->WaitIdle();

    m_pSwapchain->Initialize( width, height );

    m_pFrameBuffer->Release();
    m_pCommandPool->Release();

    m_pFrameBuffer = BalVulkan::CFrameBuffer::CreateNew( m_pDevice );
    m_pCommandPool = BalVulkan::CCommandPool::CreateNew( m_pDevice );

    m_pFrameBuffer->Initialize( m_pSwapchain );
    m_pCommandPool->Initialize( m_pQueue->GetQueFamily(), m_pSwapchain );

//#ifdef BALBINO_EDITOR
//	m_pInterface->Resize( m_pCommandPool, m_pQueue);
//#endif

    g_pDevice = m_pDevice;
    g_pCommandPool = m_pCommandPool;
    g_pQueue = m_pQueue;
    g_pFrameBuffer = m_pFrameBuffer;
    g_pSwapChain = m_pSwapchain;

    m_pScene->RecreateBuffers( m_pCommandPool, m_pQueue );
    m_pScene->GetSystem()->GetResourceManager()->ReloadAll( m_pCommandPool, m_pQueue );
}

void Balbino::CRenderer::Setup(SDL_Window* pWindow, const char** extensions, uint32_t extensionsCount)
{
    m_pWindow = pWindow;
    VkSurfaceKHR surface;

    m_pInstance = BalVulkan::CInstance::CreateNew();
    m_pInstance->Initialize( extensions, extensionsCount );

    SDL_Vulkan_CreateSurface( pWindow, m_pInstance->GetHandle(), &surface );
    SDL_GetWindowSize( pWindow, &m_width, &m_height );
    m_pInstance->SetSurface( surface );
    m_pDevice = m_pInstance->CreateDevice( m_pInstance->FindBestPhysicalDeviceIndex( surface ));

    m_aspectRation = static_cast<float>( m_height ) / static_cast<float>( m_width );
    m_pSwapchain = BalVulkan::CSwapchain::CreateNew( m_pDevice, surface );

    m_pFrameBuffer = BalVulkan::CFrameBuffer::CreateNew( m_pDevice );
    m_pQueue = BalVulkan::CQueue::CreateNew( m_pDevice );
    m_pCommandPool = BalVulkan::CCommandPool::CreateNew( m_pDevice );

    m_pSwapchain->Initialize( m_width, m_height );

    m_pQueue->Initialize();
    m_pCommandPool->Initialize( m_pQueue->GetQueFamily(), m_pSwapchain );

    //////////////////////////////////////
    m_pFrameBuffer->Initialize( m_pSwapchain );

    m_pSignalingSemaphore = BalVulkan::CSemaphore::CreateNew( m_pDevice );
    m_pWaitingSemaphore = BalVulkan::CSemaphore::CreateNew( m_pDevice );
    m_pSignalingSemaphore->Initialize();
    m_pWaitingSemaphore->Initialize();
    const uint32_t size = m_pCommandPool->GetCommandBufferCount();
    m_pFences.reserve( size );
    for( uint32_t i = 0; i < size; ++i )
    {
        m_pFences.push_back( BalVulkan::CFence::CreateNew( m_pDevice ));
        m_pFences.back()->Initialize();
        m_pInFlightFences.push_back( nullptr );
    }
}

void Balbino::CRenderer::Cleanup()
{
    if( m_pSignalingSemaphore )
    {
        m_pSignalingSemaphore->Release();
        m_pSignalingSemaphore = nullptr;
    }
    if( m_pWaitingSemaphore )
    {
        m_pWaitingSemaphore->Release();
        m_pWaitingSemaphore = nullptr;
    }
    if( m_pCommandPool )
    {
        const uint64_t size = m_pCommandPool->GetCommandBufferCount();
        for( uint64_t i = 0; i < size; ++i )
        {
            m_pFences[i]->Release();
            m_pInFlightFences[i] = nullptr;
        }
    }
    if( m_pFences.size())
    {
        m_pFences.clear();
    }
    if( m_pFrameBuffer )
    {
        m_pFrameBuffer->Release();
        m_pFrameBuffer = nullptr;
    }
    if( m_pCommandPool )
    {
        m_pCommandPool->Release();
        m_pCommandPool = nullptr;
    }
    if( m_pQueue )
    {
        m_pQueue->Release();
        m_pQueue = nullptr;
    }
    if( m_pSwapchain )
    {
        m_pSwapchain->Release();
        m_pSwapchain = nullptr;
    }
    if( m_pDevice )
    {
        m_pDevice->Release();
        m_pDevice = nullptr;
    }

    if( m_pInstance )
    {
        delete m_pInstance;
        m_pInstance = nullptr;
    }
}

bool Balbino::CRenderer::StartDraw()
{
    m_pFences[m_pCommandPool->GetCurrentIndex()]->Wait();
    if( m_pSwapchain->AcquireNextImage( m_pSignalingSemaphore, &m_imageIndex ))
    {
        RecreateSwapChain();
        return true;
    }
    m_pCommandPool->BeginRender( m_pFrameBuffer, m_pSwapchain );
    return false;
}

bool Balbino::CRenderer::EndDraw()
{
#ifdef BALBINO_EDITOR
    m_pInterface->Draw( m_pCommandPool );
#endif // BALBINO_EDITOR
    if( m_pInFlightFences[m_imageIndex] != nullptr )
    {
        m_pInFlightFences[m_imageIndex]->Wait();
    }
    m_pInFlightFences[m_imageIndex] = m_pFences[m_pCommandPool->GetCurrentIndex()];
    m_pFences[m_pCommandPool->GetCurrentIndex()]->Reset();
    m_pCommandPool->EndRender();
    m_pQueue->SubmitPass( m_pWaitingSemaphore, m_pSignalingSemaphore, m_pCommandPool, m_pFences[m_pCommandPool->GetCurrentIndex()] );
    if( m_pQueue->PresentToScreen( m_pSwapchain, m_pWaitingSemaphore, m_imageIndex ))
    {
        RecreateSwapChain();
        return true;
    }
    m_pQueue->WaitIdle();
    m_pCommandPool->UpdateFrameIndex();
    return false;
}

float Balbino::CRenderer::GetAspectRatio() const
{
    return m_aspectRation;
}

void Balbino::CRenderer::GiveSceneRenderData(CScene* pScene)
{
    m_pScene = pScene;
    pScene->SetRenderSettings( m_pDevice, m_pCommandPool, m_pQueue );
}




