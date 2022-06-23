#pragma once
#include "Base.h"

#include <vulkan/vulkan.hpp>

namespace BalVulkan
{
	class CSampler;
	class CSwapchain;
	class CImageResource;
	class CImageView;

	class CFrameBuffer final : public CDeviceObject
	{
	public:
		explicit CFrameBuffer( const CDevice* device );
		CFrameBuffer( CFrameBuffer&& ) = default;
		CFrameBuffer& operator=( CFrameBuffer&& ) = default;
		~CFrameBuffer() override;

		void Initialize( const CSwapchain* pSwapchain );
		VkRenderPass GetRenderPass() const;
		VkFramebuffer GetFrameBuffer( uint32_t idx ) const;
		static CFrameBuffer* CreateNew( const CDevice* pDevice );
		CImageView* GetImageView( uint32_t idx ) const;
	private:
		VkRenderPass m_renderPass;
		std::vector<VkFramebuffer> m_frameBuffer;

		std::vector<CImageResource*> m_swapchainResources;
		std::vector<CImageView*> m_swapchainViews;
		CImageResource* m_pDepthImage;
		CImageView* m_pDepthImageView;
		//std::vector<CImageResource*> m_imageResources;
		//std::vector<CImageView*> m_imageViews;

		VkFormat GetDepthFormat() const;
	};
}
