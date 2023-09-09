#pragma once

#include "../../FawnVisionCore.h"
#include <vulkan/vulkan.hpp>

namespace DeerVulkan
{
    struct SPhysicalDeviceInfo final {
        VkPhysicalDevice device;
        VkPhysicalDeviceProperties deviceProperties;
        VkPhysicalDeviceMemoryProperties deviceMemoryProperties;
        VkPhysicalDeviceFeatures deviceFeatures;

        std::vector<VkQueueFamilyProperties> queueFamilyProperties;

        [[nodiscard]] VkFormat FindSupportedFormat( const std::vector<FawnVision::image_format>& candidates, const VkImageTiling& tiling, const VkFormatFeatureFlags& features ) const;
        [[nodiscard]] VkFormat GetDepthFormat() const;
        [[nodiscard]] VkSampleCountFlagBits GetMaxUsableSampleCount() const;
    };
    class CInstanceHolder
    {
    public:
        constexpr CInstanceHolder()                                    = default;
        constexpr CInstanceHolder( const CInstanceHolder& )            = delete;
        constexpr CInstanceHolder( CInstanceHolder&& )                 = delete;
        constexpr CInstanceHolder& operator=( const CInstanceHolder& ) = delete;
        constexpr CInstanceHolder& operator=( CInstanceHolder&& )      = delete;
        constexpr ~CInstanceHolder();
        [[nodiscard]] constexpr const VkInstance& GetHandle() const
        {
            return m_instanceHandle;
        }

    protected:
        VkInstance m_instanceHandle { VK_NULL_HANDLE };
    };

    class CInstance : public CInstanceHolder
    {
    public:
        constexpr CInstance()
            : CInstanceHolder {}
        {
        }
        ~CInstance();
        bool Initialize( const char** extensions, uint32_t extensionsCount );
        void InitializeDevice( FawnVision::Device* device, uint32_t physicalDeviceIndex ) noexcept;
        constexpr void SetSurface( const VkSurfaceKHR& newSurface )
        {
            m_surfaceKhr = newSurface;
        }
        constexpr uint32_t FindBestPhysicalDeviceIndex( const VkSurfaceKHR& surf )
        {
            for ( auto& physicalDevice : m_physicalDevices )
            {
                for ( uint32_t j = 0; j < static_cast<uint32_t>( physicalDevice.queueFamilyProperties.size() ); ++j )
                {
                    VkBool32 supportsPresent { VK_FALSE };
                    vkGetPhysicalDeviceSurfaceSupportKHR( physicalDevice.device, j, surf, &supportsPresent );
                    if ( supportsPresent && ( physicalDevice.queueFamilyProperties[ j ].queueFlags & VK_QUEUE_GRAPHICS_BIT ) && physicalDevice.deviceFeatures.samplerAnisotropy )
                    {
                        return j;
                    }
                }
            }
            return 0;
        }

    private:
        VkDebugReportCallbackEXT m_debugReport { VK_NULL_HANDLE };
        VkSurfaceKHR m_surfaceKhr { VK_NULL_HANDLE };

        std::vector<SPhysicalDeviceInfo> m_physicalDevices;

#ifdef _DEBUG
        PFN_vkCreateDebugReportCallbackEXT vkpfn_CreateDebugReportCallbackEXT   = VK_NULL_HANDLE;
        PFN_vkDestroyDebugReportCallbackEXT vkpfn_DestroyDebugReportCallbackEXT = VK_NULL_HANDLE;
#endif
    };
}// namespace DeerVulkan
