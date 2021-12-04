#pragma once

#include "imgui.h"
#include "backends/imgui_impl_sdl.h"
#include "backends/imgui_impl_vulkan.h"

#include <vulkan/vulkan.hpp>

class CGameView;
class CMainScreen;
struct SDL_Window;

#ifdef _DEBUG
#define IMGUI_VULKAN_DEBUG_REPORT
#endif

namespace Balbino
{
	class CInterface final
	{
	public:
		CInterface();
		~CInterface() = default;
		void GetValues(VkSwapchainKHR& swapchain, VkExtent2D& swapchainExtent, VkPresentModeKHR& presentMode,
		               VkRenderPass& renderPass) const;
		CInterface(const CInterface&) = delete;
		CInterface(CInterface&&) = delete;
		CInterface& operator=(const CInterface&) = delete;
		CInterface& operator=(CInterface&&) = delete;

		void SetupVulkan(const char** extensions, uint32_t extensionsCount, VkInstanceCreateInfo& createInfo,
		                 VkInstance& instance, const VkAllocationCallbacks* pCallback,
		                 VkDebugReportCallbackEXT& debugReport);
		void SetupVulkanWindow(const VkSurfaceKHR& surface, int width, int height, const VkInstance& instance,
		                       const VkPhysicalDevice& physicalDevice, const VkDevice& device,
		                       const VkAllocationCallbacks* pCallback, uint32_t queueFamily, uint32_t minImageCount);

		void Setup(SDL_Window* pWindow, const VkInstance& instance, const VkPhysicalDevice& physicalDevice,
		           const VkDevice& device, uint32_t queueFamily, const VkQueue& queue,
		           const VkPipelineCache& pipelineCache, const VkDescriptorPool& descriptorPool,
		           const VkAllocationCallbacks* pCallback, uint32_t minImageCount) const;
		void UploadFont(const VkDevice& device, const VkQueue& queue) const;

		void HandleEvents(SDL_Event e);
		void ResizeSwapChain(SDL_Window* pWindow, const VkInstance& instance, const VkPhysicalDevice& physicalDevice,
		                     const VkDevice& device, uint32_t queueFamily, const VkAllocationCallbacks* pCallback,
		                     uint32_t minImageCount);

		void DrawStart(SDL_Window* pWindow);
		void Draw();
		void Render(const VkDevice& device, const VkQueue& queue);

		void Cleanup() const;
		void CleanupVulkan(const VkInstance& instance, const VkAllocationCallbacks* pCallback,
		                   const VkDebugReportCallbackEXT& debugReport) const;
		void CleanupVulkanWindow(const VkInstance& instance, const VkAllocationCallbacks* pCallback,
		                         const VkDevice& device);
	private:
		CMainScreen* m_pMain;
		CGameView* m_pGameView;
		// Our state
		ImGui_ImplVulkanH_Window m_MainWindowData;
		ImVec4 m_ClearColor;
		bool m_SwapChainRebuild;

		bool m_showDemoWindow;
		bool m_showAnotherWindow;

		PFN_vkCreateDebugReportCallbackEXT vkpfn_CreateDebugReportCallbackEXT = nullptr;
		PFN_vkDestroyDebugReportCallbackEXT vkpfn_DestroyDebugReportCallbackEXT = nullptr;

		static void SetImGuiStyle();
		void FrameRender(ImDrawData* drawData, const VkDevice& device, const VkQueue& queue);
		void FramePresent(const VkQueue& queue);

		static void CheckVkResult(VkResult err);
	};
}
