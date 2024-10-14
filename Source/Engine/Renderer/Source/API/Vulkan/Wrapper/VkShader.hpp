//
// Copyright (c) 2024.
// Author: Joran.
//

#pragma once
#include "../DeerVulkan_Core.hpp"

#include "VkDescriptor.hpp"
#include "VkDevice.hpp"

#include <cstddef>
#include <vector>

namespace DeerVulkan
{

struct SVkShader
{
    std::vector<VkShaderEXT> shaders;
    std::vector<VkShaderStageFlagBits> stages;
    PFN_vkCreateShadersEXT vkCreateShadersEXT{VK_NULL_HANDLE};
    PFN_vkDestroyShaderEXT vkDestroyShaderEXT{VK_NULL_HANDLE};
};

inline auto Initialize(const SVkDevice& device, const std::vector<uint32_t>& stages, const std::vector<std::vector<uint8_t>>& codes, SVkShader& shader, const SVkDescriptor& descriptor = {}) noexcept -> int32_t
{
    if (device.device == VK_NULL_HANDLE || !shader.shaders.empty())
    {
        return -1;
    }
    shader.vkCreateShadersEXT = std::bit_cast<PFN_vkCreateShadersEXT>(vkGetDeviceProcAddr(device.device, "vkCreateShadersEXT"));
    shader.vkDestroyShaderEXT = std::bit_cast<PFN_vkDestroyShaderEXT>(vkGetDeviceProcAddr(device.device, "vkDestroyShaderEXT"));

    std::vector<VkShaderCreateInfoEXT> createInfos{};
    createInfos.reserve(stages.size());
    shader.stages.reserve(stages.size());
    shader.shaders.resize(stages.size());
    for (std::size_t i{}; i < stages.size(); ++i)
    {
        const auto stage{static_cast<VkShaderStageFlagBits>(stages[i])};
        const VkShaderStageFlags nextStage{i + 1 < stages.size() ? stages[i + 1] : 0};
        createInfos.emplace_back(VK_STRUCTURE_TYPE_SHADER_CREATE_INFO_EXT, VK_NULL_HANDLE, VK_SHADER_CREATE_LINK_STAGE_BIT_EXT, stage, nextStage, VK_SHADER_CODE_TYPE_SPIRV_EXT, codes[i].size(), codes[i].data(), "main",
                                 descriptor.descriptorSetLayout == VK_NULL_HANDLE ? 0 : 1, descriptor.descriptorSetLayout == VK_NULL_HANDLE ?VK_NULL_HANDLE : &descriptor.descriptorSetLayout, 0, VK_NULL_HANDLE, VK_NULL_HANDLE);
        shader.stages.emplace_back(stage);
    }
    if (!CheckVkResult(shader.vkCreateShadersEXT(device.device, static_cast<uint32_t>(createInfos.size()), createInfos.data(), VK_NULL_HANDLE, shader.shaders.data())))
    {
        return -1;
    }
    return 0;
}
inline void Cleanup(const SVkDevice& device, SVkShader& shader) noexcept
{
    if (device.device == VK_NULL_HANDLE)
    {
        return;
    }
    for (const VkShaderEXT& shaderObject : shader.shaders)
    {
        shader.vkDestroyShaderEXT(device.device, shaderObject, VK_NULL_HANDLE);
    }
    shader.shaders.clear();
    shader.stages.clear();
}
} // namespace DeerVulkan