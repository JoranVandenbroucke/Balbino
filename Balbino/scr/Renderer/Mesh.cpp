#include "pch.h"
#include "Mesh.h"
#include "../Managers/ShaderManager.h"

Balbino::CMesh::CMesh()
	: m_vertexBuffer{}
	, m_indexBuffer{}
	, m_vertices{
		Balbino::SVertex{.position = { -0.5f, -0.5f, 0.f }, .colour = { 1.0f, 0.0f, 0.0f, 1.0f }, .uv = { 1, 0 } },
		Balbino::SVertex{.position = { 0.5f, -0.5f, 0.f }, .colour = { 0.0f, 1.0f, 0.0f, 1.0f }, .uv = { 0, 0 } },
		Balbino::SVertex{.position = { 0.5f, 0.5f, 0.f }, .colour = { 0.0f, 0.0f, 1.0f, 1.0f }, .uv = { 0, 1 } },
		Balbino::SVertex{.position = { -0.5f, 0.5f, 0.f }, .colour = { 1.0f, 1.0f, 1.0f, 1.0f }, .uv = { 1, 1 } }
}
, m_indices{0, 1, 2, 2, 3, 0}
, m_materialData{{0,0,6}}
{
}

void Balbino::CMesh::Draw(const VkCommandBuffer& commandBuffer) const
{
	m_indexBuffer.Bind(commandBuffer);
	m_vertexBuffer.Bind(commandBuffer);
	for (const SMeshMaterialData& data : m_materialData)
	{
		CShaderManager::BindShader(data.materialIndex, commandBuffer);//todo: change to material
		vkCmdDrawIndexed(commandBuffer, data.indexCount, 1, data.indexStart, 0, 0);
	}
}

void Balbino::CMesh::Initialize(const VkDevice& device, const VkCommandPool& commandPool, const VkQueue& queue, const VkPhysicalDevice& physicalDevice, const VkAllocationCallbacks* callbacks)
{
	m_vertexBuffer.Initialize(device, commandPool, queue, physicalDevice, m_vertices, callbacks);
	m_indexBuffer.Initialize(device, commandPool, queue, physicalDevice, m_indices, callbacks);
}

void Balbino::CMesh::Cleanup(const VkDevice& device, const VkAllocationCallbacks* callbacks)
{
	m_vertexBuffer.Cleanup(device, callbacks);
	m_indexBuffer.Cleanup(device, callbacks);
}
