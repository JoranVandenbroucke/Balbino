#include "pch.h"
#include "Mesh.h"

Balbino::CMesh::CMesh()
	: m_vertexBuffer{}
	, m_indexBuffer{}
	, m_material{}
	, m_vertices{
		Balbino::SVertex{ .position = { 0.0f, -0.5f, 0.f }, .colour = { 1.0f, 0.0f, 0.0f, 1.0f } },
		Balbino::SVertex{ .position = { 0.5f, 0.5f, 0.f }, .colour = { 0.0f, 1.0f, 0.0f, 1.0f } },
		Balbino::SVertex{ .position = { -0.5f, 0.5f, 0.f }, .colour = { 0.0f, 0.0f, 1.0f, 1.0f } }
	}
{
}

void Balbino::CMesh::Draw(const VkCommandBuffer& commandBuffer)
{
	m_material.Bind(commandBuffer);
	m_vertexBuffer.Bind(commandBuffer);
	vkCmdDraw(commandBuffer, static_cast<uint32_t>(m_vertices.size()), 1, 0, 0);
}

void Balbino::CMesh::Initialize(const VkDevice& device, const VkPhysicalDevice& physicalDevice, const VkExtent2D& swapchainExtent, const VkRenderPass& renderPass, const VkAllocationCallbacks* callbacks)
{
	m_material.Compile(device, swapchainExtent, renderPass, callbacks);
	m_vertexBuffer.Initialize(device, physicalDevice, m_vertices);
}

void Balbino::CMesh::Cleanup( const VkDevice& device, const VkAllocationCallbacks* callbacks) const
{
	m_vertexBuffer.Cleanup(device);
	m_material.Cleanup(device, callbacks);
}
