#include "IndexBuffer.h"

#include <Buffer.h>
#include <CommandPool.h>
#include <Queue.h>

void FawnVision::CIndexBuffer::Initialize ( const std::vector<uint32_t>& indices, FawnVision::Device device, FawnVision::CommandPool commandPool, FawnVision::Queue queue )
{
    commandPool->AddRef();
    queue->AddRef();

    const uint64_t size { sizeof( uint32_t ) * indices.size() };
    FawnVision::Buffer stagingBuffer { FawnVision::CreateBuffer(
            device,
            commandPool,
            queue,
            size, FawnVision::buffer_usage_flag::buffer_usage_flag_transfer_src,
                                                                 FawnVision::memory_property_flag( FawnVision::memory_property_flag::memory_property_flag_host_visible | FawnVision::memory_property_flag::memory_property_flag_host_coherent ))
    };
    stagingBuffer->UpdateData(indices.data(), size);
    m_pIndexBuffer = FawnVision::CreateBuffer(
        device,
        commandPool,
        queue,
        size, FawnVision::buffer_usage_flag( FawnVision::buffer_usage_flag::buffer_usage_flag_transfer_dst | FawnVision::buffer_usage_flag::buffer_usage_flag_index_buffer ),
                                               FawnVision::memory_property_flag::memory_property_flag_device_local
    );
    stagingBuffer->CopyBuffer( m_pIndexBuffer, size );

    stagingBuffer->Release();
    commandPool->Release();
    queue->Release();
}

void FawnVision::CIndexBuffer::Cleanup ()
{
    m_pIndexBuffer->Release();
    m_pIndexBuffer = BALBINO_NULL;
}

void FawnVision::CIndexBuffer::Bind () const
{
    m_pIndexBuffer->Bind( true );
}
