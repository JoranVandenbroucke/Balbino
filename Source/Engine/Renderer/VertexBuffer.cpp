#include "VertexBuffer.h"
#include <DeerVulkan.h>

void FawnVision::CVertexBuffer::Initialize( const std::vector<FawnVision::SVertex>& vertices, FawnVision::Device device, FawnVision::CommandPool commandPool, FawnVision::Queue queue )
{
    commandPool->AddRef();
    queue->AddRef();
    
    const uint64_t   size{ sizeof( FawnVision::SVertex ) * vertices.size() };
    FawnVision::Buffer stagingBuffer { FawnVision::CreateBuffer(
            device,
            commandPool,
            queue,
            size, FawnVision::buffer_usage_flag::buffer_usage_flag_transfer_src,
                                                                 FawnVision::memory_property_flag( FawnVision::memory_property_flag::memory_property_flag_host_visible | FawnVision::memory_property_flag::memory_property_flag_host_coherent ))
    };
    stagingBuffer->UpdateData(vertices.data(), size);
    m_vertexBuffer = FawnVision::CreateBuffer(
        device,
        commandPool,
        queue,
        size, FawnVision::buffer_usage_flag( FawnVision::buffer_usage_flag::buffer_usage_flag_transfer_dst | FawnVision::buffer_usage_flag::buffer_usage_flag_vertex_buffer ),
                                               FawnVision::memory_property_flag::memory_property_flag_device_local
    );
    stagingBuffer->CopyBuffer( m_vertexBuffer, size );

    commandPool->Release();
    queue->Release();
}

void FawnVision::CVertexBuffer::Cleanup()
{
    if ( !m_vertexBuffer )
    {
        return;
    }
    m_vertexBuffer->Release();
    m_vertexBuffer = BALBINO_NULL;
}

void FawnVision::CVertexBuffer::Bind() const
{
    m_vertexBuffer->Bind( false );
}
