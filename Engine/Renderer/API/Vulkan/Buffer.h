#pragma once
#include <vulkan/vulkan.hpp>
#include "Base.h"
#include "Common.h"

namespace FawnVision
{
    class CCommandPool;
    class CQueue;
    class CImageResource;
    
    class CBuffer final : public CDeviceObject
    {
    public:
        explicit CBuffer( const CDevice* pDevice, CCommandPool* commandPool, CQueue* queue );
        ~CBuffer() override;
        
        void Initialize( uint64_t size, EBufferUsageFlagBits::Enum bufferUsage, EMemoryPropertyFlagBits::Enum memoryProperty );
        void UpdateData( const void* data, uint64_t size );
        
        
        void Bind( bool isIndexBuffer, bool isInstanceBuffer = false ) const;
        
        void CopyBuffer( const CBuffer& dstBuffer, uint64_t size ) const;
        void CopyBufferToImage( CImageResource* resource ) const;
        
        void BeginSingleTimeCommands() const;
        void EndSingleTimeCommands() const;
        void PipelineBarrier( EPipelineStageFlagBits::Enum srcStageMask, EPipelineStageFlagBits::Enum destStageMask, const VkImageMemoryBarrier& barrier ) const;
        void Blit( const VkImage& image, const VkImageBlit& blit ) const;
        
        const VkBuffer& GetBuffer() const;
        uint64_t GetRange() const;
        void Rebuild( uint64_t size, EBufferUsageFlagBits::Enum bufferUsage, EMemoryPropertyFlagBits::Enum memoryProperty );
        void* GetMapped() const;
        void Flush( VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0 );
        
        void Unmapped();
        void Map( uint64_t size = VK_WHOLE_SIZE, uint64_t offset = 0 );
        void ReassignCommandPool( CCommandPool* commandPool );
    private:
        VkBuffer        m_buffer{VK_NULL_HANDLE};
        VkCommandBuffer m_commandBuffer{VK_NULL_HANDLE};
        VkDeviceMemory  m_bufferMemory{VK_NULL_HANDLE};
        
        CCommandPool* m_commandPool;
        CQueue      * m_queue;
        uint64_t m_currentSize{};
        
        void* m_mappedData{ nullptr};
    };
}
