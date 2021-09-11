#pragma once
#include <vulkan/vulkan_core.h>

#include "Mesh.h"
#include "Shader.h"
#include "TextureSampler.h"

#define FRAME_COUNT 2
#define MAX_PRESENT_MODE_COUNT 4
#define PRESENT_MODE_MAILBOX_IMAGE_COUNT 3
#define PRESENT_MODE_DEFAULT_IMAGE_COUNT 2
struct SDL_Window;

namespace Balbino
{
	class CInterface;
	class CTexture;

	class CRenderer
	{
	public:
		CRenderer();
		~CRenderer() = default;

		CRenderer( const CRenderer& ) = delete;
		CRenderer( CRenderer&& ) = delete;
		CRenderer& operator=( const CRenderer& ) = delete;
		CRenderer& operator=( CRenderer&& ) = delete;


		void SetupVulkan( const char** extensions, uint32_t extensionsCount );
		void SetupVulkanWindow( SDL_Window* pWindow );
		void Setup( SDL_Window* pWindow, const char** extensions, const uint32_t extensionsCount);

		void Cleanup();

		void Draw( SDL_Window* pWindow, float dt);
		void CreateBuffer( VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory );
		uint32_t FindMemoryType( uint32_t typeFilter, VkMemoryPropertyFlags properties ) const;

#if BL_EDITOR
		void SetInterface( CInterface* const pInterface );
#endif
#ifdef _DEBUG
		PFN_vkCreateDebugReportCallbackEXT vkpfn_CreateDebugReportCallbackEXT = nullptr;
		PFN_vkDestroyDebugReportCallbackEXT vkpfn_DestroyDebugReportCallbackEXT = nullptr;
#endif

		bool GetDevice(VkDevice& device) const;
		bool GetAllocationCallbacks(VkAllocationCallbacks*& pAlloc) const;
		bool GetPhysicalDevice(VkPhysicalDevice& physicalDevice) const;
		bool GetQueue(VkQueue& queue) const;
		bool GetCommandPool(VkCommandPool& commandPool) const;
		bool GetSwatChainExtend( VkExtent2D& swapchainExtend)const;
		bool GetRenderPass( VkRenderPass& renderPass)const;
		bool GetDescriptorSetLayout( VkDescriptorSetLayout& descriptorSetLayout)const;

	private:
		CMesh m_mesh;
		CShader* m_pShader;
		CTextureSampler m_textureSampler;
		CTexture* m_pTexture;

		VkSurfaceKHR m_surface;
		VkInstance m_instance;
		VkAllocationCallbacks* m_pCallback;
		VkDebugReportCallbackEXT m_debugReport;
		VkPhysicalDevice m_physicalDevice;
		VkCommandPool m_commandPool;
		VkDevice m_device;
		VkQueue m_queue;
		VkDescriptorPool m_descriptorPool;
		VkPipelineCache m_pipelineCache;	//todo: this is for materials

		VkCommandBuffer m_commandBuffers[FRAME_COUNT];
		VkFence m_frameFences[FRAME_COUNT]; // Create with VK_FENCE_CREATE_SIGNALED_BIT.
		VkSemaphore m_imageAvailableSemaphores[FRAME_COUNT];
		VkSemaphore m_renderFinishedSemaphores[FRAME_COUNT];

		VkSwapchainKHR m_swapchain;
		VkRenderPass m_renderPass;
		std::vector<VkImageView> m_backBuffer;
		std::vector<VkFramebuffer> m_framebuffers;
		VkSurfaceFormatKHR m_surfaceFormat;
		VkPresentModeKHR m_presentMode;
		VkExtent2D  m_swapchainExtent;
		VkSurfaceCapabilitiesKHR m_surfaceCapabilities;
		VkImage m_images[MAX_PRESENT_MODE_COUNT];

		VkDescriptorSetLayout m_descriptorSetLayout;
		std::vector<VkBuffer> m_uniformBuffers;
		std::vector<VkDeviceMemory> m_uniformBuffersMemory;
		std::vector<VkDescriptorSet> m_descriptorSets;

		uint32_t m_queueFamily;
		uint32_t m_minImageCount;
		uint32_t m_frameIndex;


		uint32_t m_width;
		uint32_t m_height;
		uint32_t m_imageCount;
		bool m_swapChainRebuild;

#if BL_EDITOR
		CInterface* m_pInterface;
#endif

		void CleanupVulkan() const;
		void CleanupVulkanWindow() const;

		static void CheckVkResult( VkResult err );
	};
}
