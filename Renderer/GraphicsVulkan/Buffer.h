#pragma once
#include "Base.h"
#include "Common.h"

namespace BalVulkan
{
	class CImageResource;
	class CCommandPool;
	class CQueue;

	class CBuffer final : public CDeviceObject
	{
	public:
		explicit CBuffer( const CDevice* pDevice, const CCommandPool* commandPool, const CQueue* queue );
		CBuffer( CBuffer&& ) noexcept = default;
		CBuffer& operator=( CBuffer&& ) noexcept = default;
		~CBuffer() override;

		void Initialize( uint64_t size, EBufferType bufferType );
		void UpdateData( const void* pData, uint64_t size );

		void Bind( const bool isIndexBuffer ) const;

		void CopyBuffer( const CBuffer& dstBuffer, uint64_t size ) const;
		void CopyBufferToImage( const CImageResource* resource ) const;

		void BeginSingleTimeCommands() const;
		void EndSingleTimeCommands() const;
		void PipelineBarrier( EPipelineStageFlagBits srcStageMask, EPipelineStageFlagBits destStageMask, VkImageMemoryBarrier* pBarrier ) const;
		void Blit( const VkImage* pImage, const VkImageBlit* pBlit ) const;

		const VkBuffer& GetBuffer()const;
		uint64_t GetRange()const;
		static CBuffer* CreateNew( const CDevice* pDevice, const CCommandPool* commandPool, const CQueue* queue );
	private:
		VkBuffer m_buffer;
		VkCommandBuffer m_commandBuffer;
		VkDeviceMemory m_bufferMemory;

		const CCommandPool* m_commandPool;
		const CQueue* m_queue;
		uint64_t m_currentSize;
	};
}

