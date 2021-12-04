#include "pch.h"
#include "ImageResource.h"

#include "Buffer.h"
#include "Device.h"
#include "Funtions.h"
#include "Instance.h"

BalVulkan::CImageResource::CImageResource( const CDevice* const device )
	: CDeviceObject{ device }
	, m_ownedBySwapchain{ false }
	, m_mipLevels{ 0 }
	, m_image{ VK_NULL_HANDLE }
	, m_imageLayout{}
	, m_createInfo{}
	, m_memory{ VK_NULL_HANDLE }
	, m_format{}
{
}

uint32_t BalVulkan::CImageResource::GetWidth() const
{
	return m_createInfo.extent.width;
}

uint32_t BalVulkan::CImageResource::GetHeight() const
{
	return m_createInfo.extent.height;
}

uint32_t BalVulkan::CImageResource::GetDepth() const
{
	return m_createInfo.extent.depth;
}

void BalVulkan::CImageResource::TransitionImageLayout( uint32_t mipLevels, const CBuffer* pCommand )
{
	pCommand->BeginSingleTimeCommands();

	EPipelineStageFlagBits sourceStage;
	EPipelineStageFlagBits destinationStage;
	VkImageMemoryBarrier barrier{
		.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
		.oldLayout = m_imageLayout,
		.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,	//todo:: make this "cusomizable"
		.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
		.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
		.image = m_image,
		.subresourceRange{
			.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
			.baseMipLevel = 1,
			.levelCount = mipLevels,
			.baseArrayLayer = 1,
			.layerCount = 1,
		},
	};

	if ( m_imageLayout == VK_IMAGE_LAYOUT_UNDEFINED && barrier.newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL )
	{
		barrier.srcAccessMask = 0;
		barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

		sourceStage = EPipelineStageFlagBits::BottomOfPipeBit;
		destinationStage = EPipelineStageFlagBits::TransferBit;
	}
	else if ( m_imageLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && barrier.newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL )
	{
		barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

		sourceStage = EPipelineStageFlagBits::TransferBit;
		destinationStage = EPipelineStageFlagBits::FragmentShaderBit;
	}
	else
	{
		throw std::invalid_argument( "unsupported layout transition!" );
	}
	pCommand->PipelineBarrier( sourceStage, destinationStage, &barrier );

	pCommand->EndSingleTimeCommands();
}

void BalVulkan::CImageResource::GenerateMipMaps( uint32_t mipLevels, const CBuffer* pCommand )
{
	VkFormatProperties formatProperties;
	vkGetPhysicalDeviceFormatProperties( GetDevice()->GetPhysicalDeviceInfo()->device, m_format, &formatProperties );

	if ( !( formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT ) )
	{
		throw std::runtime_error( "texture image format does not support linear blitting!" );
	}

	pCommand->BeginSingleTimeCommands();

	VkImageMemoryBarrier barrier{
		.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
		.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
		.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
		.image = m_image,
		.subresourceRange{
			.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
			.levelCount = 1,
			.baseArrayLayer = 0,
			.layerCount = 1,
		},
	};

	int32_t mipWidth = static_cast< int32_t >( m_createInfo.extent.width );
	int32_t mipHeight = static_cast< int32_t >( m_createInfo.extent.height );

	for ( uint32_t i = 1; i < mipLevels; i++ )
	{
		barrier.subresourceRange.baseMipLevel = i - 1;
		barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
		barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
		barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
		pCommand->PipelineBarrier( EPipelineStageFlagBits::TransferBit, EPipelineStageFlagBits::TransferBit, &barrier );
		const VkImageBlit blit{
			.srcSubresource{
				.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
				.mipLevel = i - 1,
				.baseArrayLayer = 0,
				.layerCount = 1,
			},
			.srcOffsets = { { 0, 0, 0 }, { mipWidth, mipHeight, 0 } },
			.dstSubresource =
			{
				.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
				.mipLevel = i,
				.baseArrayLayer = 0,
				.layerCount = 1,
			},
			.dstOffsets = { { 0, 0, 0 }, { mipWidth > 1 ? mipWidth / 2 : 1, mipHeight > 1 ? mipHeight / 2 : 1, 1 } },
		};
		pCommand->Blit( &m_image, &blit );

		barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
		barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
		barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

		pCommand->PipelineBarrier( EPipelineStageFlagBits::TransferBit, EPipelineStageFlagBits::FragmentShaderBit, &barrier );

		if ( mipWidth > 1 )
			mipWidth /= 2;
		if ( mipHeight > 1 )
			mipHeight /= 2;
	}

	barrier.subresourceRange.baseMipLevel = mipLevels - 1;
	barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
	barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
	barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

	pCommand->PipelineBarrier( EPipelineStageFlagBits::TransferBit, EPipelineStageFlagBits::FragmentShaderBit, &barrier );
	pCommand->EndSingleTimeCommands();
}

BalVulkan::CImageResource* BalVulkan::CImageResource::CreateNew( const CDevice* pDevice )
{
	return new CImageResource{pDevice};
}

BalVulkan::CImageResource::~CImageResource()
{
	if ( !m_ownedBySwapchain )
	{
		vkDestroyImage( GetDevice()->GetVkDevice(), m_image, nullptr );
		vkFreeMemory( GetDevice()->GetVkDevice(), m_memory, nullptr );
	}
}

VkResult BalVulkan::CImageResource::InitFromSwapchain( const VkImage image, const VkImageLayout layout, const uint32_t width, const uint32_t height, const VkFormat format )
{
	if ( m_image )
		return VK_ERROR_UNKNOWN;
	m_image = image;
	m_createInfo.format = format;
	m_createInfo.extent.width = width;
	m_createInfo.extent.height = height;
	m_createInfo.extent.depth = 1;
	m_createInfo.arrayLayers = 1;
	m_createInfo.mipLevels = 1;
	m_ownedBySwapchain = true;
	m_imageLayout = layout;

	return VK_SUCCESS;
}

VkResult BalVulkan::CImageResource::Initialize( const uint32_t width,
												const uint32_t height,
												const uint32_t mipLevels,
												const uint32_t layers,
												const BalVulkan::EImageLayout layout,
												const BalVulkan::EFormat format,
												const BalVulkan::EImageUsageFlagBits usage)
{
	if ( m_image )
		return VK_ERROR_UNKNOWN;

	VkImageLayout imageLayout{};
	VkImageUsageFlagBits usageflags{( VkImageUsageFlagBits ) usage };
	switch ( layout )
	{
		case BalVulkan::EImageLayout::Color:
			imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
			break;
		case BalVulkan::EImageLayout::Depth:
			imageLayout = VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL;
			break;
	}
	const VkFormat imageFormat{( VkFormat )format};
	
	m_imageLayout = imageLayout;
	m_createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	m_createInfo.flags = 0;
	m_createInfo.imageType = VK_IMAGE_TYPE_2D;
	m_createInfo.format = imageFormat;
	m_createInfo.extent = VkExtent3D{ width, height, 1 };
	m_createInfo.mipLevels = mipLevels;
	m_createInfo.arrayLayers = layers;
	m_createInfo.samples = VK_SAMPLE_COUNT_1_BIT;
	m_createInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
	m_createInfo.usage = usageflags;
	m_createInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	m_createInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

	CheckVkResult( vkCreateImage( GetDevice()->GetVkDevice(), &m_createInfo, nullptr, &m_image ), "failed to create image!" );
	m_mipLevels = mipLevels;
	m_format = imageFormat;

	VkMemoryRequirements memRequirements;
	vkGetImageMemoryRequirements( GetDevice()->GetVkDevice(), m_image, &memRequirements );

	VkMemoryAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = FindMemoryType( GetDevice()->GetPhysicalDeviceInfo()->device, memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT );

	if ( vkAllocateMemory( GetDevice()->GetVkDevice(), &allocInfo, nullptr, &m_memory ) != VK_SUCCESS ) {
		throw std::runtime_error( "failed to allocate image memory!" );
	}

	CheckVkResult( vkBindImageMemory( GetDevice()->GetVkDevice(), m_image, m_memory, 0 ));
	return VK_SUCCESS;
}

const VkImage& BalVulkan::CImageResource::GetImage() const
{
	return m_image;
}

const VkImageLayout& BalVulkan::CImageResource::GetImageLayout() const
{
	return m_imageLayout;
}

const VkImageCreateInfo& BalVulkan::CImageResource::GetImageCreateInfo() const
{
	return m_createInfo;
}

VkImageType BalVulkan::CImageResource::GetDimensionality() const
{
	return m_createInfo.imageType;
}

uint32_t BalVulkan::CImageResource::GetMipCount() const
{
	return m_createInfo.mipLevels;
}

uint32_t BalVulkan::CImageResource::GetLayerCount() const
{
	return m_createInfo.arrayLayers;
}

VkFormat BalVulkan::CImageResource::GetFormat() const
{
	return m_createInfo.format;
}