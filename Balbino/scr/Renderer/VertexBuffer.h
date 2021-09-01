#pragma once
#include <vulkan/vulkan_core.h>

#include "Vertex.h"

namespace Balbino
{
	class CVertexBuffer
	{
	public:
		CVertexBuffer();
		~CVertexBuffer();
		CVertexBuffer(const CVertexBuffer&) = delete;
		CVertexBuffer(CVertexBuffer&&) = delete;
		CVertexBuffer& operator=(const CVertexBuffer&) = delete;
		CVertexBuffer& operator=(CVertexBuffer&&) = delete;

		void Initialize(const VkDevice& device, const VkCommandPool& commandPool, const VkQueue& queue, const VkPhysicalDevice& physicalDevice, const std::vector<SVertex>& vertices, const VkAllocationCallbacks* pCallback);
		void Cleanup( const VkDevice& device, const VkAllocationCallbacks* pAlloc );
		void Bind(const VkCommandBuffer& commandBuffer) const;
	private:
		VkBuffer m_vertexBuffer;
		VkDeviceMemory m_vertexBufferMemory;

		static void CopyBuffer(const VkDevice& device, const VkCommandPool& commandPool, const VkQueue& queue, const VkBuffer& srcBuffer, const VkBuffer& dstBuffer, VkDeviceSize& size);
		static void CreateBuffer(const VkDevice& device, const VkAllocationCallbacks* pCallback, const VkPhysicalDevice& physicalDevice, const VkDeviceSize size, const VkBufferUsageFlags usage, const VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
		static uint32_t FindMemoryType(uint32_t typeFilter, const VkMemoryPropertyFlags properties, const VkPhysicalDevice& physicalDevice);
	};
}

