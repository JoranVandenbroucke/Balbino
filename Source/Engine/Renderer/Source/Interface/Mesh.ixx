//
// Copyright (c) 2024.
// Author: Joran.
//

module;
#include "API/Vulkan/DeerVulkan.hpp"

#include <cstddef>
export module FawnVision.Mesh;
import FawnAlgebra.Arithmetics;
import FawnVision.Enum;
import FawnVision.Renderer;
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
export struct SMesh;
export template <typename Vertex, typename IndexContainer, typename VertexContainer>
    requires IsContainer<IndexContainer> && IsContainer<VertexContainer> && std::is_same_v<typename VertexContainer::value_type, Vertex>
    && (std::is_same_v<typename IndexContainer::value_type, uint16> || std::is_same_v<typename IndexContainer::value_type, uint32>)
auto Initialize(const SRenderer& renderer, SMesh& mesh, const IndexContainer& indices, const VertexContainer& vertices) noexcept;
export inline void Cleanup(const SRenderer& renderer, SMesh& mesh) noexcept;

template <typename Container>
    requires IsContainer<Container> && std::is_same_v<typename Container::value_type, uint32>
auto CreateIndexBuffer(const SRenderer& renderer, SMesh& mesh, const Container& indices) noexcept;

template <typename Vertex, typename Container>
    requires IsContainer<Container> && std::is_same_v<typename Container::value_type, Vertex>
auto CreateVertexBuffer(const SRenderer& renderer, SMesh& mesh, const Container& vertices) noexcept;

template <typename Container>
    requires IsContainer<Container> && std::is_same_v<typename Container::value_type, uint32>
auto RecreateIndexBuffer(const DeerVulkan::SVkDevice& device, SMesh& mesh, const Container& indices) noexcept;
template <typename Vertex, typename Container>
    requires IsContainer<Container> && std::is_same_v<typename Container::value_type, Vertex>
auto RecreateVertexBuffer(const DeerVulkan::SVkDevice& device, SMesh& mesh, const Container& vertices) noexcept;
#pragma endregion

#pragma region implementation
struct SMesh
{
    DeerVulkan::SVkBuffer indexBuffer;
    DeerVulkan::SVkBuffer vertexBuffer;
};

template <typename Container>
    requires IsContainer<Container> && std::is_same_v<typename Container::value_type, uint32>
auto CreateIndexBuffer(const SRenderer& renderer, SMesh& mesh, const Container& indices) noexcept
{
    const uint64 size{std::size(indices) * sizeof(uint32)};
    // DeerVulkan::SVkBuffer stagingBuffer;
    // if (Initialize(renderer.device, stagingBuffer, size, static_cast<uint32>(buffer_usage::transfer_src), static_cast<uint32>(memory_property::host_visible) | static_cast<uint32>(memory_property::host_coherent)) != 0)
    // {
    //     return -1;
    // }
    //
    // CopyData(renderer.device, stagingBuffer, size, std::data(indeces));
    //
    // if (Initialize(renderer.device, mesh.indexBuffer, size, static_cast<uint32>(buffer_usage::transfer_dst) | static_cast<uint32>(buffer_usage::index_buffer), static_cast<uint32>(memory_property::device_local)) != 0)
    // {
    //     return -1;
    // }
    // CopyBuffers(renderer.commandBuffer, stagingBuffer, mesh.indexBuffer, size);
    // Cleanup(renderer.device, stagingBuffer);

    if (Initialize(renderer.device, mesh.indexBuffer, size, static_cast<uint32>(buffer_usage::index_buffer), static_cast<uint32>(memory_property::host_visible) | static_cast<uint32>(memory_property::host_coherent)) != 0)
    {
        return -1;
    }

    CopyData(renderer.device, mesh.indexBuffer, size, std::data(indices));
    return 0;
}

template <typename Vertex, typename Container>
    requires IsContainer<Container> && std::is_same_v<typename Container::value_type, Vertex>
auto CreateVertexBuffer(const SRenderer& renderer, SMesh& mesh, const Container& vertices) noexcept
{
    const uint64 size{std::size(vertices) * sizeof(vertices[0])};
    // DeerVulkan::SVkBuffer stagingBuffer;
    // if (Initialize(renderer.device, stagingBuffer, size, static_cast<uint32>(buffer_usage::transfer_src), static_cast<uint32>(memory_property::host_visible) | static_cast<uint32>(memory_property::host_coherent)) != 0)
    // {
    //     return -1;
    // }
    //
    // CopyData(renderer.device, stagingBuffer, size, std::data(vertices));
    //
    // if (Initialize(renderer.device, mesh.vertexBuffer, size, static_cast<uint32>(buffer_usage::transfer_dst) | static_cast<uint32>(buffer_usage::vertex_buffer), static_cast<uint32>(memory_property::device_local)) != 0)
    // {
    //     return -1;
    // }
    // CopyBuffers(renderer.commandBuffer, stagingBuffer, mesh.vertexBuffer, size);
    // Cleanup(renderer.device, stagingBuffer);

    if (Initialize(renderer.device, mesh.vertexBuffer, size, static_cast<uint32>(buffer_usage::vertex_buffer), static_cast<uint32>(memory_property::host_visible) | static_cast<uint32>(memory_property::host_coherent)) != 0)
    {
        return -1;
    }

    CopyData(renderer.device, mesh.vertexBuffer, size, std::data(vertices));
    return 0;
}

template <typename Container>
    requires IsContainer<Container> && std::is_same_v<typename Container::value_type, uint32>
auto RecreateIndexBuffer(const DeerVulkan::SVkDevice& device, SMesh& mesh, const Container& indices) noexcept
{
    Cleanup(device, mesh.indexBuffer);
    return CreateIndexBuffer({.device = device}, mesh.indexBuffer, indices);
}
template <typename Vertex, typename Container>
    requires IsContainer<Container> && std::is_same_v<typename Container::value_type, Vertex>
auto RecreateVertexBuffer(const DeerVulkan::SVkDevice& device, SMesh& mesh, const Container& vertices) noexcept
{
    Cleanup(device, mesh.vertexBuffer);
    return CreateVertexBuffer({.device = device}, mesh.vertexBuffer, vertices);
}

template <typename Vertex, typename IndexContainer, typename VertexContainer>
    requires IsContainer<IndexContainer> && IsContainer<VertexContainer> && std::is_same_v<typename VertexContainer::value_type, Vertex>
    && (std::is_same_v<typename IndexContainer::value_type, uint16> || std::is_same_v<typename IndexContainer::value_type, uint32>)
auto Initialize(const SRenderer& renderer, SMesh& mesh, const IndexContainer& indices, const VertexContainer& vertices) noexcept
{
    if (CreateIndexBuffer<IndexContainer>(renderer, mesh, indices) != 0)
    {
        return -1;
    }
    if (CreateVertexBuffer<Vertex, VertexContainer>(renderer, mesh, vertices) != 0)
    {
        return -1;
    }
    return 0;
}

inline void Cleanup(const SRenderer& renderer, SMesh& mesh) noexcept
{
    Cleanup(renderer.device, mesh.indexBuffer);
    Cleanup(renderer.device, mesh.vertexBuffer);
}
#pragma endregion
} // namespace FawnVision
