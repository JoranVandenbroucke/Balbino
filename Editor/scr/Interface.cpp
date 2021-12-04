#include "pch.h"
#include "Interface.h"

#include <SDL.h>
#include <vulkan/vulkan.hpp>

#include "Windows/GameView.h"
#include "Windows/MainScreen.h"


#ifdef IMGUI_VULKAN_DEBUG_REPORT
static VKAPI_ATTR VkBool32 VKAPI_CALL DebugReport(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objectType,
                                                  uint64_t object, size_t location, int32_t messageCode,
                                                  const char* pLayerPrefix, const char* pMessage, void* pUserData)
{
	(void)flags;
	(void)object;
	(void)location;
	(void)messageCode;
	(void)pUserData;
	(void)pLayerPrefix; // Unused arguments
	fprintf(stderr, "[vulkan] Debug report from ObjectType: %i\nMessage: %s\n\n", objectType, pMessage);
	return VK_FALSE;
}
#endif // IMGUI_VULKAN_DEBUG_REPORT


Balbino::CInterface::CInterface()
	: m_pMain{DBG_NEW CMainScreen{}}
	, m_pGameView{DBG_NEW CGameView{}}
	, m_ClearColor{0.645f, 0.98f, 0.123f, 1.00f}
	, m_SwapChainRebuild{false}
	, m_showDemoWindow{true}
	, m_showAnotherWindow{false}
{
}

void Balbino::CInterface::GetValues(VkSwapchainKHR& swapchain, VkExtent2D& swapchainExtent,
                                    VkPresentModeKHR& presentMode, VkRenderPass& renderPass) const
{
	swapchain = m_MainWindowData.Swapchain;
	swapchainExtent.width = m_MainWindowData.Width;
	swapchainExtent.height = m_MainWindowData.Height;
	presentMode = m_MainWindowData.PresentMode;
	renderPass = m_MainWindowData.RenderPass;
}

void Balbino::CInterface::SetupVulkan(const char** extensions, const uint32_t extensionsCount,
                                      VkInstanceCreateInfo& createInfo, VkInstance& instance,
                                      const VkAllocationCallbacks* pCallback, VkDebugReportCallbackEXT& debugReport)
{
	(void)extensions;
	(void)extensionsCount;
	(void)createInfo;
	(void)instance;
	(void)pCallback;
	(void)debugReport;
	VkResult err = VK_ERROR_INITIALIZATION_FAILED;
#ifdef IMGUI_VULKAN_DEBUG_REPORT
	// Enabling validation layers
	const char* layers[]{"VK_LAYER_KHRONOS_validation"};
	createInfo.enabledLayerCount = 1;
	createInfo.ppEnabledLayerNames = layers;

	// Enable debug report extension (we need additional storage, so we duplicate the user array to add our new extension to it)
	const auto extensionsExt{static_cast<const char**>(malloc(sizeof(const char*) * (extensionsCount + 1)))};
	memcpy(extensionsExt, extensions, extensionsCount * sizeof(const char*));
	extensionsExt[extensionsCount] = "VK_EXT_debug_report";
	createInfo.enabledExtensionCount = extensionsCount + 1;
	createInfo.ppEnabledExtensionNames = extensionsExt;

	err = vkCreateInstance(&createInfo, pCallback, &instance);
	CheckVkResult(err);
	free(extensionsExt);
	if (err == VK_SUCCESS)
	{
		vkpfn_CreateDebugReportCallbackEXT =
			reinterpret_cast<PFN_vkCreateDebugReportCallbackEXT>(vkGetInstanceProcAddr(
				instance, "vkCreateDebugReportCallbackEXT"));
		vkpfn_DestroyDebugReportCallbackEXT =
			reinterpret_cast<PFN_vkDestroyDebugReportCallbackEXT>(vkGetInstanceProcAddr(
				instance, "vkDestroyDebugReportCallbackEXT"));
		if (vkpfn_CreateDebugReportCallbackEXT && vkpfn_DestroyDebugReportCallbackEXT)
		{
			VkDebugReportCallbackCreateInfoEXT debugReportCi{};
			debugReportCi.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
			debugReportCi.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT |
				VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT;
			debugReportCi.pfnCallback = DebugReport;
			debugReportCi.pUserData = nullptr;
			vkpfn_CreateDebugReportCallbackEXT(instance, &debugReportCi, pCallback, &debugReport);
			CheckVkResult(err);
		}
	}
#endif
	if (err != VK_SUCCESS)
	{
		CheckVkResult(vkCreateInstance(&createInfo, pCallback, &instance));
	}
}

void Balbino::CInterface::SetupVulkanWindow(const VkSurfaceKHR& surface, const int width, const int height,
                                            const VkInstance& instance, const VkPhysicalDevice& physicalDevice,
                                            const VkDevice& device, const VkAllocationCallbacks* pCallback,
                                            const uint32_t queueFamily, const uint32_t minImageCount)
{
	m_MainWindowData.Surface = surface;

	VkBool32 supportsPresent = VK_FALSE;
	if (vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, queueFamily, m_MainWindowData.Surface, &supportsPresent) !=
		VK_SUCCESS)
	{
		throw std::runtime_error("Error no WSI support on physical device 0\n");
	}

	// Select Surface Format
	const VkFormat requestSurfaceImageFormat[]
	{
		VK_FORMAT_B8G8R8A8_UNORM,
		VK_FORMAT_R8G8B8A8_UNORM,
		VK_FORMAT_B8G8R8_UNORM,
		VK_FORMAT_R8G8B8_UNORM

	};
	m_MainWindowData.SurfaceFormat = ImGui_ImplVulkanH_SelectSurfaceFormat(
		physicalDevice, m_MainWindowData.Surface, requestSurfaceImageFormat,
		static_cast<size_t>(IM_ARRAYSIZE(requestSurfaceImageFormat)), VK_COLORSPACE_SRGB_NONLINEAR_KHR);

	// Select Present Mode
#ifdef IMGUI_UNLIMITED_FRAME_RATE
	VkPresentModeKHR presentMode[]
	{
		VkPresentModeKHRMailbox,
		VkPresentModeKHRImmediate,
		VkPresentModeKHRFifo
	};
#else
	const VkPresentModeKHR presentMode[]
	{
		VK_PRESENT_MODE_FIFO_KHR
	};
#endif

	m_MainWindowData.PresentMode = ImGui_ImplVulkanH_SelectPresentMode(physicalDevice, m_MainWindowData.Surface,
	                                                                   &presentMode[0], IM_ARRAYSIZE(presentMode));

	// Create SwapChain, RenderPass, Frame-buffer, etc.
	IM_ASSERT(minImageCount >= 2);
	ImGui_ImplVulkanH_CreateOrResizeWindow(instance, physicalDevice, device, &m_MainWindowData, queueFamily, pCallback,
	                                       width, height, minImageCount);
}

void Balbino::CInterface::CleanupVulkan(const VkInstance& instance, const VkAllocationCallbacks* pCallback,
                                        const VkDebugReportCallbackEXT& debugReport) const
{
	// Remove the debug report callback
	(void)instance;
	(void)pCallback;
	(void)debugReport;
#ifdef IMGUI_VULKAN_DEBUG_REPORT
	if (vkpfn_DestroyDebugReportCallbackEXT && debugReport)
	{
		vkpfn_DestroyDebugReportCallbackEXT(instance, debugReport, pCallback);
	}
#endif
}

void Balbino::CInterface::CleanupVulkanWindow(const VkInstance& instance, const VkAllocationCallbacks* pCallback,
                                              const VkDevice& device)
{
	ImGui_ImplVulkanH_DestroyWindow(instance, device, &m_MainWindowData, pCallback);
}

void Balbino::CInterface::Setup(SDL_Window* pWindow, const VkInstance& instance, const VkPhysicalDevice& physicalDevice,
                                const VkDevice& device, const uint32_t queueFamily, const VkQueue& queue,
                                const VkPipelineCache& pipelineCache, const VkDescriptorPool& descriptorPool,
                                const VkAllocationCallbacks* pCallback, const uint32_t minImageCount) const
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	//ImGuiSDL::Initialize( Renderer::Get().GetSDLRenderer(), w, h );
	ImGuiIO& io = ImGui::GetIO();
	(void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; // Enable Docking
	//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
	//io.ConfigViewportsNoAutoMerge = true;
	//io.ConfigViewportsNoTaskBarIcon = true;

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
	SetImGuiStyle();

	// Setup Platform/Renderer backend
	ImGui_ImplSDL2_InitForVulkan(pWindow);
	ImGui_ImplVulkan_InitInfo initInfo{};
	initInfo.Instance = instance;
	initInfo.PhysicalDevice = physicalDevice;
	initInfo.Device = device;
	initInfo.QueueFamily = queueFamily;
	initInfo.Queue = queue;
	initInfo.PipelineCache = pipelineCache;
	initInfo.DescriptorPool = descriptorPool;
	initInfo.Allocator = pCallback;
	initInfo.MinImageCount = minImageCount;
	initInfo.ImageCount = m_MainWindowData.ImageCount;
	initInfo.CheckVkResultFn = CheckVkResult;
	ImGui_ImplVulkan_Init(&initInfo, m_MainWindowData.RenderPass);
}

void Balbino::CInterface::UploadFont(const VkDevice& device, const VkQueue& queue) const
{
	// Use any command queue
	const VkCommandPool& commandPool{m_MainWindowData.Frames[m_MainWindowData.FrameIndex].CommandPool};
	const VkCommandBuffer& commandBuffer{m_MainWindowData.Frames[m_MainWindowData.FrameIndex].CommandBuffer};
	/*err = */
	vkResetCommandPool(device, commandPool, VK_COMMAND_POOL_RESET_RELEASE_RESOURCES_BIT);
	//CheckVkResult( err );
	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
	VkResult err = vkBeginCommandBuffer(commandBuffer, &beginInfo);
	CheckVkResult(err);

	ImGui_ImplVulkan_CreateFontsTexture(commandBuffer);

	VkSubmitInfo endInfo = {};
	endInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	endInfo.commandBufferCount = 1;
	endInfo.pCommandBuffers = &commandBuffer;
	err = vkEndCommandBuffer(commandBuffer);
	CheckVkResult(err);
	err = vkQueueSubmit(queue, 1, &endInfo, VK_NULL_HANDLE);
	CheckVkResult(err);

	err = vkDeviceWaitIdle(device);
	CheckVkResult(err);
	ImGui_ImplVulkan_DestroyFontUploadObjects();
}

void Balbino::CInterface::HandleEvents(SDL_Event e)
{
	ImGui_ImplSDL2_ProcessEvent(&e);
}

void Balbino::CInterface::ResizeSwapChain(SDL_Window* pWindow, const VkInstance& instance,
                                          const VkPhysicalDevice& physicalDevice, const VkDevice& device,
                                          const uint32_t queueFamily, const VkAllocationCallbacks* pCallback,
                                          const uint32_t minImageCount)
{
	if (m_SwapChainRebuild)
	{
		int width, height;
		SDL_GetWindowSize(pWindow, &width, &height);
		if (width > 0 && height > 0)
		{
			ImGui_ImplVulkan_SetMinImageCount(minImageCount);
			ImGui_ImplVulkanH_CreateOrResizeWindow(instance, physicalDevice, device, &m_MainWindowData, queueFamily,
			                                       pCallback, width, height, minImageCount);
			m_MainWindowData.FrameIndex = 0;
			m_SwapChainRebuild = false;
		}
	}
}

void Balbino::CInterface::DrawStart(SDL_Window* pWindow)
{
	// Start the Dear ImGui frame
	ImGui_ImplVulkan_NewFrame();
	ImGui_ImplSDL2_NewFrame(pWindow);
	ImGui::NewFrame();
}

void Balbino::CInterface::Draw()
{
	m_pMain->Draw();
	//todo remove demo code
	// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
	if (m_showDemoWindow)
		ImGui::ShowDemoWindow(&m_showDemoWindow);
}

void Balbino::CInterface::Render(const VkDevice& device, const VkQueue& queue)
{
	m_pGameView->Draw();

	// Rendering
	ImGui::Render();
	ImDrawData* pDrawData = ImGui::GetDrawData();
	const bool mainIsMinimized = (pDrawData->DisplaySize.x <= 0.0f || pDrawData->DisplaySize.y <= 0.0f);
	m_MainWindowData.ClearValue.color.float32[0] = m_ClearColor.x * m_ClearColor.w;
	m_MainWindowData.ClearValue.color.float32[1] = m_ClearColor.y * m_ClearColor.w;
	m_MainWindowData.ClearValue.color.float32[2] = m_ClearColor.z * m_ClearColor.w;
	m_MainWindowData.ClearValue.color.float32[3] = m_ClearColor.w;
	if (!mainIsMinimized)
		FrameRender(pDrawData, device, queue);

	ImGuiIO& io{ImGui::GetIO()};
	(void)io;
	// Update and Render additional Platform Windows
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}

	// Present Main Platform Window
	if (!mainIsMinimized)
		FramePresent(queue);
}

void Balbino::CInterface::Cleanup() const
{
	delete m_pMain;
	delete m_pGameView;
	ImGui_ImplVulkan_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
}

void Balbino::CInterface::FrameRender(ImDrawData* drawData, const VkDevice& device, const VkQueue& queue)
{
	const VkSemaphore& imageAcquiredSemaphore{
		m_MainWindowData.FrameSemaphores[m_MainWindowData.SemaphoreIndex].ImageAcquiredSemaphore
	};
	const VkSemaphore& renderCompleteSemaphore{
		m_MainWindowData.FrameSemaphores[m_MainWindowData.SemaphoreIndex].RenderCompleteSemaphore
	};
	VkResult err{
		vkAcquireNextImageKHR(device, m_MainWindowData.Swapchain, UINT64_MAX, imageAcquiredSemaphore, VK_NULL_HANDLE,
		                      &m_MainWindowData.FrameIndex)
	};
	if (err == VK_ERROR_OUT_OF_DATE_KHR || err == VK_SUBOPTIMAL_KHR)
	{
		m_SwapChainRebuild = true;
		return;
	}
	CheckVkResult(err);

	const ImGui_ImplVulkanH_Frame* fd = &m_MainWindowData.Frames[m_MainWindowData.FrameIndex];
	{
		const VkFence& fence{fd->Fence};
		err = vkWaitForFences(device, 1, &fence, VK_TRUE, UINT64_MAX);
		// wait indefinitely instead of periodically checking
		CheckVkResult(err);
		err = vkResetFences(device, 1, &fence);
		CheckVkResult(err);
	}
	{
		vkResetCommandPool(device, fd->CommandPool, VK_COMMAND_POOL_RESET_RELEASE_RESOURCES_BIT);
		//CheckVkResult( err );
		VkCommandBufferBeginInfo info{};
		info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
		const VkCommandBuffer& commandBuffer{fd->CommandBuffer};
		err = vkBeginCommandBuffer(commandBuffer, &info);
		CheckVkResult(err);
	}
	{
		VkRenderPassBeginInfo info{};
		VkClearValue clear;
		const VkCommandBuffer& command{fd->CommandBuffer};
		clear.depthStencil = m_MainWindowData.ClearValue.depthStencil;
		clear.color.float32[0] = m_MainWindowData.ClearValue.color.float32[0];
		clear.color.float32[1] = m_MainWindowData.ClearValue.color.float32[1];
		clear.color.float32[2] = m_MainWindowData.ClearValue.color.float32[2];
		clear.color.float32[3] = m_MainWindowData.ClearValue.color.float32[3];
		info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		info.renderPass = m_MainWindowData.RenderPass;
		info.framebuffer = fd->Framebuffer;
		info.renderArea.extent.width = m_MainWindowData.Width;
		info.renderArea.extent.height = m_MainWindowData.Height;
		info.clearValueCount = 1;
		info.pClearValues = &clear;
		vkCmdBeginRenderPass(command, &info, VK_SUBPASS_CONTENTS_INLINE);
	}

	// Record dear imgui primitives into command buffer
	ImGui_ImplVulkan_RenderDrawData(drawData, fd->CommandBuffer);

	// Submit command buffer
	vkCmdEndRenderPass(fd->CommandBuffer);
	{
		const VkPipelineStageFlags& waitStage{VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};

		VkSubmitInfo info{};
		info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		info.waitSemaphoreCount = 1;
		info.pWaitSemaphores = &imageAcquiredSemaphore;
		info.pWaitDstStageMask = &waitStage;
		info.commandBufferCount = 1;
		info.pCommandBuffers = &fd->CommandBuffer;
		info.signalSemaphoreCount = 1;
		info.pSignalSemaphores = &renderCompleteSemaphore;

		vkEndCommandBuffer(fd->CommandBuffer);
		err = vkQueueSubmit(queue, 1, &info, fd->Fence);
		CheckVkResult(err);
	}
}

void Balbino::CInterface::FramePresent(const VkQueue& queue)
{
	if (m_SwapChainRebuild)
		return;
	const VkSemaphore& renderCompleteSemaphore{
		m_MainWindowData.FrameSemaphores[m_MainWindowData.SemaphoreIndex].RenderCompleteSemaphore
	};
	VkPresentInfoKHR info{};
	info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	info.waitSemaphoreCount = 1;
	info.pWaitSemaphores = &renderCompleteSemaphore;
	info.swapchainCount = 1;
	info.pSwapchains = &m_MainWindowData.Swapchain;
	info.pImageIndices = &m_MainWindowData.FrameIndex;
	const VkResult err = vkQueuePresentKHR(queue, &info);
	if (err == VK_ERROR_OUT_OF_DATE_KHR || err == VK_SUBOPTIMAL_KHR)
	{
		m_SwapChainRebuild = true;
		return;
	}
	CheckVkResult(err);
	m_MainWindowData.SemaphoreIndex = (m_MainWindowData.SemaphoreIndex + 1) % m_MainWindowData.ImageCount;
	// Now we can use the next set of semaphores
}

void Balbino::CInterface::CheckVkResult(const VkResult err)
{
	if (err == VK_SUCCESS)
		return;
	fprintf(stderr, "[Vulkan] Error: VkResult = %d\n", err);
	if (err < VK_SUCCESS)
		abort();
}


void Balbino::CInterface::SetImGuiStyle()
{
	ImGuiStyle* style = &ImGui::GetStyle();
	ImVec4* colors = style->Colors;

	colors[ImGuiCol_Text] = ImVec4(1.000f, 1.000f, 1.000f, 1.000f);
	colors[ImGuiCol_TextDisabled] = ImVec4(0.500f, 0.500f, 0.500f, 1.000f);
	colors[ImGuiCol_WindowBg] = ImVec4(0.180f, 0.180f, 0.180f, 1.000f);
	colors[ImGuiCol_ChildBg] = ImVec4(0.280f, 0.280f, 0.280f, 0.000f);
	colors[ImGuiCol_PopupBg] = ImVec4(0.313f, 0.313f, 0.313f, 1.000f);
	colors[ImGuiCol_Border] = ImVec4(0.266f, 0.266f, 0.266f, 1.000f);
	colors[ImGuiCol_BorderShadow] = ImVec4(0.000f, 0.000f, 0.000f, 0.000f);
	colors[ImGuiCol_FrameBg] = ImVec4(0.160f, 0.160f, 0.160f, 1.000f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.200f, 0.200f, 0.200f, 1.000f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.280f, 0.280f, 0.280f, 1.000f);
	colors[ImGuiCol_TitleBg] = ImVec4(0.148f, 0.148f, 0.148f, 1.000f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.148f, 0.148f, 0.148f, 1.000f);
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.148f, 0.148f, 0.148f, 1.000f);
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.195f, 0.195f, 0.195f, 1.000f);
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.160f, 0.160f, 0.160f, 1.000f);
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.277f, 0.277f, 0.277f, 1.000f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.300f, 0.300f, 0.300f, 1.000f);
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.038f, 0.420f, 0.000f, 1.000f);
	colors[ImGuiCol_CheckMark] = ImVec4(1.000f, 1.000f, 1.000f, 1.000f);
	colors[ImGuiCol_SliderGrab] = ImVec4(0.391f, 0.391f, 0.391f, 1.000f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(0.038f, 0.420f, 0.000f, 1.000f);
	colors[ImGuiCol_Button] = ImVec4(1.000f, 1.000f, 1.000f, 0.000f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(1.000f, 1.000f, 1.000f, 0.156f);
	colors[ImGuiCol_ButtonActive] = ImVec4(1.000f, 1.000f, 1.000f, 0.391f);
	colors[ImGuiCol_Header] = ImVec4(0.313f, 0.313f, 0.313f, 1.000f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.469f, 0.469f, 0.469f, 1.000f);
	colors[ImGuiCol_HeaderActive] = ImVec4(0.469f, 0.469f, 0.469f, 1.000f);
	colors[ImGuiCol_Separator] = colors[ImGuiCol_Border];
	colors[ImGuiCol_SeparatorHovered] = ImVec4(0.391f, 0.391f, 0.391f, 1.000f);
	colors[ImGuiCol_SeparatorActive] = ImVec4(0.038f, 0.420f, 0.000f, 1.000f);
	colors[ImGuiCol_ResizeGrip] = ImVec4(1.000f, 1.000f, 1.000f, 0.250f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(1.000f, 1.000f, 1.000f, 0.670f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(0.038f, 0.420f, 0.000f, 1.000f);
	colors[ImGuiCol_Tab] = ImVec4(0.098f, 0.098f, 0.098f, 1.000f);
	colors[ImGuiCol_TabHovered] = ImVec4(0.352f, 0.352f, 0.352f, 1.000f);
	colors[ImGuiCol_TabActive] = ImVec4(0.195f, 0.195f, 0.195f, 1.000f);
	colors[ImGuiCol_TabUnfocused] = ImVec4(0.098f, 0.098f, 0.098f, 1.000f);
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.195f, 0.195f, 0.195f, 1.000f);
	colors[ImGuiCol_DockingPreview] = ImVec4(0.038f, 0.420f, 0.000f, 0.781f);
	colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.180f, 0.180f, 0.180f, 1.000f);
	colors[ImGuiCol_PlotLines] = ImVec4(0.469f, 0.469f, 0.469f, 1.000f);
	colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.038f, 0.420f, 0.000f, 1.000f);
	colors[ImGuiCol_PlotHistogram] = ImVec4(0.586f, 0.586f, 0.586f, 1.000f);
	colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.038f, 0.420f, 0.000f, 1.000f);
	colors[ImGuiCol_TextSelectedBg] = ImVec4(1.000f, 1.000f, 1.000f, 0.156f);
	colors[ImGuiCol_DragDropTarget] = ImVec4(0.038f, 0.420f, 0.000f, 1.000f);
	colors[ImGuiCol_NavHighlight] = ImVec4(0.038f, 0.420f, 0.000f, 1.000f);
	colors[ImGuiCol_NavWindowingHighlight] = ImVec4(0.038f, 0.420f, 0.000f, 1.000f);
	colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.000f, 0.000f, 0.000f, 0.586f);
	colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.000f, 0.000f, 0.000f, 0.586f);

	style->WindowPadding = ImVec2(2.0f, 2.0f);
	style->FramePadding = ImVec2(2.0f, 2.0f);
	style->CellPadding = ImVec2(2.0f, 2.0f);
	style->ItemSpacing = ImVec2(4.0f, 4.0f);
	style->ItemInnerSpacing = ImVec2(4.0f, 4.0f);
	style->TouchExtraPadding = ImVec2(0.0f, 0.0f);
	style->IndentSpacing = 22.0f;
	style->ScrollbarSize = 12.0f;
	style->GrabMinSize = 12.0f;

	style->WindowBorderSize = 0.0f;
	style->ChildBorderSize = 1.0f;
	style->PopupBorderSize = 1.0f;
	style->FrameBorderSize = 1.0f;
	style->TabBorderSize = 1.0f;

	style->WindowRounding = 0.0f;
	style->ChildRounding = 4.0f;
	style->FrameRounding = 4.0f;
	style->PopupRounding = 4.0f;
	style->ScrollbarSize = 4.0f;
	style->GrabRounding = 4.0f;
	style->LogSliderDeadzone = 4.0f;
	style->TabRounding = 4.0f;
}
