#include "pch.h"
#include "Renderer.h"

#include <SDL_vulkan.h>
#include <scr/Interface.h>

#include "Camera.h"
#include "../Managers/CameraManager.h"

Balbino::CRenderer::CRenderer()
	: m_pAllocator{nullptr}
	, m_commandPool{nullptr}
	, m_instance{VK_NULL_HANDLE}
	, m_physicalDevice{VK_NULL_HANDLE}
	, m_device{VK_NULL_HANDLE}
	, m_queueFamily{static_cast<uint32_t>(-1)}
	, m_queue{VK_NULL_HANDLE}
	, m_commandBuffers{}
	, m_frameFences{}
	, m_imageAvailableSemaphores{}
	, m_renderFinishedSemaphores{}
	, m_debugReport{VK_NULL_HANDLE}
	, m_pipelineCache{VK_NULL_HANDLE}
	, m_descriptorPool{VK_NULL_HANDLE}
	, m_minImageCount{2}
	, m_frameIndex{0}
	, m_swapChainRebuild{false}
#ifdef  BL_EDITOR
	, m_pInterface{nullptr}
#endif
{
}

void Balbino::CRenderer::Setup(SDL_Window* pWindow)
{
	(void) pWindow;
#if BL_EDITOR
	m_pInterface->Setup(pWindow, m_instance, m_physicalDevice, m_device, m_queueFamily, m_queue, m_pipelineCache, m_descriptorPool, m_pAllocator, m_minImageCount);
	m_pInterface->UploadFont(m_device, m_queue);
#endif
	VkCommandPoolCreateInfo commandPoolCreateInfo{};
	commandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	commandPoolCreateInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	commandPoolCreateInfo.queueFamilyIndex = m_queueFamily;

	VkResult err = vkCreateCommandPool(m_device, &commandPoolCreateInfo, nullptr, &m_commandPool);
	CheckVkResult(err);

	VkCommandBufferAllocateInfo commandBufferAllocInfo{};
	commandBufferAllocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	commandBufferAllocInfo.commandPool = m_commandPool;
	commandBufferAllocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	commandBufferAllocInfo.commandBufferCount = FRAME_COUNT;

	err = vkAllocateCommandBuffers(m_device, &commandBufferAllocInfo, m_commandBuffers);
	CheckVkResult(err);

	VkSemaphoreCreateInfo semaphoreCreateInfo{};
	semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	err = vkCreateSemaphore(m_device, &semaphoreCreateInfo, nullptr, &m_imageAvailableSemaphores[0]);
	CheckVkResult(err);
	err = vkCreateSemaphore(m_device, &semaphoreCreateInfo, nullptr, &m_imageAvailableSemaphores[1]);
	CheckVkResult(err);
	err = vkCreateSemaphore(m_device, &semaphoreCreateInfo, nullptr, &m_renderFinishedSemaphores[0]);
	CheckVkResult(err);
	err = vkCreateSemaphore(m_device, &semaphoreCreateInfo, nullptr, &m_renderFinishedSemaphores[1]);
	CheckVkResult(err);

	VkFenceCreateInfo fenceCreateInfo{};
	fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	err = vkCreateFence(m_device, &fenceCreateInfo, nullptr, &m_frameFences[0]);
	CheckVkResult(err);
	err = vkCreateFence(m_device, &fenceCreateInfo, nullptr, &m_frameFences[1]);
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
			m_pInterface->SetupVulkan(extensions, extensionsCount, createInfo, m_instance, m_pAllocator, m_debugReport);
#else
		// Create Vulkan Instance without any debug feature
		err = vkCreateInstance(&createInfo, m_pAllocator, &m_instance);
		CheckVkResult(err);
#endif
	}

	// Select GPU
	{
		uint32_t gpuCount{};
		err = vkEnumeratePhysicalDevices(m_instance, &gpuCount, nullptr);
		CheckVkResult(err);

		VkPhysicalDevice* pGpus{static_cast<VkPhysicalDevice*>(malloc(sizeof(VkPhysicalDevice) * gpuCount))};
		err = vkEnumeratePhysicalDevices(m_instance, &gpuCount, pGpus);
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

		m_physicalDevice = pGpus[useGpu];
		free(pGpus);
	}

	// Select graphics queue family
	{
		uint32_t count;
		vkGetPhysicalDeviceQueueFamilyProperties(m_physicalDevice, &count, nullptr);
		VkQueueFamilyProperties* queues{static_cast<VkQueueFamilyProperties*>(malloc(sizeof(VkQueueFamilyProperties) * count))};
		vkGetPhysicalDeviceQueueFamilyProperties(m_physicalDevice, &count, queues);
		for (uint32_t i = 0; i < count; i++)
			if (queues[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
			{
				m_queueFamily = i;
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
		queueInfo[0].queueFamilyIndex = m_queueFamily;
		queueInfo[0].queueCount = 1;
		queueInfo[0].pQueuePriorities = queuePriority;
		VkDeviceCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		createInfo.queueCreateInfoCount = static_cast<uint32_t>(std::size(queueInfo));
		createInfo.pQueueCreateInfos = queueInfo;
		createInfo.enabledExtensionCount = deviceExtensionCount;
		createInfo.ppEnabledExtensionNames = deviceExtensions;
		err = vkCreateDevice(m_physicalDevice, &createInfo, m_pAllocator, &m_device);
		CheckVkResult(err);
		vkGetDeviceQueue(m_device, m_queueFamily, 0, &m_queue);
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
		poolInfo.maxSets = 1000 * static_cast<uint32_t>(std::size(poolSizes));
		poolInfo.poolSizeCount = static_cast<uint32_t>(std::size(poolSizes));
		poolInfo.pPoolSizes = poolSizes;
		err = vkCreateDescriptorPool(m_device, &poolInfo, m_pAllocator, &m_descriptorPool);
		CheckVkResult(err);
	}
}

void Balbino::CRenderer::SetupVulkanWindow(SDL_Window* pWindow)
{
	(void) pWindow;
	VkSurfaceKHR surface;
	if (SDL_Vulkan_CreateSurface(pWindow, m_instance, &surface) == 0)
	{
		throw std::runtime_error("Failed to create Vulkan surface.\n");
	}

	// Create Framebuffers
	int w, h;
	SDL_GetWindowSize(pWindow, &w, &h);

#if BL_EDITOR
	if (m_pInterface)
	{
		m_pInterface->SetupVulkanWindow(surface, w, h, m_instance, m_physicalDevice, m_device, m_pAllocator, m_queueFamily, m_minImageCount);
	}
#else
	vkDeviceWaitIdle(m_device);
	//create swap chain (https://sopyer.github.io/Blog/post/minimal-vulkan-sample/)
	{
		//Use first available format
		uint32_t formatCount = 1;
		VkResult err = vkGetPhysicalDeviceSurfaceFormatsKHR(m_physicalDevice, surface, &formatCount, nullptr); // suppress validation layer
		CheckVkResult(err);
		err = vkGetPhysicalDeviceSurfaceFormatsKHR(m_physicalDevice, surface, &formatCount, &m_surfaceFormat);
		CheckVkResult(err);
		m_surfaceFormat.format = m_surfaceFormat.format == VK_FORMAT_UNDEFINED ? VK_FORMAT_R8G8B8A8_UNORM : m_surfaceFormat.format;


		uint32_t presentModeCount{};
		err = vkGetPhysicalDeviceSurfacePresentModesKHR(m_physicalDevice, surface, &presentModeCount, nullptr);
		CheckVkResult(err);
		VkPresentModeKHR presentModes[MAX_PRESENT_MODE_COUNT]{};
		presentModeCount = presentModeCount > MAX_PRESENT_MODE_COUNT ? MAX_PRESENT_MODE_COUNT : presentModeCount;
		err = vkGetPhysicalDeviceSurfacePresentModesKHR(m_physicalDevice, surface, &presentModeCount, presentModes);
		CheckVkResult(err);

		VkPresentModeKHR presentMode{VK_PRESENT_MODE_FIFO_KHR};   // always supported.
		for (uint32_t i = 0; i < presentModeCount; ++i)
		{
			if (presentModes[i] == VK_PRESENT_MODE_MAILBOX_KHR)
			{
				presentMode = VK_PRESENT_MODE_MAILBOX_KHR;
				break;
			}
		}
		if (presentMode == VK_PRESENT_MODE_MAILBOX_KHR)
			m_imageCount = PRESENT_MODE_MAILBOX_IMAGE_COUNT;
		else
			m_imageCount = PRESENT_MODE_DEFAULT_IMAGE_COUNT;

		err = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_physicalDevice, surface, &m_SurfaceCapabilities);
		CheckVkResult(err);

		m_swapchainExtent = m_SurfaceCapabilities.currentExtent;
		if (m_swapchainExtent.width == UINT32_MAX)
		{
			m_swapchainExtent.width = std::clamp(m_width, m_SurfaceCapabilities.minImageExtent.width, m_SurfaceCapabilities.maxImageExtent.width);
			m_swapchainExtent.height = std::clamp(m_height, m_SurfaceCapabilities.minImageExtent.height, m_SurfaceCapabilities.maxImageExtent.height);
		}


		VkSwapchainCreateInfoKHR swapChainCreateInfo{};
		swapChainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		swapChainCreateInfo.surface = surface;
		swapChainCreateInfo.minImageCount = m_minImageCount;
		swapChainCreateInfo.imageFormat = m_surfaceFormat.format;
		swapChainCreateInfo.imageColorSpace = m_surfaceFormat.colorSpace;
		swapChainCreateInfo.imageExtent = m_swapchainExtent;
		swapChainCreateInfo.imageArrayLayers = 1; // 2 for stereo
		swapChainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
		swapChainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		swapChainCreateInfo.preTransform = m_SurfaceCapabilities.currentTransform;
		swapChainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		swapChainCreateInfo.presentMode = VK_PRESENT_MODE_MAILBOX_KHR;
		swapChainCreateInfo.clipped = VK_TRUE;

		err = vkCreateSwapchainKHR(m_device, &swapChainCreateInfo, nullptr, &m_swapchain);
		CheckVkResult(err);
		err = vkGetSwapchainImagesKHR(m_device, m_swapchain, &m_minImageCount, nullptr);
		CheckVkResult(err);
		err = vkGetSwapchainImagesKHR(m_device, m_swapchain, &m_minImageCount, m_Images);
		CheckVkResult(err);
	}
#endif
}

void Balbino::CRenderer::Cleanup() const
{
	vkDeviceWaitIdle(m_device);
#if BL_EDITOR
	m_pInterface->Cleanup();
#endif
	CleanupVulkanWindow();
	CleanupVulkan();
}

void Balbino::CRenderer::CleanupVulkan() const
{
	vkDeviceWaitIdle(m_device);
#ifndef BL_EDITOR
	vkDestroySwapchainKHR(m_device, m_swapchain, nullptr);
#endif

	vkDestroyFence(m_device, m_frameFences[0], nullptr);
	vkDestroyFence(m_device, m_frameFences[1], nullptr);
	vkDestroySemaphore(m_device, m_renderFinishedSemaphores[0], nullptr);
	vkDestroySemaphore(m_device, m_renderFinishedSemaphores[1], nullptr);
	vkDestroySemaphore(m_device, m_imageAvailableSemaphores[0], nullptr);
	vkDestroySemaphore(m_device, m_imageAvailableSemaphores[1], nullptr);
	vkDestroyCommandPool(m_device, m_commandPool, nullptr);

	vkDestroyDescriptorPool(m_device, m_descriptorPool, m_pAllocator);

#if defined(BL_EDITOR) && defined(_DEBUG)
	if (m_pInterface)
		m_pInterface->CleanupVulkan(m_instance, m_pAllocator, m_debugReport);
#endif // IMGUI_VULKAN_DEBUG_REPORT

	vkDestroyDevice(m_device, m_pAllocator);
	vkDestroyInstance(m_instance, m_pAllocator);
}

void Balbino::CRenderer::CleanupVulkanWindow() const
{
#if BL_EDITOR
	if (m_pInterface)
		m_pInterface->CleanupVulkanWindow(m_instance, m_pAllocator, m_device);
#endif
}

void Balbino::CRenderer::CheckVkResult(VkResult err)
{
	if (err == 0)
		return;
	fprintf(stderr, "[vulkan] Error: VkResult = %d\n", err);
	if (err < 0)
		abort();
}

void Balbino::CRenderer::Draw(SDL_Window* pWindow)
{
	(void) pWindow;
#if BL_EDITOR
	m_pInterface->ResizeSwapChain(pWindow, m_instance, m_physicalDevice, m_device, m_queueFamily, m_pAllocator, m_minImageCount);
	m_pInterface->DrawStart(pWindow);

	//todo: Render stuff

	m_pInterface->Draw();
	m_pInterface->Render(m_device, m_queue);
#else
	const std::vector<CCamera*>& allCameras{CCameraManager::GetCameras()};
	if (allCameras.size())
	{
		const uint32_t index = (m_frameIndex++) % FRAME_COUNT;
		VkResult err = vkWaitForFences(m_device, 1, &m_frameFences[index], VK_TRUE, UINT64_MAX);
		CheckVkResult(err);
		err = vkResetFences(m_device, 1, &m_frameFences[index]);
		CheckVkResult(err);

		uint32_t imageIndex;
		err = vkAcquireNextImageKHR(m_device, m_swapchain, UINT64_MAX, m_imageAvailableSemaphores[index], VK_NULL_HANDLE, &imageIndex);
		CheckVkResult(err);

		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

		const VkClearColorValue clearColor{164.0f / 256.0f, 30.0f / 256.0f, 34.0f / 256.0f, 0.0f};
		VkImageSubresourceRange imageRange{};
		imageRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		imageRange.levelCount = 1;
		imageRange.layerCount = 1;

		err = vkBeginCommandBuffer(m_commandBuffers[index], &beginInfo);
		CheckVkResult(err);
		vkCmdClearColorImage(m_commandBuffers[index], m_images[index], VK_IMAGE_LAYOUT_GENERAL, &clearColor, 1, &imageRange);
		//todo: draw things


		err = vkEndCommandBuffer(m_commandBuffers[index]);
		CheckVkResult(err);

		constexpr VkPipelineStageFlags waitStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = &m_imageAvailableSemaphores[index];
		submitInfo.pWaitDstStageMask = &waitStage;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &m_commandBuffers[index];
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = &m_renderFinishedSemaphores[index];
		err = vkQueueSubmit(m_queue, 1, &submitInfo, m_frameFences[index]);
		CheckVkResult(err);

		VkPresentInfoKHR presentInfo{};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = &m_renderFinishedSemaphores[index];
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = &m_swapchain;
		presentInfo.pImageIndices = &imageIndex;
		err = vkQueuePresentKHR(m_queue, &presentInfo);
		CheckVkResult(err);
	}
#endif
}

#if BL_EDITOR
void Balbino::CRenderer::SetInterface(CInterface* const pInterface)
{
	m_pInterface = pInterface;
}
#endif
