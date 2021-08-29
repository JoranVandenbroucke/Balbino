#include "pch.h"
#include "Renderer.h"

#include <SDL_vulkan.h>
#include <scr/Interface.h>

#include "Camera.h"
#include "../Managers/CameraManager.h"

Balbino::CRenderer::CRenderer()
	: m_pAllocator{nullptr}
	, m_CommandPool{nullptr}
	, m_Instance{VK_NULL_HANDLE}
	, m_PhysicalDevice{VK_NULL_HANDLE}
	, m_Device{VK_NULL_HANDLE}
	, m_QueueFamily{static_cast<uint32_t>(-1)}
	, m_Queue{VK_NULL_HANDLE}
	, m_CommandBuffers{}
	, m_FrameFences{}
	, m_ImageAvailableSemaphores{}
	, m_RenderFinishedSemaphores{}
	, m_DebugReport{VK_NULL_HANDLE}
	, m_PipelineCache{VK_NULL_HANDLE}
	, m_DescriptorPool{VK_NULL_HANDLE}
	, m_MinImageCount{2}
	, m_FrameIndex{0}
	, m_SwapChainRebuild{false}
#ifdef  BL_EDITOR
	, m_pInterface{nullptr}
#endif
{
}

void Balbino::CRenderer::Setup(SDL_Window* pWindow)
{
	(void) pWindow;
#if BL_EDITOR
	m_pInterface->Setup(pWindow, m_Instance, m_PhysicalDevice, m_Device, m_QueueFamily, m_Queue, m_PipelineCache, m_DescriptorPool, m_pAllocator, m_MinImageCount);
	m_pInterface->UploadFont(m_Device, m_Queue);
#endif
	VkCommandPoolCreateInfo commandPoolCreateInfo{};
	commandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	commandPoolCreateInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	commandPoolCreateInfo.queueFamilyIndex = m_QueueFamily;

	VkResult err = vkCreateCommandPool(m_Device, &commandPoolCreateInfo, nullptr, &m_CommandPool);
	CheckVkResult(err);

	VkCommandBufferAllocateInfo commandBufferAllocInfo{};
	commandBufferAllocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	commandBufferAllocInfo.commandPool = m_CommandPool;
	commandBufferAllocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	commandBufferAllocInfo.commandBufferCount = FRAME_COUNT;

	err = vkAllocateCommandBuffers(m_Device, &commandBufferAllocInfo, m_CommandBuffers);
	CheckVkResult(err);

	VkSemaphoreCreateInfo semaphoreCreateInfo{};
	semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	err = vkCreateSemaphore(m_Device, &semaphoreCreateInfo, nullptr, &m_ImageAvailableSemaphores[0]);
	CheckVkResult(err);
	err = vkCreateSemaphore(m_Device, &semaphoreCreateInfo, nullptr, &m_ImageAvailableSemaphores[1]);
	CheckVkResult(err);
	err = vkCreateSemaphore(m_Device, &semaphoreCreateInfo, nullptr, &m_RenderFinishedSemaphores[0]);
	CheckVkResult(err);
	err = vkCreateSemaphore(m_Device, &semaphoreCreateInfo, nullptr, &m_RenderFinishedSemaphores[1]);
	CheckVkResult(err);

	VkFenceCreateInfo fenceCreateInfo{};
	fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	err = vkCreateFence(m_Device, &fenceCreateInfo, nullptr, &m_FrameFences[0]);
	CheckVkResult(err);
	err = vkCreateFence(m_Device, &fenceCreateInfo, nullptr, &m_FrameFences[1]);
	CheckVkResult(err);

}

void Balbino::CRenderer::SetupVulkan(const char** extensions, const uint32_t extensionsCount)
{
	VkResult err;

	// Create Vulkan Instance
	{
		VkInstanceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.enabledExtensionCount = extensionsCount;
		createInfo.ppEnabledExtensionNames = extensions;

#if defined(BL_EDITOR) && defined(_DEBUG)
		if (m_pInterface)
			m_pInterface->SetupVulkan(extensions, extensionsCount, createInfo, m_Instance, m_pAllocator, m_DebugReport);
#else
		// Create Vulkan Instance without any debug feature
		err = vkCreateInstance(&createInfo, m_pAllocator, &m_Instance);
		CheckVkResult(err);
#endif
	}

	// Select GPU
	{
		uint32_t gpuCount{};
		err = vkEnumeratePhysicalDevices(m_Instance, &gpuCount, nullptr);
		CheckVkResult(err);

		VkPhysicalDevice* pGpus{static_cast<VkPhysicalDevice*>(malloc(sizeof(VkPhysicalDevice) * gpuCount))};
		err = vkEnumeratePhysicalDevices(m_Instance, &gpuCount, pGpus);
		CheckVkResult(err);

		// If a number >1 of GPUs got reported, find discrete GPU if present, or use first one available. This covers
		// most common cases (multi-gpu/integrated+dedicated graphics). Handling more complicated setups (multiple
		// dedicated GPUs) is out of scope of this sample.
		int useGpu{0};
		for (int i = 0; i < static_cast<int>(gpuCount); i++)
		{
			VkPhysicalDeviceProperties properties;
			vkGetPhysicalDeviceProperties(pGpus[i], &properties);
			if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
			{
				useGpu = i;
				break;
			}
		}

		m_PhysicalDevice = pGpus[useGpu];
		free(pGpus);
	}

	// Select graphics queue family
	{
		uint32_t count;
		vkGetPhysicalDeviceQueueFamilyProperties(m_PhysicalDevice, &count, nullptr);
		VkQueueFamilyProperties* queues{static_cast<VkQueueFamilyProperties*>(malloc(sizeof(VkQueueFamilyProperties) * count))};
		vkGetPhysicalDeviceQueueFamilyProperties(m_PhysicalDevice, &count, queues);
		for (uint32_t i = 0; i < count; i++)
			if (queues[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
			{
				m_QueueFamily = i;
				break;
			}
		free(queues);
	}

	// Create Logical Device (with 1 queue)
	{
		constexpr int deviceExtensionCount{1};
		const char* deviceExtensions[]{"VK_KHR_swapchain"};
		const float queuePriority[]{1.0f};
		VkDeviceQueueCreateInfo queueInfo[1]{};
		queueInfo[0].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueInfo[0].queueFamilyIndex = m_QueueFamily;
		queueInfo[0].queueCount = 1;
		queueInfo[0].pQueuePriorities = queuePriority;
		VkDeviceCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		createInfo.queueCreateInfoCount = static_cast<uint32_t>( std::size( queueInfo ) );
		createInfo.pQueueCreateInfos = queueInfo;
		createInfo.enabledExtensionCount = deviceExtensionCount;
		createInfo.ppEnabledExtensionNames = deviceExtensions;
		err = vkCreateDevice(m_PhysicalDevice, &createInfo, m_pAllocator, &m_Device);
		CheckVkResult(err);
		vkGetDeviceQueue(m_Device, m_QueueFamily, 0, &m_Queue);
	}

	// Create Descriptor Pool
	{
		const VkDescriptorPoolSize poolSizes[]
		{
			{ VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
			{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
			{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
			{ VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
			{ VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
			{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
			{ VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 }
		};
		VkDescriptorPoolCreateInfo poolInfo{};
		poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
		poolInfo.maxSets = 1000 * static_cast<uint32_t>( std::size( poolSizes ) );
		poolInfo.poolSizeCount = static_cast<uint32_t>(std::size(poolSizes));
		poolInfo.pPoolSizes = poolSizes;
		err = vkCreateDescriptorPool(m_Device, &poolInfo, m_pAllocator, &m_DescriptorPool);
		CheckVkResult(err);
	}
}

void Balbino::CRenderer::SetupVulkanWindow(SDL_Window* pWindow) const
{
	(void) pWindow;
	VkSurfaceKHR surface;
	if (SDL_Vulkan_CreateSurface(pWindow, m_Instance, &surface) == 0)
	{
		throw std::runtime_error("Failed to create Vulkan surface.\n");
	}

	// Create Framebuffers
	int w, h;
	SDL_GetWindowSize(pWindow, &w, &h);

	const std::vector<CCamera*>& cam = CCameraManager::GetCameras();
	for (CCamera* camera : cam)
	{
		camera->Setup(m_Device, m_PhysicalDevice, surface, FRAME_COUNT);
	}
#if BL_EDITOR
	if (m_pInterface)
	{
		m_pInterface->SetupVulkanWindow(surface, w, h, m_Instance, m_PhysicalDevice, m_Device, m_pAllocator, m_QueueFamily, m_MinImageCount);
	}
#else

#endif
}

void Balbino::CRenderer::Cleanup() const
{
	vkDeviceWaitIdle(m_Device);
	const std::vector<CCamera*>& cam = CCameraManager::GetCameras();
	for ( const CCamera* camera : cam)
	{
		camera->Destroy(m_Device);
	}
	vkDeviceWaitIdle(m_Device);
#if BL_EDITOR
	m_pInterface->Cleanup();
#endif
	CleanupVulkanWindow();
	CleanupVulkan();
}

void Balbino::CRenderer::CleanupVulkan() const
{
	vkDeviceWaitIdle(m_Device);
	vkDestroyFence(m_Device, m_FrameFences[0], nullptr);
	vkDestroyFence(m_Device, m_FrameFences[1], nullptr);
	vkDestroySemaphore(m_Device, m_RenderFinishedSemaphores[0], nullptr);
	vkDestroySemaphore(m_Device, m_RenderFinishedSemaphores[1], nullptr);
	vkDestroySemaphore(m_Device, m_ImageAvailableSemaphores[0], nullptr);
	vkDestroySemaphore(m_Device, m_ImageAvailableSemaphores[1], nullptr);
	vkDestroyCommandPool(m_Device, m_CommandPool, nullptr);

	vkDestroyDescriptorPool(m_Device, m_DescriptorPool, m_pAllocator);

#if defined(BL_EDITOR) && defined(_DEBUG)
	if (m_pInterface)
		m_pInterface->CleanupVulkan(m_Instance, m_pAllocator, m_DebugReport);
#endif // IMGUI_VULKAN_DEBUG_REPORT

	vkDestroyDevice(m_Device, m_pAllocator);
	vkDestroyInstance(m_Instance, m_pAllocator);
}

void Balbino::CRenderer::CleanupVulkanWindow() const
{
#if BL_EDITOR
	if (m_pInterface)
		m_pInterface->CleanupVulkanWindow(m_Instance, m_pAllocator, m_Device);
#endif
}

void Balbino::CRenderer::CheckVkResult( VkResult err )
{
	if ( err == 0 )
		return;
	fprintf( stderr, "[vulkan] Error: VkResult = %d\n", err );
	if ( err < 0 )
		abort();
}

void Balbino::CRenderer::Draw( const SDL_Window* pWindow )
{
	(void) pWindow;
#if BL_EDITOR
	m_pInterface->ResizeSwapChain(pWindow, m_Instance, m_PhysicalDevice, m_Device, m_QueueFamily, m_pAllocator, m_MinImageCount);
	m_pInterface->DrawStart(pWindow);
	m_pInterface->Draw();
	m_pInterface->Render(m_Device, m_Queue);
#endif

	const std::vector<CCamera*>& allCameras{CCameraManager::GetCameras()};
	if (allCameras.size())
	{
		const uint32_t index = (m_FrameIndex++) % FRAME_COUNT;
		VkResult err = vkWaitForFences(m_Device, 1, &m_FrameFences[index], VK_TRUE, UINT64_MAX);
		CheckVkResult(err);
		err = vkResetFences(m_Device, 1, &m_FrameFences[index]);
		CheckVkResult(err);

		uint32_t imageIndex;
		err = vkAcquireNextImageKHR(m_Device, allCameras[0]->GetSwapchainKhr(), UINT64_MAX, m_ImageAvailableSemaphores[index], VK_NULL_HANDLE, &imageIndex);
		CheckVkResult(err);

		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
		err = vkBeginCommandBuffer(m_CommandBuffers[index], &beginInfo);
		CheckVkResult(err);

		//todo: draw things

		err = vkEndCommandBuffer(m_CommandBuffers[index]);
		CheckVkResult(err);

		constexpr VkPipelineStageFlags waitStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = &m_ImageAvailableSemaphores[index];
		submitInfo.pWaitDstStageMask = &waitStage;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &m_CommandBuffers[index];
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = &m_RenderFinishedSemaphores[index];
		err = vkQueueSubmit(m_Queue, 1, &submitInfo, m_FrameFences[index]);
		CheckVkResult(err);

		VkPresentInfoKHR presentInfo{};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = &m_RenderFinishedSemaphores[index];
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = &allCameras[0]->GetSwapchainKhr();
		presentInfo.pImageIndices = &imageIndex;
		err = vkQueuePresentKHR(m_Queue, &presentInfo);
		CheckVkResult(err);
	}
}

#if BL_EDITOR
void Balbino::Renderer::SetInterface(Interface* const pInterface)
{
	m_pInterface = pInterface;
}
#endif
