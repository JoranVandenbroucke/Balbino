#include "Buffer.h"

#include "Device.h"
#include "Funtions.h"

#include "CommandPool.h"
#include "ImageResource.h"
#include "Queue.h"

FawnVision::CBuffer::CBuffer( const CDevice* pDevice, CCommandPool* commandPool, CQueue* queue )
        : CDeviceObject{ pDevice }
        , m_commandPool{ commandPool }
        , m_queue{ queue }
{
    commandPool->AddRef();
    queue->AddRef();
}

FawnVision::CBuffer::~CBuffer()
{
    vkDestroyBuffer( GetDevice()->GetVkDevice(), m_buffer, nullptr );
    vkFreeMemory( GetDevice()->GetVkDevice(), m_bufferMemory, nullptr );
    m_buffer = VK_NULL_HANDLE;
    m_commandBuffer = VK_NULL_HANDLE;
    m_bufferMemory = VK_NULL_HANDLE;
    
    if(m_commandPool)
    {
        m_commandPool->Release();
        m_commandPool= nullptr;
    }
    if(m_queue)
    {
        m_queue->Release();
        m_queue= nullptr;
    }
    m_currentSize = 0;
    m_mappedData = nullptr;
}

void FawnVision::CBuffer::Bind( const bool isIndexBuffer, bool isInstanceBuffer ) const
{
    if ( isInstanceBuffer )
    {
        constexpr VkDeviceSize offsets[] = { 0 };
        vkCmdBindVertexBuffers( m_commandPool->GetCommandBuffer(), 1, 1, &m_buffer, offsets );
    }
    else if ( isIndexBuffer )
    {
        vkCmdBindIndexBuffer( m_commandPool->GetCommandBuffer(), m_buffer, 0, VK_INDEX_TYPE_UINT32 );
    }
    else
    {
        constexpr VkDeviceSize offsets[] = { 0 };
        vkCmdBindVertexBuffers( m_commandPool->GetCommandBuffer(), 0, 1, &m_buffer, offsets );
    }
}

void FawnVision::CBuffer::CopyBufferToImage( CImageResource* resource ) const
{
    resource->AddRef();
    BeginSingleTimeCommands();
    const VkBufferImageCopy region{
            .bufferOffset = 0, .bufferRowLength = 0, .bufferImageHeight = 0, .imageSubresource{
                    .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT, .mipLevel = 0, .baseArrayLayer = 0, .layerCount = 1,
            }, .imageOffset = { 0, 0, 0 }, .imageExtent = {
                    resource->GetWidth(), resource->GetHeight(), 1
            },
    };
    vkCmdCopyBufferToImage(
            m_commandBuffer,
            m_buffer,
            resource->GetImage(),
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            1,
            &region
    );
    
    EndSingleTimeCommands();
    resource->Release();
}

void FawnVision::CBuffer::UpdateData( const void* pData, const uint64_t size )
{
    if ( size == 0 )
    {
        return;
    }
    void* data;
    vkMapMemory( GetDevice()->GetVkDevice(), m_bufferMemory, 0, size, 0, &data );
    memcpy( data, pData, size );
    vkUnmapMemory( GetDevice()->GetVkDevice(), m_bufferMemory );
}

void FawnVision::CBuffer::Initialize( uint64_t size, EBufferUsageFlagBits::Enum bufferUsage, EMemoryPropertyFlagBits::Enum memoryProperty )
{
    if ( size > 0 )
    {
        m_currentSize = size;
        
        const VkBufferCreateInfo bufferInfo{
                .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
                .size = size, .usage = static_cast<VkBufferUsageFlags>( bufferUsage ),
                .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
        };
        CheckVkResult(
                vkCreateBuffer( GetDevice()->GetVkDevice(), &bufferInfo, nullptr, &m_buffer ), "failed to create buffer!"
        );
        
        VkMemoryRequirements memRequirements;
        vkGetBufferMemoryRequirements( GetDevice()->GetVkDevice(), m_buffer, &memRequirements );
        
        const VkMemoryAllocateInfo allocInfo{
                .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO, .allocationSize = memRequirements.size, .memoryTypeIndex = FindMemoryType(
                        GetDevice()->GetPhysicalDeviceInfo().device,
                        memRequirements.memoryTypeBits,
                        static_cast<VkMemoryPropertyFlagBits>( memoryProperty )),
        };
        
        CheckVkResult(
                vkAllocateMemory( GetDevice()->GetVkDevice(), &allocInfo, nullptr, &m_bufferMemory ),
                "failed to allocate buffer memory!"
        );
        CheckVkResult( vkBindBufferMemory( GetDevice()->GetVkDevice(), m_buffer, m_bufferMemory, 0 ));
    }
    
    const VkCommandBufferAllocateInfo allocCommandBufferInfo{
            .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO, .commandPool = m_commandPool->GetHandle(), .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY, .commandBufferCount = 1,
    };
    CheckVkResult( vkAllocateCommandBuffers( GetDevice()->GetVkDevice(), &allocCommandBufferInfo, &m_commandBuffer ));
}

const VkBuffer& FawnVision::CBuffer::GetBuffer() const
{
    return m_buffer;
}

uint64_t FawnVision::CBuffer::GetRange() const
{
    return m_currentSize;
}

void FawnVision::CBuffer::Rebuild( uint64_t size, EBufferUsageFlagBits::Enum bufferUsage, EMemoryPropertyFlagBits::Enum memoryProperty )
{
    if ( m_buffer )
    {
        vkDestroyBuffer( GetDevice()->GetVkDevice(), m_buffer, nullptr );
        m_buffer = VK_NULL_HANDLE;
    }
    if ( m_bufferMemory )
    {
        vkFreeMemory( GetDevice()->GetVkDevice(), m_bufferMemory, nullptr );
        m_bufferMemory = VK_NULL_HANDLE;
    }
    if ( m_commandBuffer )
    {
        vkFreeCommandBuffers( GetDevice()->GetVkDevice(), m_commandPool->GetHandle(), 1, &m_commandBuffer );
        m_commandBuffer = nullptr;
    }
    Initialize( size, bufferUsage, memoryProperty );
}

void* FawnVision::CBuffer::GetMapped() const
{
    return m_mappedData;
}

void FawnVision::CBuffer::Flush( const VkDeviceSize size, const VkDeviceSize offset )
{
    VkMappedMemoryRange mappedRange{
            .sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE, .memory = m_bufferMemory, .offset = offset, .size = size,
    };
    vkFlushMappedMemoryRanges( GetDevice()->GetVkDevice(), 1, &mappedRange );
}

void FawnVision::CBuffer::Unmapped()
{
    if ( m_mappedData )
    {
        vkUnmapMemory( GetDevice()->GetVkDevice(), m_bufferMemory );
        m_mappedData = nullptr;
    }
}

void FawnVision::CBuffer::Map( const uint64_t size, const uint64_t offset )
{
    vkMapMemory( GetDevice()->GetVkDevice(), m_bufferMemory, offset, size, 0, &m_mappedData );
}

void FawnVision::CBuffer::ReassignCommandPool( CCommandPool* commandPool )
{
    commandPool->AddRef();
    m_commandPool->Release();
    m_commandPool = commandPool;
}

void FawnVision::CBuffer::CopyBuffer( const CBuffer& dstBuffer, uint64_t size ) const
{
    BeginSingleTimeCommands();
    
    const VkBufferCopy copyRegion{ .size = size };
    vkCmdCopyBuffer( m_commandBuffer, m_buffer, dstBuffer.GetBuffer(), 1, &copyRegion );
    
    EndSingleTimeCommands();
}

void FawnVision::CBuffer::BeginSingleTimeCommands() const
{
    constexpr VkCommandBufferBeginInfo beginInfo{
            .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO, .flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
    };
    
    CheckVkResult( vkBeginCommandBuffer( m_commandBuffer, &beginInfo ));
}

void FawnVision::CBuffer::EndSingleTimeCommands() const
{
    vkEndCommandBuffer( m_commandBuffer );
    
    const VkSubmitInfo submitInfo{
            .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO, .commandBufferCount = 1, .pCommandBuffers = &m_commandBuffer,
    };
    vkQueueSubmit( m_queue->GetHandle(), 1, &submitInfo, VK_NULL_HANDLE );
    vkQueueWaitIdle( m_queue->GetHandle());
}

void FawnVision::CBuffer::PipelineBarrier( EPipelineStageFlagBits::Enum srcStageMask, EPipelineStageFlagBits::Enum destStageMask, const VkImageMemoryBarrier& barrier) const
{
    vkCmdPipelineBarrier(
            m_commandBuffer,
            static_cast<VkPipelineStageFlagBits>( srcStageMask ),
            static_cast<VkPipelineStageFlagBits>( destStageMask ),
            0,
            0,
            nullptr,
            0,
            nullptr,
            1,
            &barrier
    );
}

void FawnVision::CBuffer::Blit( const VkImage& image, const VkImageBlit& blit ) const
{
    vkCmdBlitImage(
            m_commandBuffer,
            image,
            VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
            image,
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            1,
            &blit,
            VK_FILTER_LINEAR
    );
}
