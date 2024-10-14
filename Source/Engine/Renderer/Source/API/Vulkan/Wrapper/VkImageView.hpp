//
// Copyright (c) 2024.
// Author: Joran.
//

#pragma once
#include "../DeerVulkan_Core.hpp"

#include "VkDevice.hpp"
#include "VkImage.hpp"

namespace DeerVulkan
{
struct SImageViewCreateInfo
{
    uint8_t imageType;
    uint32_t format;
    uint8_t rSwizzle;
    uint8_t gSwizzle;
    uint8_t bSwizzle;
    uint8_t aSwizzle;
    uint32_t aspectFlag;
    uint32_t mipOffset;
    uint32_t mipCount;
    uint32_t layerOffset;
    uint32_t layerCount;
};
struct SVkImageView
{
    VkImageView imageView;
};

inline auto Initialize(const SVkDevice& device, const SVkImage& image, SVkImageView& imageView, const SImageViewCreateInfo& createInfo)  noexcept-> int32_t
{
    if (device.device == VK_NULL_HANDLE || image.image == VK_NULL_HANDLE)
    {
        return -1;
    }
    if (const VkImageViewCreateInfo imageViewCreateInfo{
        .sType            = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
        .pNext            = VK_NULL_HANDLE,
        .flags            = 0,
        .image            = image.image,
        .viewType         = static_cast<VkImageViewType>(createInfo.imageType),
        .format           = static_cast<VkFormat>(createInfo.format),
        .components       = {
            .r = static_cast<VkComponentSwizzle>(createInfo.rSwizzle),
            .g = static_cast<VkComponentSwizzle>(createInfo.gSwizzle),
            .b = static_cast<VkComponentSwizzle>(createInfo.bSwizzle),
            .a = static_cast<VkComponentSwizzle>(createInfo.aSwizzle),},
        .subresourceRange = {
            .aspectMask = createInfo.aspectFlag,
            .baseMipLevel = createInfo.mipOffset,
            .levelCount = createInfo.mipCount,
            .baseArrayLayer = createInfo.layerOffset,
            .layerCount = createInfo.layerCount,
        }
    };
    imageView.imageView != VK_NULL_HANDLE || !CheckVkResult(vkCreateImageView(device.device, &imageViewCreateInfo, VK_NULL_HANDLE, &imageView.imageView)))
    {
        return -1;
    }
    return 0;
}
inline void Cleanup(const SVkDevice& device, SVkImageView& imageView) noexcept
{
    if (device.device == VK_NULL_HANDLE)
    {
        return;
    }
    if (imageView.imageView != VK_NULL_HANDLE)
    {
        vkDestroyImageView(device.device, imageView.imageView, VK_NULL_HANDLE);
        imageView.imageView = VK_NULL_HANDLE;
    }
}
} // namespace DeerVulkan