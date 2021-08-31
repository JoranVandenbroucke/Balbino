#pragma once
#include <vulkan/vulkan_core.h>

#include "Vertex.h"

namespace Balbino
{
	class CVertexBuffer
	{
	public:
		CVertexBuffer() = default;
		void Initialize(const VkDevice& device, const VkPhysicalDevice& physicalDevice, const std::vector<SVertex>& vertices);
		void Cleanup(const VkDevice& device) const;
		void Bind(const VkCommandBuffer& commandBuffer);
	private:
		VkBuffer m_vertexBuffer;
		VkDeviceMemory m_vertexBufferMemory;

		static uint32_t FindMemoryType(uint32_t typeFilter, const VkMemoryPropertyFlags properties, const VkPhysicalDevice& physicalDevice);
	};
}

