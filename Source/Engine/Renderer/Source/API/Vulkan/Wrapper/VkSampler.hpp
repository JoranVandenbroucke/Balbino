//
// Copyright (c) 2024.
// Author: Joran.
//

#pragma once
#include "../DeerVulkan_Core.hpp"

#include "VkDevice.hpp"

namespace DeerVulkan
{
struct SVkSampler
{
    VkSampler sampler;
};
inline auto Initialize(const SVkDevice& device, SVkSampler& sampler, const bool useNearest, const uint32_t addressMode, const float mipLevels) noexcept -> int32_t
{
    if (device.device == VK_NULL_HANDLE || sampler.sampler != VK_NULL_HANDLE)
    {
        return -1;
    }
    if (const VkSamplerCreateInfo createInfo{
            .sType                   = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
            .pNext                   = VK_NULL_HANDLE,
            .flags                   = 0,
            .magFilter               = useNearest ? VK_FILTER_NEAREST : VK_FILTER_LINEAR,
            .minFilter               = useNearest ? VK_FILTER_NEAREST : VK_FILTER_LINEAR,
            .mipmapMode              = useNearest ? VK_SAMPLER_MIPMAP_MODE_NEAREST : VK_SAMPLER_MIPMAP_MODE_LINEAR,
            .addressModeU            = static_cast<VkSamplerAddressMode>(addressMode),
            .addressModeV            = static_cast<VkSamplerAddressMode>(addressMode),
            .addressModeW            = static_cast<VkSamplerAddressMode>(addressMode),
            .mipLodBias              = 0,
            .anisotropyEnable        = VK_FALSE,
            .maxAnisotropy           = 1,
            .compareEnable           = VK_FALSE,
            .compareOp               = VK_COMPARE_OP_ALWAYS,
            .minLod                  = 0,
            .maxLod                  = mipLevels,
            .borderColor             = VK_BORDER_COLOR_INT_OPAQUE_BLACK,
            .unnormalizedCoordinates = VK_FALSE,
        };
        !CheckVkResult(vkCreateSampler(device.device, &createInfo, VK_NULL_HANDLE, &sampler.sampler)))
    {
        return -1;
    }
    return 0;
}
inline void Cleanup(const SVkDevice& device, SVkSampler& sampler) noexcept
{
    if (device.device == VK_NULL_HANDLE)
    {
        return;
    }
    if (sampler.sampler != VK_NULL_HANDLE)
    {
        vkDestroySampler(device.device, sampler.sampler, VK_NULL_HANDLE);
        sampler.sampler = VK_NULL_HANDLE;
    }
}
} // namespace DeerVulkan