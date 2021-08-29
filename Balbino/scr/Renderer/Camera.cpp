#include "pch.h"
#include "Camera.h"

#include "../Managers/CameraManager.h"

void* Balbino::CCamera::operator new( const size_t size )
{
	if ( CCamera* pCam = static_cast<CCamera*>( ::operator new( size ) ) )
	{
		CCameraManager::GetInstance().AddCamera( pCam );
		return pCam;
	}
	return nullptr;
}

void* Balbino::CCamera::operator new( size_t size, int b, const char* f, int l)
{
	if (CCamera* pCam = static_cast<CCamera*>(::operator new(size, b, f, l)))
	{
		CCameraManager::GetInstance().AddCamera(pCam);
		return pCam;
	}
	return nullptr;
}

void Balbino::CCamera::operator delete( void* ptr ) noexcept
{
	if ( ptr )
		free( ptr );
	ptr = nullptr;
}

void Balbino::CCamera::SetDrawIndex( const int index )
{
	m_index = index;
}

int Balbino::CCamera::GetDrawIndex() const
{
	return m_index;
}

void Balbino::CCamera::Destroy( const VkDevice& device ) const
{
	vkDestroySwapchainKHR(device, m_swapchain, nullptr);
}

void Balbino::CCamera::CheckVkResult( const VkResult err )
{
	if (err == VK_SUCCESS)
		return;
	fprintf(stderr, "[Vulkan] Error: VkResult = %d\n", err);
	if (err < VK_SUCCESS)
		abort();
}

void Balbino::CCamera::Setup( const VkDevice& device, const VkPhysicalDevice& physicalDevice, const VkSurfaceKHR& surf, uint32_t minImageCount )
{
	vkDeviceWaitIdle(device);
	//create swap chain (https://sopyer.github.io/Blog/post/minimal-vulkan-sample/)
	{
		//Use first available format
		uint32_t formatCount = 1;
		VkResult err = vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surf, &formatCount, nullptr); // suppress validation layer
		CheckVkResult(err);
		err = vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surf, &formatCount, &m_surfaceFormat );
		CheckVkResult(err);
		m_surfaceFormat.format = m_surfaceFormat.format == VK_FORMAT_UNDEFINED ? VK_FORMAT_R8G8B8A8_UNORM : m_surfaceFormat.format;


		uint32_t presentModeCount{};
		err = vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surf, &presentModeCount, nullptr );
		CheckVkResult(err);
		VkPresentModeKHR presentModes[MAX_PRESENT_MODE_COUNT]{};
		presentModeCount = presentModeCount > MAX_PRESENT_MODE_COUNT ? MAX_PRESENT_MODE_COUNT : presentModeCount;
		err = vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surf, &presentModeCount, presentModes);
		CheckVkResult(err);

		VkPresentModeKHR presentMode{ VK_PRESENT_MODE_FIFO_KHR };   // always supported.
		for ( uint32_t i = 0; i < presentModeCount; ++i )
		{
			if ( presentModes[i] == VK_PRESENT_MODE_MAILBOX_KHR )
			{
				presentMode = VK_PRESENT_MODE_MAILBOX_KHR;
				break;
			}
		}
		if ( presentMode == VK_PRESENT_MODE_MAILBOX_KHR)
			m_ImageCount = PRESENT_MODE_MAILBOX_IMAGE_COUNT;
		else
			m_ImageCount = PRESENT_MODE_DEFAULT_IMAGE_COUNT;

		VkSurfaceCapabilitiesKHR surfaceCapabilities;
		err = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surf, &surfaceCapabilities );
		CheckVkResult(err);

		m_swapchainExtent = surfaceCapabilities.currentExtent;
		if ( m_swapchainExtent.width == UINT32_MAX )
		{
			m_swapchainExtent.width = std::clamp( m_Width, surfaceCapabilities.minImageExtent.width, surfaceCapabilities.maxImageExtent.width );
			m_swapchainExtent.height = std::clamp( m_Height, surfaceCapabilities.minImageExtent.height, surfaceCapabilities.maxImageExtent.height );
		}

		VkSwapchainCreateInfoKHR swapChainCreateInfo{};
		swapChainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		swapChainCreateInfo.surface = surf;
		swapChainCreateInfo.minImageCount = minImageCount;
		swapChainCreateInfo.imageFormat = m_surfaceFormat.format;
		swapChainCreateInfo.imageColorSpace = m_surfaceFormat.colorSpace;
		swapChainCreateInfo.imageExtent = m_swapchainExtent;
		swapChainCreateInfo.imageArrayLayers = 1; // 2 for stereo
		swapChainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT| VK_IMAGE_USAGE_TRANSFER_DST_BIT;
		swapChainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		swapChainCreateInfo.preTransform = m_SurfaceCapabilities.currentTransform;
		swapChainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		swapChainCreateInfo.presentMode = VK_PRESENT_MODE_MAILBOX_KHR;
		swapChainCreateInfo.clipped = VK_TRUE;

		err = vkCreateSwapchainKHR(device, &swapChainCreateInfo, nullptr, &m_swapchain );
		CheckVkResult(err);
		err = vkGetSwapchainImagesKHR(device, m_swapchain, &minImageCount, nullptr );
		CheckVkResult(err);
		err = vkGetSwapchainImagesKHR( device, m_swapchain, &minImageCount, m_Images );
		CheckVkResult(err);
	}
}
