#include "ImageResource.h"

#include "Device.h"

#include "Buffer.h"

DeerVulkan::CImageResource::~CImageResource ()
{
    if ( !m_ownedBySwapchain )
    {
        vkDestroyImage( GetDevice()->GetVkDevice(), m_image, VK_NULL_HANDLE );
        vkFreeMemory( GetDevice()->GetVkDevice(), m_memory, VK_NULL_HANDLE );
    }
}

VkResult DeerVulkan::CImageResource::InitFromSwapchain ( const VkImage& image, const VkImageLayout layout, const uint32_t width, const uint32_t height, const VkFormat format )
{
     if(m_image)
    {
        return VK_ERROR_UNKNOWN;
    }
    m_image = image;
    m_createInfo.format        = format;
    m_createInfo.extent.width  = width;
    m_createInfo.extent.height = height;
    m_createInfo.extent.depth  = 1;
    m_createInfo.arrayLayers   = 1;
    m_createInfo.mipLevels     = 1;
    m_createInfo.imageType     = VK_IMAGE_TYPE_2D;
    m_ownedBySwapchain = true;
    m_imageLayout      = layout;
    m_format = (uint8_t)FawnVision::ConvertImageFormat((uint32_t)format);
    return VK_SUCCESS;
}

void DeerVulkan::CImageResource::Initialize ( const FawnVision::SImageCreateInfo& createInfo )
{
     if(m_image)
    {
        return;
    }

    const auto imageLayout { static_cast<VkImageLayout>( createInfo.initialLayout ) };
    const auto usageFlags { static_cast<VkImageUsageFlagBits>( createInfo.usage ) };

    VkImageType imageType {};
    uint32_t    flags {};
    switch ((FawnVision::image_view_type) createInfo.imageType )
    {
        case FawnVision::image_view_type::view_1d:
        case FawnVision::image_view_type::view_1d_array:
            imageType = VK_IMAGE_TYPE_1D;
            break;
        case FawnVision::image_view_type::view_cube:
        case FawnVision::image_view_type::view_cube_array:
            flags = VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT;
        case FawnVision::image_view_type::view_2d:
        case FawnVision::image_view_type::view_2d_array:
        case FawnVision::image_view_type::view_rect:
        case FawnVision::image_view_type::view_rect_array:
            imageType = VK_IMAGE_TYPE_2D;
            break;
        case FawnVision::image_view_type::view_3d:
            imageType = VK_IMAGE_TYPE_3D;
            break;
    }

    m_imageLayout = imageLayout;
    m_createInfo.sType         = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    m_createInfo.flags         = flags;
    m_createInfo.imageType     = imageType;
    m_createInfo.format        = (VkFormat)FawnVision::ConvertImageFormat( createInfo.format );
    m_createInfo.extent        = VkExtent3D { createInfo.width, createInfo.height, createInfo.depth };
    m_createInfo.mipLevels     = createInfo.mipLevels;
    m_createInfo.arrayLayers   = createInfo.arrayLayers;
    //  todo:  m_createInfo.samples       = VkSampleCountFlagBits(( createInfo.samples == 7 )
    //                                                       ? GetDevice()->GetPhysicalDeviceInfo().GetMaxUsableSampleCount()
    //                                                       : 1 << createInfo.samples
    //    );
    m_createInfo.samples       = VK_SAMPLE_COUNT_1_BIT;
    m_createInfo.tiling        = VK_IMAGE_TILING_OPTIMAL;
    m_createInfo.usage         = usageFlags;
    m_createInfo.sharingMode   = VK_SHARING_MODE_EXCLUSIVE;
    m_createInfo.initialLayout = imageLayout;

    CheckVkResult( vkCreateImage( GetDevice()->GetVkDevice(), &m_createInfo, VK_NULL_HANDLE, &m_image ), "failed to create image!" );
    m_format = createInfo.format;

    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements( GetDevice()->GetVkDevice(), m_image, &memRequirements );

    VkMemoryAllocateInfo allocInfo {};
    allocInfo.sType           = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize  = memRequirements.size;
    allocInfo.memoryTypeIndex = FindMemoryType(
    GetDevice()->GetPhysicalDeviceInfo().device,
        memRequirements.memoryTypeBits,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
    );

    if ( vkAllocateMemory( GetDevice()->GetVkDevice(), &allocInfo, VK_NULL_HANDLE, &m_memory ) != VK_SUCCESS )
    {
        throw std::runtime_error( "failed to allocate image memory!" );
    }

    CheckVkResult( vkBindImageMemory( GetDevice()->GetVkDevice(), m_image, m_memory, 0 ));
}

void DeerVulkan::CImageResource::TransitionImageLayout ( uint32_t mipLevels, uint32_t layers, FawnVision::Buffer command, FawnVision::image_layout newLayout ) noexcept
{
    command->AddRef();
    command->BeginSingleTimeCommands();

    FawnVision::pipeline_stage_flag sourceStage { FawnVision::pipeline_stage_flag::all_commands_bit };
    FawnVision::pipeline_stage_flag destinationStage { FawnVision::pipeline_stage_flag::all_commands_bit };
    VkImageAspectFlags  aspectMask { VK_IMAGE_ASPECT_COLOR_BIT };

    switch ( newLayout )
    {
        case FawnVision::image_layout::depth_stencil_attachment_optimal:
        case FawnVision::image_layout::depth_stencil_read_only_optimal:
        case FawnVision::image_layout::depth_read_only_stencil_attachment_optimal:
        case FawnVision::image_layout::depth_attachment_stencil_read_only_optimal:
            aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;
            break;
        case FawnVision::image_layout::depth_attachment_optimal:
        case FawnVision::image_layout::depth_read_only_optimal:
            // todo:          aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
            // todo:          break;
        case FawnVision::image_layout::stencil_attachment_optimal:
        case FawnVision::image_layout::stencil_read_only_optimal:
            aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
            break;
        default:
            break;
    }

    // CreateNew an image barrier object
    VkImageMemoryBarrier barrier {
        .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
        .oldLayout = m_imageLayout,
        .newLayout = static_cast<VkImageLayout>( newLayout ),
        .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
        .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
        .image = m_image,
        .subresourceRange{
            .aspectMask = aspectMask,
            .baseMipLevel = 0,
            .levelCount = mipLevels,
            .baseArrayLayer = 0,
            .layerCount = layers,
        },
    };
    m_imageLayout = barrier.newLayout;
    // Source layouts (old)
    // Source access mask controls actions that have to be finished on the old layout
    // constexpr before it will be transitioned to the new layout
    switch ( barrier.oldLayout )
    {
        case VK_IMAGE_LAYOUT_UNDEFINED:
            // image layout is undefined (or does not matter)
            // Only valid as initial layout
            // No flags required, listed only for completeness
            barrier.srcAccessMask = 0;
            break;

        case VK_IMAGE_LAYOUT_PREINITIALIZED:
            // image is preinitialized
            // Only valid as initial layout for linear images, preserves memory contents
            // Make sure host writes have been finished
            barrier.srcAccessMask = VK_ACCESS_HOST_WRITE_BIT;
            break;

        case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
            // image is a color attachment
            // Make sure any writes to the color buffer have been finished
            barrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
            break;

        case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
            // image is a depth/stencil attachment
            // Make sure any writes to the depth/stencil buffer have been finished
            barrier.srcAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
            break;

        case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:
            // image is a transfer source
            // Make sure any reads from the image have been finished
            barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
            break;

        case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
            // image is a transfer destination
            // Make sure any writes to the image have been finished
            barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            break;

        case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
            // image is read by a shader
            // Make sure any shader reads from the image have been finished
            barrier.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
            break;
        default:
            // Other source layouts aren't handled (yet)
            break;
    }

    // Target layouts (new)
    // constexpr Destination access mask controls the dependency for the new image layout
    switch ( barrier.newLayout )
    {
        case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
            // image will be used as a transfer destination
            // Make sure any writes to the image have been finished
            barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            break;

        case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:
            // image will be used as a transfer source
            // Make sure any reads from the image have been finished
            barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
            break;

        case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
            // image will be used as a color attachment
            // Make sure any writes to the color buffer have been finished
            barrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
            break;

        case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
            // image layout will be used as a depth/stencil attachment
            // Make sure any writes to depth/stencil buffer have been finished
            barrier.dstAccessMask = barrier.dstAccessMask | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
            break;

        case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
            // image will be read in a shader (sampler, input attachment)
            // Make sure any writes to the image have been finished
            if ( barrier.srcAccessMask == 0 )
            {
                barrier.srcAccessMask = VK_ACCESS_HOST_WRITE_BIT | VK_ACCESS_TRANSFER_WRITE_BIT;
            }
            barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
            break;
        default:
            // Other source layouts aren't handled (yet)
            break;
    }
    command->PipelineBarrier( sourceStage, destinationStage, barrier );

    command->EndSingleTimeCommands();
    command->Release();
}

void DeerVulkan::CImageResource::GenerateMipMaps ( uint32_t mipLevels, FawnVision::Buffer command )
{
    command->AddRef();
    VkFormatProperties formatProperties;
    vkGetPhysicalDeviceFormatProperties( GetDevice()->GetPhysicalDeviceInfo().device, (VkFormat)FawnVision::ConvertImageFormat(m_format), &formatProperties );

    if ( !( formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT ))
    {
        throw std::runtime_error( "texture image format does not support linear blitting!" );
    }

    command->BeginSingleTimeCommands();

    VkImageMemoryBarrier barrier {
        .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER, .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED, .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED, .image = m_image, .subresourceRange{
            .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT, .levelCount = 1, .baseArrayLayer = 0, .layerCount = 1,
        },
    };

    int32_t mipWidth  = (int32_t) m_createInfo.extent.width;
    int32_t mipHeight = (int32_t) m_createInfo.extent.height;

    for ( uint32_t i = 1; i < mipLevels; i++ )
    {
        barrier.subresourceRange.baseMipLevel = i - 1;
        barrier.oldLayout                     = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
        barrier.newLayout                     = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
        barrier.srcAccessMask                 = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.dstAccessMask                 = VK_ACCESS_TRANSFER_READ_BIT;
        command->PipelineBarrier( FawnVision::pipeline_stage_flag::transfer_bit, FawnVision::pipeline_stage_flag::transfer_bit, barrier );
        const VkImageBlit blit {
            .srcSubresource{
                .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT, .mipLevel = i - 1, .baseArrayLayer = 0, .layerCount = 1,
            }, .srcOffsets = {{ 0,        0,         0 },
                              { mipWidth, mipHeight, 0 }}, .dstSubresource =
                {
                    .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT, .mipLevel = i, .baseArrayLayer = 0, .layerCount = 1,
                }, .dstOffsets = {{ 0,   0,   0 },
                                  {
                                    mipWidth > 1
                                    ? mipWidth / 2
                                    : 1, mipHeight > 1
                                         ? mipHeight / 2
                                         : 1, 1
                                  }},
        };
        command->Blit( m_image, blit );

        barrier.oldLayout     = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
        barrier.newLayout     = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

        command->PipelineBarrier(
            FawnVision::pipeline_stage_flag::transfer_bit, FawnVision::pipeline_stage_flag::fragment_shader_bit, barrier
        );

        if ( mipWidth > 1 )
        {
            mipWidth /= 2;
        }
        if ( mipHeight > 1 )
        {
            mipHeight /= 2;
        }
    }

    barrier.subresourceRange.baseMipLevel = mipLevels - 1;
    barrier.oldLayout                     = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    barrier.newLayout                     = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    barrier.srcAccessMask                 = VK_ACCESS_TRANSFER_WRITE_BIT;
    barrier.dstAccessMask                 = VK_ACCESS_SHADER_READ_BIT;

    command->PipelineBarrier( FawnVision::pipeline_stage_flag::transfer_bit, FawnVision::pipeline_stage_flag::fragment_shader_bit, barrier );
    command->EndSingleTimeCommands();
}
