//
// Copyright (c) 2024.
// Author: Joran.
//

#pragma once
#include "../DeerVulkan_Core.hpp"

#include "VkDevice.hpp"
#include "VkSemaphore.hpp"
#include "VkSurface.hpp"

#include <cstddef>
#include <vector>

namespace DeerVulkan
{
struct SSwapChainSupportDetails
{
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
    VkSurfaceCapabilitiesKHR capabilities{};
};

struct SVkSwapChain
{
    std::vector<SVkImage> images;
    std::vector<SVkImageView> imageViews;
    VkExtent2D extent{};
    VkSwapchainKHR swapchain;
    VkFormat imageFormat;
    uint32_t currentFrameIdx;
};

inline auto ChooseSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& formats) noexcept -> VkSurfaceFormatKHR
{
    if (formats.empty())
    {
        return {VK_FORMAT_UNDEFINED, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR};
    }
    for (const auto& format : formats)
    {
        if (format.format == VK_FORMAT_R8G8B8A8_SRGB && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
        {
            return format;
        }
    }
    return formats[0];
}

[[nodiscard]] inline auto ChoosePresentMode(const std::vector<VkPresentModeKHR>& presentModes) noexcept -> VkPresentModeKHR
{
    for (const auto& presentMode : presentModes)
    {
        if (presentMode == VK_PRESENT_MODE_MAILBOX_KHR)
        {
            return presentMode;
        }
    }

    return VK_PRESENT_MODE_FIFO_KHR;
}

inline auto GetExtent(const VkSurfaceCapabilitiesKHR& capabilities, const int32_t width, const int32_t height) noexcept -> VkExtent2D
{
    if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
    {
        return capabilities.currentExtent;
    }

    const VkExtent2D actualExtent{std::clamp(static_cast<uint32_t>(width), capabilities.minImageExtent.width, capabilities.maxImageExtent.width),
                                  std::clamp(static_cast<uint32_t>(height), capabilities.minImageExtent.height, capabilities.maxImageExtent.height)};

    return actualExtent;
}

inline auto GenerateSupportDetail(const SVkDevice& device, const SVkSurface& surface) noexcept -> SSwapChainSupportDetails
{
    if (device.device == VK_NULL_HANDLE || surface.surface == VK_NULL_HANDLE)
    {
        return SSwapChainSupportDetails{};
    }
    SSwapChainSupportDetails details;
    if (!CheckVkResult(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device.deviceInfo.device, surface.surface, &details.capabilities)))
    {
        return SSwapChainSupportDetails{};
    }

    uint32_t formatCount{};
    if (!CheckVkResult(vkGetPhysicalDeviceSurfaceFormatsKHR(device.deviceInfo.device, surface.surface, &formatCount, VK_NULL_HANDLE)))
    {
        return SSwapChainSupportDetails{};
    }
    if (formatCount != 0)
    {
        details.formats.resize(formatCount);
        if (!CheckVkResult(vkGetPhysicalDeviceSurfaceFormatsKHR(device.deviceInfo.device, surface.surface, &formatCount, details.formats.data())))
        {
            return SSwapChainSupportDetails{};
        }
    }

    uint32_t presentModeCount{};
    if (!CheckVkResult(vkGetPhysicalDeviceSurfacePresentModesKHR(device.deviceInfo.device, surface.surface, &presentModeCount, VK_NULL_HANDLE)))
    {
        return SSwapChainSupportDetails{};
    }
    if (presentModeCount != 0)
    {
        details.presentModes.resize(presentModeCount);
        if (!CheckVkResult(vkGetPhysicalDeviceSurfacePresentModesKHR(device.deviceInfo.device, surface.surface, &presentModeCount, details.presentModes.data())))
        {
            return SSwapChainSupportDetails{};
        }
    }

    return details;
}

inline auto Initialize(const SVkDevice& device, const SVkSurface& surface, SVkSwapChain& swapChain, const int32_t width, const int32_t height) noexcept -> int32_t
{
    if (device.device == VK_NULL_HANDLE || surface.surface == VK_NULL_HANDLE || swapChain.swapchain != VK_NULL_HANDLE || width <= 0 || height <= 0)
    {
        return -1;
    }
    const auto& [formats, presentModes, capabilities]{GenerateSupportDetail(device, surface)};
    const auto [format, colorSpace]{ChooseSurfaceFormat(formats)};
    const VkPresentModeKHR presentMode{ChoosePresentMode(presentModes)};
    swapChain.extent = GetExtent(capabilities, width, height);

    uint32_t imageCount{capabilities.minImageCount + 1};
    if (capabilities.maxImageCount > 0 && imageCount > capabilities.maxImageCount)
    {
        imageCount = capabilities.maxImageCount;
    }

    if (const VkSwapchainCreateInfoKHR swapChainCreateInfo{
            .sType                 = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
            .pNext                 = VK_NULL_HANDLE,
            .flags                 = 0,
            .surface               = surface.surface,
            .minImageCount         = imageCount,
            .imageFormat           = format,
            .imageColorSpace       = colorSpace,
            .imageExtent           = swapChain.extent,
            .imageArrayLayers      = 1, // 2 for stereo
            .imageUsage            = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
            .imageSharingMode      = VK_SHARING_MODE_EXCLUSIVE,
            .queueFamilyIndexCount = 0U,
            .pQueueFamilyIndices   = VK_NULL_HANDLE,
            .preTransform          = capabilities.currentTransform,
            .compositeAlpha        = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
            .presentMode           = presentMode,
            .clipped               = VK_TRUE,
            .oldSwapchain          = swapChain.swapchain,
        };
        !CheckVkResult(vkCreateSwapchainKHR(device.device, &swapChainCreateInfo, VK_NULL_HANDLE, &swapChain.swapchain)))
    {
        return -1;
    }

    if (!CheckVkResult(vkGetSwapchainImagesKHR(device.device, swapChain.swapchain, &imageCount, VK_NULL_HANDLE)))
    {
        return -1;
    }

    std::vector<VkImage> images{imageCount};
    swapChain.images.resize(imageCount);
    swapChain.imageViews.resize(imageCount);
    if (!CheckVkResult(vkGetSwapchainImagesKHR(device.device, swapChain.swapchain, &imageCount, images.data())))
    {
        return -1;
    }
    for (std::size_t i{}; i < images.size(); i++)
    {
        swapChain.images[i].image  = images[i];
        if (const SImageViewCreateInfo imageViewCreateInfo{
                .imageType   = VK_IMAGE_VIEW_TYPE_2D,
                .format      = static_cast<uint32_t>(format),
                .rSwizzle    = VK_COMPONENT_SWIZZLE_IDENTITY,
                .gSwizzle    = VK_COMPONENT_SWIZZLE_IDENTITY,
                .bSwizzle    = VK_COMPONENT_SWIZZLE_IDENTITY,
                .aSwizzle    = VK_COMPONENT_SWIZZLE_IDENTITY,
                .aspectFlag  = VK_IMAGE_ASPECT_COLOR_BIT,
                .mipOffset   = 0,
                .mipCount    = 1,
                .layerOffset = 0,
                .layerCount  = 1,
            };
            Initialize(device, swapChain.images[i], swapChain.imageViews[i], imageViewCreateInfo) != 0)
        {
            return -1;
        }
    }

    swapChain.imageFormat = format;
    return 0;
}
inline void Cleanup(const SVkDevice& device, SVkSwapChain& swapChain) noexcept
{
    if (device.device == VK_NULL_HANDLE)
    {
        return;
    }
    if (!swapChain.imageViews.empty())
    {
        std::ranges::for_each(swapChain.imageViews,
                              [&device](SVkImageView& imageView)
                              {
                                  Cleanup(device, imageView);
                              });
        swapChain.images.clear();
        swapChain.imageViews.clear();
    }
    if (swapChain.swapchain != VK_NULL_HANDLE)
    {
        vkDestroySwapchainKHR(device.device, swapChain.swapchain, VK_NULL_HANDLE);
        swapChain.swapchain = VK_NULL_HANDLE;
    }
}

inline auto NextImage(const SVkDevice& device, SVkSwapChain& swapChain, const SVkSemaphore& semaphore) noexcept -> int32_t
{
    if (device.device == VK_NULL_HANDLE || swapChain.swapchain == VK_NULL_HANDLE || semaphore.semaphore == VK_NULL_HANDLE)
    {
        return -1;
    }
    if (const VkAcquireNextImageInfoKHR acquireNextImageInfo{
            .sType      = VK_STRUCTURE_TYPE_ACQUIRE_NEXT_IMAGE_INFO_KHR,
            .pNext      = VK_NULL_HANDLE,
            .swapchain  = swapChain.swapchain,
            .timeout    = UINT64_MAX,
            .semaphore  = semaphore.semaphore,
            .fence      = VK_NULL_HANDLE,
            .deviceMask = 1U,
        };
        !CheckVkResult(vkAcquireNextImage2KHR(device.device, &acquireNextImageInfo, &swapChain.currentFrameIdx)))
    {
        return -1;
    }
    return 0;
}

constexpr auto CurrentImage(const SVkSwapChain& swapChain) -> const SVkImage&
{
    return swapChain.images[swapChain.currentFrameIdx];
}
constexpr auto CurrentImage(SVkSwapChain& swapChain) -> SVkImage&
{
    return swapChain.images[swapChain.currentFrameIdx];
}
constexpr auto CurrentImageView(const SVkSwapChain& swapChain) -> const SVkImageView&
{
    return swapChain.imageViews[swapChain.currentFrameIdx];
}
constexpr void NextFrame(SVkSwapChain& swapChain) noexcept
{
    if (swapChain.swapchain != VK_NULL_HANDLE)
    {
        swapChain.currentFrameIdx = (swapChain.currentFrameIdx + 1) % swapChain.images.size();
    }
}
} // namespace DeerVulkan