#pragma once

#include "imgui.h"
#include "backends/imgui_impl_sdl.h"
#include "backends/imgui_impl_vulkan.h"

#include <cstdio>

#include <SDL.h>
#include <SDL_vulkan.h>
#include <vulkan/vulkan.hpp>

#ifdef _DEBUG
#define IMGUI_VULKAN_DEBUG_REPORT
#endif

#ifdef IMGUI_VULKAN_DEBUG_REPORT
static VKAPI_ATTR VkBool32 VKAPI_CALL DebugReport( VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objectType, uint64_t object, size_t location, int32_t messageCode, const char* pLayerPrefix, const char* pMessage, void* pUserData )
{
	( void )flags;
	( void )object;
	( void )location;
	( void )messageCode;
	( void )pUserData;
	( void )pLayerPrefix; // Unused arguments
	fprintf( stderr, "[vulkan] Debug report from ObjectType: %i\nMessage: %s\n\n", objectType, pMessage );
	return VK_FALSE;
}
#else
static VKAPI_ATTR VkBool32 VKAPI_CALL DebugReport( VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objectType, uint64_t object, size_t location, int32_t messageCode, const char* pLayerPrefix, const char* pMessage, void* pUserData )
{
	(void) flags;
	(void) objectType;
	(void) object;
	(void) location;
	(void) messageCode;
	(void) pUserData;
	(void) pMessage;
	(void) pLayerPrefix; // Unused arguments
	return 0;
}
#endif // IMGUI_VULKAN_DEBUG_REPORT

class Interface
{
public:
	Interface();
	~Interface() = default;

	void SetupVulkan( const char** extensions, uint32_t extensionsCount, vk::InstanceCreateInfo& createInfo, vk::Instance& instance, const vk::AllocationCallbacks* pAllocator, vk::DebugReportCallbackEXT& debugReport ) const;
	void SetupVulkanWindow( const vk::SurfaceKHR& surface, int width, int height, const vk::Instance& instance, const vk::PhysicalDevice& physicalDevice, const vk::Device& device, const vk::AllocationCallbacks* pCallback, uint32_t queueFamily, uint32_t minImageCount );
	void CleanupVulkan( const vk::Instance& instance, const vk::AllocationCallbacks* pCallback, const vk::DebugReportCallbackEXT& debugReport )const;
	void CleanupVulkanWindow( const vk::Instance& instance, const vk::AllocationCallbacks* pCallback, const vk::Device& device );

	void Setup( SDL_Window* pWindow, const vk::Instance& instance, const vk::PhysicalDevice& physicalDevice, const vk::Device& device, uint32_t queueFamily, const vk::Queue& queue, const vk::PipelineCache& pipelineCache, const vk::DescriptorPool& descriptorPool, const vk::AllocationCallbacks* pCallback, uint32_t minImageCount ) const;
	void UploadFont( const vk::Device& device, const vk::Queue& queue ) const;

	void HandleEvents( SDL_Event e );
	void ResizeSwapChain( SDL_Window* pWindow, const vk::Instance& instance, const vk::PhysicalDevice& physicalDevice, const vk::Device& device, const uint32_t queueFamily, const vk::AllocationCallbacks* pCallback, const uint32_t minImageCount );

	void DrawStart( SDL_Window* pWindow );
	void Draw();
	void Render( const vk::Device& device, const vk::Queue& queue );


private:
	// Our state
	ImGui_ImplVulkanH_Window m_MainWindowData;
	ImVec4 m_ClearColor;
	bool m_SwapChainRebuild;

	bool m_ShowDemoWindow;
	bool m_ShowAnotherWindow;

	void FrameRender( ImDrawData* drawData, const vk::Device& device, const vk::Queue& queue );
	void FramePresent( const vk::Queue& queue );
	
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
