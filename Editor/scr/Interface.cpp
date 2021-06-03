#include "pch.h"
#include "Interface.h"


Interface::Interface()
	: m_ClearColor{ 0.45f, 0.55f, 0.60f, 1.00f }
	, m_SwapChainRebuild{ false }
	, m_ShowDemoWindow{ true }
	, m_ShowAnotherWindow{ false }
{
}

void Interface::SetupVulkan( const char** extensions, const uint32_t extensionsCount, vk::InstanceCreateInfo& createInfo, vk::Instance& instance, const vk::AllocationCallbacks* pAllocator, vk::DebugReportCallbackEXT& debugReport ) const
{
	(void) extensions;
	(void) extensionsCount;
	(void) createInfo;
	(void) instance;
	(void) pAllocator;
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
	vk::Result err{ createInstance( &createInfo, pAllocator, &instance ) };
	CheckVkResult( err );
	free( extensionsExt );

	// Get the function pointer (required for any extensions)
	const auto vkCreateDebugReportCallbackExt{ reinterpret_cast<PFN_vkCreateDebugReportCallbackEXT>( vkGetInstanceProcAddr( instance, "vkCreateDebugReportCallbackEXT" ) ) };
	IM_ASSERT( vkCreateDebugReportCallbackExt != nullptr );

	// Setup the debug report callback
	vk::DebugReportCallbackCreateInfoEXT debugReportCi{};
	debugReportCi.sType = vk::StructureType::eDebugReportCallbackCreateInfoEXT;
	debugReportCi.flags = vk::DebugReportFlagBitsEXT::eError | vk::DebugReportFlagBitsEXT::eWarning | vk::DebugReportFlagBitsEXT::ePerformanceWarning;
	debugReportCi.pfnCallback = DebugReport;
	debugReportCi.pUserData = nullptr;
	err = instance.createDebugReportCallbackEXT( &debugReportCi, pAllocator, &debugReport );
	CheckVkResult( err );
#endif
}

void Interface::SetupVulkanWindow( const vk::SurfaceKHR& surface, const int width, const int height, const vk::Instance& instance, const vk::PhysicalDevice& physicalDevice, const vk::Device& device, const vk::AllocationCallbacks* pCallback, const uint32_t queueFamily, const uint32_t minImageCount )
{
	m_MainWindowData.Surface = surface;

	if ( physicalDevice.getSurfaceSupportKHR( queueFamily, m_MainWindowData.Surface ) != VK_TRUE )
	{
		throw std::runtime_error( "Error no WSI support on physical device 0\n" );
	}

	// Select Surface Format
	const VkFormat requestSurfaceImageFormat[]
	{
		VK_FORMAT_B8G8R8A8_UNORM,
		VK_FORMAT_R8G8B8A8_UNORM,
		VK_FORMAT_B8G8R8_UNORM,
		VK_FORMAT_R8G8B8_UNORM
	};
	const VkColorSpaceKHR requestSurfaceColorSpace{ VK_COLORSPACE_SRGB_NONLINEAR_KHR };
	m_MainWindowData.SurfaceFormat = ImGui_ImplVulkanH_SelectSurfaceFormat( physicalDevice, m_MainWindowData.Surface, requestSurfaceImageFormat, static_cast<size_t>( IM_ARRAYSIZE( requestSurfaceImageFormat ) ), requestSurfaceColorSpace );

	// Select Present Mode
#ifdef IMGUI_UNLIMITED_FRAME_RATE
	VkPresentModeKHR presentMode[]
	{
		VK_PRESENT_MODE_MAILBOX_KHR,
		VK_PRESENT_MODE_IMMEDIATE_KHR,
		VK_PRESENT_MODE_FIFO_KHR
	};
#else
	VkPresentModeKHR presentMode[]
	{
		VK_PRESENT_MODE_FIFO_KHR
	};
#endif

	m_MainWindowData.PresentMode = ImGui_ImplVulkanH_SelectPresentMode( physicalDevice, m_MainWindowData.Surface, &presentMode[0], IM_ARRAYSIZE( presentMode ) );
	//printf("[vulkan] Selected PresentMode = %d\n", m_MainWindowData.PresentMode);

	// Create SwapChain, RenderPass, Framebuffer, etc.
	IM_ASSERT( minImageCount >= 2 );
	VkAllocationCallbacks callbacks;
	callbacks.pUserData = pCallback->pUserData;
	callbacks.pfnAllocation = pCallback->pfnAllocation;
	callbacks.pfnFree = pCallback->pfnFree;
	callbacks.pfnInternalAllocation = pCallback->pfnInternalAllocation;
	callbacks.pfnInternalFree = pCallback->pfnInternalFree;
	callbacks.pfnReallocation = pCallback->pfnReallocation;
	ImGui_ImplVulkanH_CreateOrResizeWindow( instance, physicalDevice, device, &m_MainWindowData, queueFamily, &callbacks, width, height, minImageCount );
}

void Interface::CleanupVulkan( const vk::Instance& instance, const vk::AllocationCallbacks* pCallback, const vk::DebugReportCallbackEXT& debugReport ) const
{
	// Remove the debug report callback
	const auto vkDestroyDebugReportCallbackExt{ reinterpret_cast<PFN_vkDestroyDebugReportCallbackEXT>( vkGetInstanceProcAddr( instance, "vkDestroyDebugReportCallbackEXT" ) ) };
	VkAllocationCallbacks callbacks;
	callbacks.pUserData = pCallback->pUserData;
	callbacks.pfnAllocation = pCallback->pfnAllocation;
	callbacks.pfnFree = pCallback->pfnFree;
	callbacks.pfnInternalAllocation = pCallback->pfnInternalAllocation;
	callbacks.pfnInternalFree = pCallback->pfnInternalFree;
	callbacks.pfnReallocation = pCallback->pfnReallocation;
	vkDestroyDebugReportCallbackExt( instance, debugReport, &callbacks );
}

void Interface::CleanupVulkanWindow( const vk::Instance& instance, const vk::AllocationCallbacks* pCallback, const vk::Device& device )
{
	ImGui_ImplVulkan_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
	
	VkAllocationCallbacks callbacks;
	callbacks.pUserData = pCallback->pUserData;
	callbacks.pfnAllocation = pCallback->pfnAllocation;
	callbacks.pfnFree = pCallback->pfnFree;
	callbacks.pfnInternalAllocation = pCallback->pfnInternalAllocation;
	callbacks.pfnInternalFree = pCallback->pfnInternalFree;
	callbacks.pfnReallocation = pCallback->pfnReallocation;
	ImGui_ImplVulkanH_DestroyWindow( instance, device, &m_MainWindowData, &callbacks );
}

void Interface::Setup( SDL_Window* pWindow, const vk::Instance& instance, const vk::PhysicalDevice& physicalDevice, const vk::Device& device, const uint32_t queueFamily, const vk::Queue& queue, const vk::PipelineCache& pipelineCache, const vk::DescriptorPool& descriptorPool, const vk::AllocationCallbacks* pCallback, const uint32_t minImageCount ) const
{
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io{ ImGui::GetIO() };
	(void) io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable Multi-Viewport / Platform Windows
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

	// Setup Platform/Renderer backends
	ImGui_ImplSDL2_InitForVulkan( pWindow );
	VkAllocationCallbacks callbacks;
	callbacks.pUserData = pCallback->pUserData;
	callbacks.pfnAllocation = pCallback->pfnAllocation;
	callbacks.pfnFree = pCallback->pfnFree;
	callbacks.pfnInternalAllocation = pCallback->pfnInternalAllocation;
	callbacks.pfnInternalFree = pCallback->pfnInternalFree;
	callbacks.pfnReallocation = pCallback->pfnReallocation;
	ImGui_ImplVulkan_InitInfo initInfo{};
	initInfo.Instance = instance;
	initInfo.PhysicalDevice = physicalDevice;
	initInfo.Device = device;
	initInfo.QueueFamily = queueFamily;
	initInfo.Queue = queue;
	initInfo.PipelineCache = pipelineCache;
	initInfo.DescriptorPool = descriptorPool;
	initInfo.Allocator = &callbacks;
	initInfo.MinImageCount = minImageCount;
	initInfo.ImageCount = m_MainWindowData.ImageCount;
	initInfo.CheckVkResultFn = VKCheckVkResult;
	ImGui_ImplVulkan_Init( &initInfo, m_MainWindowData.RenderPass );
}

void Interface::UploadFont( const vk::Device& device, const vk::Queue& queue ) const
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

void Interface::HandleEvents( SDL_Event e )
{
	ImGui_ImplSDL2_ProcessEvent( &e );
}

void Interface::ResizeSwapChain( SDL_Window* pWindow, const vk::Instance& instance, const vk::PhysicalDevice& physicalDevice, const vk::Device& device, const uint32_t queueFamily, const vk::AllocationCallbacks* pCallback, const uint32_t minImageCount )
{
	if ( m_SwapChainRebuild )
	{
		int width, height;
		SDL_GetWindowSize( pWindow, &width, &height );
		if ( width > 0 && height > 0 )
		{
			VkAllocationCallbacks callbacks;
			callbacks.pUserData = pCallback->pUserData;
			callbacks.pfnAllocation = pCallback->pfnAllocation;
			callbacks.pfnFree = pCallback->pfnFree;
			callbacks.pfnInternalAllocation = pCallback->pfnInternalAllocation;
			callbacks.pfnInternalFree = pCallback->pfnInternalFree;
			callbacks.pfnReallocation = pCallback->pfnReallocation;
			ImGui_ImplVulkan_SetMinImageCount( minImageCount );
			ImGui_ImplVulkanH_CreateOrResizeWindow( instance, physicalDevice, device, &m_MainWindowData, queueFamily, &callbacks, width, height, minImageCount );
			m_MainWindowData.FrameIndex = 0;
			m_SwapChainRebuild = false;
		}
	}
}

void Interface::DrawStart( SDL_Window* pWindow )
{
	// Start the Dear ImGui frame
	ImGui_ImplVulkan_NewFrame();
	ImGui_ImplSDL2_NewFrame( pWindow );
	ImGui::NewFrame();
}

void Interface::Draw()
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

		ImGui::Text( "Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate );
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

void Interface::Render( const vk::Device& device, const vk::Queue& queue )
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

void Interface::FrameRender( ImDrawData* drawData, const vk::Device& device, const vk::Queue& queue )
{
	vk::Semaphore imageAcquiredSemaphore{ m_MainWindowData.FrameSemaphores[m_MainWindowData.SemaphoreIndex].ImageAcquiredSemaphore };
	vk::Semaphore renderCompleteSemaphore{ m_MainWindowData.FrameSemaphores[m_MainWindowData.SemaphoreIndex].RenderCompleteSemaphore };
	vk::Result err{ device.acquireNextImageKHR( m_MainWindowData.Swapchain, UINT64_MAX, imageAcquiredSemaphore, VK_NULL_HANDLE, &m_MainWindowData.FrameIndex ) };
	//err = vkAcquireNextImageKHR( device, m_MainWindowData.Swapchain, UINT64_MAX, image_acquired_semaphore, VK_NULL_HANDLE, &m_MainWindowData.FrameIndex );
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
		info.sType = vk::StructureType::eBufferCreateInfo;
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
	vkCmdEndRenderPass( fd->CommandBuffer );
	{
		vk::PipelineStageFlags waitStage{ vk::PipelineStageFlagBits::eColorAttachmentOutput };
		const vk::CommandBuffer command{ fd->CommandBuffer };
		const vk::Fence fence{ fd->Fence };

		vk::SubmitInfo info{};
		info.sType = vk::StructureType::eSubmitInfo;
		info.waitSemaphoreCount = 1;
		info.pWaitSemaphores = &imageAcquiredSemaphore;
		info.pWaitDstStageMask = &waitStage;
		info.commandBufferCount = 1;
		info.pCommandBuffers = &command;
		info.signalSemaphoreCount = 1;
		info.pSignalSemaphores = &renderCompleteSemaphore;

		command.end();
		vkEndCommandBuffer( fd->CommandBuffer );
		//CheckVkResult( err );
		err = queue.submit( 1, &info, fence );
		CheckVkResult( err );
	}
}

void Interface::FramePresent( const vk::Queue& queue)
{
	if ( m_SwapChainRebuild )
		return;
	vk::Semaphore render_complete_semaphore {m_MainWindowData.FrameSemaphores[m_MainWindowData.SemaphoreIndex].RenderCompleteSemaphore};
	vk::SwapchainKHR swapchain{ m_MainWindowData.Swapchain };
	vk::PresentInfoKHR info {};
	info.sType = vk::StructureType::eDisplayPresentInfoKHR;
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
