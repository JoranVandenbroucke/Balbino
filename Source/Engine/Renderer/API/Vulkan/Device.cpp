#include "Device.h"
#include <utility>

constexpr DeerVulkan::CDeviceHolder::~CDeviceHolder()
{
    if ( m_device != VK_NULL_HANDLE )
    {
        vkDestroyDevice( m_device, VK_NULL_HANDLE );
    }
}

DeerVulkan::CDevice::~CDevice()
{
    vkDeviceWaitIdle( m_device );
}
void DeerVulkan::CDevice::Initialize( const std::vector<const char*>& layersToEnable, const std::vector<const char*>& extensionsToEnable )
{
    (void)layersToEnable;
    (void)extensionsToEnable;

    constexpr int deviceExtensionCount { 1 };
    const char* deviceExtensions[] { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
    const float queuePriority[] { 1.0f };
    const VkDeviceQueueCreateInfo queueInfo[ 1 ] { {
        .sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
        .queueFamilyIndex = 0,
        .queueCount       = 1,
        .pQueuePriorities = queuePriority,
    } };

    const VkPhysicalDeviceFeatures deviceFeatures {
        .sampleRateShading = VK_TRUE,
        .samplerAnisotropy = VK_TRUE,
    };
    const VkDeviceCreateInfo createInfo {
        .sType                   = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
        .queueCreateInfoCount    = static_cast<uint32_t>( std::size( queueInfo ) ),
        .pQueueCreateInfos       = queueInfo,
        .enabledLayerCount       = static_cast<uint32_t>( layersToEnable.size() ),
        .ppEnabledLayerNames     = !layersToEnable.empty() ? layersToEnable.data() : VK_NULL_HANDLE,
        .enabledExtensionCount   = deviceExtensionCount,//1
        .ppEnabledExtensionNames = deviceExtensions,
        .pEnabledFeatures        = &deviceFeatures,
    };
    CheckVkResult( vkCreateDevice( m_deviceInfo.device, &createInfo, VK_NULL_HANDLE, &m_device ), "Could not create a device" );
}