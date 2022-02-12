#pragma once
#include "Base.h"
#include "DescriptorSet.h"

#include <vulkan/vulkan.hpp>

namespace BalVulkan
{
	class CImageResource;
	class CShaderPipeline;
	class CFrameBuffer;
	class CSwapchain;

	class CCommandPool final : public CDeviceObject
	{
	public:
		explicit CCommandPool( const CDevice* device );

		CCommandPool( CCommandPool&& ) = default;
		CCommandPool& operator=( CCommandPool&& ) = default;
		~CCommandPool() override;

		void Initialize( uint32_t queueFamilyIndex, const CSwapchain* pSwapchain );
		void UpdateFrameIndex();
		void BeginRender( CFrameBuffer* pFrameBuffer, CSwapchain* pSwapchain ) const;
		void BindShader( CShaderPipeline* pPipeline, CDescriptorSet* pDescriptorSet ) const;
		uint32_t GetCommandBufferCount() const;
		uint32_t GetCurrentIndex() const;

		const VkCommandPool& GetCommandPool() const;
		const VkCommandBuffer& GetCommandBuffer() const;
		void EndRender() const;

		static CCommandPool* CreateNew( const CDevice* pDevice );
	private:
		VkCommandPool m_commandPool;
		std::vector<VkCommandBuffer> m_commandBuffers;
		std::vector<CImageResource*> m_imageResources;
		uint32_t m_currentFrameIndex;
	};
}
