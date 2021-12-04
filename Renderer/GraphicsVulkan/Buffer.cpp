#include "pch.h"
#include "Buffer.h"

#include "CommandPool.h"
#include "Device.h"
#include "Funtions.h"
#include "ImageResource.h"
#include "Instance.h"
#include "Queue.h"

BalVulkan::CBuffer::CBuffer( const CDevice* pDevice, const CCommandPool* commandPool, const CQueue* queue )
	: CDeviceObject{ pDevice }
	, m_buffer{ VK_NULL_HANDLE }
	, m_commandBuffer{ VK_NULL_HANDLE }
	, m_bufferMemory{ VK_NULL_HANDLE }
	, m_commandPool{ commandPool }
	, m_queue{ queue }
	, m_currentSize{ 0 }
{
}

BalVulkan::CBuffer::~CBuffer()
{
	vkDestroyBuffer( GetDevice()->GetVkDevice(), m_buffer, nullptr );
	vkFreeMemory( GetDevice()->GetVkDevice(), m_bufferMemory, nullptr );

	m_buffer = VK_NULL_HANDLE;
	m_commandBuffer = VK_NULL_HANDLE;
	m_bufferMemory = VK_NULL_HANDLE;
	m_commandPool = nullptr;
	m_queue = nullptr;

}

void BalVulkan::CBuffer::Bind( const bool isIndexBuffer ) const
{
	if ( isIndexBuffer )
		vkCmdBindIndexBuffer( m_commandPool->GetCommandBuffer(), m_buffer, 0, VK_INDEX_TYPE_UINT32 );
	else
		vkCmdBindVertexBuffers( m_commandPool->GetCommandBuffer(), 0, 1, &m_buffer, VK_NULL_HANDLE );
}

void BalVulkan::CBuffer::CopyBufferToImage( const BalVulkan::CImageResource* pResource ) const
{
	BeginSingleTimeCommands();

	const VkBufferImageCopy region{
		.bufferOffset = 0,
		.bufferRowLength = 0,
		.bufferImageHeight = 0,
		.imageSubresource{
			.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
			.mipLevel = 0,
			.baseArrayLayer = 0,
			.layerCount = 1,
		},
		.imageOffset = { 0, 0, 0 },
		.imageExtent = {
			pResource->GetWidth(),
			pResource->GetHeight(),
			1
		},
	};
	vkCmdCopyBufferToImage( m_commandBuffer, m_buffer, pResource->GetImage(), VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region );

	EndSingleTimeCommands();
}

void BalVulkan::CBuffer::UpdateData( const void* pData, const uint64_t size )
{
	void* data;
	vkMapMemory( GetDevice()->GetVkDevice(), m_bufferMemory, 0, size, 0, &data );
	memcpy( data, pData, size );
	vkUnmapMemory( GetDevice()->GetVkDevice(), m_bufferMemory );
	m_currentSize = size;
}

void BalVulkan::CBuffer::Initialize( uint64_t size, BalVulkan::EBufferType bufferType )
{
	m_currentSize = size;
	VkBufferUsageFlags usage{};
	VkMemoryPropertyFlags properties{};
	switch ( bufferType )
	{
		case EBufferType::IndexBuffer:
			usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
			properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
			break;
		case EBufferType::VertexBuffer:
			usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
			properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
			break;
		case EBufferType::StagingBuffer:
			usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
			properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
			break;
		case EBufferType::UniformBuffer:
			usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
			properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
			break;
		default:;
	}

	const VkBufferCreateInfo bufferInfo{
		.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
		.size = size,
		.usage = usage,
		.sharingMode = VK_SHARING_MODE_EXCLUSIVE,
	};
	CheckVkResult( vkCreateBuffer( GetDevice()->GetVkDevice(), &bufferInfo, nullptr, &m_buffer ), "failed to create buffer!" );

	VkMemoryRequirements memRequirements;
	vkGetBufferMemoryRequirements( GetDevice()->GetVkDevice(), m_buffer, &memRequirements );

	const VkMemoryAllocateInfo allocInfo{
		.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
		.allocationSize = memRequirements.size,
		.memoryTypeIndex = FindMemoryType( GetDevice()->GetPhysicalDeviceInfo()->device, memRequirements.memoryTypeBits, properties ),
	};

	CheckVkResult( vkAllocateMemory( GetDevice()->GetVkDevice(), &allocInfo, nullptr, &m_bufferMemory ), "failed to allocate buffer memory!" );
	CheckVkResult( vkBindBufferMemory( GetDevice()->GetVkDevice(), m_buffer, m_bufferMemory, 0 ) );

	const VkCommandBufferAllocateInfo allocCommandBufferInfo{
		.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
		.commandPool = m_commandPool->GetCommandPool(),
		.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
		.commandBufferCount = 1,
	};
	CheckVkResult( vkAllocateCommandBuffers( GetDevice()->GetVkDevice(), &allocCommandBufferInfo, &m_commandBuffer ) );

}

const VkBuffer& BalVulkan::CBuffer::GetBuffer() const
{
	return m_buffer;
}

uint64_t BalVulkan::CBuffer::GetRange() const
{
	return m_currentSize;
}

BalVulkan::CBuffer* BalVulkan::CBuffer::CreateNew( const CDevice* pDevice, const CCommandPool* commandPool, const CQueue* queue )
{
	return new CBuffer{ pDevice, commandPool, queue };
}

void BalVulkan::CBuffer::CopyBuffer( const BalVulkan::CBuffer& dstBuffer, uint64_t size ) const
{
	BeginSingleTimeCommands();

	const VkBufferCopy copyRegion{ .size = size };
	vkCmdCopyBuffer( m_commandBuffer, m_buffer, dstBuffer.GetBuffer(), 1, &copyRegion );

	EndSingleTimeCommands();
}

void BalVulkan::CBuffer::BeginSingleTimeCommands() const
{
	constexpr VkCommandBufferBeginInfo beginInfo{
		.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
		.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
	};

	CheckVkResult( vkBeginCommandBuffer( m_commandBuffer, &beginInfo ) );
}

void BalVulkan::CBuffer::EndSingleTimeCommands() const
{
	vkEndCommandBuffer( m_commandBuffer );

	const VkSubmitInfo submitInfo{
		.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
		.commandBufferCount = 1,
		.pCommandBuffers = &m_commandBuffer,
	};
	vkQueueSubmit( m_queue->GetQueue(), 1, &submitInfo, VK_NULL_HANDLE );
	vkQueueWaitIdle( m_queue->GetQueue() );

	vkFreeCommandBuffers( GetDevice()->GetVkDevice(), m_commandPool->GetCommandPool(), 1, &m_commandBuffer );
}

void BalVulkan::CBuffer::PipelineBarrier( EPipelineStageFlagBits srcStageMask, EPipelineStageFlagBits destStageMask, VkImageMemoryBarrier* pBarrier ) const
{
	vkCmdPipelineBarrier( m_commandBuffer, ( VkPipelineStageFlagBits ) srcStageMask, ( VkPipelineStageFlagBits ) destStageMask, 0, 0, nullptr, 0, nullptr, 1, pBarrier );

}

void BalVulkan::CBuffer::Blit( const VkImage* pImage, const VkImageBlit* pBlit ) const
{
	vkCmdBlitImage( m_commandBuffer, *pImage, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, *pImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, pBlit, VK_FILTER_LINEAR );

}