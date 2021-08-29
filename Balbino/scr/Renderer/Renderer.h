#pragma once
#include <vulkan/vulkan_core.h>
#define FRAME_COUNT 2
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
		void SetupVulkanWindow( SDL_Window* pWindow ) const;
		void Setup( SDL_Window* pWindow );

		void Cleanup() const;

		void Draw( const SDL_Window* pWindow );

#if BL_EDITOR
		void SetInterface( Interface* const pInterface );
#endif

	private:
		VkAllocationCallbacks* m_pAllocator;
		VkCommandPool m_CommandPool;
		VkDebugReportCallbackEXT m_DebugReport;
		VkDescriptorPool m_DescriptorPool;
		VkDevice m_Device;
		VkInstance m_Instance;
		VkPhysicalDevice m_PhysicalDevice;
		VkPipelineCache m_PipelineCache;
		VkQueue m_Queue;

		VkCommandBuffer m_CommandBuffers[FRAME_COUNT];
		VkFence m_FrameFences[FRAME_COUNT]; // Create with VK_FENCE_CREATE_SIGNALED_BIT.
		VkSemaphore m_ImageAvailableSemaphores[FRAME_COUNT];
		VkSemaphore m_RenderFinishedSemaphores[FRAME_COUNT];
		
		uint32_t m_QueueFamily;
		uint32_t m_MinImageCount;
		uint32_t m_FrameIndex;
		bool m_SwapChainRebuild;

#if BL_EDITOR
		Interface* m_pInterface;
#endif

		void CleanupVulkan() const;
		void CleanupVulkanWindow() const;

		static void CheckVkResult( VkResult err );
	};
}
