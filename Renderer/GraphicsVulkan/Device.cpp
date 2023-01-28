#include "Device.h"

#include "Funtions.h"
#include "Instance.h"

FawnVision::CDeviceHolder::CDeviceHolder( VkDevice device, VkAllocationCallbacks* hostAllocator )
        : m_pCallbacks( hostAllocator )
          , m_device( device )
{
}

FawnVision::CDeviceHolder::~CDeviceHolder()
{
    if ( m_device != VK_NULL_HANDLE )
    {
        vkDestroyDevice( m_device, m_pCallbacks );
    }
}

FawnVision::CDevice::CDevice( const SPhysicalDeviceInfo* pDeviceInfo, VkAllocationCallbacks* pCallbacks, VkDevice device )
        : CDeviceHolder( device, pCallbacks )
          , CRefCounted{}
          , m_pDeviceInfo( pDeviceInfo )
{
}

FawnVision::CDevice::~CDevice()
{
    vkDeviceWaitIdle( m_device );
    //for ( auto & deferredImageView : m_deferredImageViews )
    //{
    //	for ( CImageView& imageView : deferredImageView )
    //	{
    //		imageView.Release();
    //	}
    //}
    //m_deferredBufferViews->clear();
    //m_deferredImageViews->clear();
    //m_deferredSamplers->clear();
    //m_deferredRenderPasses->clear();
    //m_deferredPipelines->clear();
    
    //vkDestroyDevice( GetVkDevice(), nullptr );
}

void FawnVision::CDevice::WaitIdle() const
{
    vkDeviceWaitIdle( m_device );
}

const VkDevice& FawnVision::CDevice::GetVkDevice() const
{
    return m_device;
}

const FawnVision::SPhysicalDeviceInfo* FawnVision::CDevice::GetPhysicalDeviceInfo() const
{
    return m_pDeviceInfo;
}


FawnVision::CDevice* FawnVision::CDevice::Create( const SPhysicalDeviceInfo* pDeviceInfo, VkAllocationCallbacks* pCallbacks, const std::vector<const char*>& layersToEnable, const std::vector<const char*>& extensionsToEnable )
{
    (void) layersToEnable;
    (void) extensionsToEnable;
    std::vector<VkDeviceQueueCreateInfo> wantedQueues;
    const std::vector                    queuePriorities( pDeviceInfo->queueFamilyProperties.size(), 1.0f );
    
    constexpr int deviceExtensionCount{ 1 };
    const char* deviceExtensions[]{ VK_KHR_SWAPCHAIN_EXTENSION_NAME };
    const float queuePriority[]{ 1.0f };
    const VkDeviceQueueCreateInfo queueInfo[1]{{
                                                       .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO, .queueFamilyIndex = 0, .queueCount = 1, .pQueuePriorities = queuePriority,
                                               }};
    
    const VkPhysicalDeviceFeatures deviceFeatures{
            .sampleRateShading = VK_TRUE, .samplerAnisotropy = VK_TRUE,
    };
    const VkDeviceCreateInfo       createInfo{
            .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO, .queueCreateInfoCount = static_cast<uint32_t>( std::size(
                    queueInfo
            )), .pQueueCreateInfos = queueInfo, .enabledLayerCount = static_cast< uint32_t >( layersToEnable.size()), .ppEnabledLayerNames = !layersToEnable.empty()
                                                                                                                                             ? layersToEnable.data()
                                                                                                                                             : nullptr, .enabledExtensionCount = deviceExtensionCount,    //1
            .ppEnabledExtensionNames = deviceExtensions, .pEnabledFeatures = &deviceFeatures,
    };
    VkDevice                       device;
    CheckVkResult(
            vkCreateDevice( pDeviceInfo->device, &createInfo, pCallbacks, &device ), "Could not create a device"
    );
    auto pDevice{ new CDevice{ pDeviceInfo, pCallbacks, device }};
    return pDevice;
}

FawnVision::CDevice::SRenderPass::SRenderPass( SRenderPass&& ) noexcept = default;

FawnVision::CDevice::SRenderPass::SRenderPass( const VkDevice& s, const VkRenderPass& r, const VkFramebuffer& f )
        : self( s )
          , renderPass( r )
          , frameBuffer( f )
{
}

FawnVision::CDevice::SRenderPass::~SRenderPass()
{
    vkDestroyRenderPass( self, renderPass, nullptr );
    vkDestroyFramebuffer( self, frameBuffer, nullptr );
}

FawnVision::CDevice::SPipeline::SPipeline( SPipeline&& ) noexcept = default;

FawnVision::CDevice::SPipeline::SPipeline( const VkDevice& s, const VkPipeline& p )
        : self( s )
          , pipeline( p )
{
}

FawnVision::CDevice::SPipeline::~SPipeline()
{
    vkDestroyPipeline( self, pipeline, nullptr );
}
