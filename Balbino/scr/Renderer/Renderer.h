#pragma once
#include <vulkan/vulkan_core.h>

#include "Mesh.h"
#define FRAME_COUNT 2
#define MAX_PRESENT_MODE_COUNT 3
#define PRESENT_MODE_MAILBOX_IMAGE_COUNT 3
#define PRESENT_MODE_DEFAULT_IMAGE_COUNT 3
struct SDL_Window;

namespace Balbino
{
	class CInterface;

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
		void Setup( SDL_Window* pWindow );

		void Cleanup() const;

		void Draw( SDL_Window* pWindow );

#if BL_EDITOR
		void SetInterface( CInterface* const pInterface );
#endif

	private:
		CMesh m_mesh;

		VkAllocationCallbacks* m_pAllocator;
		VkCommandPool m_commandPool;
		VkDebugReportCallbackEXT m_debugReport;
		VkDescriptorPool m_descriptorPool;
		VkDevice m_device;
		VkInstance m_instance;
		VkPhysicalDevice m_physicalDevice;
		VkPipelineCache m_pipelineCache;
		VkQueue m_queue;

		VkCommandBuffer m_commandBuffers[FRAME_COUNT];
		VkFence m_frameFences[FRAME_COUNT]; // Create with VK_FENCE_CREATE_SIGNALED_BIT.
		VkSemaphore m_imageAvailableSemaphores[FRAME_COUNT];
		VkSemaphore m_renderFinishedSemaphores[FRAME_COUNT];
		
		uint32_t m_queueFamily;
		uint32_t m_minImageCount;
		uint32_t m_frameIndex;


		uint32_t m_width;
		uint32_t m_height;
		uint32_t m_imageCount;
		bool m_swapChainRebuild;

#if BL_EDITOR
		CInterface* m_pInterface;
#else
		VkSwapchainKHR m_swapchain;
		VkSurfaceFormatKHR m_surfaceFormat;
		VkPresentModeKHR m_presentMode;
		VkExtent2D  m_swapchainExtent;
		VkSurfaceCapabilitiesKHR m_surfaceCapabilities;
		VkRenderPass m_renderPass;
		VkImage m_images[MAX_PRESENT_MODE_COUNT];
#endif

		void CleanupVulkan() const;
		void CleanupVulkanWindow() const;

		static void CheckVkResult( VkResult err );
	};
}
