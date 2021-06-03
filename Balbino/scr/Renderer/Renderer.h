#pragma once
#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_core.h>

struct SDL_Window;

class Renderer
{
public:
	Renderer();
	~Renderer() = default;

	Renderer( const Renderer& ) = delete;
	Renderer( Renderer&& ) = delete;
	Renderer& operator=( const Renderer& ) = delete;
	Renderer& operator=( Renderer&& ) = delete;

private:
	vk::AllocationCallbacks* m_pAllocator;
	vk::Instance m_Instance;
	vk::PhysicalDevice m_PhysicalDevice;
	vk::Device m_Device;
	uint32_t m_QueueFamily;
	vk::Queue m_Queue;
	vk::DebugReportCallbackEXT m_DebugReport;
	vk::PipelineCache m_PipelineCache;
	vk::DescriptorPool m_DescriptorPool;

	uint32_t m_MinImageCount;
	bool m_SwapChainRebuild;

	void SetupVulkan( const char** extensions, uint32_t extensionsCount );
	void SetupVulkanWindow(SDL_Window* pWindow, VkSurfaceKHR surface, int width, int height );
	void CleanupVulkan();
	void CleanupVulkanWindow();

	void Draw();



	
	static void CheckVkResult( const vk::Result err )
	{
		if ( err == vk::Result::eSuccess )
			return;
		fprintf( stderr, "[vulkan] Error: VkResult = %d\n", err );
		if ( err < vk::Result::eSuccess )
			abort();
	}
	static void VKCheckVkResult( VkResult err )
	{
		if ( err == 0 )
			return;
		fprintf( stderr, "[vulkan] Error: VkResult = %d\n", err );
		if ( err < 0 )
			abort();
	}
};
