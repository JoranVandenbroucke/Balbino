#include "pch.h"
#include "ImageView.h"

#include "Device.h"
#include "Funtions.h"
#include "ImageResource.h"

BalVulkan::CImageView::CImageView( const CImageResource& pImage, BalVulkan::EImageViewType type, uint32_t firstMip, uint32_t numMips, uint32_t firstLayer, uint32_t numLayers )
	:CDeviceObject{ pImage.GetDevice() }
	,m_ownedBySwapchain{ false }
{
	const uint32_t actualNumMips = ( numMips == VK_REMAINING_MIP_LEVELS ) ? pImage.GetMipCount() - firstMip : numMips;
	const uint32_t actualNumLayers = ( numLayers == VK_REMAINING_ARRAY_LAYERS ) ? pImage.GetLayerCount() - firstLayer : numLayers;
	VkImageViewType viewType{ VK_IMAGE_VIEW_TYPE_MAX_ENUM };
	switch ( type )
	{
		case BalVulkan::EImageViewType::Cube:
			viewType = VK_IMAGE_VIEW_TYPE_CUBE;
			break;
		case BalVulkan::EImageViewType::CubeArray:
			viewType = VK_IMAGE_VIEW_TYPE_CUBE_ARRAY;
			break;
		case BalVulkan::EImageViewType::View1D:
			viewType = VK_IMAGE_VIEW_TYPE_1D;
			break;
		case BalVulkan::EImageViewType::View1DArray:
			viewType = VK_IMAGE_VIEW_TYPE_1D_ARRAY;
			break;
		case BalVulkan::EImageViewType::View2D:
			viewType = VK_IMAGE_VIEW_TYPE_2D;
			break;
		case BalVulkan::EImageViewType::View2DArray:
			viewType = VK_IMAGE_VIEW_TYPE_2D_ARRAY;
			break;
		case BalVulkan::EImageViewType::View3D:
			viewType = VK_IMAGE_VIEW_TYPE_3D;
			break;
	}
	if ( viewType == VK_IMAGE_VIEW_TYPE_CUBE_ARRAY - VK_IMAGE_VIEW_TYPE_1D + 1 ) // Handle "default" view
	{
		switch ( pImage.GetDimensionality() )
		{
			case VK_IMAGE_TYPE_1D:
				viewType = actualNumLayers == 1 ? VK_IMAGE_VIEW_TYPE_1D : VK_IMAGE_VIEW_TYPE_1D_ARRAY;
				break;
			case VK_IMAGE_TYPE_2D:
				//todo: actually check flags in the image to see if its a cubemap or not
				if ( actualNumLayers <= 6 )
					if ( actualNumLayers == 6 )
						viewType = VK_IMAGE_VIEW_TYPE_CUBE;
					else
						if ( actualNumLayers == 1 )
							viewType = VK_IMAGE_VIEW_TYPE_2D;
						else
							viewType = VK_IMAGE_VIEW_TYPE_2D_ARRAY;
				else
					if ( actualNumLayers == 1 )
						if ( actualNumLayers == 6 )
							viewType = VK_IMAGE_VIEW_TYPE_CUBE_ARRAY;
						else
							viewType = VK_IMAGE_VIEW_TYPE_2D;
					else
						if ( actualNumLayers == 6 )
							viewType = VK_IMAGE_VIEW_TYPE_CUBE_ARRAY;
						else
							viewType = VK_IMAGE_VIEW_TYPE_2D_ARRAY;
				break;
			case VK_IMAGE_TYPE_3D:
				viewType = VK_IMAGE_VIEW_TYPE_3D;
				break;

				//just to be complete
			case VK_IMAGE_TYPE_MAX_ENUM:
			default:
				assert( false && "Used Wrong Dementionality" );
				break;
		}
	}

#if _DEBUG
	const bool bIsCubeLike = ( viewType == VK_IMAGE_VIEW_TYPE_CUBE ) || ( viewType == VK_IMAGE_VIEW_TYPE_CUBE_ARRAY );
	const bool bIsArrayLike = ( viewType == VK_IMAGE_VIEW_TYPE_1D_ARRAY ) || ( viewType == VK_IMAGE_VIEW_TYPE_2D_ARRAY ) || ( viewType == VK_IMAGE_VIEW_TYPE_CUBE_ARRAY );
	assert( firstMip + actualNumMips <= pImage.GetMipCount() && "Image mips out of range" );
	assert( firstLayer + actualNumLayers <= pImage.GetLayerCount() && "Image Layers out of range" );
	assert( actualNumMips * actualNumLayers >= 1 && "No subresources selected" );
	assert( ( !bIsCubeLike || ( actualNumLayers % 6 == 0 ) ) && "Cannot create cubes when number of Layers is not a multiple of 6" );
	assert( ( bIsArrayLike || ( actualNumLayers == ( bIsCubeLike ? 6u : 1u ) ) )&& "Cannot create non-array view when Layer count is not 1" );
	assert( ( viewType != VK_IMAGE_VIEW_TYPE_3D || actualNumLayers == 1 ) && "Cannot create array view on volume textures" );
#endif

	VkImageViewCreateInfo info{
		.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
		.pNext = nullptr,
		.flags = 0,
		.image = pImage.GetImage(),
		.viewType = viewType,
		.format = pImage.GetFormat(),
		.subresourceRange = {
			.baseMipLevel = firstMip,
			.levelCount = actualNumMips,
			.baseArrayLayer = firstLayer,
			.layerCount = actualNumLayers,
		},
	};
	//todo: map components

	// Find aspect mask
	{
		bool bHasDepth{};
		bool bHasStencil{};
		switch ( pImage.GetFormat() )
		{
			case VK_FORMAT_S8_UINT:
				bHasDepth = !bHasDepth;
			case VK_FORMAT_D32_SFLOAT_S8_UINT:
			case VK_FORMAT_D16_UNORM_S8_UINT:
			case VK_FORMAT_D24_UNORM_S8_UINT:
				bHasStencil = true;
			case VK_FORMAT_D16_UNORM:
			case VK_FORMAT_D32_SFLOAT:
				bHasDepth = !bHasDepth;

				//bHasDepth &= swizzle != kImageSwizzleSS00;
				//bHasStencil &= swizzle != kImageSwizzleDD00;
				assert( ( bHasDepth || bHasStencil ) && "Invalid swizzle for depth/stencil format" );

				info.format = pImage.GetFormat(); // Views can never re-interpret depth/stencil formats
				info.subresourceRange.aspectMask = bHasDepth ? VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT : 0;
				info.subresourceRange.aspectMask |= bHasStencil ? VK_IMAGE_ASPECT_STENCIL_BIT : 0;
				break;
			default:
				info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		}
	}
	CheckVkResult( vkCreateImageView( GetDevice()->GetVkDevice(), &info, nullptr, &m_imageView ) );
}

BalVulkan::CImageView::~CImageView()
{
	if ( !m_ownedBySwapchain )
		vkDestroyImageView( GetDevice()->GetVkDevice(), m_imageView, nullptr );
}

VkImageView BalVulkan::CImageView::GetImageView() const
{
	return m_imageView;
}

void BalVulkan::CImageView::Destroy()
{
	CDeviceObject::Destroy();
}

BalVulkan::CImageView* BalVulkan::CImageView::CreateNew( const CImageResource& pResource, EImageViewType type, uint32_t firstMip, uint32_t numMips, uint32_t firstLayer, uint32_t numLayers )
{
	return new CImageView{pResource, type, firstMip, numMips, firstLayer, numLayers};
}
