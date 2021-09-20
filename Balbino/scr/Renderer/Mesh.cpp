#include "pch.h"
#include "Mesh.h"
#include "../Managers/Manager.h"
#include "../Managers/ShaderManager.h"

void Balbino::CMesh::Draw(const VkCommandBuffer& commandBuffer, const VkDescriptorSet* descriptorSet) const
{
		m_indexBuffer.Bind(commandBuffer);
		m_vertexBuffer.Bind(commandBuffer);
	for (int i = 0; i < m_metadatas.size(); ++i)
	{
		CManager::GetShaderManager()->BindShader(m_metadatas[0].shaderId, commandBuffer, descriptorSet);
		vkCmdDrawIndexed(commandBuffer, m_metadatas[i].indexCount, 1, m_metadatas[i].indexStart, 0, 0);
	}
}

void Balbino::CMesh::Initialize(const VkDevice& device, const VkCommandPool& commandPool, const VkQueue& queue, const VkPhysicalDevice& physicalDevice, const VkAllocationCallbacks* callbacks, const std::vector<SVertex>& vertices, const std::vector<uint32_t >& indices, const std::vector<SMeshMetadata>& metadatas)
{
	m_metadatas = std::move(metadatas);
	m_vertexBuffer.Initialize(device, commandPool, queue, physicalDevice, vertices, callbacks);
	m_indexBuffer.Initialize(device, commandPool, queue, physicalDevice, indices, callbacks);
}

void Balbino::CMesh::Cleanup(const VkDevice& device, const VkAllocationCallbacks* callbacks)
{
	m_vertexBuffer.Cleanup(device, callbacks);
	m_indexBuffer.Cleanup(device, callbacks);
	m_metadatas.clear();
}
