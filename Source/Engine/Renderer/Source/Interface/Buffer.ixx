//
// Copyright (c) 2024.
// Author: Joran.
//

module;
#include "API/Vulkan/DeerVulkan.hpp"

export module FawnVision.Buffer;
import FawnAlgebra.Arithmetics;
import FawnVision.Enum;
import FawnVision.Renderer;
using namespace FawnAlgebra;

namespace FawnVision
{
#pragma region defenition
export struct SBuffer;
export inline auto Initialize(const SRenderer& renderer, SBuffer& buffer, uint64 size, uint32 bufferusage, uint32 memoryProperties) noexcept -> int32;
export inline void Cleanup(const SRenderer& renderer, SBuffer& buffer) noexcept;
export template <typename T>
void CopyData(const SRenderer& renderer, const SBuffer& buffer, uint64 size, T const* pData) noexcept;
#pragma endregion

#pragma region implementation
struct SBuffer
{
    DeerVulkan::SVkBuffer buffer;
};

inline auto Initialize(const SRenderer& renderer, SBuffer& buffer, const uint64 size, const uint32 bufferusage, const uint32 memoryProperties) noexcept -> int32
{
    if (Initialize(renderer.device, buffer.buffer, size, bufferusage, memoryProperties))
    {
        return -1;
    }
    return 0;
}
inline void Cleanup(const SRenderer& renderer, SBuffer& buffer) noexcept
{
    Cleanup(renderer.device, buffer.buffer);
}
template <typename T>
void CopyData(const SRenderer& renderer, const SBuffer& buffer, uint64 size, T const* pData) noexcept
{
    CopyData(renderer.device, buffer.buffer, size, pData);
}
#pragma endregion
} // namespace FawnVision