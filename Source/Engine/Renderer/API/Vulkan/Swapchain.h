#pragma once
#include "Base.h"
#include <vulkan/vulkan.hpp>

namespace DeerVulkan
{
    class CSwapchain final : public CDeviceObject
    {
    public:
        constexpr CSwapchain( FawnVision::Device device, const VkSurfaceKHR& surface )
        : CDeviceObject{device}
        , m_surfaceKhr{surface}
        {
        }
        ~CSwapchain() override;
        void Initialize( uint32_t width, uint32_t height ) noexcept;
        bool AcquireNextImage( FawnVision::Semaphore resentCompleteSemaphore, uint32_t& imageIndex ) const;
        void GetImages( std::vector<FawnVision::ImageResource>& swapChainImages, uint32_t& count ) noexcept;

        constexpr VkExtent2D GetExtend() const{return m_swapchainExtent;}
        constexpr VkSurfaceFormatKHR GetSurfaceFormat() const{return m_swapSurfaceFormat;}
        constexpr const VkSwapchainKHR& GetHandle() const{return m_swapchain;}
        constexpr uint32_t GetImageCount() const{return m_imageCount;}
        constexpr uint32_t GetMinImage() const{return m_minImageCount;}

    private:
        VkSwapchainKHR m_swapchain { VK_NULL_HANDLE };
        VkSurfaceKHR m_surfaceKhr { VK_NULL_HANDLE };
        VkSurfaceCapabilitiesKHR m_surfaceCapabilities {};
        VkSurfaceFormatKHR m_swapSurfaceFormat {};
        VkPresentModeKHR m_presentMode {};
        VkExtent2D m_swapchainExtent {};
        uint32_t m_minImageCount {};
        uint32_t m_imageCount {};

        std::vector<VkQueueFamilyProperties> m_queueFamilies;
        std::vector<VkSurfaceFormatKHR> m_surfaceFormats;
        std::vector<VkPresentModeKHR> m_presentModes;
        std::vector<VkImage> m_images;
        void GetSwapExtent( uint32_t w, uint32_t h ) noexcept;
        void GetSwapSurfaceFormat() noexcept;
        void GetQueueFamiliesProperties() noexcept;
    };
}// namespace DeerVulkan
