#pragma once
#include "Base.h"
#include "Vulkan/Vulkan.hpp"

namespace DeerVulkan
{
    class CBuffer final : public CDeviceObject
    {
    public:
        explicit CBuffer( FawnVision::Device Device, FawnVision::CommandPool commandPool, FawnVision::Queue queue );
        ~CBuffer() override;
        void Initialize( uint64_t size, uint32_t bufferUsage, uint16_t memoryProperty ) noexcept;
        void Rebuild( uint64_t size, FawnVision::buffer_usage_flag bufferUsage, FawnVision::memory_property_flag memoryProperty ) noexcept;
        constexpr const VkBuffer& GetBuffer() const
        {
            return m_buffer;
        }
        constexpr uint64_t GetRange() const
        {
            return m_currentSize;
        }
        void* GetMapped() const
        {
            return m_mappedData;
        }
        void BeginSingleTimeCommands() const;
        void EndSingleTimeCommands() const;
        void Bind( bool isIndexBuffer, bool isInstanceBuffer = false ) const;
        void Blit( const VkImage& image, const VkImageBlit& blit ) const;
        void CopyBuffer( FawnVision::Buffer dstBuffer, uint64_t size ) const;
        void CopyBufferToImage( FawnVision::ImageResource resource ) const;
        void UpdateData( const void* data, uint64_t size );
        void Flush( VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0 ) noexcept;
        void Map( uint64_t size = VK_WHOLE_SIZE, uint64_t offset = 0 ) noexcept;
        void Unmapped() noexcept;
        void PipelineBarrier( FawnVision::pipeline_stage_flag srcStageMask, FawnVision::pipeline_stage_flag destStageMask, const VkImageMemoryBarrier& barrier ) const;
        void ReassignCommandPool( FawnVision::CommandPool commandPool ) noexcept;

    private:
        VkBuffer m_buffer { VK_NULL_HANDLE };
        VkCommandBuffer m_commandBuffer { VK_NULL_HANDLE };
        VkDeviceMemory m_bufferMemory { VK_NULL_HANDLE };

        FawnVision::CommandPool m_commandPool;
        FawnVision::Queue m_queue;
        uint64_t m_currentSize {};

        void* m_mappedData { BALBINO_NULL };
    };
}// namespace DeerVulkan
