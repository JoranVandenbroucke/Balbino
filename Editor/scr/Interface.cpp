#include "pch.h"
#include "Interface.h"

#include <SDL.h>
#include <vulkan/vulkan.hpp>


#ifdef IMGUI_VULKAN_DEBUG_REPORT
static VKAPI_ATTR VkBool32 VKAPI_CALL DebugReport( VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objectType, uint64_t object, size_t location, int32_t messageCode, const char* pLayerPrefix, const char* pMessage, void* pUserData )
{
	(void) flags;
	(void) object;
	(void) location;
	(void) messageCode;
	(void) pUserData;
	(void) pLayerPrefix; // Unused arguments
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


Balbino::Interface::Interface()
	: m_ClearColor{ 0.45f, 0.55f, 0.60f, 1.00f }
	, m_SwapChainRebuild{ false }
	, m_ShowDemoWindow{ true }
	, m_ShowAnotherWindow{ false }
{
}

void Balbino::Interface::SetupVulkan( const char** extensions, const uint32_t extensionsCount, vk::InstanceCreateInfo& createInfo, vk::Instance& instance, vk::AllocationCallbacks* pCallback, vk::DebugReportCallbackEXT& debugReport ) const
{
	(void) extensions;
	(void) extensionsCount;
	(void) createInfo;
	(void) instance;
	(void) pCallback;
	(void) debugReport;
#ifdef IMGUI_VULKAN_DEBUG_REPORT
	// Enabling validation layers
	const char* layers[]{ "VK_LAYER_KHRONOS_validation" };
	createInfo.enabledLayerCount = 1;
	createInfo.ppEnabledLayerNames = layers;

	// Enable debug report extension (we need additional storage, so we duplicate the user array to add our new extension to it)
	const auto extensionsExt{ static_cast<const char**>( malloc( sizeof( const char* ) * ( extensionsCount + 1 ) ) ) };
	memcpy( extensionsExt, extensions, extensionsCount * sizeof( const char* ) );
	extensionsExt[extensionsCount] = "VK_EXT_debug_report";
	createInfo.enabledExtensionCount = extensionsCount + 1;
	createInfo.ppEnabledExtensionNames = extensionsExt;

	// Create Vulkan Instance
	CheckVkResult( createInstance( &createInfo, pCallback, &instance ) );
	free( extensionsExt );

	// Get the function pointer (required for any extensions)
	// Setup the debug report callback
	vk::DebugReportCallbackCreateInfoEXT debugReportCi{};
	debugReportCi.sType = vk::StructureType::eDebugReportCallbackCreateInfoEXT;
	debugReportCi.flags = vk::DebugReportFlagBitsEXT::eError | vk::DebugReportFlagBitsEXT::eWarning | vk::DebugReportFlagBitsEXT::ePerformanceWarning;
	debugReportCi.pfnCallback = DebugReport;
	debugReportCi.pUserData = nullptr;

	auto dldi = vk::DispatchLoaderDynamic( instance, vkGetInstanceProcAddr );
	const vk::Result err = instance.createDebugReportCallbackEXT( &debugReportCi, pCallback, &debugReport, dldi );
	CheckVkResult( err );
#endif
}

void Balbino::Interface::SetupVulkanWindow( const vk::SurfaceKHR& surface, const int width, const int height, const vk::Instance& instance, const vk::PhysicalDevice& physicalDevice, const vk::Device& device, const vk::AllocationCallbacks* pCallback, const uint32_t queueFamily, const uint32_t minImageCount )
{
	m_MainWindowData.Surface = surface;

	if ( physicalDevice.getSurfaceSupportKHR( queueFamily, m_MainWindowData.Surface ) != VK_TRUE )
	{
		throw std::runtime_error( "Error no WSI support on physical device 0\n" );
	}

	// Select Surface Format
	const vk::Format requestSurfaceImageFormat[]
	{
		vk::Format::eB8G8R8A8Unorm,
		vk::Format::eR8G8B8A8Unorm,
		vk::Format::eB8G8R8Unorm,
		vk::Format::eR8G8B8Unorm

	};
	const vk::ColorSpaceKHR requestSurfaceColorSpace{ vk::ColorSpaceKHR::eSrgbNonlinear };
	m_MainWindowData.SurfaceFormat = ImGui_ImplVulkanH_SelectSurfaceFormat( physicalDevice, m_MainWindowData.Surface, requestSurfaceImageFormat, static_cast<size_t>( IM_ARRAYSIZE( requestSurfaceImageFormat ) ), requestSurfaceColorSpace );

	// Select Present Mode
#ifdef IMGUI_UNLIMITED_FRAME_RATE
	vk::PresentModeKHR presentMode[]
	{
		vk::PresentModeKHR::eMailbox,
		vk::PresentModeKHR::eImmediate,
		vk::PresentModeKHR::eFifo
	};
#else
	vk::PresentModeKHR presentMode[]
	{
		vk::PresentModeKHR::eFifo
	};
#endif

	m_MainWindowData.PresentMode = ImGui_ImplVulkanH_SelectPresentMode( physicalDevice, m_MainWindowData.Surface, &presentMode[0], IM_ARRAYSIZE( presentMode ) );
	//printf("[vulkan] Selected PresentMode = %d\n", m_MainWindowData.PresentMode);

	// Create SwapChain, RenderPass, Framebuffer, etc.
	IM_ASSERT( minImageCount >= 2 );
	ImGui_ImplVulkanH_CreateOrResizeWindow( instance, physicalDevice, device, &m_MainWindowData, queueFamily, pCallback, width, height, minImageCount );
}

void Balbino::Interface::CleanupVulkan( const vk::Instance& instance, const vk::AllocationCallbacks* pCallback, const vk::DebugReportCallbackEXT& debugReport ) const
{
	// Remove the debug report callback
	(void) instance;
	(void) pCallback;
	(void) debugReport;
	auto dldi = vk::DispatchLoaderDynamic( instance, vkGetInstanceProcAddr );
	instance.destroyDebugReportCallbackEXT( debugReport, pCallback, dldi );
}

void Balbino::Interface::CleanupVulkanWindow( const vk::Instance& instance, const vk::AllocationCallbacks* pCallback, const vk::Device& device )
{
	ImGui_ImplVulkanH_DestroyWindow( instance, device, &m_MainWindowData, pCallback );
}

void Balbino::Interface::Setup( SDL_Window* pWindow, const vk::Instance& instance, const vk::PhysicalDevice& physicalDevice, const vk::Device& device, const uint32_t queueFamily, const vk::Queue& queue, const vk::PipelineCache& pipelineCache, const vk::DescriptorPool& descriptorPool, const vk::AllocationCallbacks* pCallback, const uint32_t minImageCount ) const
{
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io{ ImGui::GetIO() };
	(void) io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; // Enable Docking
	//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable Multi-Viewport / Platform Windows
	//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
	//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
	ImGuiStyle& style{ ImGui::GetStyle() };
	if ( io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable )
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}
	SetImGuiStyle();

	// Setup Platform/Renderer backends
	ImGui_ImplSDL2_InitForVulkan( pWindow );
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
	ImGui_ImplVulkan_Init( &initInfo, m_MainWindowData.RenderPass );
}

void Balbino::Interface::UploadFont( const vk::Device& device, const vk::Queue& queue ) const
{
	// Use any command queue
	const vk::CommandPool commandPool{ m_MainWindowData.Frames[m_MainWindowData.FrameIndex].CommandPool };
	vk::CommandBuffer commandBuffer{ m_MainWindowData.Frames[m_MainWindowData.FrameIndex].CommandBuffer };
	/*err = */
	device.resetCommandPool( commandPool, vk::CommandPoolResetFlagBits::eReleaseResources );
	//CheckVkResult( err );
	vk::CommandBufferBeginInfo beginInfo{};
	beginInfo.sType = vk::StructureType::eCommandBufferBeginInfo;
	beginInfo.flags |= vk::CommandBufferUsageFlagBits::eOneTimeSubmit;
	vk::Result err = commandBuffer.begin( &beginInfo );
	CheckVkResult( err );

	ImGui_ImplVulkan_CreateFontsTexture( commandBuffer );

	vk::SubmitInfo endInfo = {};
	endInfo.sType = vk::StructureType::eSubmitInfo;
	endInfo.commandBufferCount = 1;
	endInfo.pCommandBuffers = &commandBuffer;
	commandBuffer.end();
	//CheckVkResult( err );
	err = queue.submit( 1, &endInfo, VK_NULL_HANDLE );
	CheckVkResult( err );

	device.waitIdle();
	//CheckVkResult( err );
	ImGui_ImplVulkan_DestroyFontUploadObjects();
}

void Balbino::Interface::HandleEvents( SDL_Event e )
{
	ImGui_ImplSDL2_ProcessEvent( &e );
}

void Balbino::Interface::ResizeSwapChain( SDL_Window* pWindow, const vk::Instance& instance, const vk::PhysicalDevice& physicalDevice, const vk::Device& device, const uint32_t queueFamily, const vk::AllocationCallbacks* pCallback, const uint32_t minImageCount )
{
	if ( m_SwapChainRebuild )
	{
		int width, height;
		SDL_GetWindowSize( pWindow, &width, &height );
		if ( width > 0 && height > 0 )
		{
			ImGui_ImplVulkan_SetMinImageCount( minImageCount );
			ImGui_ImplVulkanH_CreateOrResizeWindow( instance, physicalDevice, device, &m_MainWindowData, queueFamily, pCallback, width, height, minImageCount );
			m_MainWindowData.FrameIndex = 0;
			m_SwapChainRebuild = false;
		}
	}
}

void Balbino::Interface::DrawStart( SDL_Window* pWindow )
{
	// Start the Dear ImGui frame
	ImGui_ImplVulkan_NewFrame();
	ImGui_ImplSDL2_NewFrame( pWindow );
	ImGui::NewFrame();
}

void Balbino::Interface::Draw()
{
	//todo remove demo code
	 // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
	if ( m_ShowDemoWindow )
		ImGui::ShowDemoWindow( &m_ShowDemoWindow );

	// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
	{
		static float f = 0.0f;
		static int counter = 0;

		ImGui::Begin( "Hello, world!" );                          // Create a window called "Hello, world!" and append into it.

		ImGui::Text( "This is some useful text." );               // Display some text (you can use a format strings too)
		ImGui::Checkbox( "Demo Window", &m_ShowDemoWindow );      // Edit bools storing our window open/close state
		ImGui::Checkbox( "Another Window", &m_ShowAnotherWindow );

		ImGui::SliderFloat( "float", &f, 0.0f, 1.0f );            // Edit 1 float using a slider from 0.0f to 1.0f
		ImGui::ColorEdit3( "clear color", reinterpret_cast<float*>( &m_ClearColor ) ); // Edit 3 floats representing a color

		if ( ImGui::Button( "Button" ) )                            // Buttons return true when clicked (most widgets return true when edited/activated)
			counter++;
		ImGui::SameLine();
		ImGui::Text( "counter = %d", counter );

		ImGui::Text( "Application average %.3f ms/frame (%.1f FPS)", 1000.0f / static_cast<double>( ImGui::GetIO().Framerate ), static_cast<double>( ImGui::GetIO().Framerate ) );
		ImGui::End();
	}

	// 3. Show another simple window.
	if ( m_ShowAnotherWindow )
	{
		ImGui::Begin( "Another Window", &m_ShowAnotherWindow );   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
		ImGui::Text( "Hello from another window!" );
		if ( ImGui::Button( "Close Me" ) )
			m_ShowAnotherWindow = false;
		ImGui::End();
	}
}

void Balbino::Interface::Render( const vk::Device& device, const vk::Queue& queue )
{
	// Rendering
	ImGui::Render();
	ImDrawData* pDrawData = ImGui::GetDrawData();
	const bool mainIsMinimized = ( pDrawData->DisplaySize.x <= 0.0f || pDrawData->DisplaySize.y <= 0.0f );
	m_MainWindowData.ClearValue.color.float32[0] = m_ClearColor.x * m_ClearColor.w;
	m_MainWindowData.ClearValue.color.float32[1] = m_ClearColor.y * m_ClearColor.w;
	m_MainWindowData.ClearValue.color.float32[2] = m_ClearColor.z * m_ClearColor.w;
	m_MainWindowData.ClearValue.color.float32[3] = m_ClearColor.w;
	if ( !mainIsMinimized )
		FrameRender( pDrawData, device, queue );

	ImGuiIO& io{ ImGui::GetIO() };
	(void) io;
	// Update and Render additional Platform Windows
	if ( io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable )
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}

	// Present Main Platform Window
	if ( !mainIsMinimized )
		FramePresent( queue );
}

void Balbino::Interface::Cleanup()
{
	ImGui_ImplVulkan_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
}

void Balbino::Interface::FrameRender( ImDrawData* drawData, const vk::Device& device, const vk::Queue& queue )
{
	vk::Semaphore imageAcquiredSemaphore{ m_MainWindowData.FrameSemaphores[m_MainWindowData.SemaphoreIndex].ImageAcquiredSemaphore };
	vk::Semaphore renderCompleteSemaphore{ m_MainWindowData.FrameSemaphores[m_MainWindowData.SemaphoreIndex].RenderCompleteSemaphore };
	vk::Result err{ device.acquireNextImageKHR( m_MainWindowData.Swapchain, UINT64_MAX, imageAcquiredSemaphore, VK_NULL_HANDLE, &m_MainWindowData.FrameIndex ) };
	if ( err == vk::Result::eErrorOutOfDateKHR || err == vk::Result::eSuboptimalKHR )
	{
		m_SwapChainRebuild = true;
		return;
	}
	CheckVkResult( err );

	ImGui_ImplVulkanH_Frame* fd = &m_MainWindowData.Frames[m_MainWindowData.FrameIndex];
	{
		vk::Fence fence{ fd->Fence };
		err = device.waitForFences( 1, &fence, VK_TRUE, UINT64_MAX );    // wait indefinitely instead of periodically checking
		CheckVkResult( err );
		err = device.resetFences( 1, &fence );
		CheckVkResult( err );
	}
	{
		device.resetCommandPool( fd->CommandPool, vk::CommandPoolResetFlagBits::eReleaseResources );
		//CheckVkResult( err );
		vk::CommandBufferBeginInfo info{};
		info.sType = vk::StructureType::eCommandBufferBeginInfo;
		info.flags |= vk::CommandBufferUsageFlagBits::eOneTimeSubmit;
		const vk::CommandBuffer commandBuffer{ fd->CommandBuffer };
		err = commandBuffer.begin( &info );
		CheckVkResult( err );
	}
	{
		vk::RenderPassBeginInfo info{};
		vk::ClearValue clear;
		const vk::CommandBuffer command{ fd->CommandBuffer };
		clear.depthStencil = m_MainWindowData.ClearValue.depthStencil;
		clear.color.float32[0] = m_MainWindowData.ClearValue.color.float32[0];
		clear.color.float32[1] = m_MainWindowData.ClearValue.color.float32[1];
		clear.color.float32[2] = m_MainWindowData.ClearValue.color.float32[2];
		clear.color.float32[3] = m_MainWindowData.ClearValue.color.float32[3];
		info.sType = vk::StructureType::eRenderPassBeginInfo;
		info.renderPass = m_MainWindowData.RenderPass;
		info.framebuffer = fd->Framebuffer;
		info.renderArea.extent.width = m_MainWindowData.Width;
		info.renderArea.extent.height = m_MainWindowData.Height;
		info.clearValueCount = 1;
		info.pClearValues = &clear;
		command.beginRenderPass( &info, vk::SubpassContents::eInline );
	}

	// Record dear imgui primitives into command buffer
	ImGui_ImplVulkan_RenderDrawData( drawData, fd->CommandBuffer );

	// Submit command buffer
	fd->CommandBuffer.endRenderPass();
	{
		vk::PipelineStageFlags waitStage{ vk::PipelineStageFlagBits::eColorAttachmentOutput };

		vk::SubmitInfo info{};
		info.sType = vk::StructureType::eSubmitInfo;
		info.waitSemaphoreCount = 1;
		info.pWaitSemaphores = &imageAcquiredSemaphore;
		info.pWaitDstStageMask = &waitStage;
		info.commandBufferCount = 1;
		info.pCommandBuffers = &fd->CommandBuffer;
		info.signalSemaphoreCount = 1;
		info.pSignalSemaphores = &renderCompleteSemaphore;

		fd->CommandBuffer.end();
		err = queue.submit( 1, &info, fd->Fence );
		CheckVkResult( err );
	}
}

void Balbino::Interface::FramePresent( const vk::Queue& queue )
{
	if ( m_SwapChainRebuild )
		return;
	vk::Semaphore render_complete_semaphore{ m_MainWindowData.FrameSemaphores[m_MainWindowData.SemaphoreIndex].RenderCompleteSemaphore };
	vk::SwapchainKHR swapchain{ m_MainWindowData.Swapchain };
	vk::PresentInfoKHR info{};
	info.sType = vk::StructureType::ePresentInfoKHR;
	info.waitSemaphoreCount = 1;
	info.pWaitSemaphores = &render_complete_semaphore;
	info.swapchainCount = 1;
	info.pSwapchains = &swapchain;
	info.pImageIndices = &m_MainWindowData.FrameIndex;
	const vk::Result err = queue.presentKHR( &info );
	if ( err == vk::Result::eErrorOutOfDateKHR || err == vk::Result::eSuboptimalKHR )
	{
		m_SwapChainRebuild = true;
		return;
	}
	CheckVkResult( err );
	m_MainWindowData.SemaphoreIndex = ( m_MainWindowData.SemaphoreIndex + 1 ) % m_MainWindowData.ImageCount; // Now we can use the next set of semaphores
}

void Balbino::Interface::CheckVkResult( const vk::Result err )
{
	if ( err == vk::Result::eSuccess )
		return;
	fprintf( stderr, "[vulkan] Error: VkResult = %d\n", err );
	if ( err < vk::Result::eSuccess )
		abort();
}

void Balbino::Interface::VkCheckVkResult( VkResult err )
{
	if ( err == 0 )
		return;
	fprintf( stderr, "[vulkan] Error: VkResult = %d\n", err );
	if ( err < 0 )
		abort();
}


void Balbino::Interface::SetImGuiStyle() const
{
	ImGuiStyle* style = &ImGui::GetStyle();
	ImVec4* colors = style->Colors;

	colors[ImGuiCol_Text] = ImVec4( 1.000f, 1.000f, 1.000f, 1.000f );
	colors[ImGuiCol_TextDisabled] = ImVec4( 0.500f, 0.500f, 0.500f, 1.000f );
	colors[ImGuiCol_WindowBg] = ImVec4( 0.180f, 0.180f, 0.180f, 1.000f );
	colors[ImGuiCol_ChildBg] = ImVec4( 0.280f, 0.280f, 0.280f, 0.000f );
	colors[ImGuiCol_PopupBg] = ImVec4( 0.313f, 0.313f, 0.313f, 1.000f );
	colors[ImGuiCol_Border] = ImVec4( 0.266f, 0.266f, 0.266f, 1.000f );
	colors[ImGuiCol_BorderShadow] = ImVec4( 0.000f, 0.000f, 0.000f, 0.000f );
	colors[ImGuiCol_FrameBg] = ImVec4( 0.160f, 0.160f, 0.160f, 1.000f );
	colors[ImGuiCol_FrameBgHovered] = ImVec4( 0.200f, 0.200f, 0.200f, 1.000f );
	colors[ImGuiCol_FrameBgActive] = ImVec4( 0.280f, 0.280f, 0.280f, 1.000f );
	colors[ImGuiCol_TitleBg] = ImVec4( 0.148f, 0.148f, 0.148f, 1.000f );
	colors[ImGuiCol_TitleBgActive] = ImVec4( 0.148f, 0.148f, 0.148f, 1.000f );
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4( 0.148f, 0.148f, 0.148f, 1.000f );
	colors[ImGuiCol_MenuBarBg] = ImVec4( 0.195f, 0.195f, 0.195f, 1.000f );
	colors[ImGuiCol_ScrollbarBg] = ImVec4( 0.160f, 0.160f, 0.160f, 1.000f );
	colors[ImGuiCol_ScrollbarGrab] = ImVec4( 0.277f, 0.277f, 0.277f, 1.000f );
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4( 0.300f, 0.300f, 0.300f, 1.000f );
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4( 0.038f, 0.420f, 0.000f, 1.000f );
	colors[ImGuiCol_CheckMark] = ImVec4( 1.000f, 1.000f, 1.000f, 1.000f );
	colors[ImGuiCol_SliderGrab] = ImVec4( 0.391f, 0.391f, 0.391f, 1.000f );
	colors[ImGuiCol_SliderGrabActive] = ImVec4( 0.038f, 0.420f, 0.000f, 1.000f );
	colors[ImGuiCol_Button] = ImVec4( 1.000f, 1.000f, 1.000f, 0.000f );
	colors[ImGuiCol_ButtonHovered] = ImVec4( 1.000f, 1.000f, 1.000f, 0.156f );
	colors[ImGuiCol_ButtonActive] = ImVec4( 1.000f, 1.000f, 1.000f, 0.391f );
	colors[ImGuiCol_Header] = ImVec4( 0.313f, 0.313f, 0.313f, 1.000f );
	colors[ImGuiCol_HeaderHovered] = ImVec4( 0.469f, 0.469f, 0.469f, 1.000f );
	colors[ImGuiCol_HeaderActive] = ImVec4( 0.469f, 0.469f, 0.469f, 1.000f );
	colors[ImGuiCol_Separator] = colors[ImGuiCol_Border];
	colors[ImGuiCol_SeparatorHovered] = ImVec4( 0.391f, 0.391f, 0.391f, 1.000f );
	colors[ImGuiCol_SeparatorActive] = ImVec4( 0.038f, 0.420f, 0.000f, 1.000f );
	colors[ImGuiCol_ResizeGrip] = ImVec4( 1.000f, 1.000f, 1.000f, 0.250f );
	colors[ImGuiCol_ResizeGripHovered] = ImVec4( 1.000f, 1.000f, 1.000f, 0.670f );
	colors[ImGuiCol_ResizeGripActive] = ImVec4( 0.038f, 0.420f, 0.000f, 1.000f );
	colors[ImGuiCol_Tab] = ImVec4( 0.098f, 0.098f, 0.098f, 1.000f );
	colors[ImGuiCol_TabHovered] = ImVec4( 0.352f, 0.352f, 0.352f, 1.000f );
	colors[ImGuiCol_TabActive] = ImVec4( 0.195f, 0.195f, 0.195f, 1.000f );
	colors[ImGuiCol_TabUnfocused] = ImVec4( 0.098f, 0.098f, 0.098f, 1.000f );
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4( 0.195f, 0.195f, 0.195f, 1.000f );
	colors[ImGuiCol_DockingPreview] = ImVec4( 0.038f, 0.420f, 0.000f, 0.781f );
	colors[ImGuiCol_DockingEmptyBg] = ImVec4( 0.180f, 0.180f, 0.180f, 1.000f );
	colors[ImGuiCol_PlotLines] = ImVec4( 0.469f, 0.469f, 0.469f, 1.000f );
	colors[ImGuiCol_PlotLinesHovered] = ImVec4( 0.038f, 0.420f, 0.000f, 1.000f );
	colors[ImGuiCol_PlotHistogram] = ImVec4( 0.586f, 0.586f, 0.586f, 1.000f );
	colors[ImGuiCol_PlotHistogramHovered] = ImVec4( 0.038f, 0.420f, 0.000f, 1.000f );
	colors[ImGuiCol_TextSelectedBg] = ImVec4( 1.000f, 1.000f, 1.000f, 0.156f );
	colors[ImGuiCol_DragDropTarget] = ImVec4( 0.038f, 0.420f, 0.000f, 1.000f );
	colors[ImGuiCol_NavHighlight] = ImVec4( 0.038f, 0.420f, 0.000f, 1.000f );
	colors[ImGuiCol_NavWindowingHighlight] = ImVec4( 0.038f, 0.420f, 0.000f, 1.000f );
	colors[ImGuiCol_NavWindowingDimBg] = ImVec4( 0.000f, 0.000f, 0.000f, 0.586f );
	colors[ImGuiCol_ModalWindowDimBg] = ImVec4( 0.000f, 0.000f, 0.000f, 0.586f );

	style->ChildRounding = 4.0f;
	style->FrameBorderSize = 1.0f;
	style->FrameRounding = 2.0f;
	style->GrabMinSize = 7.0f;
	style->PopupRounding = 2.0f;
	style->ScrollbarRounding = 12.0f;
	style->ScrollbarSize = 13.0f;
	style->TabBorderSize = 1.0f;
	style->TabRounding = 0.0f;
	style->WindowRounding = 4.0f;
}