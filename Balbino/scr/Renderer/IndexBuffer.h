#pragma once
#include <vulkan/vulkan.h>

namespace Balbino
{
	class CIndexBuffer
	{
	public:
		CIndexBuffer();
		~CIndexBuffer();
		CIndexBuffer(const CIndexBuffer&) = delete;
		CIndexBuffer(CIndexBuffer&&) = delete;
		CIndexBuffer& operator=(const CIndexBuffer&) = delete;
		CIndexBuffer& operator=(CIndexBuffer&&) = delete;

		void Initialize(const VkDevice& device, const VkCommandPool& commandPool, const VkQueue& queue, const VkPhysicalDevice& physicalDevice, const std::vector<uint16_t>& indices, const VkAllocationCallbacks* pCallback);
		void Cleanup(const VkDevice& device, const VkAllocationCallbacks* pAlloc);
		void Bind(const VkCommandBuffer& commandBuffer) const;
	private:
		VkBuffer m_indexBuffer;
		VkDeviceMemory m_indexBufferMemory;

		static void CopyBuffer(const VkDevice& device, const VkCommandPool& commandPool, const VkQueue& queue, const VkBuffer& srcBuffer, const VkBuffer& dstBuffer, VkDeviceSize& size);
		static void CreateBuffer(const VkDevice& device, const VkAllocationCallbacks* pCallback, const VkPhysicalDevice& physicalDevice, const VkDeviceSize size, const VkBufferUsageFlags usage, const VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
		static uint32_t FindMemoryType( uint32_t typeFilter, VkMemoryPropertyFlags properties, const VkPhysicalDevice& physicalDevice );
	};
}			
