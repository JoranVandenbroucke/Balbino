#include "pch.h"
#include "Swapchain.h"


#include "Device.h"
#include "Funtions.h"
#include "ImageResource.h"
#include "Instance.h"
#include "Semaphore.h"

BalVulkan::CSwapchain::CSwapchain( const CDevice* device, const VkSurfaceKHR& surface )
	: CDeviceObject{ device }
	, m_surfaceCapabilities{}
	, m_swapchain{ VK_NULL_HANDLE }
	, m_surfaceKhr{ surface }
	, m_swapchainExtent{}
	, m_minImageCount{ 0 }
	, m_imageCount{ 0 }
	, m_swapSurfaceFormat{}
	, m_swapChainCreateInfo{}
{
}

BalVulkan::CSwapchain::~CSwapchain()
{
	vkDestroySwapchainKHR( GetDevice()->GetVkDevice(), m_swapchain, nullptr );
}

//create swap chain (https://sopyer.github.io/Blog/post/minimal-vulkan-sample/)
void BalVulkan::CSwapchain::Initialize( uint32_t width, uint32_t height )
{
	VkSwapchainKHR oldSwapchain = m_swapchain;
	GetQueueFamiliesProperties();
	GetSwapSurfaceFormat();
	GetSwapExtent( width, height );
	const VkSwapchainCreateInfoKHR swapChainCreateInfo{
		.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
		.surface = m_surfaceKhr,
		.minImageCount = m_minImageCount,
		.imageFormat = m_swapSurfaceFormat.format,
		.imageColorSpace = m_swapSurfaceFormat.colorSpace,
		.imageExtent = m_swapchainExtent,
		.imageArrayLayers = 1, // 2 for stereo
		.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT,
		.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE,
		.preTransform = m_surfaceCapabilities.currentTransform,
		.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
		.presentMode = m_presentMode,
		.clipped = VK_TRUE,
		.oldSwapchain = oldSwapchain,
	};
	CheckVkResult( vkCreateSwapchainKHR( GetDevice()->GetVkDevice(), &swapChainCreateInfo, nullptr, &m_swapchain ) );
	if ( oldSwapchain != VK_NULL_HANDLE )
	{
		/*for ( uint32_t i = 0; i < m_imageCount; i++ )
		{
			vkDestroyImageView( m_pDevice->GetVkDevice(), buffers[i].view, nullptr );
		}*/
		vkDestroySwapchainKHR( GetDevice()->GetVkDevice(), oldSwapchain, nullptr );
	}

	CheckVkResult( vkGetSwapchainImagesKHR( GetDevice()->GetVkDevice(), m_swapchain, &m_imageCount, nullptr ) );
	m_images.resize( m_imageCount );
	CheckVkResult( vkGetSwapchainImagesKHR( GetDevice()->GetVkDevice(), m_swapchain, &m_imageCount, m_images.data() ) );
}

bool BalVulkan::CSwapchain::AcquireNextImage( CSemaphore* presentCompleteSemaphore, uint32_t* imageIndex ) const
{
	const VkResult result = vkAcquireNextImageKHR( GetDevice()->GetVkDevice(), m_swapchain, UINT64_MAX, presentCompleteSemaphore->Get(), VK_NULL_HANDLE, imageIndex );
	CheckVkResult( result, "Could not get next Image" );
	return result == VK_ERROR_OUT_OF_DATE_KHR;
}

VkSurfaceFormatKHR BalVulkan::CSwapchain::GetSurfaceFormat() const
{
	return m_swapSurfaceFormat;
}

VkExtent2D BalVulkan::CSwapchain::GetExtend() const
{
	return m_swapchainExtent;
}

const VkSwapchainKHR& BalVulkan::CSwapchain::GetVkSwapchain() const
{
	return m_swapchain;
}

uint32_t BalVulkan::CSwapchain::GetMinImage() const
{
	return m_minImageCount;
}

BalVulkan::CSwapchain* BalVulkan::CSwapchain::CreateNew( const CDevice* device, const VkSurfaceKHR& surface )
{
	return new CSwapchain{ device, surface };
}

void BalVulkan::CSwapchain::GetImages( std::vector<BalVulkan::CImageResource*>& swapChainImages, uint32_t& count ) const
{
	swapChainImages.clear();
	swapChainImages.reserve( m_imageCount );
	for ( uint32_t i{}; i < m_imageCount; ++i )
	{
		swapChainImages.push_back( new CImageResource{ GetDevice() } );
		swapChainImages[i]->InitFromSwapchain( m_images[i], VK_IMAGE_LAYOUT_PRESENT_SRC_KHR, m_swapchainExtent.width, m_swapchainExtent.height, m_swapSurfaceFormat.format );
	}
	count = m_imageCount;
}

void BalVulkan::CSwapchain::GetSwapExtent( uint32_t w, uint32_t h )
{
	CheckVkResult( vkGetPhysicalDeviceSurfaceCapabilitiesKHR( GetDevice()->GetPhysicalDeviceInfo()->device, m_surfaceKhr, &m_surfaceCapabilities ) );
	m_minImageCount = m_surfaceCapabilities.minImageCount;

	if ( m_surfaceCapabilities.currentExtent.width != std::numeric_limits<uint32_t>::max() && m_surfaceCapabilities.currentExtent.width != 0 ) {
		m_swapchainExtent = m_surfaceCapabilities.currentExtent;
		return;
	}

	m_swapchainExtent = { w,h, };

	m_swapchainExtent.width = std::clamp( m_swapchainExtent.width, m_surfaceCapabilities.minImageExtent.width, m_surfaceCapabilities.maxImageExtent.width );
	m_swapchainExtent.height = std::clamp( m_swapchainExtent.height, m_surfaceCapabilities.minImageExtent.height, m_surfaceCapabilities.maxImageExtent.height );
}

void BalVulkan::CSwapchain::GetSwapSurfaceFormat()
{
	VkSurfaceFormatKHR& swapSurfaceFormat{ m_surfaceFormats[0] };
	for ( const VkSurfaceFormatKHR& surfaceFormat : m_surfaceFormats )
	{
		if ( surfaceFormat.format == VK_FORMAT_B8G8R8A8_SRGB && surfaceFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR ) {
			swapSurfaceFormat = surfaceFormat;
			break;
		}
	}
	m_swapSurfaceFormat = swapSurfaceFormat;
}
void BalVulkan::CSwapchain::GetQueueFamiliesProperties()
{
	uint32_t formatCount{ 1 };
	CheckVkResult( vkGetPhysicalDeviceSurfaceFormatsKHR( GetDevice()->GetPhysicalDeviceInfo()->device, m_surfaceKhr, &formatCount, nullptr ) );
	m_surfaceFormats.resize( formatCount );
	CheckVkResult( vkGetPhysicalDeviceSurfaceFormatsKHR( GetDevice()->GetPhysicalDeviceInfo()->device, m_surfaceKhr, &formatCount, m_surfaceFormats.data() ) );

	for ( VkSurfaceFormatKHR& format : m_surfaceFormats )
	{
		format.format = format.format == VK_FORMAT_UNDEFINED ? VK_FORMAT_R8G8B8A8_UNORM : format.format;
	}

	uint32_t presentModeCount{};
	vkGetPhysicalDeviceSurfacePresentModesKHR( GetDevice()->GetPhysicalDeviceInfo()->device, m_surfaceKhr, &presentModeCount, nullptr );
	m_presentModes.resize( presentModeCount );
	vkGetPhysicalDeviceSurfacePresentModesKHR( GetDevice()->GetPhysicalDeviceInfo()->device, m_surfaceKhr, &presentModeCount, m_presentModes.data() );

	m_presentMode = VK_PRESENT_MODE_FIFO_KHR;
	for ( const auto& availablePresentMode : m_presentModes ) {
		if ( availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR ) {
			m_presentMode = VK_PRESENT_MODE_MAILBOX_KHR;
			break;
		}
	}
}
