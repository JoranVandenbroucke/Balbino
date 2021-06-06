#pragma once

#include "imgui.h"
#include "backends/imgui_impl_sdl.h"
#include "backends/imgui_impl_vulkan.h"

#include <vulkan/vulkan.hpp>

class GameView;
class MainScreen;
struct SDL_Window;

#ifdef _DEBUG
#define IMGUI_VULKAN_DEBUG_REPORT
#endif

namespace Balbino
{
	class Interface final
	{
	public:
		Interface();
		~Interface() = default;
		Interface( const Interface& ) = delete;
		Interface( Interface&& ) = delete;
		Interface& operator=( const Interface& ) = delete;
		Interface& operator=( Interface&& ) = delete;

		void SetupVulkan( const char** extensions, uint32_t extensionsCount, vk::InstanceCreateInfo& createInfo, vk::Instance& instance, vk::AllocationCallbacks* pCallback, vk::DebugReportCallbackEXT& debugReport ) const;
		void SetupVulkanWindow( const vk::SurfaceKHR& surface, int width, int height, const vk::Instance& instance, const vk::PhysicalDevice& physicalDevice, const vk::Device& device, const vk::AllocationCallbacks* pCallback, uint32_t queueFamily, uint32_t minImageCount );

		void Setup( SDL_Window* pWindow, const vk::Instance& instance, const vk::PhysicalDevice& physicalDevice, const vk::Device& device, uint32_t queueFamily, const vk::Queue& queue, const vk::PipelineCache& pipelineCache, const vk::DescriptorPool& descriptorPool, const vk::AllocationCallbacks* pCallback, uint32_t minImageCount ) const;
		void UploadFont( const vk::Device& device, const vk::Queue& queue ) const;

		void HandleEvents( SDL_Event e );
		void ResizeSwapChain( SDL_Window* pWindow, const vk::Instance& instance, const vk::PhysicalDevice& physicalDevice, const vk::Device& device, const uint32_t queueFamily, const vk::AllocationCallbacks* pCallback, const uint32_t minImageCount );

		void DrawStart( SDL_Window* pWindow );
		void Draw();
		void Render( const vk::Device& device, const vk::Queue& queue );

		void Cleanup();
		void CleanupVulkan( const vk::Instance& instance, const vk::AllocationCallbacks* pCallback, const vk::DebugReportCallbackEXT& debugReport )const;
		void CleanupVulkanWindow( const vk::Instance& instance, const vk::AllocationCallbacks* pCallback, const vk::Device& device );
	private:
		MainScreen* m_pMain;
		GameView* m_pGameView;
		// Our state
		ImGui_ImplVulkanH_Window m_MainWindowData;
		ImVec4 m_ClearColor;
		bool m_SwapChainRebuild;

		bool m_ShowDemoWindow;
		bool m_ShowAnotherWindow;

		void SetImGuiStyle() const;
		void FrameRender( ImDrawData* drawData, const vk::Device& device, const vk::Queue& queue );
		void FramePresent( const vk::Queue& queue );

		static void CheckVkResult( const vk::Result err );
		static void VkCheckVkResult( VkResult err );
	};
}
