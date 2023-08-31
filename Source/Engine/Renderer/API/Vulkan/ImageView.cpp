#include "ImageView.h"

#include "Device.h"

DeerVulkan::CImageView::~CImageView()
{
    vkDestroyImageView( GetDevice()->GetVkDevice(), m_imageView, VK_NULL_HANDLE );
}

void DeerVulkan::CImageView::Initialize( const FawnVision::SImageViewCreateInfo& createInfo, FawnVision::ImageResource pImage )
{
    const uint32_t actualNumMips   = createInfo.mipCount - createInfo.firstMip;
    const uint32_t actualNumLayers = createInfo.layerCount - createInfo.firstLayer;
    VkImageViewType viewType {};
    switch ( (FawnVision::image_view_type)createInfo.imageViewType )
    {
        case FawnVision::image_view_type::view_1d: viewType = VK_IMAGE_VIEW_TYPE_1D; break;
        case FawnVision::image_view_type::view_1d_array: viewType = VK_IMAGE_VIEW_TYPE_1D_ARRAY; break;
        case FawnVision::image_view_type::view_2d: viewType = VK_IMAGE_VIEW_TYPE_2D; break;
        case FawnVision::image_view_type::view_2d_array: viewType = VK_IMAGE_VIEW_TYPE_2D_ARRAY; break;
        case FawnVision::image_view_type::view_3d: viewType = VK_IMAGE_VIEW_TYPE_3D; break;
        case FawnVision::image_view_type::view_rect: viewType = VK_IMAGE_VIEW_TYPE_2D; break;
        case FawnVision::image_view_type::view_rect_array: viewType = VK_IMAGE_VIEW_TYPE_2D_ARRAY; break;
        case FawnVision::image_view_type::view_cube: viewType = VK_IMAGE_VIEW_TYPE_CUBE; break;
        case FawnVision::image_view_type::view_cube_array: viewType = VK_IMAGE_VIEW_TYPE_CUBE_ARRAY; break;
    }

#if _DEBUG
    const bool bIsCubeLike  = ( viewType == VK_IMAGE_VIEW_TYPE_CUBE ) || ( viewType == VK_IMAGE_VIEW_TYPE_CUBE_ARRAY );
    const bool bIsArrayLike = ( viewType == VK_IMAGE_VIEW_TYPE_1D_ARRAY ) || ( viewType == VK_IMAGE_VIEW_TYPE_2D_ARRAY ) || ( viewType == VK_IMAGE_VIEW_TYPE_CUBE_ARRAY );
    assert( createInfo.firstMip + actualNumMips <= pImage->GetMipCount() && "image mips out of range" );
    assert( createInfo.firstLayer + actualNumLayers <= pImage->GetLayerCount() && "image Layers out of range" );
    assert( actualNumMips * actualNumLayers >= 1 && "No subresources selected" );
    assert( ( !bIsCubeLike || ( actualNumLayers % 6 == 0 ) ) && "Cannot create cubes when number of Layers is not a multiple of 6" );
    assert( ( bIsArrayLike || ( actualNumLayers == ( bIsCubeLike ? 6u : 1u ) ) ) && "Cannot create non-array view when Layer count is not 1" );
    assert( ( viewType != VK_IMAGE_VIEW_TYPE_3D || actualNumLayers == 1 ) && "Cannot create array view on volume textures" );
#endif

    FawnVision::SComponentMapping componentMapping {
        .r = (FawnVision::component_swizzle)( createInfo.componentMapping & 0xFF ),
        .g = (FawnVision::component_swizzle)( ( createInfo.componentMapping >> 8 ) & 0xFF ),
        .b = (FawnVision::component_swizzle)( ( createInfo.componentMapping >> 16 ) & 0xFF ),
        .a = (FawnVision::component_swizzle)( ( createInfo.componentMapping >> 24 ) & 0xFF ),
    };

    VkImageViewCreateInfo info {
        .sType    = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
        .pNext    = VK_NULL_HANDLE,
        .flags    = 0,
        .image    = pImage->GetImage(),
        .viewType = viewType,
        .format   = (VkFormat)FawnVision::ConvertImageFormat( pImage->GetFormat() ),
        .components {
            .r = VkComponentSwizzle( componentMapping.r ),
            .g = VkComponentSwizzle( componentMapping.g ),
            .b = VkComponentSwizzle( componentMapping.b ),
            .a = VkComponentSwizzle( componentMapping.a ),
        },
        .subresourceRange {
            .baseMipLevel   = createInfo.firstMip,
            .levelCount     = actualNumMips,
            .baseArrayLayer = createInfo.firstLayer,
            .layerCount     = actualNumLayers,
        },
    };

    // Find aspect mask
    bool bHasDepth {};
    bool bHasStencil {};
    switch ( (VkFormat)FawnVision::ConvertImageFormat( pImage->GetFormat() ) )
    {
        case VK_FORMAT_S8_UINT: bHasDepth = true;
        case VK_FORMAT_D32_SFLOAT_S8_UINT:
        case VK_FORMAT_D16_UNORM_S8_UINT:
        case VK_FORMAT_D24_UNORM_S8_UINT: bHasStencil = true;
        case VK_FORMAT_D16_UNORM:
        case VK_FORMAT_D32_SFLOAT: {
            bHasDepth = !bHasDepth;
            assert( ( bHasDepth || bHasStencil ) && "Invalid swizzle for depth/stencil format" );
            info.format = (VkFormat)FawnVision::ConvertImageFormat( pImage->GetFormat() );// Views can never re-interpret depth/stencil formats

            info.subresourceRange.aspectMask = bHasDepth ? VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT : 0;
            info.subresourceRange.aspectMask |= bHasStencil ? VK_IMAGE_ASPECT_STENCIL_BIT : 0;
            break;
        }
        default: info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    }

    CheckVkResult( vkCreateImageView( GetDevice()->GetVkDevice(), &info, VK_NULL_HANDLE, &m_imageView ) );
}
