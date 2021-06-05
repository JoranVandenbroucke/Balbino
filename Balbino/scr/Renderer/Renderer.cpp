#include "pch.h"
#include "Renderer.h"

#include <SDL_vulkan.h>
#include <scr/Interface.h>

Balbino::Renderer::Renderer()
	: m_pAllocator{ nullptr }
	, m_Instance{ VK_NULL_HANDLE }
	, m_PhysicalDevice{ VK_NULL_HANDLE }
	, m_Device{ VK_NULL_HANDLE }
	, m_QueueFamily{ static_cast<uint32_t>( -1 ) }
	, m_Queue{ VK_NULL_HANDLE }
	, m_DebugReport{ VK_NULL_HANDLE }
	, m_PipelineCache{ VK_NULL_HANDLE }
	, m_DescriptorPool{ VK_NULL_HANDLE }
	, m_MinImageCount{ 2 }
	, m_SwapChainRebuild{ false }
#ifdef  BL_EDITOR
	, m_pInterface{ nullptr }
#endif
{
}

void Balbino::Renderer::Setup( SDL_Window* pWindow )
{
	(void) pWindow;
#if BL_EDITOR
	m_pInterface->Setup( pWindow, m_Instance, m_PhysicalDevice, m_Device, m_QueueFamily, m_Queue, m_PipelineCache, m_DescriptorPool, m_pAllocator, m_MinImageCount );
	m_pInterface->UploadFont( m_Device, m_Queue );
#endif

}

void Balbino::Renderer::SetupVulkan( const char** extensions, const uint32_t extensionsCount )
{
	vk::Result err;

	// Create Vulkan Instance
	{
		vk::InstanceCreateInfo createInfo{};
		createInfo.sType = vk::StructureType::eInstanceCreateInfo;
		createInfo.enabledExtensionCount = extensionsCount;
		createInfo.ppEnabledExtensionNames = extensions;

#if defined(BL_EDITOR) && defined(_DEBUG)
		if ( m_pInterface )
			m_pInterface->SetupVulkan( extensions, extensionsCount, createInfo, m_Instance, m_pAllocator, m_DebugReport );
#else
		// Create Vulkan Instance without any debug feature
		err = vk::createInstance( &createInfo, m_pAllocator, &m_Instance );
		CheckVkResult( err );
#endif
	}

	// Select GPU
	{
		uint32_t gpuCount{};
		err = m_Instance.enumeratePhysicalDevices( &gpuCount, nullptr );
		CheckVkResult( err );

		vk::PhysicalDevice* pGpus{ static_cast<vk::PhysicalDevice*>( malloc( sizeof( vk::PhysicalDevice ) * gpuCount ) ) };
		err = m_Instance.enumeratePhysicalDevices( &gpuCount, pGpus );
		CheckVkResult( err );

		// If a number >1 of GPUs got reported, find discrete GPU if present, or use first one available. This covers
		// most common cases (multi-gpu/integrated+dedicated graphics). Handling more complicated setups (multiple
		// dedicated GPUs) is out of scope of this sample.
		int useGpu{ 0 };
		for ( int i = 0; i < static_cast<int>( gpuCount ); i++ )
		{
			vk::PhysicalDeviceProperties properties;
			pGpus[i].getProperties( &properties );
			if ( properties.deviceType == vk::PhysicalDeviceType::eDiscreteGpu )
			{
				useGpu = i;
				break;
			}
		}

		m_PhysicalDevice = pGpus[useGpu];
		free( pGpus );
	}

	// Select graphics queue family
	{
		uint32_t count;
		m_PhysicalDevice.getQueueFamilyProperties( &count, nullptr );
		vk::QueueFamilyProperties* queues{ static_cast<vk::QueueFamilyProperties*>( malloc( sizeof( vk::QueueFamilyProperties ) * count ) ) };
		m_PhysicalDevice.getQueueFamilyProperties( &count, queues );
		for ( uint32_t i = 0; i < count; i++ )
			if ( queues[i].queueFlags & vk::QueueFlagBits::eGraphics )
			{
				m_QueueFamily = i;
				break;
			}
		free( queues );
	}

	// Create Logical Device (with 1 queue)
	{
		const int deviceExtensionCount{ 1 };
		const char* deviceExtensions[]{ "VK_KHR_swapchain" };
		const float queuePriority[]{ 1.0f };
		vk::DeviceQueueCreateInfo queueInfo[1]{};
		queueInfo[0].sType = vk::StructureType::eDeviceQueueCreateInfo;
		queueInfo[0].queueFamilyIndex = m_QueueFamily;
		queueInfo[0].queueCount = 1;
		queueInfo[0].pQueuePriorities = queuePriority;
		vk::DeviceCreateInfo createInfo = {};
		createInfo.sType = vk::StructureType::eDeviceCreateInfo;
		createInfo.queueCreateInfoCount = sizeof( queueInfo ) / sizeof( queueInfo[0] );
		createInfo.pQueueCreateInfos = queueInfo;
		createInfo.enabledExtensionCount = deviceExtensionCount;
		createInfo.ppEnabledExtensionNames = deviceExtensions;
		err = m_PhysicalDevice.createDevice( &createInfo, m_pAllocator, &m_Device );
		CheckVkResult( err );
		m_Device.getQueue( m_QueueFamily, 0, &m_Queue );
	}

	// Create Descriptor Pool
	{
		vk::DescriptorPoolSize poolSizes[]
		{
			{ vk::DescriptorType::eSampler, 1000 },
			{ vk::DescriptorType::eCombinedImageSampler, 1000 },
			{ vk::DescriptorType::eCombinedImageSampler, 1000 },
			{ vk::DescriptorType::eStorageImage, 1000 },
			{ vk::DescriptorType::eUniformTexelBuffer, 1000 },
			{ vk::DescriptorType::eStorageTexelBuffer, 1000 },
			{ vk::DescriptorType::eUniformBuffer, 1000 },
			{ vk::DescriptorType::eStorageBuffer, 1000 },
			{ vk::DescriptorType::eUniformBufferDynamic, 1000 },
			{ vk::DescriptorType::eStorageBufferDynamic, 1000 },
			{ vk::DescriptorType::eInputAttachment, 1000 }
		};
		vk::DescriptorPoolCreateInfo poolInfo{};
		poolInfo.sType = vk::StructureType::eDescriptorPoolCreateInfo;
		poolInfo.flags = vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet;
		poolInfo.maxSets = 1000 * ( sizeof( poolSizes ) / sizeof( poolSizes[0] ) );
		poolInfo.poolSizeCount = static_cast<uint32_t>( sizeof( poolSizes ) / sizeof( poolSizes[0] ) );
		poolInfo.pPoolSizes = poolSizes;
		err = m_Device.createDescriptorPool( &poolInfo, m_pAllocator, &m_DescriptorPool );
		CheckVkResult( err );
	}
}

void Balbino::Renderer::SetupVulkanWindow( SDL_Window* pWindow ) const
{
	(void) pWindow;
#if BL_EDITOR
	VkSurfaceKHR surface;
	if ( SDL_Vulkan_CreateSurface( pWindow, m_Instance, &surface ) == 0 )
	{
		throw std::runtime_error( "Failed to create Vulkan surface.\n" );
	}

	// Create Framebuffers
	int w, h;
	SDL_GetWindowSize( pWindow, &w, &h );
	if ( m_pInterface )
	{
		m_pInterface->SetupVulkanWindow( surface, w, h, m_Instance, m_PhysicalDevice, m_Device, m_pAllocator, m_QueueFamily, m_MinImageCount );
	}
#endif
}

void Balbino::Renderer::Cleanup() const
{
	m_Device.waitIdle();
#if BL_EDITOR
	m_pInterface->Cleanup();
#endif
	CleanupVulkanWindow();
	CleanupVulkan();
}

void Balbino::Renderer::CleanupVulkan() const
{
	m_Device.destroyDescriptorPool( m_DescriptorPool, m_pAllocator );

#if defined(BL_EDITOR) && defined(_DEBUG)
	if ( m_pInterface )
		m_pInterface->CleanupVulkan( m_Instance, m_pAllocator, m_DebugReport );
#endif // IMGUI_VULKAN_DEBUG_REPORT

	m_Device.destroy( m_pAllocator );
	m_Instance.destroy( m_pAllocator );
}

void Balbino::Renderer::CleanupVulkanWindow() const
{
#if BL_EDITOR
	if ( m_pInterface )
		m_pInterface->CleanupVulkanWindow( m_Instance, m_pAllocator, m_Device );
#endif
}

void Balbino::Renderer::Draw( SDL_Window* pWindow ) const
{
	(void) pWindow;
#if BL_EDITOR
	m_pInterface->ResizeSwapChain( pWindow, m_Instance, m_PhysicalDevice, m_Device, m_QueueFamily, m_pAllocator, m_MinImageCount );
	m_pInterface->DrawStart( pWindow );
	m_pInterface->Draw();
	m_pInterface->Render( m_Device, m_Queue );
#endif
}

#if BL_EDITOR
void Balbino::Renderer::SetInterface( Interface* const pInterface )
{
	m_pInterface = pInterface;
}
#endif
