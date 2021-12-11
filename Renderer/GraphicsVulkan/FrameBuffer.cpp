#include "pch.h"
#include "FrameBuffer.h"

#include "Common.h"
#include "Device.h"
#include "Funtions.h"
#include "Instance.h"
#include "Swapchain.h"

#include "ImageResource.h"
#include "ImageView.h"

BalVulkan::CFrameBuffer::CFrameBuffer( const CDevice* device )
	: CDeviceObject{ device }
	, m_renderPass{ VK_NULL_HANDLE }
	, m_pDepthImage{ nullptr }
	, m_pDepthImageView{ nullptr }
{
}


BalVulkan::CFrameBuffer::~CFrameBuffer()
{
	m_pDepthImageView->Release();
	m_pDepthImage->Release();
	for ( const auto & frameBuffer : m_frameBuffer )
	{
		vkDestroyFramebuffer( GetDevice()->GetVkDevice(), frameBuffer, nullptr );
	}
	for ( const auto& swapchainView : m_swapchainViews )
	{
		swapchainView->Release();
	}
	for ( const auto& swapchainResource : m_swapchainResources )
	{
		swapchainResource->Release();
	}
	vkDestroyRenderPass( GetDevice()->GetVkDevice(), m_renderPass, nullptr );
	m_renderPass = VK_NULL_HANDLE;
	m_pDepthImage = nullptr;
	m_pDepthImageView = nullptr;
	m_frameBuffer.clear();
	m_swapchainResources.clear();
	m_swapchainViews.clear();
}

void BalVulkan::CFrameBuffer::Initialize( const CSwapchain* pSwapchain )
{
	uint32_t imageResourceSize;

	pSwapchain->GetImages( m_swapchainResources, imageResourceSize );
	m_swapchainViews.reserve( imageResourceSize );
	for ( uint32_t i = 0; i < imageResourceSize; ++i )
	{
		m_swapchainViews.push_back( new CImageView{ *m_swapchainResources[i], EImageViewType::View2D } );
	}

	//based on https://github.com/SaschaWillems/Vulkan/blob/master/examples/deferred/deferred.cpp
	std::vector<VkAttachmentDescription> attachmentDescs{
		VkAttachmentDescription{
			.format = pSwapchain->GetSurfaceFormat().format,
			.samples = VK_SAMPLE_COUNT_1_BIT,
			.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
			.storeOp = VK_ATTACHMENT_STORE_OP_STORE,
			.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
			.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
			.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
			.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
		},
		VkAttachmentDescription{
			.format = GetDepthFormat(),
			.samples = VK_SAMPLE_COUNT_1_BIT,
			.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
			.storeOp = VK_ATTACHMENT_STORE_OP_STORE,
			.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
			.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
			.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
			.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
		}
	};
	VkAttachmentReference colorAttachmentRef{
		.attachment = 0,
		.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
	};

	const VkAttachmentReference depthAttachmentRef{
		.attachment = 1,
		.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
	};
	
	const VkSubpassDescription surpass{
		.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
		.inputAttachmentCount = 0,
		.pInputAttachments = nullptr,
		.colorAttachmentCount = 1,
		.pColorAttachments = &colorAttachmentRef,
		.pResolveAttachments = nullptr,
		.pDepthStencilAttachment = &depthAttachmentRef,
		.preserveAttachmentCount = 0,
		.pPreserveAttachments = nullptr,
	};

	const VkSubpassDependency dependency
	{
		.srcSubpass = VK_SUBPASS_EXTERNAL,
		.dstSubpass = 0,
		.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
		.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
		.srcAccessMask = 0,
		.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,
	};
	const VkRenderPassCreateInfo renderPassInfo{
		.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
		.attachmentCount = static_cast< uint32_t >( attachmentDescs.size() ),
		.pAttachments = attachmentDescs.data(),
		.subpassCount = 1,
		.pSubpasses = &surpass,
		.dependencyCount = 1,
		.pDependencies = &dependency,
	};
	CheckVkResult( vkCreateRenderPass( GetDevice()->GetVkDevice(), &renderPassInfo, nullptr, &m_renderPass ), "failed to create render pass!" );
	
	m_pDepthImage = CImageResource::CreateNew( GetDevice() );
	m_pDepthImage->Initialize( pSwapchain->GetExtend().width,
							   pSwapchain->GetExtend().height,
							   1,
							   1,
							   //EImageLayout::DepthAttachmentOptimal,
							   EImageLayout::Undefined,
							   static_cast<EFormat>( attachmentDescs[1].format ),
							   EImageUsageFlagBits::DepthStencilAttachmentBit );
	m_pDepthImageView = CImageView::CreateNew( *m_pDepthImage, EImageViewType::View2D, 0, 1, 0, 1 );
	m_frameBuffer.resize( m_swapchainViews.size(), VK_NULL_HANDLE );
	for ( size_t i = 0; i < m_swapchainViews.size(); i++ ) {

		std::vector attachments{
			//m_imageViews[0]->GetImageView(),
			//m_imageViews[1]->GetImageView(),
			m_swapchainViews[i]->GetImageView(),
			m_pDepthImageView->GetImageView(),
		};
		const VkFramebufferCreateInfo bufCreateInfo = {
			.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
			.pNext = VK_NULL_HANDLE,
			.renderPass = m_renderPass,
			.attachmentCount = static_cast< uint32_t >( attachments.size() ),
			.pAttachments = attachments.data(),
			.width = pSwapchain->GetExtend().width,
			.height = pSwapchain->GetExtend().height,
			.layers = 1,
		};
		CheckVkResult( vkCreateFramebuffer( GetDevice()->GetVkDevice(), &bufCreateInfo, nullptr, &m_frameBuffer[i]), "failed to create frame buffer!");
	}
}

VkRenderPass BalVulkan::CFrameBuffer::GetRenderPass() const
{
	return m_renderPass;
}

VkFramebuffer BalVulkan::CFrameBuffer::GetFrameBuffer( const uint32_t idx ) const
{
	return m_frameBuffer[idx];
}

BalVulkan::CFrameBuffer* BalVulkan::CFrameBuffer::CreateNew( const CDevice* pDevice )
{
	return new CFrameBuffer{pDevice};
}

BalVulkan::CImageView* BalVulkan::CFrameBuffer::GetImageView( const uint32_t idx ) const
{
	//return m_imageViews[idx];
	return m_swapchainViews[idx];
}

VkFormat BalVulkan::CFrameBuffer::GetDepthFormat() const
{
	return GetSupportedFormat(
		std::vector{
				VK_FORMAT_D32_SFLOAT_S8_UINT,
				VK_FORMAT_D32_SFLOAT,
				VK_FORMAT_D24_UNORM_S8_UINT,
				VK_FORMAT_D16_UNORM_S8_UINT,
				VK_FORMAT_D16_UNORM
		} ,
		VK_IMAGE_TILING_OPTIMAL,
		VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
	);
}


VkFormat BalVulkan::CFrameBuffer::GetSupportedFormat( const std::vector<VkFormat>& candidates, const VkImageTiling tiling, const VkFormatFeatureFlags features ) const
{
	for ( const VkFormat format : candidates )
	{
		VkFormatProperties props;
		vkGetPhysicalDeviceFormatProperties( GetDevice()->GetPhysicalDeviceInfo()->device, format, &props );

		if ( tiling == VK_IMAGE_TILING_LINEAR && ( props.linearTilingFeatures & features ) == features )
		{
			return format;
		}
		if ( tiling == VK_IMAGE_TILING_OPTIMAL && ( props.optimalTilingFeatures & features ) == features )
		{
			return format;
		}
	}

	throw std::runtime_error( "failed to find supported format!" );
}