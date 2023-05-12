#pragma once

#include <vulkan/vulkan.hpp>

namespace FawnVision
{
    class CDevice;
    
    struct SPhysicalDeviceInfo
    {
        VkPhysicalDevice                 device;
        VkPhysicalDeviceProperties       deviceProperties;
        VkPhysicalDeviceMemoryProperties deviceMemoryProperties;
        VkPhysicalDeviceFeatures         deviceFeatures;
        
        std::vector<VkQueueFamilyProperties>                                                      queueFamilyProperties;
        
        [[nodiscard]] VkFormat FindSupportedFormat( const std::vector<VkFormat>& candidates, const VkImageTiling& tiling, const VkFormatFeatureFlags& features ) const;
        [[nodiscard]] VkFormat GetDepthFormat() const;
        [[nodiscard]] VkSampleCountFlagBits GetMaxUsableSampleCount() const;
    };
    
    class CInstanceHolder
    {
    public:
        CInstanceHolder();
        CInstanceHolder( const CInstanceHolder& ) = delete;
        CInstanceHolder( CInstanceHolder&& ) = delete;
        CInstanceHolder& operator=( const CInstanceHolder& ) = delete;
        CInstanceHolder& operator=( CInstanceHolder&& ) = delete;
        ~CInstanceHolder();
        
        [[nodiscard]] const VkInstance& GetHandle() const;
    protected:
        VkInstance m_instanceHandle;
    };
    
    class CInstance : public CInstanceHolder
    {
    public:
        CInstance();
        ~CInstance();
        
        bool Initialize( const char** extensions, uint32_t extensionsCount );
        void SetSurface( const VkSurfaceKHR& newSurface );
        [[nodiscard]] uint8_t DeviceCount() const;
        void InitializeDevice( CDevice*& pDevice, uint32_t physicalDeviceIndex );
        uint32_t FindBestPhysicalDeviceIndex( const VkSurfaceKHR& surf );
        [[nodiscard]] VkDebugReportCallbackEXT GetReportCallbackExt() const;
        [[nodiscard]] VkSurfaceKHR GetSurface() const;
    
    private:
        VkDebugReportCallbackEXT m_debugReport;
        VkAllocationCallbacks* m_pCallbacks;
        VkSurfaceKHR m_surfaceKhr;
        
        std::vector<SPhysicalDeviceInfo> m_physicalDevices;
        
        #ifdef _DEBUG
        PFN_vkCreateDebugReportCallbackEXT  vkpfn_CreateDebugReportCallbackEXT  = nullptr;
        PFN_vkDestroyDebugReportCallbackEXT vkpfn_DestroyDebugReportCallbackEXT = nullptr;
        #endif
    };
}
