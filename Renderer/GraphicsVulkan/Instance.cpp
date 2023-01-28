#include "Instance.h"
#include "Device.h"
#include "Funtions.h"

#if defined(_DEBUG) && !defined(BL_EDITOR)

static VKAPI_ATTR VkBool32 VKAPI_CALL DebugReport( VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objectType, uint64_t object, size_t location, int32_t messageCode, const char* pLayerPrefix, const char* pMessage, void* pUserData )
{
    (void) flags;
    (void) object;
    (void) location;
    (void) messageCode;
    (void) pUserData;
    (void) pLayerPrefix; // Unused arguments
    fprintf(stderr, "[vulkan] Debug report from ObjectType: %i\r\nMessage: %s\r\n\r\n", objectType, pMessage );
    return VK_FALSE;
}

#endif //_DEBUG && !BL_EDITOR

FawnVision::CInstanceHolder::CInstanceHolder()
        : m_instanceHandle( VK_NULL_HANDLE )
{
}

FawnVision::CInstanceHolder::~CInstanceHolder()
{
    if ( m_instanceHandle != VK_NULL_HANDLE )
    {
        vkDestroyInstance( m_instanceHandle, VK_NULL_HANDLE );
    }
}

FawnVision::CInstance::CInstance()
        : CInstanceHolder{}
          , m_debugReport{ VK_NULL_HANDLE }
          , m_pCallbacks{ nullptr }
          , m_surfaceKhr{ VK_NULL_HANDLE }
{
}

FawnVision::CInstance::~CInstance()
{
    #ifdef _DEBUG
    if ( vkpfn_DestroyDebugReportCallbackEXT && m_debugReport )
    {
        vkpfn_DestroyDebugReportCallbackEXT( m_instanceHandle, m_debugReport, m_pCallbacks );
    }
    vkDestroySurfaceKHR( m_instanceHandle, m_surfaceKhr, m_pCallbacks );
    //vkDestroyDebugReportCallbackEXT( m_instanceHandle, m_debugReport, m_pCallbacks );
    #endif
}

bool FawnVision::CInstance::Initialize( const char** extensions, const uint32_t extensionsCount )
{
    if ( m_instanceHandle )
    {
        return false;
    }
    
    // CreateNew Vulkan Instance
    {
        VkApplicationInfo    applicationInfo{
                .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO, .pNext = nullptr, .pApplicationName = "Balbino",  //todo read form file or pass in
                .applicationVersion = VK_MAKE_VERSION( 0, 0, 0 ),   //todo read from file
                .pEngineName = "Balbino Engine", .engineVersion = VK_MAKE_VERSION( 0,
                                                                                   1,
                                                                                   12 ), .apiVersion = VK_API_VERSION_1_3,
        };
        VkInstanceCreateInfo createInfo{
                .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO, .pNext = nullptr, .flags = 0, .pApplicationInfo = &applicationInfo,
                //.enabledLayerCount = m_enabledInstanceLayers.size(),
                //.ppEnabledLayerNames = (!m_enabledInstanceLayers.empty()) ? m_enabledInstanceLayers.data() : NULL,
                .enabledExtensionCount = extensionsCount, .ppEnabledExtensionNames = extensions,
        };
        VkResult             err = VK_ERROR_INITIALIZATION_FAILED;
        #if defined(_DEBUG)
        
        // Enabling validation layers
        const char* layers[]{ "VK_LAYER_KHRONOS_validation" };
        createInfo.enabledLayerCount   = 1;
        createInfo.ppEnabledLayerNames = layers;
        
        // Enable debug report extension (we need additional storage, so we duplicate the user array to add our new extension to it)
        const auto extensionsExt{
                static_cast<const char**>( malloc( sizeof( const char* ) * ( extensionsCount + 1 )))
        };
        memcpy( extensionsExt, extensions, extensionsCount * sizeof( const char* ));
        extensionsExt[extensionsCount] = "VK_EXT_debug_report";
        createInfo.enabledExtensionCount   = extensionsCount + 1;
        createInfo.ppEnabledExtensionNames = extensionsExt;
        
        err = vkCreateInstance( &createInfo, m_pCallbacks, &m_instanceHandle );
        CheckVkResult( err );
        free( extensionsExt );
        if ( err == VK_SUCCESS )
        {
            vkpfn_CreateDebugReportCallbackEXT  = reinterpret_cast<PFN_vkCreateDebugReportCallbackEXT>( vkGetInstanceProcAddr(
                    m_instanceHandle, "vkCreateDebugReportCallbackEXT"
            ));
            vkpfn_DestroyDebugReportCallbackEXT = reinterpret_cast<PFN_vkDestroyDebugReportCallbackEXT>( vkGetInstanceProcAddr(
                    m_instanceHandle, "vkDestroyDebugReportCallbackEXT"
            ));
            if ( vkpfn_CreateDebugReportCallbackEXT && vkpfn_DestroyDebugReportCallbackEXT )
            {
                VkDebugReportCallbackCreateInfoEXT debugReportCi{};
                debugReportCi.sType       = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
                debugReportCi.flags       = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT | VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT;
                debugReportCi.pfnCallback = DebugReport;
                debugReportCi.pUserData   = nullptr;
                vkpfn_CreateDebugReportCallbackEXT( m_instanceHandle, &debugReportCi, m_pCallbacks, &m_debugReport );
                CheckVkResult( err );
            }
        }
        #endif
        if ( err != VK_SUCCESS )
        {
            CheckVkResult( vkCreateInstance( &createInfo, m_pCallbacks, &m_instanceHandle ));
        }
    }
    //get graphics stuff
    {
        uint32_t gpuCount{};
        CheckVkResult( vkEnumeratePhysicalDevices( m_instanceHandle, &gpuCount, nullptr ));
        
        if ( gpuCount == 0 )
        {
            throw std::runtime_error( "failed to find GPUs with Vulkan support!" );
        }
        
        std::vector<VkPhysicalDevice> gpus( gpuCount, nullptr );
        CheckVkResult( vkEnumeratePhysicalDevices( m_instanceHandle, &gpuCount, gpus.data()));
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
            vkGetPhysicalDeviceQueueFamilyProperties( info.device, &count, info.queueFamilyProperties.data());
        }
    }
    return true;
}

void FawnVision::CInstance::SetSurface( const VkSurfaceKHR& newSurface )
{
    m_surfaceKhr = newSurface;
}

uint8_t FawnVision::CInstance::DeviceCount() const
{
    return static_cast<uint8_t>( m_physicalDevices.size());
}

FawnVision::CDevice* FawnVision::CInstance::CreateDevice( uint32_t physicalDeviceIndex )
{
    const SPhysicalDeviceInfo& info{
            m_physicalDevices[std::max(
                    static_cast<uint32_t>( 0 ), std::min(
                            physicalDeviceIndex, static_cast<uint32_t>( m_physicalDevices.size())))]
    };
    return CDevice::Create(
            &info, m_pCallbacks
            #ifdef _DEBUG
            , { "VK_LAYER_KHRONOS_validation" }
            #else
            , {}
            #endif
            , { VK_KHR_SWAPCHAIN_EXTENSION_NAME }
    );
}

uint32_t FawnVision::CInstance::FindBestPhysicalDeviceIndex( const VkSurfaceKHR& surf )
{
    for ( uint32_t i = 0; i < static_cast<uint32_t>( m_physicalDevices.size()); ++i )
    {
        for ( uint32_t j = 0; j < static_cast<uint32_t>( m_physicalDevices[i].queueFamilyProperties.size()); ++j )
        {
            VkBool32 supportsPresent{ VK_FALSE };
            vkGetPhysicalDeviceSurfaceSupportKHR( m_physicalDevices[i].device, j, surf, &supportsPresent );
            if ( supportsPresent && ( m_physicalDevices[i].queueFamilyProperties[j].queueFlags & VK_QUEUE_GRAPHICS_BIT ) && m_physicalDevices[i].deviceFeatures.samplerAnisotropy )
            {
                return j;
            }
        }
    }
    return 0;
}

VkDebugReportCallbackEXT FawnVision::CInstance::GetReportCallbackExt() const
{
    return m_debugReport;
}

VkSurfaceKHR FawnVision::CInstance::GetSurface() const
{
    return m_surfaceKhr;
}

FawnVision::CInstance* FawnVision::CInstance::CreateNew()
{
    return new CInstance{};
}

const VkInstance& FawnVision::CInstanceHolder::GetHandle() const
{
    return m_instanceHandle;
}

VkFormat FawnVision::SPhysicalDeviceInfo::FindSupportedFormat( const std::vector<VkFormat>& candidates, const VkImageTiling& tiling, const VkFormatFeatureFlags& features ) const
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

VkFormat FawnVision::SPhysicalDeviceInfo::GetDepthFormat() const
{
    return FindSupportedFormat(
            std::vector{
                    VK_FORMAT_D32_SFLOAT_S8_UINT,
                    VK_FORMAT_D32_SFLOAT,
                    VK_FORMAT_D24_UNORM_S8_UINT,
                    VK_FORMAT_D16_UNORM_S8_UINT,
                    VK_FORMAT_D16_UNORM
            }, VK_IMAGE_TILING_OPTIMAL, VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
    );
}

VkSampleCountFlagBits FawnVision::SPhysicalDeviceInfo::GetMaxUsableSampleCount() const
{
    //VkPhysicalDeviceProperties physicalDeviceProperties;
    //vkGetPhysicalDeviceProperties( device, &physicalDeviceProperties );
    
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