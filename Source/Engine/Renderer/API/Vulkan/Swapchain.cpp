#include "Swapchain.h"

#include "Device.h"

#include "ImageResource.h"
#include "Semaphore.h"

DeerVulkan::CSwapchain::~CSwapchain()
{
    vkDestroySwapchainKHR( GetDevice()->GetVkDevice(), m_swapchain, VK_NULL_HANDLE );
    m_queueFamilies.clear();
    m_surfaceFormats.clear();
    m_presentModes.clear();
    m_images.clear();
}

//create swap chain (https://sopyer.github.io/Blog/post/minimal-vulkan-sample/)
void DeerVulkan::CSwapchain::Initialize( uint32_t width, uint32_t height ) noexcept
{
    VkSwapchainKHR oldSwapchain = m_swapchain;
    GetQueueFamiliesProperties();
    GetSwapSurfaceFormat();
    GetSwapExtent( width, height );
    const VkSwapchainCreateInfoKHR swapChainCreateInfo {
        .sType            = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
        .surface          = m_surfaceKhr,
        .minImageCount    = m_minImageCount,
        .imageFormat      = m_swapSurfaceFormat.format,
        .imageColorSpace  = m_swapSurfaceFormat.colorSpace,
        .imageExtent      = m_swapchainExtent,
        .imageArrayLayers = 1,// 2 for stereo
        .imageUsage       = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
        .imageSharingMode = VK_SHARING_MODE_EXCLUSIVE,
        .preTransform     = m_surfaceCapabilities.currentTransform,
        .compositeAlpha   = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
        .presentMode      = m_presentMode,
        .clipped          = VK_TRUE,
        .oldSwapchain     = oldSwapchain,
    };
    CheckVkResult( vkCreateSwapchainKHR( GetDevice()->GetVkDevice(), &swapChainCreateInfo, VK_NULL_HANDLE, &m_swapchain ) );
    if ( oldSwapchain != VK_NULL_HANDLE )
    {
        vkDestroySwapchainKHR( GetDevice()->GetVkDevice(), oldSwapchain, VK_NULL_HANDLE );
    }

    CheckVkResult( vkGetSwapchainImagesKHR( GetDevice()->GetVkDevice(), m_swapchain, &m_imageCount, VK_NULL_HANDLE ) );
    m_images.resize( m_imageCount );
    CheckVkResult( vkGetSwapchainImagesKHR( GetDevice()->GetVkDevice(), m_swapchain, &m_imageCount, m_images.data() ) );
}

bool DeerVulkan::CSwapchain::AcquireNextImage( FawnVision::Semaphore resentCompleteSemaphore, uint32_t& imageIndex ) const
{
    resentCompleteSemaphore->AddRef();

    const VkResult result = vkAcquireNextImageKHR( GetDevice()->GetVkDevice(), m_swapchain, UINT64_MAX, resentCompleteSemaphore->Get(), VK_NULL_HANDLE, &imageIndex );
    CheckVkResult( result, "Could not get next image" );

    resentCompleteSemaphore->Release();

    return result == VK_ERROR_OUT_OF_DATE_KHR;
}

void DeerVulkan::CSwapchain::GetImages( std::vector<FawnVision::ImageResource>& swapChainImages, uint32_t& count ) noexcept
{
    swapChainImages.clear();
    swapChainImages.reserve( m_imageCount );
    for ( uint32_t i {}; i < m_imageCount; ++i )
    {
        swapChainImages.push_back( new CImageResource { GetDevice() } );
        swapChainImages[ i ]->InitFromSwapchain( m_images[ i ], VK_IMAGE_LAYOUT_PRESENT_SRC_KHR, m_swapchainExtent.width, m_swapchainExtent.height, m_swapSurfaceFormat.format );
    }
    count = m_imageCount;
}

void DeerVulkan::CSwapchain::GetSwapExtent( uint32_t w, uint32_t h ) noexcept
{
    CheckVkResult( vkGetPhysicalDeviceSurfaceCapabilitiesKHR( GetDevice()->GetPhysicalDeviceInfo().device, m_surfaceKhr, &m_surfaceCapabilities ) );
    m_minImageCount = m_surfaceCapabilities.minImageCount;

    if ( m_surfaceCapabilities.currentExtent.width != std::numeric_limits<uint32_t>::max() && m_surfaceCapabilities.currentExtent.width != 0 )
    {
        m_swapchainExtent = m_surfaceCapabilities.currentExtent;
        return;
    }

    m_swapchainExtent = {
        w,
        h,
    };

    m_swapchainExtent.width  = std::clamp( m_swapchainExtent.width, m_surfaceCapabilities.minImageExtent.width, m_surfaceCapabilities.maxImageExtent.width );
    m_swapchainExtent.height = std::clamp( m_swapchainExtent.height, m_surfaceCapabilities.minImageExtent.height, m_surfaceCapabilities.maxImageExtent.height );
}

void DeerVulkan::CSwapchain::GetSwapSurfaceFormat() noexcept
{
    VkSurfaceFormatKHR& swapSurfaceFormat { m_surfaceFormats[ 0 ] };
    for ( const VkSurfaceFormatKHR& surfaceFormat : m_surfaceFormats )
    {
        if ( surfaceFormat.format == VK_FORMAT_B8G8R8A8_SRGB && surfaceFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR )
        {
            swapSurfaceFormat = surfaceFormat;
            break;
        }
    }
    m_swapSurfaceFormat = swapSurfaceFormat;
}

void DeerVulkan::CSwapchain::GetQueueFamiliesProperties() noexcept
{
    uint32_t formatCount {};
    CheckVkResult( vkGetPhysicalDeviceSurfaceFormatsKHR( GetDevice()->GetPhysicalDeviceInfo().device, m_surfaceKhr, &formatCount, VK_NULL_HANDLE ) );
    if ( formatCount )
    {
        m_surfaceFormats.resize( formatCount );
        CheckVkResult( vkGetPhysicalDeviceSurfaceFormatsKHR( GetDevice()->GetPhysicalDeviceInfo().device, m_surfaceKhr, &formatCount, m_surfaceFormats.data() ) );
    }

    for ( VkSurfaceFormatKHR& format : m_surfaceFormats )
    {
        format.format = format.format == VK_FORMAT_UNDEFINED ? VK_FORMAT_R8G8B8A8_UNORM : format.format;
    }

    uint32_t presentModeCount {};
    vkGetPhysicalDeviceSurfacePresentModesKHR( GetDevice()->GetPhysicalDeviceInfo().device, m_surfaceKhr, &presentModeCount, VK_NULL_HANDLE );
    if ( presentModeCount )
    {
        m_presentModes.resize( presentModeCount );
        vkGetPhysicalDeviceSurfacePresentModesKHR( GetDevice()->GetPhysicalDeviceInfo().device, m_surfaceKhr, &presentModeCount, m_presentModes.data() );
    }

    m_presentMode = VK_PRESENT_MODE_FIFO_KHR;
    for ( const auto& availablePresentMode : m_presentModes )
    {
        if ( availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR )
        {
            m_presentMode = VK_PRESENT_MODE_MAILBOX_KHR;
            break;
        }
    }
}
