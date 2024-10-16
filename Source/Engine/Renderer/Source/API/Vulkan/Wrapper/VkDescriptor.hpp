//
// Copyright (c) 2024.
// Author: Joran.
//

#pragma once
#include "../DeerVulkan_Core.hpp"

#include "VkDevice.hpp"
#include "VkImageView.hpp"
#include "VkSampler.hpp"

#include <cstddef>

namespace DeerVulkan
{
struct SVkDescriptorImageInfo
{
    SVkSampler sampler;
    SVkImageView view;
    SVkImage image;
};
struct SDescriptorLayout
{
    uint32_t binding;
    uint32_t descriptorType;
    uint32_t descriptorCount;
    uint32_t stageFlags;
};
struct SVkDescriptor
{
    VkDescriptorSetLayout descriptorSetLayout{VK_NULL_HANDLE};
    VkPipelineLayout pipelineLayout{VK_NULL_HANDLE};
    VkDescriptorSet descriptorSet{VK_NULL_HANDLE};
    std::vector<VkWriteDescriptorSet> writeDescriptorSets;
    std::vector<VkDescriptorImageInfo> imageInfos;
    std::vector<VkDescriptorBufferInfo> bufferInfos;
};
inline auto Initialize(const SVkDevice& device, SVkDescriptor& descriptor, const std::vector<SDescriptorLayout>& descriptorLayouts)  noexcept-> int32_t
{
    if (device.device == VK_NULL_HANDLE || descriptorLayouts.empty())
    {
        return -1;
    }
    std::vector<VkDescriptorSetLayoutBinding> setLayoutBindings{descriptorLayouts.size()};
    for (std::size_t i = 0; i < descriptorLayouts.size(); ++i)
    {
        setLayoutBindings[i].binding         = descriptorLayouts[i].binding;
        setLayoutBindings[i].descriptorType  = static_cast<VkDescriptorType>(descriptorLayouts[i].descriptorType);
        setLayoutBindings[i].descriptorCount = descriptorLayouts[i].descriptorCount;
        setLayoutBindings[i].stageFlags      = descriptorLayouts[i].stageFlags;
    }
    if (const VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo{
            .sType        = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
            .pNext        = VK_NULL_HANDLE,
            .flags        = 0,
            .bindingCount = static_cast<uint32_t>(setLayoutBindings.size()),
            .pBindings    = setLayoutBindings.data(),
        };
        descriptor.descriptorSetLayout != VK_NULL_HANDLE || !CheckVkResult(vkCreateDescriptorSetLayout(device.device, &descriptorSetLayoutCreateInfo, VK_NULL_HANDLE, &descriptor.descriptorSetLayout)))
    {
        return -1;
    }
    if (const VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo{
            .sType                  = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
            .pNext                  = VK_NULL_HANDLE,
            .flags                  = 0,
            .setLayoutCount         = 1,
            .pSetLayouts            = &descriptor.descriptorSetLayout,
            .pushConstantRangeCount = 0,
            .pPushConstantRanges    = VK_NULL_HANDLE,
        };
        descriptor.pipelineLayout != VK_NULL_HANDLE || !CheckVkResult(vkCreatePipelineLayout(device.device, &pipelineLayoutCreateInfo, VK_NULL_HANDLE, &descriptor.pipelineLayout)))
    {
        return -1;
    }
    // Sets
    if (const VkDescriptorSetAllocateInfo allocateInfo{
            .sType              = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
            .pNext              = VK_NULL_HANDLE,
            .descriptorPool     = device.descriptorPool,
            .descriptorSetCount = 1,
            .pSetLayouts        = &descriptor.descriptorSetLayout,
        };
        descriptor.descriptorSet != VK_NULL_HANDLE || !CheckVkResult(vkAllocateDescriptorSets(device.device, &allocateInfo, &descriptor.descriptorSet)))
    {
        return -1;
    }
    descriptor.writeDescriptorSets = std::vector<VkWriteDescriptorSet>{descriptorLayouts.size(),
                                                                       {.sType            = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
                                                                        .pNext            = VK_NULL_HANDLE,
                                                                        .dstSet           = descriptor.descriptorSet,
                                                                        .dstBinding       = 0,
                                                                        .dstArrayElement  = 0,
                                                                        .descriptorCount  = 1,
                                                                        .descriptorType   = VK_DESCRIPTOR_TYPE_SAMPLER,
                                                                        .pImageInfo       = VK_NULL_HANDLE,
                                                                        .pBufferInfo      = VK_NULL_HANDLE,
                                                                        .pTexelBufferView = VK_NULL_HANDLE}};

    return 0;
}
inline void Cleanup(const SVkDevice& device, SVkDescriptor& descriptor) noexcept
{
    if (device.device == VK_NULL_HANDLE)
    {
        return;
    }
    if (descriptor.descriptorSet != VK_NULL_HANDLE)
    {
        vkFreeDescriptorSets(device.device, device.descriptorPool, 1, &descriptor.descriptorSet);
        descriptor.descriptorSet = VK_NULL_HANDLE;
    }
    if (descriptor.pipelineLayout != VK_NULL_HANDLE)
    {
        vkDestroyPipelineLayout(device.device, descriptor.pipelineLayout, VK_NULL_HANDLE);
        descriptor.pipelineLayout = VK_NULL_HANDLE;
    }
    if (descriptor.descriptorSetLayout != VK_NULL_HANDLE)
    {
        vkDestroyDescriptorSetLayout(device.device, descriptor.descriptorSetLayout, VK_NULL_HANDLE);
        descriptor.descriptorSetLayout = VK_NULL_HANDLE;
    }
    descriptor.writeDescriptorSets.clear();
}
constexpr void BindImage(SVkDescriptor& descriptor, const SVkSampler& sampler, const SVkImageView& imageView, const SVkImage& image, const uint32_t binding) noexcept
{
    const VkDescriptorImageInfo imageInfo{
        .sampler = sampler.sampler,
        .imageView = imageView.imageView,
        .imageLayout = image.layout,
    };
    descriptor.imageInfos.push_back(imageInfo);

    const VkWriteDescriptorSet writeDescriptorSet{
        .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
        .pNext = VK_NULL_HANDLE,
        .dstSet = descriptor.descriptorSet,
        .dstBinding = binding,
        .dstArrayElement = 0,
        .descriptorCount = 1,
        .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
        .pImageInfo = &descriptor.imageInfos.back(),
        .pBufferInfo = VK_NULL_HANDLE,
        .pTexelBufferView = VK_NULL_HANDLE,
    };
    descriptor.writeDescriptorSets.push_back(writeDescriptorSet);
}

inline void BindBuffer(SVkDescriptor& descriptor, const SVkBuffer& buffer, const uint64_t offset, const uint64_t range, const uint32_t binding) noexcept
{
    const VkDescriptorBufferInfo bufferInfo{
        .buffer = buffer.buffer,
        .offset = offset,
        .range = range,
    };
    descriptor.bufferInfos.push_back(bufferInfo);

    const VkWriteDescriptorSet writeDescriptorSet{
        .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
        .pNext = VK_NULL_HANDLE,
        .dstSet = descriptor.descriptorSet,
        .dstBinding = binding,
        .dstArrayElement = 0,
        .descriptorCount = 1,
        .descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, // or VK_DESCRIPTOR_TYPE_STORAGE_BUFFER
        .pImageInfo = VK_NULL_HANDLE,
        .pBufferInfo = &descriptor.bufferInfos.back(),
        .pTexelBufferView = VK_NULL_HANDLE,
    };
    descriptor.writeDescriptorSets.push_back(writeDescriptorSet);
}

inline void UpdateDescriptor(const SVkDevice& device, const SVkDescriptor& descriptor) noexcept
{
    if (device.device == VK_NULL_HANDLE || descriptor.descriptorSet == VK_NULL_HANDLE || descriptor.writeDescriptorSets.empty())
    {
        return;
    }
    vkUpdateDescriptorSets(device.device,
                           static_cast<uint32_t>(descriptor.writeDescriptorSets.size()),
                           descriptor.writeDescriptorSets.data(),
                           0,
                           VK_NULL_HANDLE);
}
} // namespace DeerVulkan
