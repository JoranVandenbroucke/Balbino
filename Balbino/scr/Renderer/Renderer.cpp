#include "pch.h"
#include "Renderer.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <SDL_vulkan.h>
#include <scr/Interface.h>

#include "../Camera.h"
#include "Texture.h"
#include "Mesh.h"
#include "UniformBufferObject.h"
#include "../Managers/CameraManager.h"
#include "../Managers/TextureManager.h"
#include "../Managers/ShaderManager.h"
#include "../Managers/MeshManager.h"
#include "../Managers/Manager.h"

#if defined(_DEBUG) && !defined(BL_EDITOR)
static VKAPI_ATTR VkBool32 VKAPI_CALL DebugReport(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objectType, uint64_t object, size_t location, int32_t messageCode, const char* pLayerPrefix, const char* pMessage, void* pUserData)
{
	(void)flags;
	(void)object;
	(void)location;
	(void)messageCode;
	(void)pUserData;
	(void)pLayerPrefix; // Unused arguments
	fprintf(stderr, "[vulkan] Debug report from ObjectType: %i\n\rMessage: %s\n\r\n\r", objectType, pMessage);
	return VK_FALSE;
}
#else
static VKAPI_ATTR VkBool32 VKAPI_CALL DebugReport(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objectType, uint64_t object, size_t location, int32_t messageCode, const char* pLayerPrefix, const char* pMessage, void* pUserData)
{
	(void)flags;
	(void)objectType;
	(void)object;
	(void)location;
	(void)messageCode;
	(void)pUserData;
	(void)pMessage;
	(void)pLayerPrefix; // Unused arguments
	return 0;
}
#endif //_DEBUG && !BL_EDITOR

Balbino::CRenderer::CRenderer()
	: m_surface{ nullptr }
	, m_instance{ VK_NULL_HANDLE }
	, m_pCallback{ VK_NULL_HANDLE }
	, m_debugReport{ VK_NULL_HANDLE }
	, m_physicalDevice{ VK_NULL_HANDLE }
	, m_commandPool{ VK_NULL_HANDLE }
	, m_device{ VK_NULL_HANDLE }
	, m_queue{ VK_NULL_HANDLE }
	, m_descriptorPool{ VK_NULL_HANDLE }
	, m_pipelineCache{ VK_NULL_HANDLE }
	, m_commandBuffers{}
	, m_frameFences{}
	, m_imageAvailableSemaphores{}
	, m_queueFamily{ static_cast<uint32_t>(-1) }
	, m_renderFinishedSemaphores{}
	, m_minImageCount{ 2 }
	, m_frameIndex{ 0 }
	, m_width{ 0 }
	, m_height{ 0 }
	, m_imageCount{ 0 }
	, m_swapChainRebuild{ false }
	, m_swapchain{ VK_NULL_HANDLE }
	, m_renderPass{ VK_NULL_HANDLE }
	, m_surfaceFormat{}
	, m_presentMode{}
	, m_swapchainExtent{}
	, m_surfaceCapabilities{}
	, m_images{}
	, m_descriptorSetLayout{ VK_NULL_HANDLE }
#ifdef  BL_EDITOR
	, m_pInterface{ nullptr }
#endif
{
}

void Balbino::CRenderer::Setup(SDL_Window* pWindow, const char** extensions, const uint32_t extensionsCount)
{
	(void)pWindow;
	CManager::GetCameraManager()->SetRenderer(this);
	CManager::GetMeshManager()->SetRenderer(this);
	CManager::GetShaderManager()->SetRenderer(this);
	CManager::GetTextureManager()->SetRenderer(this);
	SetupVulkan(extensions, extensionsCount);
	SetupVulkanWindow(pWindow);
#if BL_EDITOR
	m_pInterface->Setup(pWindow, m_instance, m_physicalDevice, m_device, m_queueFamily, m_queue, m_pipelineCache, m_descriptorPool, m_pCallback, m_minImageCount);
	m_pInterface->UploadFont(m_device, m_queue);
	m_pInterface->GetValues(m_swapchain, m_swapchainExtent, m_presentMode, m_renderPass);
#endif

	const VkSemaphoreCreateInfo semaphoreCreateInfo{
		.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
	};
	const VkFenceCreateInfo fenceCreateInfo{
		.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
		.flags = VK_FENCE_CREATE_SIGNALED_BIT,
	};
	const VkCommandPoolCreateInfo commandPoolCreateInfo{
		.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
		.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
		.queueFamilyIndex = m_queueFamily,
	};

	CheckVkResult(vkCreateCommandPool(m_device, &commandPoolCreateInfo, m_pCallback, &m_commandPool));

	const VkCommandBufferAllocateInfo commandBufferAllocInfo{
		.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
		.commandPool = m_commandPool,
		.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
		.commandBufferCount = m_imageCount,
	};
	CheckVkResult(vkAllocateCommandBuffers(m_device, &commandBufferAllocInfo, m_commandBuffers));

	for (int i{}; i < std::size(m_imageAvailableSemaphores); ++i)
		CheckVkResult(vkCreateSemaphore(m_device, &semaphoreCreateInfo, m_pCallback, &m_imageAvailableSemaphores[i]));
	for (int i{}; i < std::size(m_renderFinishedSemaphores); ++i)
		CheckVkResult(vkCreateSemaphore(m_device, &semaphoreCreateInfo, m_pCallback, &m_renderFinishedSemaphores[i]));
	for (int i{}; i < std::size(m_frameFences); ++i)
		CheckVkResult(vkCreateFence(m_device, &fenceCreateInfo, m_pCallback, &m_frameFences[i]));

	//create image view
	{
		m_backBuffer.resize(m_imageCount);
		for (size_t i = 0; i < m_imageCount; i++)
		{
			const VkImageViewCreateInfo createInfo{
				.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
				.image = m_images[i],
				.viewType = VK_IMAGE_VIEW_TYPE_2D,
				.format = m_surfaceFormat.format,
				.components = {
					VK_COMPONENT_SWIZZLE_IDENTITY,
					VK_COMPONENT_SWIZZLE_IDENTITY,
					VK_COMPONENT_SWIZZLE_IDENTITY,
					VK_COMPONENT_SWIZZLE_IDENTITY
				},
				.subresourceRange = {
					.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
					.baseMipLevel = 0,
					.levelCount = 1,
					.baseArrayLayer = 0,
					.layerCount = 1,
				},
			};
			if (vkCreateImageView(m_device, &createInfo, m_pCallback, &m_backBuffer[i]) != VK_SUCCESS)
			{
				throw std::runtime_error("failed to create image views!");
			}
		}
	}
	//depth buffer
	{
		VkFormat depthFormat = FindDepthFormat();

		const VkImageCreateInfo imageInfo{
			.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
			.imageType = VK_IMAGE_TYPE_2D,
			.format = depthFormat,
			.extent = {m_swapchainExtent.width, m_swapchainExtent.height, 1},
			.mipLevels = 1,
			.arrayLayers = 1,
			.samples = VK_SAMPLE_COUNT_1_BIT,
			.tiling = VK_IMAGE_TILING_OPTIMAL,
			.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
			.sharingMode = VK_SHARING_MODE_EXCLUSIVE,
			.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
		};

		if (vkCreateImage(m_device, &imageInfo, m_pCallback, &m_depthImage) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create image!");
		}

		VkMemoryRequirements memRequirements;
		vkGetImageMemoryRequirements(m_device, m_depthImage, &memRequirements);

		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = FindMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

		if (vkAllocateMemory(m_device, &allocInfo, m_pCallback, &m_depthImageMemory) != VK_SUCCESS) {
			throw std::runtime_error("failed to allocate image memory!");
		}

		vkBindImageMemory(m_device, m_depthImage, m_depthImageMemory, 0);

		VkImageViewCreateInfo viewInfo{
			.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
			.image = m_depthImage,
			.viewType = VK_IMAGE_VIEW_TYPE_2D,
			.format = depthFormat,
			.subresourceRange{
				.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT,
				.baseMipLevel = 0,
				.levelCount = 1,
				.baseArrayLayer = 0,
				.layerCount = 1,
			},
		};

		if (vkCreateImageView(m_device, &viewInfo, m_pCallback, &m_depthImageView) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create texture image view!");
		}
	}
	//Create Frame buffer
	{
		m_framebuffers.resize(m_imageCount);

		for (size_t i = 0; i < m_imageCount; i++)
		{
			const VkImageView attachments[]{
				m_backBuffer[i],
				m_depthImageView
			};
			const VkFramebufferCreateInfo framebufferInfo{
				.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
				.renderPass = m_renderPass,
				.attachmentCount = static_cast<uint32_t>(std::size(attachments)),
				.pAttachments = attachments,
				.width = m_swapchainExtent.width,
				.height = m_swapchainExtent.height,
				.layers = 1,
			};
			if (vkCreateFramebuffer(m_device, &framebufferInfo, nullptr, &m_framebuffers[i]) != VK_SUCCESS)
			{
				throw std::runtime_error("failed to create framebuffer!");
			}
		}
	}
	// Create Descriptor Set Layout
	{
		std::vector<VkDescriptorSetLayoutBinding> layoutBindings{
			{
				.binding = 0,
				.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
				.descriptorCount = 1,
				.stageFlags = VK_SHADER_STAGE_VERTEX_BIT,
				.pImmutableSamplers = VK_NULL_HANDLE,
			}
		};
		/*
		 *todo: initialize all samplers
		 * foreach sampler
		 * get VkDescriptorSetLayoutBinding
		 */
		layoutBindings.push_back(m_textureSampler.GetDescriptorLayoutBinding());
		const VkDescriptorSetLayoutCreateInfo layoutInfo{
			.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
			.bindingCount = static_cast<uint32_t>(layoutBindings.size()),
			.pBindings = layoutBindings.data(),
		};
		if (vkCreateDescriptorSetLayout(m_device, &layoutInfo, m_pCallback, &m_descriptorSetLayout) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create descriptor set layout!");
		}
	}
	//todo: move this to "Game" and replace it with Initialize;
	CTexture* pTexture{ CManager::GetTextureManager()->AddTexture("../Resources/Textures/UVMS1_colorgrid.png") };
	m_textureSampler.Initialize(m_device, m_pCallback, pTexture->GetMipLevels());
	//CManager::GetMeshManager()->AddMesh("../Resources/Models/cube.ply");
	//CManager::GetMeshManager()->AddMesh("../Resources/Models/cube.obj");
	CManager::GetMeshManager()->AddMesh("../Resources/Models/sponza.obj");
	//Create Uniform Buffers
	{
		const VkDeviceSize bufferSize = sizeof(SUniformBufferObject);

		m_uniformBuffers.resize(m_imageCount);
		m_uniformBuffersMemory.resize(m_imageCount);

		for (size_t i = 0; i < m_imageCount; i++)
		{
			CreateBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, m_uniformBuffers[i], m_uniformBuffersMemory[i]);
		}
	}

	// Create Descriptor Pool
	{
		std::vector<VkDescriptorPoolSize> poolSize{
			{
				.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
				.descriptorCount = m_imageCount,
			}
		};
		//todo: foreach sampler, get VkDescriptorPoolSize
		poolSize.push_back(m_textureSampler.GetDescriptorPoolSize(m_imageCount));
		const VkDescriptorPoolCreateInfo poolInfo{
			.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
			.maxSets = m_imageCount,
			.poolSizeCount = static_cast<uint32_t>(poolSize.size()),
			.pPoolSizes = poolSize.data(),
		};
		if (vkCreateDescriptorPool(m_device, &poolInfo, m_pCallback, &m_descriptorPool) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create descriptor pool!");
		}
	}
	{
		std::vector<VkDescriptorSetLayout> layouts(m_imageCount, m_descriptorSetLayout);
		VkDescriptorSetAllocateInfo allocInfo{
			.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
			.descriptorPool = m_descriptorPool,
			.descriptorSetCount = m_imageCount,
			.pSetLayouts = layouts.data(),
		};
		m_descriptorSets.resize(m_imageCount);
		if (vkAllocateDescriptorSets(m_device, &allocInfo, m_descriptorSets.data()) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to allocate descriptor sets!");
		}

		for (size_t i = 0; i < m_imageCount; i++)
		{
			VkDescriptorBufferInfo bufferInfo{
				.buffer = m_uniformBuffers[i],
				.offset = 0,
				.range = sizeof(SUniformBufferObject),
			};

			VkDescriptorImageInfo imageInfo{};
			imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			imageInfo.imageView = pTexture->GetImageView();
			imageInfo.sampler = m_textureSampler.GetSampler();

			std::vector<VkWriteDescriptorSet> descriptorWrites{
				{
					.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
					.dstSet = m_descriptorSets[i],
					.dstBinding = 0,
					.dstArrayElement = 0,
					.descriptorCount = 1,
					.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
					.pBufferInfo = &bufferInfo,
				},
				{
					.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
					.dstSet = m_descriptorSets[i],
					.dstBinding = 1,
					.dstArrayElement = 0,
					.descriptorCount = 1,
					.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
					.pImageInfo = &imageInfo,
				}
			};
			//todo: get sampler info
			//foreach sampler
			//getwritedescriptorset
			//descriptorWrites.push_back( m_textureSampler.GetWriteDescriptorSet( m_descriptorSets[i] ) );
			vkUpdateDescriptorSets(m_device, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
		}
	}
	CManager::GetShaderManager()->AddShader();

}

void Balbino::CRenderer::SetupVulkan(const char** extensions, const uint32_t extensionsCount)
{
	// Create Vulkan Instance
	{
		VkInstanceCreateInfo createInfo{
			.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
			.enabledExtensionCount = extensionsCount,
			.ppEnabledExtensionNames = extensions,
		};
		VkResult err{ VK_ERROR_INITIALIZATION_FAILED };
#if defined(_DEBUG)
#if defined(BL_EDITOR)
		m_pInterface->SetupVulkan(extensions, extensionsCount, createInfo, m_instance, m_pCallback, m_debugReport);
#else

		// Enabling validation layers
		const char* layers[]{ "VK_LAYER_KHRONOS_validation" };
		createInfo.enabledLayerCount = 1;
		createInfo.ppEnabledLayerNames = layers;

		// Enable debug report extension (we need additional storage, so we duplicate the user array to add our new extension to it)
		const auto extensionsExt{ static_cast<const char**>(malloc(sizeof(const char*) * (extensionsCount + 1))) };
		memcpy(extensionsExt, extensions, extensionsCount * sizeof(const char*));
		extensionsExt[extensionsCount] = "VK_EXT_debug_report";
		createInfo.enabledExtensionCount = extensionsCount + 1;
		createInfo.ppEnabledExtensionNames = extensionsExt;

		err = vkCreateInstance(&createInfo, m_pCallback, &m_instance);
		CheckVkResult(err);
		free(extensionsExt);
		if (err == VK_SUCCESS)
		{
			vkpfn_CreateDebugReportCallbackEXT =
				reinterpret_cast<PFN_vkCreateDebugReportCallbackEXT>(vkGetInstanceProcAddr(m_instance, "vkCreateDebugReportCallbackEXT"));
			vkpfn_DestroyDebugReportCallbackEXT =
				reinterpret_cast<PFN_vkDestroyDebugReportCallbackEXT>(vkGetInstanceProcAddr(m_instance, "vkDestroyDebugReportCallbackEXT"));
			if (vkpfn_CreateDebugReportCallbackEXT && vkpfn_DestroyDebugReportCallbackEXT)
			{
				VkDebugReportCallbackCreateInfoEXT debugReportCi{};
				debugReportCi.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
				debugReportCi.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT | VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT;
				debugReportCi.pfnCallback = DebugReport;
				debugReportCi.pUserData = nullptr;
				vkpfn_CreateDebugReportCallbackEXT(m_instance, &debugReportCi, m_pCallback, &m_debugReport);
				CheckVkResult(err);
			}
		}
#endif
#endif
		if (err != VK_SUCCESS)
		{
			CheckVkResult(vkCreateInstance(&createInfo, m_pCallback, &m_instance));
		}
	}

	// Select GPU
	{
		uint32_t gpuCount;
		CheckVkResult(vkEnumeratePhysicalDevices(m_instance, &gpuCount, nullptr));
		VkPhysicalDevice* pGpus{ static_cast<VkPhysicalDevice*>(malloc(sizeof(VkPhysicalDevice) * gpuCount)) };
		CheckVkResult(vkEnumeratePhysicalDevices(m_instance, &gpuCount, pGpus));

		// If a number >1 of GPUs got reported, find discrete GPU if present, or use first one available. This covers
		// most common cases (multi-gpu/integrated+dedicated graphics). Handling more complicated setups (multiple
		// dedicated GPUs) is out of scope of this sample.
		int useGpu{ 0 };
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
		VkQueueFamilyProperties* queues{ static_cast<VkQueueFamilyProperties*>(malloc(sizeof(VkQueueFamilyProperties) * count)) };
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
		constexpr int deviceExtensionCount{ 1 };
		const char* deviceExtensions[]{ "VK_KHR_swapchain" };
		const float queuePriority[]{ 1.0f };
		const VkDeviceQueueCreateInfo queueInfo[1]{
			{
				.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
				.queueFamilyIndex = m_queueFamily,
				.queueCount = 1,
				.pQueuePriorities = queuePriority,
			}
		};
		const VkDeviceCreateInfo createInfo{
			.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
			.queueCreateInfoCount = static_cast<uint32_t>(std::size(queueInfo)),
			.pQueueCreateInfos = queueInfo,
			.enabledExtensionCount = deviceExtensionCount,
			.ppEnabledExtensionNames = deviceExtensions,
		};
		CheckVkResult(vkCreateDevice(m_physicalDevice, &createInfo, m_pCallback, &m_device));
		vkGetDeviceQueue(m_device, m_queueFamily, 0, &m_queue);
	}
}

void Balbino::CRenderer::SetupVulkanWindow(SDL_Window* pWindow)
{
	(void)pWindow;
	if (SDL_Vulkan_CreateSurface(pWindow, m_instance, &m_surface) == 0)
	{
		throw std::runtime_error("Failed to create Vulkan surface.\n");
	}

	// Create Framebuffers
	int w, h;
	SDL_GetWindowSize(pWindow, &w, &h);

#if BL_EDITOR
	m_pInterface->SetupVulkanWindow(surface, w, h, m_instance, m_physicalDevice, m_device, m_pCallback, m_queueFamily, m_minImageCount);
#else
	vkDeviceWaitIdle(m_device);
	//create swap chain (https://sopyer.github.io/Blog/post/minimal-vulkan-sample/)
	{
		VkBool32 supportsPresent = VK_FALSE;
		uint32_t presentModeCount{};
		//Use first available format
		uint32_t formatCount = 1;
		VkPresentModeKHR presentModes[MAX_PRESENT_MODE_COUNT]{};
		VkPresentModeKHR presentMode{ VK_PRESENT_MODE_FIFO_KHR }; // always supported.

		CheckVkResult(vkGetPhysicalDeviceSurfaceSupportKHR(m_physicalDevice, m_queueFamily, m_surface, &supportsPresent));
		CheckVkResult(vkGetPhysicalDeviceSurfaceFormatsKHR(m_physicalDevice, m_surface, &formatCount, nullptr)); // suppress validation layer
		CheckVkResult(vkGetPhysicalDeviceSurfaceFormatsKHR(m_physicalDevice, m_surface, &formatCount, &m_surfaceFormat));

		m_surfaceFormat.format = m_surfaceFormat.format == VK_FORMAT_UNDEFINED ? VK_FORMAT_R8G8B8A8_UNORM : m_surfaceFormat.format;
		CheckVkResult(vkGetPhysicalDeviceSurfacePresentModesKHR(m_physicalDevice, m_surface, &presentModeCount, nullptr));
		presentModeCount = presentModeCount > MAX_PRESENT_MODE_COUNT ? MAX_PRESENT_MODE_COUNT : presentModeCount;
		CheckVkResult(vkGetPhysicalDeviceSurfacePresentModesKHR(m_physicalDevice, m_surface, &presentModeCount, presentModes));

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

		CheckVkResult(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_physicalDevice, m_surface, &m_surfaceCapabilities));

		m_swapchainExtent = m_surfaceCapabilities.currentExtent;
		if (m_swapchainExtent.width == UINT32_MAX)
		{
			m_swapchainExtent.width = std::clamp(m_width, m_surfaceCapabilities.minImageExtent.width, m_surfaceCapabilities.maxImageExtent.width);
			m_swapchainExtent.height = std::clamp(m_height, m_surfaceCapabilities.minImageExtent.height, m_surfaceCapabilities.maxImageExtent.height);
		}

		const VkSwapchainCreateInfoKHR swapChainCreateInfo{
			.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
			.surface = m_surface,
			.minImageCount = m_minImageCount,
			.imageFormat = m_surfaceFormat.format,
			.imageColorSpace = m_surfaceFormat.colorSpace,
			.imageExtent = m_swapchainExtent,
			.imageArrayLayers = 1, // 2 for stereo
			.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT,
			.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE,
			.preTransform = m_surfaceCapabilities.currentTransform,
			.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
			.presentMode = VK_PRESENT_MODE_MAILBOX_KHR,
			.clipped = VK_TRUE,
		};
		CheckVkResult(vkCreateSwapchainKHR(m_device, &swapChainCreateInfo, m_pCallback, &m_swapchain));
		CheckVkResult(vkGetSwapchainImagesKHR(m_device, m_swapchain, &m_imageCount, nullptr));
		CheckVkResult(vkGetSwapchainImagesKHR(m_device, m_swapchain, &m_imageCount, m_images));
	}
	{
		const VkAttachmentDescription colorAttachment{
			.format = m_surfaceFormat.format,
			.samples = VK_SAMPLE_COUNT_1_BIT,
			.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
			.storeOp = VK_ATTACHMENT_STORE_OP_STORE,
			.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
			.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
			.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
			.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
		};
		VkAttachmentDescription depthAttachment{
			.format = FindDepthFormat(),
			.samples = VK_SAMPLE_COUNT_1_BIT,
			.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
			.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
			.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
			.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
			.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
			.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
		};
		const VkAttachmentReference colorAttachmentRef{
			.attachment = 0,
			.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
		};
		const VkAttachmentReference depthAttachmentRef{
			.attachment = 1,
			.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
		};

		const VkSubpassDescription subpass{
			.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
			.colorAttachmentCount = 1,
			.pColorAttachments = &colorAttachmentRef,
			.pDepthStencilAttachment = &depthAttachmentRef,
		};

		const VkSubpassDependency dependency
		{
			.srcSubpass = VK_SUBPASS_EXTERNAL,
			.dstSubpass = 0,
			.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
			.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
			.srcAccessMask = 0,
			.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,
		};
		const std::array<VkAttachmentDescription, 2> attachments{ colorAttachment, depthAttachment };
		const VkRenderPassCreateInfo renderPassInfo{
			.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
			.attachmentCount = static_cast<uint32_t>(attachments.size()),
			.pAttachments = attachments.data(),
			.subpassCount = 1,
			.pSubpasses = &subpass,
			.dependencyCount = 1,
			.pDependencies = &dependency,
		};
		if (vkCreateRenderPass(m_device, &renderPassInfo, m_pCallback, &m_renderPass) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create render pass!");
		}
	}
#endif
}

void Balbino::CRenderer::Cleanup()
{
	vkDeviceWaitIdle(m_device);
	//m_pMesh->Cleanup(m_device, m_pCallback);
	CManager::GetMeshManager()->Cleanup(m_device, m_pCallback);
	CManager::GetTextureManager()->Cleanup(m_device, m_pCallback);
	CManager::GetShaderManager()->Cleanup(m_device, m_pCallback);
	m_textureSampler.Cleanup(m_device, m_pCallback);

	//vkFreeDescriptorSets(m_device, m_descriptorPool, (uint32_t) m_descriptorSets.size(), m_descriptorSets.data());
	for (const VkFramebuffer framebuffer : m_framebuffers)
		vkDestroyFramebuffer(m_device, framebuffer, m_pCallback);
	for (const VkImageView backBuffer : m_backBuffer)
		vkDestroyImageView(m_device, backBuffer, m_pCallback);
	for (const VkBuffer buffer : m_uniformBuffers)
		vkDestroyBuffer(m_device, buffer, m_pCallback);
	for (const VkDeviceMemory uniformBuffersMemory : m_uniformBuffersMemory)
		vkFreeMemory(m_device, uniformBuffersMemory, m_pCallback);

	vkDestroyDescriptorSetLayout(m_device, m_descriptorSetLayout, m_pCallback);
	vkDestroyDescriptorPool(m_device, m_descriptorPool, m_pCallback);

	for (const VkFence& fence : m_frameFences)
		vkDestroyFence(m_device, fence, m_pCallback);
	for (const VkSemaphore& semaphore : m_renderFinishedSemaphores)
		vkDestroySemaphore(m_device, semaphore, m_pCallback);
	for (const VkSemaphore& semaphore : m_imageAvailableSemaphores)
		vkDestroySemaphore(m_device, semaphore, m_pCallback);
	vkFreeCommandBuffers(m_device, m_commandPool, static_cast<uint32_t>(std::size(m_commandBuffers)), m_commandBuffers);
	vkDestroyCommandPool(m_device, m_commandPool, m_pCallback);

	vkDestroyImageView(m_device, m_depthImageView, m_pCallback);
	vkDestroyImage(m_device, m_depthImage, m_pCallback);
	vkFreeMemory(m_device, m_depthImageMemory, m_pCallback);
#if BL_EDITOR
	m_pInterface->Cleanup();
#endif
	CleanupVulkanWindow();
	CleanupVulkan();
}

float Balbino::CRenderer::GetAspectRatio() const
{
	return static_cast<float>(m_swapchainExtent.width) / static_cast<float>(m_swapchainExtent.height);;
}

bool Balbino::CRenderer::GetDevice(VkDevice& device) const
{
	if (m_device != VK_NULL_HANDLE)
	{
		device = m_device;
		return true;
	}
	return false;
}

bool Balbino::CRenderer::GetAllocationCallbacks(VkAllocationCallbacks*& pAlloc) const
{
	if (m_pCallback != VK_NULL_HANDLE)
	{
		pAlloc = m_pCallback;
		return true;
	}
	return false;
}

bool Balbino::CRenderer::GetPhysicalDevice(VkPhysicalDevice& physicalDevice) const
{
	if (m_physicalDevice != VK_NULL_HANDLE)
	{
		physicalDevice = m_physicalDevice;
		return true;
	}
	return false;
}

bool Balbino::CRenderer::GetQueue(VkQueue& queue) const
{
	if (m_queue != VK_NULL_HANDLE)
	{
		queue = m_queue;
		return true;
	}
	return false;
}

bool Balbino::CRenderer::GetCommandPool(VkCommandPool& commandPool) const
{
	if (m_commandPool != VK_NULL_HANDLE)
	{
		commandPool = m_commandPool;
		return true;
	}
	return false;
}

void Balbino::CRenderer::CleanupVulkan() const
{
#if defined(BL_EDITOR) && defined(_DEBUG)
	m_pInterface->CleanupVulkan(m_instance, m_pCallback, m_debugReport);
#endif // IMGUI_VULKAN_DEBUG_REPORT
	vkDestroyDevice(m_device, m_pCallback);
	if (vkpfn_DestroyDebugReportCallbackEXT && m_debugReport)
	{
		vkpfn_DestroyDebugReportCallbackEXT(m_instance, m_debugReport, m_pCallback);
	}
	vkDestroySurfaceKHR(m_instance, m_surface, m_pCallback);
	vkDestroyInstance(m_instance, m_pCallback);
}

void Balbino::CRenderer::CleanupVulkanWindow() const
{
#if BL_EDITOR
	if (m_pInterface)
		m_pInterface->CleanupVulkanWindow(m_instance, m_pCallback, m_device);
#endif

	vkDestroyRenderPass(m_device, m_renderPass, m_pCallback);
#ifndef BL_EDITOR
	vkDestroySwapchainKHR(m_device, m_swapchain, m_pCallback);
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
	(void)pWindow;
#if BL_EDITOR
	m_pInterface->ResizeSwapChain(pWindow, m_instance, m_physicalDevice, m_device, m_queueFamily, m_pCallback, m_minImageCount);
	m_pInterface->DrawStart(pWindow);

	//todo: Render stuff

	m_pInterface->Draw();
	m_pInterface->Render(m_device, m_queue);
#else
	CCameraManager* pCameraManager{ CManager::GetCameraManager() };
	const std::vector<CCamera>& allCameras{ pCameraManager->GetCameras() };
	if (allCameras.size())
	{
		const uint32_t index{ (m_frameIndex++) % FRAME_COUNT };
		CheckVkResult(vkWaitForFences(m_device, 1, &m_frameFences[index], VK_TRUE, UINT64_MAX));
		CheckVkResult(vkResetFences(m_device, 1, &m_frameFences[index]));

		uint32_t imageIndex;
		vkAcquireNextImageKHR(m_device, m_swapchain, UINT64_MAX, m_imageAvailableSemaphores[index], VK_NULL_HANDLE, &imageIndex);

		const VkCommandBufferBeginInfo beginInfo{
			.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
			.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT
		};
		const VkClearValue clearColor[] = {
			VkClearValue{.color = { 0.165f, 0.306f, 0.243f, 1.0f }, },
			VkClearValue{.depthStencil = {1.0f,0}}
		};
		const VkRenderPassBeginInfo renderPassInfo{
			.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
			.renderPass = m_renderPass,
			.framebuffer = m_framebuffers[index],
			.renderArea = { { 0, 0 }, m_swapchainExtent },
			.clearValueCount = 2,
			.pClearValues = clearColor,
		};
		vkBeginCommandBuffer(m_commandBuffers[index], &beginInfo);

		vkCmdBeginRenderPass(m_commandBuffers[index], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
		vkCmdBindDescriptorSets(m_commandBuffers[index], VK_PIPELINE_BIND_POINT_GRAPHICS, CManager::GetShaderManager()->GetShaders()[0].GetPipelineLayout(), 0, 1, &m_descriptorSets[index], 0, nullptr);

		const SUniformBufferObject ubo{
			.model = glm::mat4(1),
			.view = allCameras[0].GetViewBuffer(),
			.projection = allCameras[0].GetProjection(),
			//.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)),
			//.projection = glm::perspective(glm::radians(45.0f), m_swapchainExtent.width / static_cast<float>(m_swapchainExtent.height), 0.01f, 1000.0f ),
		};

		void* data;
		vkMapMemory(m_device, m_uniformBuffersMemory[index], 0, sizeof(ubo), 0, &data);
		memcpy(data, &ubo, sizeof(ubo));
		vkUnmapMemory(m_device, m_uniformBuffersMemory[index]);

		//todo: draw things
		CManager::GetMeshManager()->Draw(m_commandBuffers[index], &m_descriptorSets[index]);

		vkCmdEndRenderPass(m_commandBuffers[index]);
		vkEndCommandBuffer(m_commandBuffers[index]);

		const VkPipelineStageFlags pipelineStageFlags[]{ VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		const VkSubmitInfo submitInfo
		{
			.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
			.waitSemaphoreCount = 1,
			.pWaitSemaphores = &m_imageAvailableSemaphores[index],
			.pWaitDstStageMask = pipelineStageFlags,
			.commandBufferCount = 1,
			.pCommandBuffers = &m_commandBuffers[index],
			.signalSemaphoreCount = 1,
			.pSignalSemaphores = &m_renderFinishedSemaphores[index],
		};
		const VkPresentInfoKHR presentInfo
		{
			.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
			.waitSemaphoreCount = 1,
			.pWaitSemaphores = &m_renderFinishedSemaphores[index],
			.swapchainCount = 1,
			.pSwapchains = &m_swapchain,
			.pImageIndices = &imageIndex,
		};
		CheckVkResult(vkQueueSubmit(m_queue, 1, &submitInfo, m_frameFences[index]));
		CheckVkResult(vkQueuePresentKHR(m_queue, &presentInfo));
	}
#endif
}

void Balbino::CRenderer::CreateBuffer(const VkDeviceSize size, const VkBufferUsageFlags usage, const VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory)
{
	VkBufferCreateInfo bufferInfo{};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size = size;
	bufferInfo.usage = usage;
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	if (vkCreateBuffer(m_device, &bufferInfo, m_pCallback, &buffer) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create buffer!");
	}

	VkMemoryRequirements memRequirements;
	vkGetBufferMemoryRequirements(m_device, buffer, &memRequirements);

	VkMemoryAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = FindMemoryType(memRequirements.memoryTypeBits, properties);

	if (vkAllocateMemory(m_device, &allocInfo, m_pCallback, &bufferMemory) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to allocate buffer memory!");
	}

	vkBindBufferMemory(m_device, buffer, bufferMemory, 0);
}

uint32_t Balbino::CRenderer::FindMemoryType(uint32_t typeFilter, const VkMemoryPropertyFlags properties) const
{
	VkPhysicalDeviceMemoryProperties memProperties;
	vkGetPhysicalDeviceMemoryProperties(m_physicalDevice, &memProperties);

	for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
	{
		if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
		{
			return i;
		}
	}

	throw std::runtime_error("failed to find suitable memory type!");
}

bool Balbino::CRenderer::GetSwatChainExtend(VkExtent2D& swapchainExtend) const
{
	if (m_swapchainExtent.width > 0 || m_swapchainExtent.height > 0)
	{
		swapchainExtend = m_swapchainExtent;
		return true;
	}
	return false;
}

bool Balbino::CRenderer::GetRenderPass(VkRenderPass& renderPass) const
{
	if (m_renderPass != VK_NULL_HANDLE)
	{
		renderPass = m_renderPass;
		return true;
	}
	return false;
}

bool Balbino::CRenderer::GetDescriptorSetLayout(VkDescriptorSetLayout& descriptorSetLayout) const
{
	if (m_descriptorSetLayout != VK_NULL_HANDLE)
	{
		descriptorSetLayout = m_descriptorSetLayout;
		return true;
	}
	return false;
}

VkFormat Balbino::CRenderer::FindSupportedFormat(const std::vector<VkFormat>& candidates, const VkImageTiling tiling, const VkFormatFeatureFlags features)
{
	for (const VkFormat format : candidates)
	{
		VkFormatProperties props;
		vkGetPhysicalDeviceFormatProperties(m_physicalDevice, format, &props);

		if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features)
		{
			return format;
		}
		if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features)
		{
			return format;
		}
	}

	throw std::runtime_error("failed to find supported format!");
}

VkFormat Balbino::CRenderer::FindDepthFormat()
{
	return FindSupportedFormat(
		{ VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
		VK_IMAGE_TILING_OPTIMAL,
		VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
	);
}

#if BL_EDITOR
void Balbino::CRenderer::SetInterface(CInterface* const pInterface)
{
	m_pInterface = pInterface;
}
#endif
