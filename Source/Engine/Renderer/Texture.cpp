//
// Created by joran on 29/07/2023.
//
#include "Texture.h"
#include <DeerVulkan.h>

FawnVision::CTexture::CTexture ( FawnVision::Device device, CUuid uuid )
: m_uuid { uuid }
, m_device { device }
{
}

void FawnVision::CTexture::Initialize ( const FawnVision::STextureCreateInfo& createInfo, FawnVision::CommandPool pool, FawnVision::Queue queue, bool generateMips ) noexcept
{
    FawnVision::Buffer stagingBuffer {
        FawnVision::CreateBuffer(
            m_device,
            pool,
            queue,
            createInfo.imageDataSize, FawnVision::buffer_usage_flag_transfer_src, FawnVision::memory_property_flag_host_visible | FawnVision::memory_property_flag_host_coherent
        )
    };
    stagingBuffer->UpdateData(createInfo.pImageData, createInfo.imageDataSize);
    
    m_resource = FawnVision::CreateImageResource( m_device, createInfo.imageCreateInfo );
    m_resource->TransitionImageLayout( createInfo.imageCreateInfo.mipLevels, createInfo.imageCreateInfo.arrayLayers, stagingBuffer, FawnVision::image_layout::transfer_dst_optimal );
    
    stagingBuffer->CopyBufferToImage( m_resource );
    if ( generateMips )
    {
        uint8_t mips { (uint8_t) ( std::floor( std::log2( std::max( createInfo.imageCreateInfo.width, createInfo.imageCreateInfo.height ))) + 1 ) };
        m_resource->GenerateMipMaps( mips, stagingBuffer );
    }
    else
    {
        m_resource->TransitionImageLayout( createInfo.imageCreateInfo.mipLevels, createInfo.imageCreateInfo.arrayLayers, stagingBuffer, FawnVision::image_layout::shader_read_only_optimal );
    }
    m_view    = FawnVision::CreateImageView( m_device, m_resource, createInfo.imageViewCreateInfo );
    m_sampler = FawnVision::CreateSampler( m_device, createInfo.samplerCreateInfo );
    FawnVision::FreeBuffer(stagingBuffer);
}
void FawnVision::CTexture::Cleanup () const
{
    m_resource->Release();
    m_view->Release();
    m_sampler->Release();
}