//
// Copyright (c) 2024.
// Author: Joran.
//

module;
#include "API/Vulkan/DeerVulkan.hpp"
export module FawnVision.RenderPassContext;
import FawnAlgebra.Arithmetics;
import FawnVision.Buffer;
import FawnVision.Enum;
import FawnVision.Shader;
import FawnVision.Descriptor;
import FawnVision.Mesh;
using namespace FawnAlgebra;

namespace FawnVision
{
template <typename T>
concept IsContainer = requires(T t) {
    typename T::value_type;
    {
        std::size(t)
    } -> std::convertible_to<std::size_t>;
    {
        std::data(t)
    } -> std::convertible_to<typename T::value_type*>;
};

#pragma region defenition
export struct SVertexAttributes;
export struct SRenderPassContext;

export inline void SetViewport(const SRenderPassContext& renderContext, float xPos, float yPos, float width, float height, float minDepth = 0.0f, float maxDepth = 1.0f) noexcept;
export inline void SetScissor(const SRenderPassContext& renderContext, uint32 width, uint32 height, uint32 xPos, uint32 yPos) noexcept;
export inline void SetAlphaToCoverageEnable(const SRenderPassContext& renderContext, bool enable) noexcept;
export inline void SetColorBlendEnable(const SRenderPassContext& renderContext, bool enable) noexcept;
export inline void SetColorBlendEquation(const SRenderPassContext& renderContext, blend_factor srcColorBlendFactor, blend_factor dstColorBlendFactor, blend_operator colorBlendOp, blend_factor srcAlphaBlendFactor,
                                         blend_factor dstAlphaBlendFactor, blend_operator alphaBlendOp) noexcept;
export inline void SetColorWriteMask(const SRenderPassContext& renderContext, color_component mask) noexcept;
export inline void SetCullMode(const SRenderPassContext& renderContext, cull_mode cullMode) noexcept;
export inline void SetDepthBiasEnable(const SRenderPassContext& renderContext, bool enable) noexcept;
export inline void SetDepthCompareOperator(const SRenderPassContext& renderContext, compare_operator depthOperator) noexcept;
export inline void SetDepthTestEnable(const SRenderPassContext& renderContext, bool enable) noexcept;
export inline void SetDepthWriteEnable(const SRenderPassContext& renderContext, bool enable) noexcept;
export inline void SetFrontFace(const SRenderPassContext& renderContext, bool isClockWice) noexcept;
export inline void SetLineWidth(const SRenderPassContext& renderContext, float lineWidth) noexcept;
export inline void SetPolygonMode(const SRenderPassContext& renderContext, polygon_mode mode) noexcept;
export inline void SetPrimitiveRestartEnable(const SRenderPassContext& renderContext, bool enable) noexcept;
export inline void SetPrimitiveTopology(const SRenderPassContext& renderContext, primitive_topology topology) noexcept;
export inline void SetRasterizationSamples(const SRenderPassContext& renderContext, uint8 samples, uint32 mask = 0xFFFFFFFF) noexcept;
export inline void SetRasterizerDiscardEnable(const SRenderPassContext& renderContext, bool enable) noexcept;
export inline void SetStencilTestEnable(const SRenderPassContext& renderContext, bool enable) noexcept;

export template <typename T>
void CopyData(const SRenderPassContext& renderContext, const SBuffer& buffer, uint64 size, T const* pData) noexcept;
export template <typename Container>
    requires IsContainer<Container> && std::is_same_v<typename Container::value_type, SVertexAttributes>
void SetVertexInput(const SRenderPassContext& renderContext, const Container& attributes, uint32 vertexSize, uint32 instanceSize) noexcept;
export inline void SetVertexInput(const SRenderPassContext& renderContext) noexcept;
export inline void BindShader(const SRenderPassContext& renderContext, const SShader& shader) noexcept;
export inline void BindDescriptor(const SRenderPassContext& renderContext, const SDescriptor& descriptor) noexcept;
export inline void BindMesh(const SRenderPassContext& renderPassContext, const SMesh& mesh) noexcept;
export inline void DrawFullscreen(const SRenderPassContext& renderContext) noexcept;
export inline void Draw(const SRenderPassContext& renderContext, uint32 indexCount, uint32 instanceCount, uint32 indexOffset, uint32 instanceOffset) noexcept;
#pragma endregion

#pragma region implementation
struct SVertexAttributes
{
    uint32 location;
    uint32 binding;
    format format;
    uint32 offset;
    bool isInstance;
};
struct SRenderPassContext
{
    const DeerVulkan::SVkDevice& device;
    const DeerVulkan::SVkCommandBuffer& commandBuffer;
    DeerVulkan::SVkSwapChain& swapChain;
    const DeerVulkan::SVkSemaphore& timelineSemaphore;
    const DeerVulkan::SVkSemaphore& binarySemaphore;
    const DeerVulkan::SVkQueue* queue;
};

inline void SetViewport(const SRenderPassContext& renderContext, const float xPos, const float yPos, const float width, const float height, const float minDepth, const float maxDepth) noexcept
{
    DeerVulkan::SetViewport(renderContext.commandBuffer, xPos, yPos, width, height, minDepth, maxDepth);
}
inline void SetScissor(const SRenderPassContext& renderContext, const uint32 width, const uint32 height, const uint32 xPos, const uint32 yPos) noexcept
{
    DeerVulkan::SetScissor(renderContext.commandBuffer, width, height, xPos, yPos);
}
inline void SetAlphaToCoverageEnable(const SRenderPassContext& renderContext, const bool enable) noexcept
{
    DeerVulkan::SetAlphaToCoverageEnable(renderContext.commandBuffer, enable);
}
inline void SetColorBlendEnable(const SRenderPassContext& renderContext, const bool enable) noexcept
{
    DeerVulkan::SetColorBlendEnable(renderContext.commandBuffer, enable);
}
inline void SetColorBlendEquation(const SRenderPassContext& renderContext, const blend_factor srcColorBlendFactor, blend_factor dstColorBlendFactor, blend_operator colorBlendOp, blend_factor srcAlphaBlendFactor,
                                  blend_factor dstAlphaBlendFactor, blend_operator alphaBlendOp) noexcept
{
    DeerVulkan::SetColorBlendEquation(renderContext.commandBuffer, static_cast<uint32>(srcColorBlendFactor), static_cast<uint32>(dstColorBlendFactor), static_cast<uint32>(colorBlendOp), static_cast<uint32>(srcAlphaBlendFactor),
                                      static_cast<uint32>(dstAlphaBlendFactor), static_cast<uint32>(alphaBlendOp));
}
inline void SetColorWriteMask(const SRenderPassContext& renderContext, const color_component mask) noexcept
{
    DeerVulkan::SetColorWriteMask(renderContext.commandBuffer, static_cast<uint32>(mask));
}
inline void SetCullMode(const SRenderPassContext& renderContext, const cull_mode cullMode) noexcept
{
    DeerVulkan::SetCullMode(renderContext.commandBuffer, static_cast<uint8>(cullMode));
}
inline void SetDepthBiasEnable(const SRenderPassContext& renderContext, const bool enable) noexcept
{
    DeerVulkan::SetDepthBiasEnable(renderContext.commandBuffer, enable);
}
inline void SetDepthCompareOperator(const SRenderPassContext& renderContext, const compare_operator depthOperator) noexcept
{
    DeerVulkan::SetDepthCompareOperator(renderContext.commandBuffer, static_cast<uint8>(depthOperator));
}
inline void SetDepthTestEnable(const SRenderPassContext& renderContext, const bool enable) noexcept
{
    DeerVulkan::SetDepthTestEnable(renderContext.commandBuffer, enable);
}
inline void SetDepthWriteEnable(const SRenderPassContext& renderContext, const bool enable) noexcept
{
    DeerVulkan::SetDepthWriteEnable(renderContext.commandBuffer, enable);
}
inline void SetFrontFace(const SRenderPassContext& renderContext, const bool isClockWice) noexcept
{
    DeerVulkan::SetFrontFace(renderContext.commandBuffer, isClockWice);
}
inline void SetLineWidth(const SRenderPassContext& renderContext, const float lineWidth) noexcept
{
    DeerVulkan::SetLineWidth(renderContext.commandBuffer, lineWidth);
}
inline void SetPolygonMode(const SRenderPassContext& renderContext, const polygon_mode mode) noexcept
{
    DeerVulkan::SetPolygonMode(renderContext.commandBuffer, static_cast<uint32>(mode));
}
inline void SetPrimitiveRestartEnable(const SRenderPassContext& renderContext, const bool enable) noexcept
{
    SetPrimitiveRestartEnable(renderContext.commandBuffer, enable);
}
inline void SetPrimitiveTopology(const SRenderPassContext& renderContext, const primitive_topology topology) noexcept
{
    DeerVulkan::SetPrimitiveTopology(renderContext.commandBuffer, static_cast<uint8>(topology));
}
inline void SetRasterizationSamples(const SRenderPassContext& renderContext, const uint8 samples, const uint32 mask) noexcept
{
    DeerVulkan::SetRasterizationSamples(renderContext.commandBuffer, samples, mask);
}
inline void SetRasterizerDiscardEnable(const SRenderPassContext& renderContext, const bool enable) noexcept
{
    DeerVulkan::SetRasterizerDiscardEnable(renderContext.commandBuffer, enable);
}
inline void SetStencilTestEnable(const SRenderPassContext& renderContext, const bool enable) noexcept
{
    DeerVulkan::SetStencilTestEnable(renderContext.commandBuffer, enable);
}
template <typename T>
void CopyData(const SRenderPassContext& renderContext, const SBuffer& buffer, const uint64 size, T const* const pData) noexcept
{
    CopyData(renderContext.device, buffer.buffer, size, pData);
}
template <typename Container>
    requires IsContainer<Container> && std::is_same_v<typename Container::value_type, SVertexAttributes>
void SetVertexInput(const SRenderPassContext& renderContext, const Container& attributes, const uint32 vertexSize, const uint32 instanceSize) noexcept
{
    std::vector<DeerVulkan::SVertexAttributes> vertexAttributes{std::size(attributes)};
    for (std::size_t i = 0; i < std::size(attributes); ++i)
    {
        vertexAttributes[i].location   = attributes[i].location;
        vertexAttributes[i].binding    = attributes[i].binding;
        vertexAttributes[i].format     = static_cast<uint32>(attributes[i].format);
        vertexAttributes[i].offset     = attributes[i].offset;
        vertexAttributes[i].isInstance = attributes[i].isInstance;
    }
    DeerVulkan::SetVertexInput(renderContext.commandBuffer, vertexSize, instanceSize, vertexAttributes);
}
inline void SetVertexInput(const SRenderPassContext& renderContext) noexcept
{
    DeerVulkan::SetVertexInput(renderContext.commandBuffer);
}
inline void BindShader(const SRenderPassContext& renderContext, const SShader& shader) noexcept
{
    DeerVulkan::BindShader(renderContext.commandBuffer, shader.shader);
}
inline void BindDescriptor(const SRenderPassContext& renderContext, const SDescriptor& descriptor) noexcept
{
    BindDescriptor(renderContext.commandBuffer, descriptor.descriptor);
}
inline void BindMesh(const SRenderPassContext& renderPassContext, const SMesh& mesh) noexcept
{
    BindIndexBufferBuffer(renderPassContext.commandBuffer, mesh.indexBuffer);
    BindVertexBufferBuffer(renderPassContext.commandBuffer, mesh.vertexBuffer);
}
inline void DrawFullscreen(const SRenderPassContext& renderContext) noexcept
{
    DeerVulkan::DrawFullscreen(renderContext.commandBuffer);
}
inline void Draw(const SRenderPassContext& renderContext, const uint32 indexCount, const uint32 instanceCount, const uint32 indexOffset, const uint32 instanceOffset) noexcept
{
    DrawIndexed(renderContext.commandBuffer, indexCount, instanceCount, indexOffset, instanceOffset);
}
#pragma endregion
} // namespace FawnVision
