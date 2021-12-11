#include "pch.h"
#include "Renderer.h"
#include <SDL_vulkan.h>

#include <CommandPool.h>
#include <Device.h>
#include <FrameBuffer.h>
#include <Instance.h>
#include <Queue.h>
#include <Shader.h>
#include <Swapchain.h>

#include "Fence.h"
#include "Semaphore.h"
#include "../Camera.h"
#include "../../UBOStructs.h"
#include "../Managers/CameraManager.h"
#include "../Managers/Manager.h"
#include "../Managers/MeshManager.h"
#include "../Managers/ShaderManager.h"
#include "../Managers/TextureManager.h"

Balbino::CRenderer::CRenderer()
	: m_pInstance{ nullptr }
	, m_pDevice{ nullptr }
	, m_pSwapchain{ nullptr }
	, m_pFrameBuffer{ nullptr }
	, m_pQueue{ nullptr }
	, m_pCommandPool{ nullptr }
	, m_pSignalingSemaphore{ nullptr }
	, m_pWaitingSemaphore{ nullptr }
	, m_pModelBuffer{ nullptr }
	, m_pShadingBuffer{ nullptr }
	, m_aspectRation{ 0 }
{
}

Balbino::CRenderer::~CRenderer()
{
}

#ifdef BALBINO_EDITOR
void Balbino::CRenderer::Setup( SDL_Window* pWindow, const char** extensions, uint32_t extensionsCount, BalEditor::CInterface* pInterface )
{
	Setup( pWindow, extensions, extensionsCount );
	pInterface->Initialize( pWindow, m_width, m_height, m_pDevice, m_pQueue,m_pCommandPool, m_pFrameBuffer);
	m_pInterface = pInterface;
}
#endif

void Balbino::CRenderer::Setup( SDL_Window* pWindow, const char** extensions, uint32_t extensionsCount )
{
	VkSurfaceKHR surface;;

	m_pInstance = BalVulkan::CInstance::CreateNew();
	m_pInstance->Initialize( extensions, extensionsCount );

	SDL_Vulkan_CreateSurface( pWindow, m_pInstance->GetHandle(), &surface );
	SDL_GetWindowSize( pWindow, &m_width, &m_height );
	m_pInstance->SetSurface( surface );
	m_pDevice = m_pInstance->CreateDevice( m_pInstance->FindBestPhysicalDeviceIndex( surface ) );

	m_aspectRation = static_cast< float >( m_height ) / static_cast< float >( m_width );
	m_pSwapchain = BalVulkan::CSwapchain::CreateNew( m_pDevice, surface );

	m_pFrameBuffer = BalVulkan::CFrameBuffer::CreateNew( m_pDevice );
	m_pQueue = BalVulkan::CQueue::CreateNew( m_pDevice );
	m_pCommandPool = BalVulkan::CCommandPool::CreateNew( m_pDevice );

	m_pSwapchain->Initialize( m_width, m_height );

	m_pQueue->Initialize();
	m_pCommandPool->Initialize( m_pQueue->GetQueFamily(), m_pSwapchain );

	//////////////////////////////////////
	m_pModelBuffer = BalVulkan::CBuffer::CreateNew( m_pDevice, m_pCommandPool, m_pQueue );
	m_pShadingBuffer = BalVulkan::CBuffer::CreateNew( m_pDevice, m_pCommandPool, m_pQueue );

	m_pModelBuffer->Initialize( sizeof( SModelObject ), BalVulkan::EBufferUsageFlagBits::UniformBufferBit, BalVulkan::EMemoryPropertyFlagBits::HostVisibleBit | BalVulkan::EMemoryPropertyFlagBits::HostCoherentBit);
	m_pShadingBuffer->Initialize( sizeof( SLightObject ), BalVulkan::EBufferUsageFlagBits::UniformBufferBit, BalVulkan::EMemoryPropertyFlagBits::HostVisibleBit | BalVulkan::EMemoryPropertyFlagBits::HostCoherentBit );

	std::vector descriptorSets
	{
		BalVulkan::SDescriptorSet{BalVulkan::SDescriptorSet::EType::Buffer, m_pModelBuffer},
		BalVulkan::SDescriptorSet{BalVulkan::SDescriptorSet::EType::Buffer, m_pShadingBuffer},
		BalVulkan::SDescriptorSet{BalVulkan::SDescriptorSet::EType::Image, m_pShadingBuffer},
	};
	m_pFrameBuffer->Initialize( m_pSwapchain );

	CManager::GetMeshManager()->Initialize( m_pDevice, m_pCommandPool, m_pQueue );
	CManager::GetTextureManager()->Initialize();
	CManager::GetShaderManager()->Initialize( m_pDevice );

	m_pSignalingSemaphore = BalVulkan::CSemaphore::CreateNew( m_pDevice );
	m_pWaitingSemaphore = BalVulkan::CSemaphore::CreateNew( m_pDevice );
	m_pSignalingSemaphore->Initialize();
	m_pWaitingSemaphore->Initialize();
	const uint32_t size = m_pCommandPool->GetCommandBufferCount();
	m_pFences.reserve( size );
	for ( uint32_t i = 0; i < size; ++i )
	{
		m_pFences.push_back( BalVulkan::CFence::CreateNew( m_pDevice ) );
		m_pFences.back()->Initialize();
		m_pInFlightFences.push_back( nullptr );
	}
}

void Balbino::CRenderer::Cleanup()
{
	m_pSignalingSemaphore->Release();
	m_pWaitingSemaphore->Release();
	const uint64_t size = m_pCommandPool->GetCommandBufferCount();
	for ( uint64_t i = 0; i < size; ++i )
	{
		m_pFences[i]->Release();
		m_pInFlightFences[i] = nullptr;
	}
	m_pFences.clear();
	m_pModelBuffer->Release();
	m_pShadingBuffer->Release();
	m_pFrameBuffer->Release();
	m_pCommandPool->Release();
	m_pQueue->Release();
	m_pSwapchain->Release();
	m_pDevice->Release();
	delete m_pInstance;
	
	m_pInstance = nullptr;
	m_pDevice = nullptr;
	m_pSwapchain = nullptr;
	m_pFrameBuffer = nullptr;
	m_pQueue = nullptr;
	m_pCommandPool = nullptr;
	m_pSignalingSemaphore = nullptr;
	m_pWaitingSemaphore = nullptr;
	m_pModelBuffer = nullptr;
	m_pShadingBuffer = nullptr;

	delete this;
}

void Balbino::CRenderer::Draw()
{
	CCameraManager* pCameraManager{ CManager::GetCameraManager() };
	const std::vector<Balbino::CCamera>& allCameras{ pCameraManager->GetCameras() };
	if ( allCameras.size() )
	{
		uint32_t imageIndex;
		m_pFences[m_pCommandPool->GetCurrentIndex()]->Wait();
		m_pSwapchain->AcquireNextImage( m_pSignalingSemaphore, &imageIndex );
		m_pCommandPool->BeginRender( m_pFrameBuffer, m_pSwapchain );
		//todo: render
		//CManager::GetMeshManager()->Draw( m_pCommandPool );
#ifdef BALBINO_EDITOR
		m_pInterface->Draw( m_pCommandPool );
#endif // BALBINO_EDITOR

		if ( m_pInFlightFences[imageIndex] != nullptr ) {
			m_pInFlightFences[imageIndex]->Wait();
		}
		m_pInFlightFences[imageIndex] = m_pFences[m_pCommandPool->GetCurrentIndex()];
		m_pFences[m_pCommandPool->GetCurrentIndex()]->Reset();
		m_pCommandPool->EndRender();
		m_pQueue->SubmitPass( m_pWaitingSemaphore, m_pSignalingSemaphore, m_pCommandPool, m_pFences[m_pCommandPool->GetCurrentIndex()]);
		m_pQueue->PresentToScreen( m_pSwapchain, m_pWaitingSemaphore, imageIndex );
		m_pQueue->WaitIdle();
		m_pCommandPool->UpdateFrameIndex();
	}
}

float Balbino::CRenderer::GetAspectRatio() const
{
	return m_aspectRation;
}
