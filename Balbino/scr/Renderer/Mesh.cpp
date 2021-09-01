#include "pch.h"
#include "Mesh.h"

Balbino::CMesh::CMesh()
	: m_vertexBuffer{}
	, m_indexBuffer{}
	, m_material{}
	, m_vertices{
		Balbino::SVertex{ .position = { -0.5f, -0.5f, 0.f }, .colour = { 1.0f, 0.0f, 0.0f, 1.0f } },
		Balbino::SVertex{ .position = { 0.5f, -0.5f, 0.f }, .colour = { 0.0f, 1.0f, 0.0f, 1.0f } },
		Balbino::SVertex{ .position = { 0.5f, 0.5f, 0.f }, .colour = { 0.0f, 0.0f, 1.0f, 1.0f } },
		Balbino::SVertex{ .position = { -0.5f, 0.5f, 0.f }, .colour = { 1.0f, 1.0f, 1.0f, 1.0f } }
	}
	, m_indices{0, 1, 2, 2, 3, 0}
{
}

void Balbino::CMesh::Draw(const VkCommandBuffer& commandBuffer) const
{
	m_material.Bind(commandBuffer);
	m_indexBuffer.Bind(commandBuffer);
	m_vertexBuffer.Bind(commandBuffer);
	vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(m_indices.size()), 1, 0, 0, 0);
}

void Balbino::CMesh::Initialize(const VkDevice& device, const VkCommandPool& commandPool, const VkQueue& queue, const VkPhysicalDevice& physicalDevice, const VkExtent2D& swapchainExtent, const VkRenderPass& renderPass, const VkAllocationCallbacks* callbacks)
{
	m_material.Compile(device, swapchainExtent, renderPass, callbacks);
	m_vertexBuffer.Initialize(device, commandPool, queue, physicalDevice, m_vertices, callbacks);
	m_indexBuffer.Initialize(device, commandPool, queue, physicalDevice, m_indices, callbacks);
}

void Balbino::CMesh::Cleanup( const VkDevice& device, const VkAllocationCallbacks* callbacks) 
{
	m_vertexBuffer.Cleanup(device, callbacks);
	m_indexBuffer.Cleanup(device, callbacks);
	m_material.Cleanup(device, callbacks);
}
