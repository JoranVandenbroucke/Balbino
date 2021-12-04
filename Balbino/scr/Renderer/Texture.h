#pragma once
#include "Common.h"

namespace BalVulkan
{
	class CQueue;
	class CImageView;
	class CImageResource;
	class CSampler;
	class CDevice;
}

namespace Balbino
{
	class CTexture
	{
	public:
		explicit CTexture( const BalVulkan::CDevice* pDevice );
		~CTexture();
		CTexture(const CTexture&) = delete;
		CTexture(CTexture&&) = delete;
		CTexture& operator=(const CTexture&) = delete;
		CTexture& operator=(CTexture&&) = delete;
		void Initialize( const void* pData, uint32_t width, uint32_t height, uint32_t mipMaps, uint32_t layers, uint32_t faces, uint32_t size, BalVulkan::EImageLayout layout, BalVulkan::EImageViewType type, BalVulkan::EFormat format, const BalVulkan::CCommandPool& commandPool, const BalVulkan::CQueue* pQueue );
		void Cleanup() const;
	private:
		BalVulkan::CImageView* m_pView;
		BalVulkan::CImageResource* m_pResource;
		BalVulkan::CSampler* m_pSampler;

		const BalVulkan::CDevice* m_pDevice;
	};
}