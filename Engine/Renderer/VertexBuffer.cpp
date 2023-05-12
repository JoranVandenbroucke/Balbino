#include "VertexBuffer.h"

#include <iostream>

#include <Vertex.h>
#include <Buffer.h>
#include <CommandPool.h>
#include <Queue.h>

FawnVision::CVertexBuffer::CVertexBuffer()
        : m_vertexBuffer{ nullptr }
{
}

FawnVision::CVertexBuffer::~CVertexBuffer()
{
    if ( m_vertexBuffer != nullptr )
    {
        std::cerr << "index buffer was not destroyed" << std::endl;
    }
}

void FawnVision::CVertexBuffer::Initialize( const std::vector<FawnVision::SVertex>& vertices, const FawnVision::CDevice* pDevice, FawnVision::CCommandPool* pCommandPool, FawnVision::CQueue* pQueue )
{
    pCommandPool->AddRef();
    pQueue->AddRef();
    
    const uint64_t   size{ sizeof( FawnVision::SVertex ) * vertices.size() };
    FawnVision::CBuffer stagingBuffer{ pDevice, pCommandPool, pQueue };
    
    stagingBuffer.Initialize(
            size,
            FawnVision::EBufferUsageFlagBits::TransferSrcBit,
            FawnVision::EMemoryPropertyFlagBits::Enum( FawnVision::EMemoryPropertyFlagBits::HostVisibleBit | FawnVision::EMemoryPropertyFlagBits::HostCoherentBit ));
    stagingBuffer.UpdateData( vertices.data(), size );
    
    m_vertexBuffer = new FawnVision::CBuffer{ pDevice, pCommandPool, pQueue };
    m_vertexBuffer->Initialize(
            size,
            FawnVision::EBufferUsageFlagBits::Enum( FawnVision::EBufferUsageFlagBits::TransferDstBit | FawnVision::EBufferUsageFlagBits::VertexBufferBit ),
            FawnVision::EMemoryPropertyFlagBits::DeviceLocalBit
    );
    
    stagingBuffer.CopyBuffer( *m_vertexBuffer, size );
    
    pCommandPool->Release();
    pQueue->Release();
}

void FawnVision::CVertexBuffer::Cleanup()
{
    if ( !m_vertexBuffer )
    {
        return;
    }
    m_vertexBuffer->Release();
    m_vertexBuffer = nullptr;
}

void FawnVision::CVertexBuffer::Bind() const
{
    m_vertexBuffer->Bind( false );
}
