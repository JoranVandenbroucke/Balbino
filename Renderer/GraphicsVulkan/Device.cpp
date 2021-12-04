#include "pch.h"
#include "Device.h"

#include "Funtions.h"
#include "Instance.h"

BalVulkan::CDeviceHolder::CDeviceHolder( VkDevice device, VkAllocationCallbacks* hostAllocator )
	: m_pCallbacks( hostAllocator )
	, m_device( device )
{
}

BalVulkan::CDeviceHolder::~CDeviceHolder()
{
	if ( m_device != VK_NULL_HANDLE )
	{
		vkDestroyDevice( m_device, m_pCallbacks );
	}
}

BalVulkan::CDevice::CDevice( const SPhysicalDeviceInfo* pDeviceInfo, VkAllocationCallbacks* pCallbacks, VkDevice device )
	: CDeviceHolder( device, pCallbacks )
	, CRefCounted{}
	, m_pDeviceInfo( pDeviceInfo )
{
}

BalVulkan::CDevice::~CDevice()
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

void BalVulkan::CDevice::WaitIdle() const
{
	vkDeviceWaitIdle( m_device );
}

const VkDevice& BalVulkan::CDevice::GetVkDevice() const
{
	return m_device;
}

const BalVulkan::SPhysicalDeviceInfo* BalVulkan::CDevice::GetPhysicalDeviceInfo() const
{
	return m_pDeviceInfo;
}


BalVulkan::CDevice* BalVulkan::CDevice::Create( const SPhysicalDeviceInfo* pDeviceInfo, VkAllocationCallbacks* pCallbacks, const std::vector<const char*>& layersToEnable, const std::vector<const char*>& extensionsToEnable )
{
	( void ) layersToEnable;
	( void ) extensionsToEnable;
	std::vector<VkDeviceQueueCreateInfo> wantedQueues;
	const std::vector queuePriorities( pDeviceInfo->queueFamilyProperties.size(), 1.0f );
	//VkDeviceQueueCreateInfo queueInfo{
	//	.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
	//	.flags = 0,
	//};

	//for ( uint32_t i{ 0 }, n{ static_cast< uint32_t >( pDeviceInfo->queueFamilyProperties.size() ) }; i < n; ++i )
	//{
	//	queueInfo.queueFamilyIndex = i;
	//	queueInfo.queueCount = pDeviceInfo->queueFamilyProperties[i].queueCount;
	//	queueInfo.pQueuePriorities = queuePriorities.data();

	//	wantedQueues.push_back( queueInfo );
	//}
	//const VkDeviceCreateInfo createInfo{
	//	.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
	//	.queueCreateInfoCount = static_cast< uint32_t >(wantedQueues.size() ),	//1
	//	.pQueueCreateInfos = wantedQueues.data(),
	//	.enabledLayerCount = static_cast< uint32_t >( layersToEnable.size() ),
	//	.ppEnabledLayerNames = !layersToEnable.empty() ? layersToEnable.data() : nullptr,
	//	.enabledExtensionCount = static_cast< uint32_t >( extensionsToEnable.size() ),	//1
	//	.ppEnabledExtensionNames = extensionsToEnable.data(),
	//	.pEnabledFeatures = nullptr,
	//};
	constexpr int deviceExtensionCount{ 1 };
	const char* deviceExtensions[]{ VK_KHR_SWAPCHAIN_EXTENSION_NAME };
	const float queuePriority[]{ 1.0f };
	const VkDeviceQueueCreateInfo queueInfo[1]{
		{
			.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
			.queueFamilyIndex = 0,
			.queueCount = 1,
			.pQueuePriorities = queuePriority,
		}
	};
	const VkDeviceCreateInfo createInfo{
		.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
		.queueCreateInfoCount = static_cast< uint32_t >( std::size( queueInfo ) ),
		.pQueueCreateInfos = queueInfo,
		.enabledExtensionCount = deviceExtensionCount,
		.ppEnabledExtensionNames = deviceExtensions,
	};
	VkDevice device;
	CheckVkResult( vkCreateDevice( pDeviceInfo->device, &createInfo, pCallbacks, &device ), "Could not create a device" );
	CDevice* pDevice{ new CDevice{ pDeviceInfo, pCallbacks, device } };
	return pDevice;
}

BalVulkan::CDevice::SRenderPass::SRenderPass( SRenderPass&& ) noexcept = default;

BalVulkan::CDevice::SRenderPass::SRenderPass( const VkDevice & s, const VkRenderPass & r, const VkFramebuffer & f )
	: self( s )
	, renderPass( r )
	, frameBuffer( f )
{
}

BalVulkan::CDevice::SRenderPass::~SRenderPass()
{
	vkDestroyRenderPass( self, renderPass, nullptr );
	vkDestroyFramebuffer( self, frameBuffer, nullptr );
}

BalVulkan::CDevice::SPipeline::SPipeline( SPipeline&& ) noexcept = default;

BalVulkan::CDevice::SPipeline::SPipeline( const VkDevice & s, const VkPipeline & p )
	: self( s )
	, pipeline( p )
{
}

BalVulkan::CDevice::SPipeline::~SPipeline()
{
	vkDestroyPipeline( self, pipeline, nullptr );
}
