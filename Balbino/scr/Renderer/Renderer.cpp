#include "pch.h"
#include "Renderer.h"

Renderer::Renderer()
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
{
}

void Renderer::SetupVulkan( const char** extensions, uint32_t extensionsCount )
{
	vk::Result err;

	// Create Vulkan Instance
	{
		vk::InstanceCreateInfo createInfo{};
		createInfo.sType = vk::StructureType::eInstanceCreateInfo;
		createInfo.enabledExtensionCount = extensionsCount;
		createInfo.ppEnabledExtensionNames = extensions;

#if defined(BL_EDITOR) && defined(_DEBUG)
		//todo: add Editor callback
#else
		// Create Vulkan Instance without any debug feature
		err = vk::createInstance( &createInfo, m_pAllocator, &m_Instance );
		CheckVkResult( err );
#endif
	}

	// Select GPU
	{
		uint32_t gpuCount;
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

void Renderer::SetupVulkanWindow( SDL_Window* pWindow, VkSurfaceKHR surface, int width, int height )
{
	(void) pWindow;
	(void) surface;
	(void) width;
	(void) height;
#if BL_EDITOR
	//todo: call Interface
#endif
}

void Renderer::CleanupVulkan()
{
	m_Device.destroyDescriptorPool( m_DescriptorPool, m_pAllocator );

#if defined(BL_EDITOR) && defined(_DEBUG)
	//todo call interface
#endif // IMGUI_VULKAN_DEBUG_REPORT

	m_Device.destroy( m_pAllocator );
	m_Instance.destroy( m_pAllocator );
}

void Renderer::CleanupVulkanWindow()
{
#if BL_EDITOR
	//todo: call Interface
#endif
}

void Renderer::Draw()
{
	
}
