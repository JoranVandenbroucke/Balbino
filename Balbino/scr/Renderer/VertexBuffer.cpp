#include "pch.h"
#include "VertexBuffer.h"

#include <system_error>
#include <vector>

Balbino::CVertexBuffer::CVertexBuffer()
	: m_vertexBuffer{VK_NULL_HANDLE}
	, m_vertexBufferMemory{VK_NULL_HANDLE}
{
}

Balbino::CVertexBuffer::~CVertexBuffer()
{
	if (m_vertexBuffer != VK_NULL_HANDLE || m_vertexBufferMemory != VK_NULL_HANDLE)
		std::cerr << "index buffer was not destroyed" << std::endl;
}

void Balbino::CVertexBuffer::Initialize(const VkDevice& device, const VkCommandPool& commandPool, const VkQueue& queue, const VkPhysicalDevice& physicalDevice, const std::vector<SVertex>& vertices, const VkAllocationCallbacks* pCallback)
{
	//VkBufferCreateInfo bufferInfo{};
	//bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	//bufferInfo.size = sizeof(vertices[0]) * vertices.size();
	//bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
	//bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	//
	//if (vkCreateBuffer(device, &bufferInfo, nullptr, &m_vertexBuffer) != VK_SUCCESS)
	//{
	//    throw std::runtime_error("failed to create vertex buffer!");
	//}
	//
	//VkMemoryRequirements memRequirements;
	//vkGetBufferMemoryRequirements(device, m_vertexBuffer, &memRequirements);
	//
	//VkMemoryAllocateInfo allocInfo{};
	//allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	//allocInfo.allocationSize = memRequirements.size;
	//allocInfo.memoryTypeIndex = FindMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, physicalDevice);
	//
	//if (vkAllocateMemory(device, &allocInfo, nullptr, &m_vertexBufferMemory) != VK_SUCCESS)
	//{
	//    throw std::runtime_error("failed to allocate vertex buffer memory!");
	//}
	//
	//vkBindBufferMemory(device, m_vertexBuffer, m_vertexBufferMemory, 0);
	//
	//void* data;
	//vkMapMemory(device, m_vertexBufferMemory, 0, bufferInfo.size, 0, &data);
	//memcpy(data, vertices.data(), (size_t) bufferInfo.size);
	//vkUnmapMemory(device, m_vertexBufferMemory);

	VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	CreateBuffer(device, pCallback, physicalDevice, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

	void* data;
	vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &data);
	memcpy(data, vertices.data(), (size_t) bufferSize);
	vkUnmapMemory(device, stagingBufferMemory);

	CreateBuffer(device, pCallback, physicalDevice, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_vertexBuffer, m_vertexBufferMemory);

	CopyBuffer(device, commandPool, queue, stagingBuffer, m_vertexBuffer, bufferSize);

	vkDestroyBuffer(device, stagingBuffer, nullptr);
	vkFreeMemory(device, stagingBufferMemory, nullptr);
}
void  Balbino::CVertexBuffer::Cleanup(const VkDevice& device, const VkAllocationCallbacks* pAlloc)
{
	vkDestroyBuffer(device, m_vertexBuffer, pAlloc);
	vkFreeMemory(device, m_vertexBufferMemory, pAlloc);

	m_vertexBuffer = VK_NULL_HANDLE;
	m_vertexBufferMemory = VK_NULL_HANDLE;
}
void Balbino::CVertexBuffer::Bind(const VkCommandBuffer& commandBuffer) const
{
	const VkBuffer vertexBuffers[] = {m_vertexBuffer};
	const VkDeviceSize offsets[] = {0};
	vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);

}

uint32_t Balbino::CVertexBuffer::FindMemoryType(uint32_t typeFilter, const VkMemoryPropertyFlags properties, const VkPhysicalDevice& physicalDevice)
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
void Balbino::CVertexBuffer::CopyBuffer(const VkDevice& device, const VkCommandPool& commandPool, const VkQueue& queue, const VkBuffer& srcBuffer, const VkBuffer& dstBuffer, VkDeviceSize& size)
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
void Balbino::CVertexBuffer::CreateBuffer(const VkDevice& device, const VkAllocationCallbacks* pCallback, const VkPhysicalDevice& physicalDevice, const VkDeviceSize size, const VkBufferUsageFlags usage, const VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory)
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
