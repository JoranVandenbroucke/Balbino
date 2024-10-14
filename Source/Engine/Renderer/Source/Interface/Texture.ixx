//
// Copyright (c) 2024.
// Author: Joran.
//

module;
#include "API/Vulkan/DeerVulkan.hpp"

#include <cmath>
#include <vector>

export module FawnVision.Texture;
import FawnAlgebra.Arithmetics;
import FawnVision.Enum;
import FawnVision.Renderer;
using namespace FawnAlgebra;

namespace FawnVision
{
#pragma region defenition
export struct SImageTextureCreateInfo;
export struct SRenderTextureCreateInfo;
export struct STexture;

export inline auto Initialize(const SRenderer& renderer, const SImageTextureCreateInfo& createInfo, STexture& texture) noexcept -> int32;
export inline auto Initialize(const SRenderer& renderer, const SRenderTextureCreateInfo& createInfo, STexture& texture) noexcept -> int32;
export inline void Cleanup(const SRenderer& renderer, STexture& texture) noexcept;
#pragma endregion

#pragma region implementation
struct SImageTextureCreateInfo
{
    image_type imageType;
    format imageFormat;
    image_layout layout;
    uint32 width;
    uint32 height;
    uint32 depth;
    uint32 mipCount;
    uint32 arrayCount;
    uint32 sampleCount;
    image_tiling tiling;
    uint32 usage;
    uint32 memoryProperty;
    component_swizzle rSwizzle;
    component_swizzle gSwizzle;
    component_swizzle bSwizzle;
    component_swizzle aSwizzle;
    uint32 aspectFlag;
    uint32 mipOffset;
    uint32 layerOffset;
    uint32 layerCount;
    std::vector<uint8> pixelData;
};
struct SRenderTextureCreateInfo
{
    format format;
    image_aspect aspect;
    uint32 width;
    uint32 height;
};
struct STexture
{
    DeerVulkan::SVkImage image;
    DeerVulkan::SVkImageView view;
    DeerVulkan::SVkSampler sampler; // todo : make this in samplers independent from images or views
};
inline auto Initialize(const SRenderer& renderer, const SImageTextureCreateInfo& createInfo, STexture& texture) noexcept -> int32
{
    DeerVulkan::SVkBuffer stagingBuffer;
    const uint64 size{std::size(createInfo.pixelData)};
    const uint32 idealMip{createInfo.mipCount == 0 ? static_cast<uint32>(std::floor(std::log2(std::max(std::max(createInfo.width, createInfo.height), createInfo.depth)))) + 1 : createInfo.mipCount};
    if (Initialize(renderer.device, stagingBuffer, size, static_cast<uint32>(buffer_usage::transfer_src), static_cast<uint32>(memory_property::host_visible) | static_cast<uint32>(memory_property::host_coherent)) != 0)
    {
        return -1;
    }
    CopyData(renderer.device, stagingBuffer, size, std::data(createInfo.pixelData));
    uint8 imageType{};
    if (createInfo.imageType == image_type::type_2d || createInfo.imageType == image_type::type_2d_array || createInfo.imageType == image_type::type_cube || createInfo.imageType == image_type::type_cube_array)
    {
        imageType = 1;
    }
    else if (createInfo.imageType == image_type::type_3d)
    {
        imageType = 2;
    }
    if (Initialize(renderer.device, texture.image,
                   DeerVulkan::SImageCreateInfo{
                       .format         = static_cast<uint32>(createInfo.imageFormat),
                       .width          = createInfo.width,
                       .height         = createInfo.height,
                       .depth          = createInfo.depth,
                       .mipCount       = idealMip,
                       .arrayCount     = createInfo.arrayCount,
                       .sampleCount    = createInfo.sampleCount,
                       .usage          = createInfo.usage,
                       .memoryProperty = createInfo.memoryProperty,
                       .imageType      = imageType,
                       .tiling         = static_cast<uint8>(createInfo.tiling),
                   })
        != 0)
    {
        return -1;
    }
    if (TransitionImageLayout(renderer.commandBuffer, texture.image, static_cast<uint32>(image_layout::transfer_dst_optimal), createInfo.mipOffset, idealMip, createInfo.layerOffset, createInfo.layerCount))
    {
        return -1;
    }
    if (CopyToImage(renderer.commandBuffer, stagingBuffer, texture.image, createInfo.width, createInfo.height, createInfo.depth) != 0)
    {
        return -1;
    }
    if (createInfo.mipCount == idealMip)
    {
        if (TransitionImageLayout(renderer.commandBuffer, texture.image, static_cast<uint32>(createInfo.layout), createInfo.mipOffset, idealMip, createInfo.layerOffset, createInfo.layerCount))
        {
            return -1;
        }
    }
    else
    {
        GenerateMips(renderer.commandBuffer, texture.image, createInfo.width, createInfo.height, idealMip);
    }
    Cleanup(renderer.device, stagingBuffer);

    if (Initialize(renderer.device, texture.image, texture.view,
                   DeerVulkan::SImageViewCreateInfo{
                       .imageType   = static_cast<uint8>(createInfo.imageType),
                       .format      = static_cast<uint32>(createInfo.imageFormat),
                       .rSwizzle    = static_cast<uint8>(createInfo.rSwizzle),
                       .gSwizzle    = static_cast<uint8>(createInfo.gSwizzle),
                       .bSwizzle    = static_cast<uint8>(createInfo.bSwizzle),
                       .aSwizzle    = static_cast<uint8>(createInfo.aSwizzle),
                       .aspectFlag  = createInfo.aspectFlag,
                       .mipOffset   = createInfo.mipOffset,
                       .mipCount    = idealMip,
                       .layerOffset = createInfo.layerOffset,
                       .layerCount  = createInfo.layerCount,
                   })
        != 0)
    {
        return -1;
    }
    if (Initialize(renderer.device, texture.sampler, true, 0, static_cast<float>(idealMip)) != 0)
    {
        return -1;
    }
    return 0;
}
inline auto Initialize(const SRenderer& renderer, const SRenderTextureCreateInfo& createInfo, STexture& texture) noexcept -> int32
{
    if (Initialize(renderer.device, texture.image,
                   DeerVulkan::SImageCreateInfo{.format         = static_cast<uint32>(createInfo.format),
                                                .width          = createInfo.width,
                                                .height         = createInfo.height,
                                                .depth          = 1,
                                                .mipCount       = 1,
                                                .arrayCount     = 1,
                                                .sampleCount    = 1,
                                                .usage          = static_cast<uint32>(image_usage::color_attachment) | static_cast<uint32>(image_usage::sampled),
                                                .memoryProperty = 0,
                                                .imageType      = static_cast<uint8>(image_type::type_2d),
                                                .tiling         = static_cast<uint8>(image_tiling::optimal)})
        != 0)
    {
        return -1;
    }
    if (Initialize(renderer.device, texture.image, texture.view,
                   DeerVulkan::SImageViewCreateInfo{
                       .imageType   = static_cast<uint8>(image_type::type_2d),
                       .format      = static_cast<uint32>(createInfo.format),
                       .rSwizzle    = static_cast<uint8>(component_swizzle::swizzle_r),
                       .gSwizzle    = static_cast<uint8>(component_swizzle::swizzle_g),
                       .bSwizzle    = static_cast<uint8>(component_swizzle::swizzle_b),
                       .aSwizzle    = static_cast<uint8>(component_swizzle::swizzle_a),
                       .aspectFlag  = static_cast<uint32>(createInfo.aspect),
                       .mipOffset   = 0,
                       .mipCount    = 1,
                       .layerOffset = 0,
                       .layerCount  = 1,
                   })
        != 0)
    {
        return -1;
    }
    if (Initialize(renderer.device, texture.sampler, true, 2, 1) != 0)
    {
        return -1;
    }
    return 0;
}
inline void Cleanup(const SRenderer& renderer, STexture& texture) noexcept
{
    Cleanup(renderer.device, texture.sampler);
    Cleanup(renderer.device, texture.view);
    Cleanup(renderer.device, texture.image);
}
#pragma endregion
} // namespace FawnVision