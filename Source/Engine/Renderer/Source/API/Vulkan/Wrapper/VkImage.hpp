//
// Copyright (c) 2024.
// Author: Joran.
//

#pragma once
#include "../DeerVulkan_Core.hpp"

#include "VkDevice.hpp"

namespace DeerVulkan
{
struct SImageCreateInfo
{
    uint32_t format;
    uint32_t width;
    uint32_t height;
    uint32_t depth;
    uint32_t mipCount;
    uint32_t arrayCount;
    uint32_t sampleCount;
    uint32_t usage;
    uint32_t memoryProperty;
    uint8_t imageType;
    uint8_t tiling;
};
struct SVkImage
{
    VkImage image;
    VkDeviceMemory memory;
    VkImageLayout layout;
};

inline auto Initialize(const SVkDevice& device, SVkImage& image, const SImageCreateInfo& createInfo) noexcept -> int32_t
{
    if (device.device == VK_NULL_HANDLE)
    {
        return -1;
    }
    if (const VkImageCreateInfo imageInfo{
            .sType         = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
            .pNext = VK_NULL_HANDLE,
            .flags = 0,
            .imageType     = static_cast<VkImageType>(createInfo.imageType),
            .format        = static_cast<VkFormat>(createInfo.format),
            .extent        = {
                .width  = createInfo.width,
                .height = createInfo.height,
                .depth  = createInfo.depth,
            },
            .mipLevels     = createInfo.mipCount,
            .arrayLayers   = createInfo.arrayCount,
            .samples       = static_cast<VkSampleCountFlagBits>(createInfo.sampleCount),
            .tiling        = static_cast<VkImageTiling>(createInfo.tiling),
            .usage         = createInfo.usage,
            .sharingMode   = VK_SHARING_MODE_EXCLUSIVE,
            .queueFamilyIndexCount = 0U,
            .pQueueFamilyIndices = VK_NULL_HANDLE,
            .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
        };
        image.image != VK_NULL_HANDLE || !CheckVkResult(vkCreateImage(device.device, &imageInfo, VK_NULL_HANDLE, &image.image)))
    {
        return -1;
    }

    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements(device.device, image.image, &memRequirements);

    if (const VkMemoryAllocateInfo allocInfo{
            .sType           = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
            .pNext           = VK_NULL_HANDLE,
            .allocationSize  = memRequirements.size,
            .memoryTypeIndex = FindMemoryType(device, memRequirements.memoryTypeBits, createInfo.memoryProperty),
        };
        !CheckVkResult(vkAllocateMemory(device.device, &allocInfo, VK_NULL_HANDLE, &image.memory)))
    {
        return -1;
    }

    vkBindImageMemory(device.device, image.image, image.memory, 0);
    image.layout = VK_IMAGE_LAYOUT_UNDEFINED;
    return 0;
}
inline void Cleanup(const SVkDevice& device, SVkImage& image) noexcept
{
    if (device.device == VK_NULL_HANDLE)
    {
        return;
    }
    if(image.memory)
    {
        vkFreeMemory(device.device, image.memory, VK_NULL_HANDLE);
        image.memory = VK_NULL_HANDLE;
    }
    if (image.image != VK_NULL_HANDLE)
    {
        vkDestroyImage(device.device, image.image, VK_NULL_HANDLE);
        image.image = VK_NULL_HANDLE;
    }
}
} // namespace DeerVulkan