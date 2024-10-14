//
// Copyright (c) 2024.
// Author: Joran.
//

#pragma once
#include "../DeerVulkan_Core.hpp"

#include "VkBuffer.hpp"
#include "VkDevice.hpp"
#include "VkImage.hpp"
#include "VkShader.hpp"

#include <cstddef>

namespace DeerVulkan
{
struct SVertexAttributes
{
    uint32_t location;
    uint32_t binding;
    uint32_t format;
    uint32_t offset;
    bool isInstance;
};
struct SVkCommandPool
{
    VkCommandPool commandPool;
};
struct SVkCommandBuffer
{
    VkCommandBuffer commandBuffer;
    uint32_t count;

    PFN_vkCmdBeginRenderingKHR vkCmdBeginRenderingKHR{VK_NULL_HANDLE};
    PFN_vkCmdBindShadersEXT vkCmdBindShadersEXT{VK_NULL_HANDLE};
    PFN_vkCmdEndRenderingKHR vkCmdEndRenderingKHR{VK_NULL_HANDLE};
    PFN_vkCmdSetAlphaToCoverageEnableEXT vkCmdSetAlphaToCoverageEnableEXT{VK_NULL_HANDLE};
    PFN_vkCmdSetColorBlendEnableEXT vkCmdSetColorBlendEnableEXT{VK_NULL_HANDLE};
    PFN_vkCmdSetColorBlendEquationEXT vkCmdSetColorBlendEquationEXT{VK_NULL_HANDLE};
    PFN_vkCmdSetColorWriteMaskEXT vkCmdSetColorWriteMaskEXT{VK_NULL_HANDLE};
    PFN_vkCmdSetCullModeEXT vkCmdSetCullModeEXT{VK_NULL_HANDLE};
    PFN_vkCmdSetDepthBiasEnableEXT vkCmdSetDepthBiasEnableEXT{VK_NULL_HANDLE};
    PFN_vkCmdSetDepthCompareOpEXT vkCmdSetDepthCompareOpEXT{VK_NULL_HANDLE};
    PFN_vkCmdSetDepthTestEnableEXT vkCmdSetDepthTestEnableEXT{VK_NULL_HANDLE};
    PFN_vkCmdSetDepthWriteEnableEXT vkCmdSetDepthWriteEnableEXT{VK_NULL_HANDLE};
    PFN_vkCmdSetFrontFaceEXT vkCmdSetFrontFaceEXT{VK_NULL_HANDLE};
    PFN_vkCmdSetPolygonModeEXT vkCmdSetPolygonModeEXT{VK_NULL_HANDLE};
    PFN_vkCmdSetPrimitiveRestartEnableEXT vkCmdSetPrimitiveRestartEnableEXT{VK_NULL_HANDLE};
    PFN_vkCmdSetPrimitiveTopologyEXT vkCmdSetPrimitiveTopologyEXT{VK_NULL_HANDLE};
    PFN_vkCmdSetRasterizationSamplesEXT vkCmdSetRasterizationSamplesEXT{VK_NULL_HANDLE};
    PFN_vkCmdSetRasterizerDiscardEnableEXT vkCmdSetRasterizerDiscardEnableEXT{VK_NULL_HANDLE};
    PFN_vkCmdSetSampleMaskEXT vkCmdSetSampleMaskEXT{VK_NULL_HANDLE};
    PFN_vkCmdSetScissorWithCountEXT vkCmdSetScissorWithCountEXT{VK_NULL_HANDLE};
    PFN_vkCmdSetStencilTestEnableEXT vkCmdSetStencilTestEnableEXT{VK_NULL_HANDLE};
    PFN_vkCmdSetVertexInputEXT vkCmdSetVertexInputEXT{VK_NULL_HANDLE};
    PFN_vkCmdSetViewportWithCountEXT vkCmdSetViewportWithCountEXT{VK_NULL_HANDLE};
};
struct RenderParams
{
    const SVkImageView* colorImageView;
    const SVkImageView* depthImageView;
    std::array<float, 3> clearColor{0.0f, 0.0f, 0.0f};
    float depthClear{1.0f};
    int32_t xOffset{};
    int32_t yOffset{};
    uint32_t width{};
    uint32_t height{};
    uint32_t stencilClear{};
};

[[nodiscard]] constexpr auto IsDepthLayout(const VkImageLayout layout) noexcept -> bool
{
    return layout == VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL || layout == VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_OPTIMAL || layout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL || layout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL
        || layout == VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_STENCIL_READ_ONLY_OPTIMAL || layout == VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_STENCIL_ATTACHMENT_OPTIMAL;
}

[[nodiscard]] constexpr auto IsStencilLayout(const VkImageLayout layout) noexcept -> bool
{
    return layout == VK_IMAGE_LAYOUT_STENCIL_ATTACHMENT_OPTIMAL || layout == VK_IMAGE_LAYOUT_STENCIL_READ_ONLY_OPTIMAL || layout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL
        || layout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL || layout == VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_STENCIL_READ_ONLY_OPTIMAL || layout == VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_STENCIL_ATTACHMENT_OPTIMAL;
}

inline auto InitializeFunctions(const SVkInstance& instance, SVkCommandBuffer& commandBuffer) noexcept -> int32_t
{
    if (instance.instance == VK_NULL_HANDLE || commandBuffer.commandBuffer == VK_NULL_HANDLE)
    {
        return -1;
    }
    commandBuffer.vkCmdBeginRenderingKHR             = std::bit_cast<PFN_vkCmdBeginRenderingKHR>(vkGetInstanceProcAddr(instance.instance, "vkCmdBeginRenderingKHR"));
    commandBuffer.vkCmdBindShadersEXT                = std::bit_cast<PFN_vkCmdBindShadersEXT>(vkGetInstanceProcAddr(instance.instance, "vkCmdBindShadersEXT"));
    commandBuffer.vkCmdEndRenderingKHR               = std::bit_cast<PFN_vkCmdEndRenderingKHR>(vkGetInstanceProcAddr(instance.instance, "vkCmdEndRenderingKHR"));
    commandBuffer.vkCmdSetAlphaToCoverageEnableEXT   = std::bit_cast<PFN_vkCmdSetAlphaToCoverageEnableEXT>(vkGetInstanceProcAddr(instance.instance, "vkCmdSetAlphaToCoverageEnableEXT"));
    commandBuffer.vkCmdSetColorBlendEnableEXT        = std::bit_cast<PFN_vkCmdSetColorBlendEnableEXT>(vkGetInstanceProcAddr(instance.instance, "vkCmdSetColorBlendEnableEXT"));
    commandBuffer.vkCmdSetColorBlendEquationEXT      = std::bit_cast<PFN_vkCmdSetColorBlendEquationEXT>(vkGetInstanceProcAddr(instance.instance, "vkCmdSetColorBlendEquationEXT"));
    commandBuffer.vkCmdSetColorWriteMaskEXT          = std::bit_cast<PFN_vkCmdSetColorWriteMaskEXT>(vkGetInstanceProcAddr(instance.instance, "vkCmdSetColorWriteMaskEXT"));
    commandBuffer.vkCmdSetCullModeEXT                = std::bit_cast<PFN_vkCmdSetCullModeEXT>(vkGetInstanceProcAddr(instance.instance, "vkCmdSetCullModeEXT"));
    commandBuffer.vkCmdSetDepthBiasEnableEXT         = std::bit_cast<PFN_vkCmdSetDepthBiasEnableEXT>(vkGetInstanceProcAddr(instance.instance, "vkCmdSetDepthBiasEnableEXT"));
    commandBuffer.vkCmdSetDepthCompareOpEXT          = std::bit_cast<PFN_vkCmdSetDepthCompareOpEXT>(vkGetInstanceProcAddr(instance.instance, "vkCmdSetDepthCompareOpEXT"));
    commandBuffer.vkCmdSetDepthTestEnableEXT         = std::bit_cast<PFN_vkCmdSetDepthTestEnableEXT>(vkGetInstanceProcAddr(instance.instance, "vkCmdSetDepthTestEnableEXT"));
    commandBuffer.vkCmdSetDepthWriteEnableEXT        = std::bit_cast<PFN_vkCmdSetDepthWriteEnableEXT>(vkGetInstanceProcAddr(instance.instance, "vkCmdSetDepthWriteEnableEXT"));
    commandBuffer.vkCmdSetFrontFaceEXT               = std::bit_cast<PFN_vkCmdSetFrontFaceEXT>(vkGetInstanceProcAddr(instance.instance, "vkCmdSetFrontFaceEXT"));
    commandBuffer.vkCmdSetPolygonModeEXT             = std::bit_cast<PFN_vkCmdSetPolygonModeEXT>(vkGetInstanceProcAddr(instance.instance, "vkCmdSetPolygonModeEXT"));
    commandBuffer.vkCmdSetPrimitiveRestartEnableEXT  = std::bit_cast<PFN_vkCmdSetPrimitiveRestartEnableEXT>(vkGetInstanceProcAddr(instance.instance, "vkCmdSetPrimitiveRestartEnableEXT"));
    commandBuffer.vkCmdSetPrimitiveTopologyEXT       = std::bit_cast<PFN_vkCmdSetPrimitiveTopologyEXT>(vkGetInstanceProcAddr(instance.instance, "vkCmdSetPrimitiveTopologyEXT"));
    commandBuffer.vkCmdSetRasterizationSamplesEXT    = std::bit_cast<PFN_vkCmdSetRasterizationSamplesEXT>(vkGetInstanceProcAddr(instance.instance, "vkCmdSetRasterizationSamplesEXT"));
    commandBuffer.vkCmdSetRasterizerDiscardEnableEXT = std::bit_cast<PFN_vkCmdSetRasterizerDiscardEnableEXT>(vkGetInstanceProcAddr(instance.instance, "vkCmdSetRasterizerDiscardEnableEXT"));
    commandBuffer.vkCmdSetSampleMaskEXT              = std::bit_cast<PFN_vkCmdSetSampleMaskEXT>(vkGetInstanceProcAddr(instance.instance, "vkCmdSetSampleMaskEXT"));
    commandBuffer.vkCmdSetScissorWithCountEXT        = std::bit_cast<PFN_vkCmdSetScissorWithCountEXT>(vkGetInstanceProcAddr(instance.instance, "vkCmdSetScissorWithCountEXT"));
    commandBuffer.vkCmdSetStencilTestEnableEXT       = std::bit_cast<PFN_vkCmdSetStencilTestEnableEXT>(vkGetInstanceProcAddr(instance.instance, "vkCmdSetStencilTestEnableEXT"));
    commandBuffer.vkCmdSetVertexInputEXT             = std::bit_cast<PFN_vkCmdSetVertexInputEXT>(vkGetInstanceProcAddr(instance.instance, "vkCmdSetVertexInputEXT"));
    commandBuffer.vkCmdSetViewportWithCountEXT       = std::bit_cast<PFN_vkCmdSetViewportWithCountEXT>(vkGetInstanceProcAddr(instance.instance, "vkCmdSetViewportWithCountEXT"));

    if (!commandBuffer.vkCmdBeginRenderingKHR || !commandBuffer.vkCmdBindShadersEXT || !commandBuffer.vkCmdEndRenderingKHR || !commandBuffer.vkCmdSetAlphaToCoverageEnableEXT || !commandBuffer.vkCmdSetColorBlendEnableEXT
        || !commandBuffer.vkCmdSetColorBlendEquationEXT || !commandBuffer.vkCmdSetColorWriteMaskEXT || !commandBuffer.vkCmdSetCullModeEXT || !commandBuffer.vkCmdSetDepthBiasEnableEXT || !commandBuffer.vkCmdSetDepthCompareOpEXT
        || !commandBuffer.vkCmdSetDepthTestEnableEXT || !commandBuffer.vkCmdSetDepthWriteEnableEXT || !commandBuffer.vkCmdSetFrontFaceEXT || !commandBuffer.vkCmdSetPolygonModeEXT || !commandBuffer.vkCmdSetPrimitiveRestartEnableEXT
        || !commandBuffer.vkCmdSetPrimitiveTopologyEXT || !commandBuffer.vkCmdSetRasterizationSamplesEXT || !commandBuffer.vkCmdSetRasterizerDiscardEnableEXT || !commandBuffer.vkCmdSetSampleMaskEXT
        || !commandBuffer.vkCmdSetScissorWithCountEXT || !commandBuffer.vkCmdSetStencilTestEnableEXT || !commandBuffer.vkCmdSetVertexInputEXT || !commandBuffer.vkCmdSetViewportWithCountEXT)
    {
        return -1;
    }
    return 0;
}
inline auto Initialize(const SVkDevice& device, SVkCommandPool& commandPool, const uint32_t queueFamilyIndex) noexcept -> int32_t
{
    if (device.device == VK_NULL_HANDLE)
    {
        return -1;
    }
    if (const VkCommandPoolCreateInfo createInfo{
            .sType            = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
            .pNext            = VK_NULL_HANDLE,
            .flags            = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
            .queueFamilyIndex = queueFamilyIndex,
        };
        commandPool.commandPool != VK_NULL_HANDLE || !CheckVkResult(vkCreateCommandPool(device.device, &createInfo, VK_NULL_HANDLE, &commandPool.commandPool)))
    {
        return -1;
    }

    return 0;
}
inline auto CreateCommandBuffer(const SVkInstance& instance, const SVkDevice& device, const SVkCommandPool& commandPool, SVkCommandBuffer& commandBuffer, const uint32_t count) noexcept
{
    if (instance.instance == VK_NULL_HANDLE || device.device == VK_NULL_HANDLE || commandPool.commandPool == VK_NULL_HANDLE)
    {
        return -1;
    }
    if (const VkCommandBufferAllocateInfo allocInfo{
            .sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
            .pNext              = VK_NULL_HANDLE,
            .commandPool        = commandPool.commandPool,
            .level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
            .commandBufferCount = count,
        };
        commandBuffer.commandBuffer != VK_NULL_HANDLE || !CheckVkResult(vkAllocateCommandBuffers(device.device, &allocInfo, &commandBuffer.commandBuffer)))
    {
        return -1;
    }
    commandBuffer.count = count;
    return InitializeFunctions(instance, commandBuffer);
}
inline void Cleanup(const SVkDevice& device, SVkCommandPool& commandPool) noexcept
{
    if (device.device == VK_NULL_HANDLE)
    {
        return;
    }
    if (commandPool.commandPool != VK_NULL_HANDLE)
    {
        vkDestroyCommandPool(device.device, commandPool.commandPool, VK_NULL_HANDLE);
        commandPool.commandPool = VK_NULL_HANDLE;
    }
}
inline void Cleanup(const SVkDevice& device, const SVkCommandPool& commandPool, SVkCommandBuffer& commandBuffer) noexcept
{
    if (device.device == VK_NULL_HANDLE || commandPool.commandPool == VK_NULL_HANDLE)
    {
        return;
    }
    if (commandBuffer.commandBuffer != VK_NULL_HANDLE)
    {
        vkFreeCommandBuffers(device.device, commandPool.commandPool, commandBuffer.count, &commandBuffer.commandBuffer);
        commandBuffer.commandBuffer = VK_NULL_HANDLE;
        commandBuffer.count         = 0;
    }
}

inline auto BeginCommand(const SVkCommandBuffer& commandBuffer) noexcept -> int32_t
{
    if (constexpr VkCommandBufferBeginInfo commandBufferBeginInfo{
            .sType            = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
            .pNext            = VK_NULL_HANDLE,
            .flags            = 0,              // Optional
            .pInheritanceInfo = VK_NULL_HANDLE, // Optional
        };
        commandBuffer.commandBuffer == VK_NULL_HANDLE || !CheckVkResult(vkBeginCommandBuffer(commandBuffer.commandBuffer, &commandBufferBeginInfo)))
    {
        return -1;
    }
    return 0;
}
inline auto BeginSingleCommand(const SVkCommandBuffer& commandBuffer) noexcept -> int32_t
{
    if (constexpr VkCommandBufferBeginInfo commandBufferBeginInfo{
            .sType            = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
            .pNext            = VK_NULL_HANDLE,
            .flags            = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
            .pInheritanceInfo = VK_NULL_HANDLE, // Optional
        };
        commandBuffer.commandBuffer == VK_NULL_HANDLE || !CheckVkResult(vkBeginCommandBuffer(commandBuffer.commandBuffer, &commandBufferBeginInfo), "Could not start Vulkan Command Buffer in Single Command mode."))
    {
        return -1;
    }
    return 0;
}
inline auto EndCommand(const SVkCommandBuffer& commandBuffer) noexcept -> int32_t
{
    if (commandBuffer.commandBuffer == VK_NULL_HANDLE || !CheckVkResult(vkEndCommandBuffer(commandBuffer.commandBuffer)))
    {
        return -1;
    }
    return 0;
}

inline void BeginRender(const SVkCommandBuffer& commandBuffer, const RenderParams& params) noexcept
{
    if (commandBuffer.commandBuffer == VK_NULL_HANDLE)
    {
        return;
    }

    const VkClearValue colorClearValue{{{params.clearColor[0], params.clearColor[1], params.clearColor[2], 1.0F}}};
    const VkClearValue depthClearValue{.depthStencil{.depth = params.depthClear, .stencil = params.stencilClear}};

    const VkRenderingAttachmentInfo colorAttachmentInfo{
        .sType              = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO_KHR,
        .pNext              = VK_NULL_HANDLE,
        .imageView          = params.colorImageView == VK_NULL_HANDLE ? VK_NULL_HANDLE : params.colorImageView->imageView,
        .imageLayout        = VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL_KHR,
        .resolveMode        = VK_RESOLVE_MODE_NONE,
        .resolveImageView   = VK_NULL_HANDLE,
        .resolveImageLayout = VK_IMAGE_LAYOUT_UNDEFINED,
        .loadOp             = VK_ATTACHMENT_LOAD_OP_CLEAR,
        .storeOp            = VK_ATTACHMENT_STORE_OP_STORE,
        .clearValue         = colorClearValue,
    };
    const VkRenderingAttachmentInfo depthAttachmentInfo{
        .sType              = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO_KHR,
        .pNext              = VK_NULL_HANDLE,
        .imageView          = params.depthImageView == VK_NULL_HANDLE ? VK_NULL_HANDLE : params.depthImageView->imageView,
        .imageLayout        = VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL_KHR,
        .resolveMode        = VK_RESOLVE_MODE_NONE,
        .resolveImageView   = VK_NULL_HANDLE,
        .resolveImageLayout = VK_IMAGE_LAYOUT_UNDEFINED,
        .loadOp             = VK_ATTACHMENT_LOAD_OP_CLEAR,
        .storeOp            = VK_ATTACHMENT_STORE_OP_STORE,
        .clearValue         = depthClearValue,
    };

    const VkRenderingInfo renderingInfo{
        .sType = VK_STRUCTURE_TYPE_RENDERING_INFO_KHR,
        .pNext = VK_NULL_HANDLE,
        .flags = 0,
        .renderArea{.offset = {}, .extent = {params.width, params.height}},
        .layerCount           = 1,
        .viewMask             = 0,
        .colorAttachmentCount = params.colorImageView == VK_NULL_HANDLE ? 0U : 1U,
        .pColorAttachments    = params.colorImageView == VK_NULL_HANDLE ? VK_NULL_HANDLE : &colorAttachmentInfo,
        .pDepthAttachment     = params.depthImageView == VK_NULL_HANDLE ? VK_NULL_HANDLE : &depthAttachmentInfo,
        .pStencilAttachment   = params.depthImageView == VK_NULL_HANDLE ? VK_NULL_HANDLE : &depthAttachmentInfo,
    };

    commandBuffer.vkCmdBeginRenderingKHR(commandBuffer.commandBuffer, &renderingInfo);
}
inline void EndRender(const SVkCommandBuffer& commandBuffer) noexcept
{
    if (commandBuffer.commandBuffer == VK_NULL_HANDLE)
    {
        return;
    }
    commandBuffer.vkCmdEndRenderingKHR(commandBuffer.commandBuffer);
}

inline void SetViewport(const SVkCommandBuffer& commandBuffer, const float xPos, const float yPos, const float width, const float height, const float minDepth, const float maxDepth) noexcept
{
    if (commandBuffer.commandBuffer == VK_NULL_HANDLE)
    {
        return;
    }
    const VkViewport viewport{.x = xPos, .y = yPos, .width = width, .height = height, .minDepth = minDepth, .maxDepth = maxDepth};
    commandBuffer.vkCmdSetViewportWithCountEXT(commandBuffer.commandBuffer, 1, &viewport);
}
inline void SetScissor(const SVkCommandBuffer& commandBuffer, const uint32_t scissorWidth, const uint32_t scissorHeight, const int32_t scissorX, const int32_t scissorY) noexcept
{
    if (commandBuffer.commandBuffer == VK_NULL_HANDLE)
    {
        return;
    }
    const VkRect2D scissor{.offset = {scissorX, scissorY}, .extent = {scissorWidth, scissorHeight}};
    commandBuffer.vkCmdSetScissorWithCountEXT(commandBuffer.commandBuffer, 1, &scissor);
}
inline void SetAlphaToCoverageEnable(const SVkCommandBuffer& commandBuffer, const bool enable) noexcept
{
    if (commandBuffer.commandBuffer == VK_NULL_HANDLE)
    {
        return;
    }
    commandBuffer.vkCmdSetAlphaToCoverageEnableEXT(commandBuffer.commandBuffer, static_cast<VkBool32>(enable));
}
inline void SetColorBlendEnable(const SVkCommandBuffer& commandBuffer, const bool enable) noexcept
{
    if (commandBuffer.commandBuffer == VK_NULL_HANDLE)
    {
        return;
    }
    const VkBool32 enableColorBlend{enable};
    commandBuffer.vkCmdSetColorBlendEnableEXT(commandBuffer.commandBuffer, 0, 1, &enableColorBlend);
}
inline void SetColorBlendEquation(const SVkCommandBuffer& commandBuffer, const uint32_t srcColorBlendFactor, const uint32_t dstColorBlendFactor, const uint32_t colorBlendOp, const uint32_t srcAlphaBlendFactor,
                                  const uint32_t dstAlphaBlendFactor, const uint32_t alphaBlendOp) noexcept
{
    if (commandBuffer.commandBuffer == VK_NULL_HANDLE)
    {
        return;
    }
    const VkColorBlendEquationEXT blendEquation{static_cast<VkBlendFactor>(srcColorBlendFactor), static_cast<VkBlendFactor>(dstColorBlendFactor), static_cast<VkBlendOp>(colorBlendOp),
                                                static_cast<VkBlendFactor>(srcAlphaBlendFactor), static_cast<VkBlendFactor>(dstAlphaBlendFactor), static_cast<VkBlendOp>(alphaBlendOp)};
    commandBuffer.vkCmdSetColorBlendEquationEXT(commandBuffer.commandBuffer, 0, 1, &blendEquation);
}
inline void SetColorWriteMask(const SVkCommandBuffer& commandBuffer, const uint32_t mask) noexcept
{
    if (commandBuffer.commandBuffer == VK_NULL_HANDLE)
    {
        return;
    }
    const VkColorComponentFlags colorFlags{mask};
    commandBuffer.vkCmdSetColorWriteMaskEXT(commandBuffer.commandBuffer, 0, 1, &colorFlags);
}
inline void SetCullMode(const SVkCommandBuffer& commandBuffer, const uint8_t cullMode) noexcept
{
    if (commandBuffer.commandBuffer == VK_NULL_HANDLE)
    {
        return;
    }
    commandBuffer.vkCmdSetCullModeEXT(commandBuffer.commandBuffer, cullMode);
}
inline void SetDepthBiasEnable(const SVkCommandBuffer& commandBuffer, const bool enable) noexcept
{
    if (commandBuffer.commandBuffer == VK_NULL_HANDLE)
    {
        return;
    }
    commandBuffer.vkCmdSetDepthBiasEnableEXT(commandBuffer.commandBuffer, static_cast<VkBool32>(enable));
}
inline void SetDepthCompareOperator(const SVkCommandBuffer& commandBuffer, const uint8_t compareOp) noexcept
{
    if (commandBuffer.commandBuffer == VK_NULL_HANDLE)
    {
        return;
    }
    commandBuffer.vkCmdSetDepthCompareOpEXT(commandBuffer.commandBuffer, static_cast<VkCompareOp>(compareOp));
}
inline void SetDepthTestEnable(const SVkCommandBuffer& commandBuffer, const bool enable) noexcept
{
    if (commandBuffer.commandBuffer == VK_NULL_HANDLE)
    {
        return;
    }
    commandBuffer.vkCmdSetDepthTestEnableEXT(commandBuffer.commandBuffer, static_cast<VkBool32>(enable));
}
inline void SetDepthWriteEnable(const SVkCommandBuffer& commandBuffer, const bool enable) noexcept
{
    if (commandBuffer.commandBuffer == VK_NULL_HANDLE)
    {
        return;
    }
    commandBuffer.vkCmdSetDepthWriteEnableEXT(commandBuffer.commandBuffer, static_cast<VkBool32>(enable));
}
inline void SetFrontFace(const SVkCommandBuffer& commandBuffer, const bool isClockwise) noexcept
{
    if (commandBuffer.commandBuffer == VK_NULL_HANDLE)
    {
        return;
    }
    commandBuffer.vkCmdSetFrontFaceEXT(commandBuffer.commandBuffer, static_cast<VkFrontFace>(isClockwise));
}
inline void SetLineWidth(const SVkCommandBuffer& commandBuffer, const float lineWidth) noexcept
{
    if (commandBuffer.commandBuffer == VK_NULL_HANDLE)
    {
        return;
    }
    vkCmdSetLineWidth(commandBuffer.commandBuffer, lineWidth);
}
inline void SetPolygonMode(const SVkCommandBuffer& commandBuffer, const uint32_t mode) noexcept
{
    if (commandBuffer.commandBuffer == VK_NULL_HANDLE)
    {
        return;
    }
    commandBuffer.vkCmdSetPolygonModeEXT(commandBuffer.commandBuffer, static_cast<VkPolygonMode>(mode));
}
inline void SetPrimitiveRestartEnable(const SVkCommandBuffer& commandBuffer, const bool enable) noexcept
{
    if (commandBuffer.commandBuffer == VK_NULL_HANDLE)
    {
        return;
    }
    commandBuffer.vkCmdSetPrimitiveRestartEnableEXT(commandBuffer.commandBuffer, static_cast<VkBool32>(enable));
}
inline void SetPrimitiveTopology(const SVkCommandBuffer& commandBuffer, const uint8_t topology) noexcept
{
    if (commandBuffer.commandBuffer == VK_NULL_HANDLE)
    {
        return;
    }
    commandBuffer.vkCmdSetPrimitiveTopologyEXT(commandBuffer.commandBuffer, static_cast<VkPrimitiveTopology>(topology));
}
inline void SetRasterizationSamples(const SVkCommandBuffer& commandBuffer, const uint8_t sampleSize, const uint32_t mask) noexcept
{
    if (commandBuffer.commandBuffer == VK_NULL_HANDLE)
    {
        return;
    }
    const VkSampleMask sampleMask{mask};
    commandBuffer.vkCmdSetRasterizationSamplesEXT(commandBuffer.commandBuffer, static_cast<VkSampleCountFlagBits>(sampleSize));
    commandBuffer.vkCmdSetSampleMaskEXT(commandBuffer.commandBuffer, static_cast<VkSampleCountFlagBits>(sampleSize), &sampleMask);
}
inline void SetRasterizerDiscardEnable(const SVkCommandBuffer& commandBuffer, const bool enable) noexcept
{
    if (commandBuffer.commandBuffer == VK_NULL_HANDLE)
    {
        return;
    }
    commandBuffer.vkCmdSetRasterizerDiscardEnableEXT(commandBuffer.commandBuffer, enable ? VK_TRUE : VK_FALSE);
}
inline void SetStencilTestEnable(const SVkCommandBuffer& commandBuffer, const bool enable) noexcept
{
    if (commandBuffer.commandBuffer == VK_NULL_HANDLE)
    {
        return;
    }
    commandBuffer.vkCmdSetStencilTestEnableEXT(commandBuffer.commandBuffer, static_cast<VkBool32>(enable));
}
inline void SetVertexInput(const SVkCommandBuffer& commandBuffer, const uint32_t vertexSize, const uint32_t instanceSize, std::vector<SVertexAttributes>& attributes) noexcept
{
    if (commandBuffer.commandBuffer == VK_NULL_HANDLE || attributes.empty())
    {
        return;
    }
    std::vector<VkVertexInputBindingDescription2EXT> vertexInputBinding{{
        .sType     = VK_STRUCTURE_TYPE_VERTEX_INPUT_BINDING_DESCRIPTION_2_EXT,
        .pNext     = VK_NULL_HANDLE,
        .binding   = 0,
        .stride    = vertexSize,
        .inputRate = VK_VERTEX_INPUT_RATE_VERTEX,
        .divisor   = 1,
    }};
    if (instanceSize != 0)
    {
        vertexInputBinding.emplace_back(VK_STRUCTURE_TYPE_VERTEX_INPUT_BINDING_DESCRIPTION_2_EXT, VK_NULL_HANDLE, 1, instanceSize, VK_VERTEX_INPUT_RATE_INSTANCE, 1);
    }

    std::vector<VkVertexInputAttributeDescription2EXT> vertexAttributes{attributes.size()};
    for (std::size_t i = 0; i < attributes.size(); ++i)
    {
        vertexAttributes[i].sType    = VK_STRUCTURE_TYPE_VERTEX_INPUT_ATTRIBUTE_DESCRIPTION_2_EXT;
        vertexAttributes[i].pNext    = VK_NULL_HANDLE;
        vertexAttributes[i].location = attributes[i].location;
        vertexAttributes[i].binding  = attributes[i].binding;
        vertexAttributes[i].format   = static_cast<VkFormat>(attributes[i].format);
        vertexAttributes[i].offset   = attributes[i].offset;
    }
    commandBuffer.vkCmdSetVertexInputEXT(commandBuffer.commandBuffer, static_cast<uint32_t>(vertexInputBinding.size()), vertexInputBinding.data(), static_cast<uint32_t>(vertexAttributes.size()), vertexAttributes.data());
}
inline void SetVertexInput(const SVkCommandBuffer& commandBuffer) noexcept
{
    if (commandBuffer.commandBuffer == VK_NULL_HANDLE)
    {
        return;
    }
    constexpr VkVertexInputBindingDescription2EXT vertexInputBinding{
        .sType     = VK_STRUCTURE_TYPE_VERTEX_INPUT_BINDING_DESCRIPTION_2_EXT,
        .pNext     = VK_NULL_HANDLE,
        .binding   = 0,
        .stride    = 0,
        .inputRate = VK_VERTEX_INPUT_RATE_VERTEX,
        .divisor   = 1,
    };
    commandBuffer.vkCmdSetVertexInputEXT(commandBuffer.commandBuffer, 1, &vertexInputBinding, 0, VK_NULL_HANDLE);
}
inline void BindShader(const SVkCommandBuffer& commandBuffer, const SVkShader& shader) noexcept
{
    if (commandBuffer.commandBuffer == VK_NULL_HANDLE || shader.shaders.empty() || shader.stages.empty())
    {
        return;
    }
    commandBuffer.vkCmdBindShadersEXT(commandBuffer.commandBuffer, static_cast<uint32_t>(shader.stages.size()), shader.stages.data(), shader.shaders.data());
}
inline void BindDescriptor(const SVkCommandBuffer& commandBuffer, const SVkDescriptor& descriptor) noexcept
{
    if (commandBuffer.commandBuffer == VK_NULL_HANDLE || descriptor.pipelineLayout == VK_NULL_HANDLE || descriptor.descriptorSet == VK_NULL_HANDLE)
    {
        return;
    }
    vkCmdBindDescriptorSets(commandBuffer.commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, descriptor.pipelineLayout, 0, 1, &descriptor.descriptorSet, 0, VK_NULL_HANDLE);
}
inline void BindIndexBufferBuffer(const SVkCommandBuffer& commandBuffer, const SVkBuffer& buffer) noexcept
{
    if (commandBuffer.commandBuffer == VK_NULL_HANDLE || buffer.buffer == VK_NULL_HANDLE)
    {
        return;
    }
    vkCmdBindIndexBuffer(commandBuffer.commandBuffer, buffer.buffer, 0, VK_INDEX_TYPE_UINT32); // todo : make offset and index type flexible
}
inline void BindVertexBufferBuffer(const SVkCommandBuffer& commandBuffer, const SVkBuffer& buffer) noexcept
{
    if (commandBuffer.commandBuffer == VK_NULL_HANDLE || buffer.buffer == VK_NULL_HANDLE)
    {
        return;
    }
    const std::array vertexBuffers {buffer.buffer};
    constexpr std::array<VkDeviceSize, 1> offset{};
    vkCmdBindVertexBuffers(commandBuffer.commandBuffer, 0, 1U, vertexBuffers.data(), offset.data());
}
inline void BindInstanceBufferBuffer(const SVkCommandBuffer& commandBuffer, const SVkBuffer& buffer) noexcept
{
    if (commandBuffer.commandBuffer == VK_NULL_HANDLE || buffer.buffer == VK_NULL_HANDLE)
    {
        return;
    }
    const std::array instanceBuffer {buffer.buffer};
    constexpr std::array<VkDeviceSize, 1> offset{};
    vkCmdBindVertexBuffers(commandBuffer.commandBuffer, 1, 1U, instanceBuffer.data(), offset.data());
}

inline void CopyBuffers(const SVkCommandBuffer& commandBuffer, const SVkBuffer& from, const SVkBuffer& to, const uint64_t size) noexcept
{
    if (commandBuffer.commandBuffer == VK_NULL_HANDLE || from.buffer == VK_NULL_HANDLE)
    {
        return;
    }
    BeginSingleCommand(commandBuffer);
    const VkBufferCopy copyRegion{.srcOffset = 0ULL, .dstOffset = 0ULL, .size = size};
    vkCmdCopyBuffer(commandBuffer.commandBuffer, from.buffer, to.buffer, 1, &copyRegion);
    EndCommand(commandBuffer);
}
inline auto CopyToImage(const SVkCommandBuffer& commandBuffer, const SVkBuffer& buffer, const SVkImage& image, const uint32_t width, const uint32_t height, const uint32_t depth) noexcept
{
    if (BeginSingleCommand(commandBuffer) != 0)
    {
        return -1;
    }

    const VkBufferImageCopy region{
        .bufferOffset                    = 0,
        .bufferRowLength                 = 0,
        .bufferImageHeight               = 0,
        .imageSubresource = {
            .aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT,
            .mipLevel       = 0,
            .baseArrayLayer = 0,
            .layerCount     = 1,
        },
        .imageOffset                     = {0, 0, 0},
        .imageExtent                     = {width, height, depth},
    };

    vkCmdCopyBufferToImage(commandBuffer.commandBuffer, buffer.buffer, image.image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

    if (EndCommand(commandBuffer) != 0)
    {
        return -1;
    }
    return 0;
}

inline auto GenerateMips(const SVkCommandBuffer& commandBuffer, const SVkImage& image, const uint32_t width, const uint32_t height, const uint32_t mipCount) noexcept
{
    BeginCommand(commandBuffer);

    VkImageMemoryBarrier barrier{.sType               = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
                                 .pNext               = VK_NULL_HANDLE,
                                 .srcAccessMask       = 0,
                                 .dstAccessMask       = 0,
                                 .oldLayout           = VK_IMAGE_LAYOUT_UNDEFINED,
                                 .newLayout           = VK_IMAGE_LAYOUT_UNDEFINED,
                                 .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
                                 .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
                                 .image               = image.image,
                                 .subresourceRange    = {
                                        .aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT,
                                        .baseMipLevel   = 0,
                                        .levelCount     = 1,
                                        .baseArrayLayer = 0,
                                        .layerCount     = 1,
                                 }};

    int32_t mipWidth  = width;
    int32_t mipHeight = height;

    for (uint32_t i = 1; i < mipCount; i++)
    {
        barrier.subresourceRange.baseMipLevel = i - 1;
        barrier.oldLayout                     = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
        barrier.newLayout                     = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
        barrier.srcAccessMask                 = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.dstAccessMask                 = VK_ACCESS_TRANSFER_READ_BIT;

        vkCmdPipelineBarrier(commandBuffer.commandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, VK_NULL_HANDLE, 0, VK_NULL_HANDLE, 1, &barrier);

        const VkImageBlit blit{
        .srcSubresource = {
            .aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT,
            .mipLevel       = i - 1,
            .baseArrayLayer = 0,
            .layerCount     = 1,
        },
        .srcOffsets                 = {{0, 0, 0},{mipWidth, mipHeight, 1}},
        .dstSubresource{
            .aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT,
        .mipLevel       = i,
        .baseArrayLayer = 0,
        .layerCount     = 1,
        },
        .dstOffsets                 = {{0, 0, 0},{mipWidth > 1 ? mipWidth / 2 : 1, mipHeight > 1 ? mipHeight / 2 : 1, 1}},
        };

        vkCmdBlitImage(commandBuffer.commandBuffer, image.image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, image.image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &blit, VK_FILTER_LINEAR);

        barrier.oldLayout     = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
        barrier.newLayout     = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

        vkCmdPipelineBarrier(commandBuffer.commandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, VK_NULL_HANDLE, 0, VK_NULL_HANDLE, 1, &barrier);

        if (mipWidth > 1)
            mipWidth <<= 2;
        if (mipHeight > 1)
            mipHeight <<= 2;
    }

    barrier.subresourceRange.baseMipLevel = mipCount - 1;
    barrier.oldLayout                     = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    barrier.newLayout                     = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    barrier.srcAccessMask                 = VK_ACCESS_TRANSFER_WRITE_BIT;
    barrier.dstAccessMask                 = VK_ACCESS_SHADER_READ_BIT;

    vkCmdPipelineBarrier(commandBuffer.commandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, VK_NULL_HANDLE, 0, VK_NULL_HANDLE, 1, &barrier);

    if (EndCommand(commandBuffer) != 0)
    {
        return -1;
    }
    return 0;
}
inline auto TransitionImageLayout(const SVkCommandBuffer& commandBuffer, SVkImage& image, const uint32_t newLayout, const uint32_t mipOffset = 0, const uint32_t mipCount = 1, const uint32_t layerOffset = 0,
                                  const uint32_t layerCount = 1) noexcept
{
    if (commandBuffer.commandBuffer == nullptr || image.image == nullptr)
    {
        return -1;
    }

    VkImageMemoryBarrier barrier{
        .sType               = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
        .pNext               = VK_NULL_HANDLE,
        .srcAccessMask       = 0,
        .dstAccessMask       = 0,
        .oldLayout           = image.layout,
        .newLayout           = static_cast<VkImageLayout>(newLayout),
        .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
        .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
        .image               = image.image,
        .subresourceRange    = {
               .aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT,
               .baseMipLevel   = mipOffset,
               .levelCount     = mipCount,
               .baseArrayLayer = layerOffset,
               .layerCount     = layerCount,
        },};

    VkPipelineStageFlags sourceStage      = 0;
    VkPipelineStageFlags destinationStage = 0;

    switch (image.layout)
    {
    case VK_IMAGE_LAYOUT_UNDEFINED:
        barrier.srcAccessMask = 0;
        sourceStage           = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        break;

    case VK_IMAGE_LAYOUT_PREINITIALIZED:
        barrier.srcAccessMask = VK_ACCESS_HOST_WRITE_BIT;
        sourceStage           = VK_PIPELINE_STAGE_HOST_BIT;
        break;

    case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
        barrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        sourceStage           = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        break;

    case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
        barrier.srcAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
        sourceStage           = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
        break;

    case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
        sourceStage           = VK_PIPELINE_STAGE_TRANSFER_BIT;
        break;

    case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        sourceStage           = VK_PIPELINE_STAGE_TRANSFER_BIT;
        break;

    case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
        barrier.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
        sourceStage           = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
        break;
    default: break;
    }

    switch (newLayout)
    {
    case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        destinationStage      = VK_PIPELINE_STAGE_TRANSFER_BIT;
        break;

    case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
        destinationStage      = VK_PIPELINE_STAGE_TRANSFER_BIT;
        break;

    case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
        barrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        destinationStage      = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        break;

    case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
        barrier.dstAccessMask |= VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
        destinationStage       = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
        break;

    case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
        if (barrier.srcAccessMask == 0)
        {
            barrier.srcAccessMask = VK_ACCESS_HOST_WRITE_BIT | VK_ACCESS_TRANSFER_WRITE_BIT;
        }
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
        destinationStage      = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
        break;

    case VK_IMAGE_LAYOUT_PREINITIALIZED: destinationStage = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT; break;
    default: break;
    }

    vkCmdPipelineBarrier(commandBuffer.commandBuffer, sourceStage, destinationStage, 0, 0, VK_NULL_HANDLE, 0, VK_NULL_HANDLE, 1, &barrier);
    image.layout = static_cast<VkImageLayout>(newLayout);
    if(newLayout == VK_IMAGE_LAYOUT_PRESENT_SRC_KHR)
    {
        image.layout = VK_IMAGE_LAYOUT_UNDEFINED;
    }
    return 0;
}

inline void DrawFullscreen(const SVkCommandBuffer& commandBuffer) noexcept
{
    if (commandBuffer.commandBuffer == VK_NULL_HANDLE)
    {
        return;
    }
    vkCmdDraw(commandBuffer.commandBuffer, 3, 1, 0, 0);
}
inline void DrawIndirect(const SVkCommandBuffer& commandBuffer, const SVkBuffer& buffer, const uint32_t offset, const uint32_t count, const uint32_t stride) noexcept
{
    if (commandBuffer.commandBuffer == VK_NULL_HANDLE || buffer.buffer == VK_NULL_HANDLE)
    {
        return;
    }
    vkCmdDrawIndirect(commandBuffer.commandBuffer, buffer.buffer, offset, count, stride);
}
inline void DrawIndexed(const SVkCommandBuffer& commandBuffer, const uint32_t indexCount, const uint32_t instanceCount, const uint32_t firstIndex, const uint32_t firstInstance) noexcept
{
    if (commandBuffer.commandBuffer == VK_NULL_HANDLE || indexCount == 0)
    {
        return;
    }
    vkCmdDrawIndexed(commandBuffer.commandBuffer, indexCount, instanceCount, firstIndex, 0, firstInstance);
}
} // namespace DeerVulkan
