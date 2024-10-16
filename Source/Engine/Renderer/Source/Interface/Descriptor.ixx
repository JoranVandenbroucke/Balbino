//
// Copyright (c) 2024.
// Author: Joran.
//

module;
#include "API/Vulkan/DeerVulkan.hpp"

export module FawnVision.Descriptor;
import FawnAlgebra.Arithmetics;
import FawnVision.Enum;
import FawnVision.Renderer;
import FawnVision.Texture;
using namespace FawnAlgebra;

namespace FawnVision
{
#pragma region defenition
export struct SDescriptorData;
export struct SDescriptor;
export using DescriptorCreateInfo = std::vector<SDescriptorData>;
export inline auto Initialize(const SRenderer& renderer, const DescriptorCreateInfo& createInfo, SDescriptor& descriptor) noexcept -> int32;
export inline void Clearnup(const SRenderer& renderer, SDescriptor& descriptor) noexcept;
export inline void UpdateDescriptorSet(const SRenderer& renderer, const SDescriptor& descriptor) noexcept;
export constexpr void BindTextureToDescriptor(SDescriptor& descriptor, const STexture& texture, uint32 binding) noexcept;
#pragma endregion

#pragma region implementation
struct SDescriptorData
{
    uint32 binding;
    descriptor_type type;
    uint32 count;
    shader_stage stageFlags;
};
struct SDescriptor
{
    DeerVulkan::SVkDescriptor descriptor;
};

inline auto Initialize(const SRenderer& renderer, const DescriptorCreateInfo& createInfo, SDescriptor& descriptor) noexcept -> int32
{
    std::vector<DeerVulkan::SDescriptorLayout> descriptorLayouts{createInfo.size()};
    for (std::size_t i{}; i < createInfo.size(); ++i)
    {
        descriptorLayouts[i].binding         = createInfo[i].binding;
        descriptorLayouts[i].descriptorType  = static_cast<uint32>(createInfo[i].type);
        descriptorLayouts[i].descriptorCount = createInfo[i].count;
        descriptorLayouts[i].stageFlags      = static_cast<uint32>(createInfo[i].stageFlags);
    }
    return Initialize(renderer.device, descriptor.descriptor, descriptorLayouts);
}
inline void UpdateDescriptorSet(const SRenderer& renderer, const SDescriptor& descriptor) noexcept
{
    UpdateDescriptor(renderer.device, descriptor.descriptor);
}
constexpr void BindTextureToDescriptor(SDescriptor& descriptor, const STexture& texture, const uint32 binding) noexcept
{
    BindImage(descriptor.descriptor, texture.sampler, texture.view, texture.image, binding);
}
inline void Clearnup(const SRenderer& renderer, SDescriptor& descriptor) noexcept
{
    Cleanup(renderer.device, descriptor.descriptor);
}
#pragma endregion
} // namespace FawnVision
