#include "Instance.h"
#include "Device.h"

#if defined( _DEBUG ) && !defined( BL_EDITOR )

inline static VKAPI_ATTR VkBool32 VKAPI_CALL DebugReport( VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objectType, uint64_t object, size_t location, int32_t messageCode, const char* pLayerPrefix, const char* pMessage, void* pUserData )
{
    (void) objectType;
    (void) object;
    (void) location;
    (void) pUserData;
    const char* debugType { ( flags & VK_DEBUG_REPORT_ERROR_BIT_EXT )                     ? "ERROR"
                                : ( flags & VK_DEBUG_REPORT_WARNING_BIT_EXT )             ? "WARNING"
                                : ( flags & VK_DEBUG_REPORT_INFORMATION_BIT_EXT )         ? "INFO"
                                : ( flags & VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT ) ? "PERFORMANCE WARNING"
                                : ( flags & VK_DEBUG_REPORT_DEBUG_BIT_EXT )               ? "DEBUG"
                                                                                          : "UNKNOWN" };
    std::cout << std::format( "[{} {}] {}: {}\n", debugType, messageCode, pLayerPrefix, pMessage ) << std::endl;
    return VK_FALSE;
}
#endif//_DEBUG && !BL_EDITOR

VkFormat DeerVulkan::SPhysicalDeviceInfo::FindSupportedFormat( const std::vector<FawnVision::image_format>& candidates, const VkImageTiling& tiling, const VkFormatFeatureFlags& features ) const
{
    for ( const FawnVision::image_format format : candidates )
    {
        VkFormatProperties props;
        VkFormat f { (VkFormat)ConvertImageFormat( format ) };
        vkGetPhysicalDeviceFormatProperties( device, f, &props );

        if ( tiling == VK_IMAGE_TILING_LINEAR && ( props.linearTilingFeatures & features ) == features )
        {
            return f;
        }
        if ( tiling == VK_IMAGE_TILING_OPTIMAL && ( props.optimalTilingFeatures & features ) == features )
        {
            return f;
        }
    }

    throw std::invalid_argument( "failed to find supported format!" );
}
VkFormat DeerVulkan::SPhysicalDeviceInfo::GetDepthFormat() const
{
    return FindSupportedFormat( std::vector { FawnVision::image_format::d32_sfloat_s8_uint, FawnVision::image_format::d32_sfloat, FawnVision::image_format::d24_unorm_s8_uint, FawnVision::image_format::d16_unorm_s8_uint, FawnVision::image_format::d16_unorm }, VK_IMAGE_TILING_OPTIMAL,
                                VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT );
}
VkSampleCountFlagBits DeerVulkan::SPhysicalDeviceInfo::GetMaxUsableSampleCount() const
{
    VkSampleCountFlags counts = deviceProperties.limits.framebufferColorSampleCounts & deviceProperties.limits.framebufferDepthSampleCounts;
    if ( counts & VK_SAMPLE_COUNT_64_BIT )
    {
        return VK_SAMPLE_COUNT_64_BIT;
    }
    if ( counts & VK_SAMPLE_COUNT_32_BIT )
    {
        return VK_SAMPLE_COUNT_32_BIT;
    }
    if ( counts & VK_SAMPLE_COUNT_16_BIT )
    {
        return VK_SAMPLE_COUNT_16_BIT;
    }
    if ( counts & VK_SAMPLE_COUNT_8_BIT )
    {
        return VK_SAMPLE_COUNT_8_BIT;
    }
    if ( counts & VK_SAMPLE_COUNT_4_BIT )
    {
        return VK_SAMPLE_COUNT_4_BIT;
    }
    if ( counts & VK_SAMPLE_COUNT_2_BIT )
    {
        return VK_SAMPLE_COUNT_2_BIT;
    }

    return VK_SAMPLE_COUNT_1_BIT;
}

constexpr DeerVulkan::CInstanceHolder::~CInstanceHolder()
{
    if ( m_instanceHandle != VK_NULL_HANDLE )
    {
        vkDestroyInstance( m_instanceHandle, VK_NULL_HANDLE );
    }
}

DeerVulkan::CInstance::~CInstance()
{
#ifdef _DEBUG
    if ( vkpfn_DestroyDebugReportCallbackEXT && m_debugReport )
    {
        vkpfn_DestroyDebugReportCallbackEXT( m_instanceHandle, m_debugReport, VK_NULL_HANDLE );
    }
    vkDestroySurfaceKHR( m_instanceHandle, m_surfaceKhr, VK_NULL_HANDLE );
//vkDestroyDebugReportCallbackEXT( m_instanceHandle, m_debugReport, m_pCallbacks );
#endif
}
bool DeerVulkan::CInstance::Initialize( const char** extensions, uint32_t extensionsCount )
{
    if ( m_instanceHandle )
    {
        return false;
    }

    // CreateNew Vulkan Instance
    {
        VkApplicationInfo applicationInfo {
            .sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO,
            .pNext              = VK_NULL_HANDLE,
            .pApplicationName   = "DeerVulkan",              //todo:read form file or pass in
            .applicationVersion = VK_MAKE_VERSION( 0, 0, 0 ),//todo:read from file
            .pEngineName        = "DeerVulkan Engine",
            .engineVersion      = VK_MAKE_VERSION( 0, 1, 15 ),
            .apiVersion         = VK_API_VERSION_1_3,
        };
        VkInstanceCreateInfo createInfo {
            .sType                   = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
            .pNext                   = VK_NULL_HANDLE,
            .flags                   = 0,
            .pApplicationInfo        = &applicationInfo,
            .enabledExtensionCount   = extensionsCount,
            .ppEnabledExtensionNames = extensions,
        };
        VkResult err = VK_ERROR_INITIALIZATION_FAILED;
#if defined( _DEBUG )

        // Enabling validation layers
        const char* layers[] { "VK_LAYER_KHRONOS_validation" };
        createInfo.enabledLayerCount   = 1;
        createInfo.ppEnabledLayerNames = layers;

        // Enable debug report extension (we need additional storage, so we duplicate the user array to mix_mode_add our new extension to it)
        const auto extensionsExt { static_cast<const char**>( malloc( sizeof( const char* ) * ( extensionsCount + 1 ) ) ) };
        memcpy( extensionsExt, extensions, extensionsCount * sizeof( const char* ) );
        extensionsExt[ extensionsCount ]   = "VK_EXT_debug_report";
        createInfo.enabledExtensionCount   = extensionsCount + 1;
        createInfo.ppEnabledExtensionNames = extensionsExt;

        err = vkCreateInstance( &createInfo, VK_NULL_HANDLE, &m_instanceHandle );
        CheckVkResult( err );
        free( extensionsExt );
        if ( err == VK_SUCCESS )
        {
            vkpfn_CreateDebugReportCallbackEXT  = reinterpret_cast<PFN_vkCreateDebugReportCallbackEXT>( vkGetInstanceProcAddr( m_instanceHandle, "vkCreateDebugReportCallbackEXT" ) );
            vkpfn_DestroyDebugReportCallbackEXT = reinterpret_cast<PFN_vkDestroyDebugReportCallbackEXT>( vkGetInstanceProcAddr( m_instanceHandle, "vkDestroyDebugReportCallbackEXT" ) );
            if ( vkpfn_CreateDebugReportCallbackEXT && vkpfn_DestroyDebugReportCallbackEXT )
            {
                VkDebugReportCallbackCreateInfoEXT debugReportCi {};
                debugReportCi.sType       = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
                debugReportCi.flags       = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT | VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT;
                debugReportCi.pfnCallback = DebugReport;
                debugReportCi.pUserData   = BALBINO_NULL;
                vkpfn_CreateDebugReportCallbackEXT( m_instanceHandle, &debugReportCi, VK_NULL_HANDLE, &m_debugReport );
                CheckVkResult( err );
            }
        }
#endif
        if ( err != VK_SUCCESS )
        {
            CheckVkResult( vkCreateInstance( &createInfo, VK_NULL_HANDLE, &m_instanceHandle ) );
        }
    }
    //get graphics stuff
    {
        uint32_t gpuCount {};
        CheckVkResult( vkEnumeratePhysicalDevices( m_instanceHandle, &gpuCount, VK_NULL_HANDLE ) );

        if ( gpuCount == 0 )
        {
            throw std::length_error( "failed to find GPUs with Vulkan support!" );
        }

        std::vector<VkPhysicalDevice> gpus( gpuCount, BALBINO_NULL );
        CheckVkResult( vkEnumeratePhysicalDevices( m_instanceHandle, &gpuCount, gpus.data() ) );
        m_physicalDevices.resize( gpuCount );
        for ( uint32_t i = 0; i < gpuCount; ++i )
        {
            uint32_t count;
            SPhysicalDeviceInfo& info { m_physicalDevices[ i ] };
            info.device = gpus[ i ];
            vkGetPhysicalDeviceProperties( gpus[ i ], &info.deviceProperties );
            vkGetPhysicalDeviceMemoryProperties( gpus[ i ], &info.deviceMemoryProperties );
            vkGetPhysicalDeviceQueueFamilyProperties( info.device, &count, VK_NULL_HANDLE );
            info.queueFamilyProperties.resize( count );
            vkGetPhysicalDeviceQueueFamilyProperties( info.device, &count, info.queueFamilyProperties.data() );
        }
    }
    return true;
}
void DeerVulkan::CInstance::InitializeDevice( FawnVision::Device* device, uint32_t physicalDeviceIndex ) noexcept
{
    const SPhysicalDeviceInfo& info { m_physicalDevices[ std::max( static_cast<uint32_t>( 0 ), std::min( physicalDeviceIndex, static_cast<uint32_t>( m_physicalDevices.size() ) ) ) ] };
    auto* temp = new CDevice { info };
    temp->Initialize(
#ifdef _DEBUG
        { "VK_LAYER_KHRONOS_validation" }
#else
        {}
#endif
        ,
        { VK_KHR_SWAPCHAIN_EXTENSION_NAME } );

    *device = temp;
}