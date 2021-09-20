#include "pch.h"
#include "IndexBuffer.h"

#include <iostream>
#include <vector>

Balbino::CIndexBuffer::CIndexBuffer()
	: m_indexBuffer{m_indexBuffer}
	, m_indexBufferMemory{m_indexBufferMemory}
{
}

Balbino::CIndexBuffer::~CIndexBuffer()
{
	if (m_indexBuffer != VK_NULL_HANDLE || m_indexBufferMemory != VK_NULL_HANDLE)
		std::cerr << "index buffer was not destroyed" << std::endl;
}

void Balbino::CIndexBuffer::Initialize(const VkDevice& device, const VkCommandPool& commandPool, const VkQueue& queue, const VkPhysicalDevice& physicalDevice, const std::vector<uint32_t>& indices, const VkAllocationCallbacks* pCallback)
{
	VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();

	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	CreateBuffer(device, pCallback, physicalDevice, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

	void* data;
	vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &data);
	memcpy(data, indices.data(), bufferSize);
	vkUnmapMemory(device, stagingBufferMemory);

	CreateBuffer(device, pCallback, physicalDevice, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_indexBuffer, m_indexBufferMemory);

	CopyBuffer(device, commandPool, queue, stagingBuffer, m_indexBuffer, bufferSize);

	vkDestroyBuffer(device, stagingBuffer, pCallback);
	vkFreeMemory(device, stagingBufferMemory, pCallback);
}

void Balbino::CIndexBuffer::Cleanup(const VkDevice& device, const VkAllocationCallbacks* pAlloc)
{
	vkDestroyBuffer(device, m_indexBuffer, pAlloc);
	vkFreeMemory(device, m_indexBufferMemory, pAlloc);
	m_indexBuffer = VK_NULL_HANDLE;
	m_indexBufferMemory = VK_NULL_HANDLE;
}

void Balbino::CIndexBuffer::Bind(const VkCommandBuffer& commandBuffer) const
{
	vkCmdBindIndexBuffer(commandBuffer, m_indexBuffer, 0, VK_INDEX_TYPE_UINT32);
}

void Balbino::CIndexBuffer::CopyBuffer(const VkDevice& device, const VkCommandPool& commandPool, const VkQueue& queue, const VkBuffer& srcBuffer, const VkBuffer& dstBuffer, VkDeviceSize& size)
{
	const VkCommandBufferAllocateInfo allocInfo{
		.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
		.commandPool = commandPool,
		.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
		.commandBufferCount = 1
	};
	VkCommandBuffer commandBuffer;
	vkAllocateCommandBuffers(device, &allocInfo, &commandBuffer);

	constexpr VkCommandBufferBeginInfo beginInfo{
		.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
		.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT
	};

	vkBeginCommandBuffer(commandBuffer, &beginInfo);

	const VkBufferCopy copyRegion{.size = size};
	vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

	vkEndCommandBuffer(commandBuffer);

	const VkSubmitInfo submitInfo{
		.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
		.commandBufferCount = 1,
		.pCommandBuffers = &commandBuffer
	};

	vkQueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE);
	vkQueueWaitIdle(queue);

	vkFreeCommandBuffers(device, commandPool, 1, &commandBuffer);
}

void Balbino::CIndexBuffer::CreateBuffer(const VkDevice& device, const VkAllocationCallbacks* pCallback, const VkPhysicalDevice& physicalDevice, const VkDeviceSize size, const VkBufferUsageFlags usage, const VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory)
{
	const VkBufferCreateInfo bufferInfo{
		.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
		.size = size,
		.usage = usage,
		.sharingMode = VK_SHARING_MODE_EXCLUSIVE
	};

	if (vkCreateBuffer(device, &bufferInfo, pCallback, &buffer) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create buffer!");
	}

	VkMemoryRequirements memRequirements;
	vkGetBufferMemoryRequirements(device, buffer, &memRequirements);

	const VkMemoryAllocateInfo allocInfo{
		.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
		.allocationSize = memRequirements.size,
		.memoryTypeIndex = FindMemoryType(memRequirements.memoryTypeBits, properties, physicalDevice)
	};

	if (vkAllocateMemory(device, &allocInfo, pCallback, &bufferMemory) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to allocate buffer memory!");
	}

	vkBindBufferMemory(device, buffer, bufferMemory, 0);
}
uint32_t Balbino::CIndexBuffer::FindMemoryType(uint32_t typeFilter, const VkMemoryPropertyFlags properties, const VkPhysicalDevice& physicalDevice)
{
	VkPhysicalDeviceMemoryProperties memProperties;
	vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

	for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
	{
		if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
		{
			return i;
		}
	}

	throw std::runtime_error("failed to find suitable memory type!");

}