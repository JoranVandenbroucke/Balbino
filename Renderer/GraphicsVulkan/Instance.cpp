#include "pch.h"
#include "Instance.h"
#include "Device.h"
#include "Funtions.h"

#if defined(_DEBUG) && !defined(BL_EDITOR)
static VKAPI_ATTR VkBool32 VKAPI_CALL DebugReport( VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objectType,
												   uint64_t object, size_t location, int32_t messageCode,
												   const char* pLayerPrefix, const char* pMessage, void* pUserData )
{
	( void ) flags;
	( void ) object;
	( void ) location;
	( void ) messageCode;
	( void ) pUserData;
	( void ) pLayerPrefix; // Unused arguments
	fprintf( stderr, "[vulkan] Debug report from ObjectType: %i\r\nMessage: %s\r\n\r\n", objectType, pMessage );
	return VK_FALSE;
}
#endif //_DEBUG && !BL_EDITOR

BalVulkan::CInstanceHolder::CInstanceHolder()
	: m_instanceHandle( VK_NULL_HANDLE )
{
}

BalVulkan::CInstanceHolder::~CInstanceHolder()
{
	if ( m_instanceHandle != VK_NULL_HANDLE )
	{
		vkDestroyInstance( m_instanceHandle, VK_NULL_HANDLE );
	}
}

BalVulkan::CInstance::CInstance()
	: CInstanceHolder{}
	, m_debugReport{ nullptr }
	, m_pCallbacks{ nullptr }
	, m_surfaceKhr{ nullptr }
{
}

BalVulkan::CInstance::~CInstance()
{
	if ( vkpfn_DestroyDebugReportCallbackEXT && m_debugReport )
	{
		vkpfn_DestroyDebugReportCallbackEXT( m_instanceHandle, m_debugReport, m_pCallbacks );
	}
	vkDestroySurfaceKHR( m_instanceHandle, m_surfaceKhr, m_pCallbacks );
	//vkDestroyDebugReportCallbackEXT( m_instanceHandle, m_debugReport, m_pCallbacks );
}

bool BalVulkan::CInstance::Initialize( const char** extensions, const uint32_t extensionsCount )
{
	if(m_instanceHandle )
		return false;

	// Create Vulkan Instance
	{
		VkApplicationInfo applicationInfo
		{
			.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
			.pNext = NULL,
			.pApplicationName = "Balbino",
			.applicationVersion = 0,
			.pEngineName = "Balbino Engine",
			.engineVersion = 0,
			.apiVersion = VK_API_VERSION_1_2,
		};
		VkInstanceCreateInfo createInfo{
			.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
			.pNext = nullptr,
			.flags = 0,
			.pApplicationInfo = &applicationInfo,
			//.enabledLayerCount = m_enabledInstanceLayers.size(),
			//.ppEnabledLayerNames = (!m_enabledInstanceLayers.empty()) ? m_enabledInstanceLayers.data() : NULL,
			.enabledExtensionCount = extensionsCount,
			.ppEnabledExtensionNames = extensions,
		};
		VkResult err = VK_ERROR_INITIALIZATION_FAILED;
#if defined(_DEBUG)

		// Enabling validation layers
		const char* layers[]{ "VK_LAYER_KHRONOS_validation" };
		createInfo.enabledLayerCount = 1;
		createInfo.ppEnabledLayerNames = layers;

		// Enable debug report extension (we need additional storage, so we duplicate the user array to add our new extension to it)
		const auto extensionsExt{ static_cast< const char** >( malloc( sizeof( const char* ) * ( extensionsCount + 1 ) ) ) };
		memcpy( extensionsExt, extensions, extensionsCount * sizeof( const char* ) );
		extensionsExt[extensionsCount] = "VK_EXT_debug_report";
		createInfo.enabledExtensionCount = extensionsCount + 1;
		createInfo.ppEnabledExtensionNames = extensionsExt;

		err = vkCreateInstance( &createInfo, m_pCallbacks, &m_instanceHandle );
		CheckVkResult( err );
		free( extensionsExt );
		if ( err == VK_SUCCESS )
		{
			vkpfn_CreateDebugReportCallbackEXT =
				reinterpret_cast< PFN_vkCreateDebugReportCallbackEXT >( vkGetInstanceProcAddr( m_instanceHandle, "vkCreateDebugReportCallbackEXT" ) );
			vkpfn_DestroyDebugReportCallbackEXT =
				reinterpret_cast< PFN_vkDestroyDebugReportCallbackEXT >( vkGetInstanceProcAddr( m_instanceHandle, "vkDestroyDebugReportCallbackEXT" ) );
			if ( vkpfn_CreateDebugReportCallbackEXT && vkpfn_DestroyDebugReportCallbackEXT )
			{
				VkDebugReportCallbackCreateInfoEXT debugReportCi{};
				debugReportCi.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
				debugReportCi.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT | VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT;
				debugReportCi.pfnCallback = DebugReport;
				debugReportCi.pUserData = nullptr;
				vkpfn_CreateDebugReportCallbackEXT( m_instanceHandle, &debugReportCi, m_pCallbacks, &m_debugReport );
				CheckVkResult( err );
			}
		}
#endif
		if ( err != VK_SUCCESS )
		{
			if ( CheckVkResult( vkCreateInstance( &createInfo, m_pCallbacks, &m_instanceHandle ) ) )
				return true;
			return false;
		}
	}
	//get graphics stuff
	{
		uint32_t gpuCount{};
		CheckVkResult( vkEnumeratePhysicalDevices( m_instanceHandle, &gpuCount, nullptr ) );
		std::vector<VkPhysicalDevice> gpus( gpuCount, nullptr );
		CheckVkResult( vkEnumeratePhysicalDevices( m_instanceHandle, &gpuCount, gpus.data() ) );
		m_physicalDevices.resize( gpuCount );
		for ( uint32_t i = 0; i < gpuCount; ++i )
		{
			uint32_t count;
			SPhysicalDeviceInfo& info{ m_physicalDevices[i] };
			info.device = gpus[i];
			vkGetPhysicalDeviceProperties( gpus[i], &info.deviceProperties );
			vkGetPhysicalDeviceMemoryProperties( gpus[i], &info.deviceMemoryProperties );
			vkGetPhysicalDeviceQueueFamilyProperties( info.device, &count, nullptr );
			info.queueFamilyProperties.resize( count );
			vkGetPhysicalDeviceQueueFamilyProperties( info.device, &count, info.queueFamilyProperties.data() );
		}
	}
	return true;
}

void BalVulkan::CInstance::SetSurface( const VkSurfaceKHR& newSurface )
{
	if ( !m_surfaceKhr )
		m_surfaceKhr = newSurface;
}

uint8_t BalVulkan::CInstance::DeviceCount() const
{
	return static_cast<uint8_t>( m_physicalDevices.size() );
}

BalVulkan::CDevice* BalVulkan::CInstance::CreateDevice( uint32_t physicalDeviceIndex )
{
	const SPhysicalDeviceInfo& info{ m_physicalDevices[std::max( static_cast<uint32_t>( 0 ),std::min( physicalDeviceIndex, static_cast<uint32_t>( m_physicalDevices.size() ) ) )] };
	return CDevice::Create( &info, m_pCallbacks, {}, { "VK_KHR_swapchain" } );
}

uint32_t BalVulkan::CInstance::FindBestPhysicalDeviceIndex(const VkSurfaceKHR& surf)
{
	for ( uint32_t i = 0; i < static_cast< uint32_t >( m_physicalDevices.size() ); ++i )
	{
		for ( uint32_t j = 0; i < static_cast< uint32_t >( m_physicalDevices[i].queueFamilyProperties.size() ); ++j )
		{
			VkBool32 supportsPresent{ VK_FALSE };
			vkGetPhysicalDeviceSurfaceSupportKHR( m_physicalDevices[i].device, j, surf, &supportsPresent );
				if ( supportsPresent && (m_physicalDevices[i].queueFamilyProperties[j].queueFlags & VK_QUEUE_GRAPHICS_BIT ))
					 return j;
		}
	}
	return 0;
}

BalVulkan::CInstance* BalVulkan::CInstance::CreateNew()
{
	return new CInstance{};
}

const VkInstance& BalVulkan::CInstanceHolder::GetHandle() const
{
	return m_instanceHandle;
}

VkFormat BalVulkan::SPhysicalDeviceInfo::FindSupportedFormat( const std::vector<VkFormat>&candidates, const VkImageTiling & tiling, const VkFormatFeatureFlags & features ) const
{
	for ( const VkFormat format : candidates )
	{
		VkFormatProperties props;
			vkGetPhysicalDeviceFormatProperties( device, format, &props );

		if ( tiling == VK_IMAGE_TILING_LINEAR && ( props.linearTilingFeatures & features ) == features )
		{
			return format;
		}
		if ( tiling == VK_IMAGE_TILING_OPTIMAL && ( props.optimalTilingFeatures & features ) == features )
		{
			return format;
		}
	}

	throw std::runtime_error( "failed to find supported format!" );
}
