#include "VertexBuffer.h"

#include <iostream>

#include "Vertex.h"
#include "../Renderer/GraphicsVulkan/Buffer.h"

Balbino::CVertexBuffer::CVertexBuffer()
        : m_vertexBuffer{ nullptr }
{
}

Balbino::CVertexBuffer::~CVertexBuffer()
{
    if ( m_vertexBuffer != nullptr )
    {
        std::cerr << "index buffer was not destroyed" << std::endl;
    }
}

void Balbino::CVertexBuffer::Initialize( const std::vector<FawnVision::SVertex>& vertices, const FawnVision::CDevice* pDevice, const FawnVision::CCommandPool* pCommandPool, const FawnVision::CQueue* pQueue )
{
    const uint64_t     size{ sizeof( FawnVision::SVertex ) * vertices.size() };
    FawnVision::CBuffer stagingBuffer{ pDevice, pCommandPool, pQueue };
    
    stagingBuffer.Initialize(
            size,
            FawnVision::EBufferUsageFlagBits::TransferSrcBit,
            FawnVision::EMemoryPropertyFlagBits::Enum( FawnVision::EMemoryPropertyFlagBits::HostVisibleBit | FawnVision::EMemoryPropertyFlagBits::HostCoherentBit ));
    stagingBuffer.UpdateData( vertices.data(), size );
    
    m_vertexBuffer = FawnVision::CBuffer::CreateNew( pDevice, pCommandPool, pQueue );
    m_vertexBuffer->Initialize(
            size,
            FawnVision::EBufferUsageFlagBits::Enum( FawnVision::EBufferUsageFlagBits::TransferDstBit | FawnVision::EBufferUsageFlagBits::VertexBufferBit ),
            FawnVision::EMemoryPropertyFlagBits::DeviceLocalBit
    );
    
    stagingBuffer.CopyBuffer( *m_vertexBuffer, size );
}

void Balbino::CVertexBuffer::Cleanup()
{
    if ( !m_vertexBuffer )
    {
        return;
    }
    m_vertexBuffer->Release();
    m_vertexBuffer = nullptr;
}

void Balbino::CVertexBuffer::Bind() const
{
    m_vertexBuffer->Bind( false );
}
