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

void Balbino::CVertexBuffer::Initialize( const std::vector<BalVulkan::SVertex>& vertices, const BalVulkan::CDevice* pDevice, const BalVulkan::CCommandPool* pCommandPool, const BalVulkan::CQueue* pQueue )
{
    const uint64_t     size{ sizeof( BalVulkan::SVertex ) * vertices.size() };
    BalVulkan::CBuffer stagingBuffer{ pDevice, pCommandPool, pQueue };
    
    stagingBuffer.Initialize(
            size,
            BalVulkan::EBufferUsageFlagBits::TransferSrcBit,
            BalVulkan::EMemoryPropertyFlagBits::Enum( BalVulkan::EMemoryPropertyFlagBits::HostVisibleBit | BalVulkan::EMemoryPropertyFlagBits::HostCoherentBit ));
    stagingBuffer.UpdateData( vertices.data(), size );
    
    m_vertexBuffer = BalVulkan::CBuffer::CreateNew( pDevice, pCommandPool, pQueue );
    m_vertexBuffer->Initialize(
            size,
            BalVulkan::EBufferUsageFlagBits::Enum( BalVulkan::EBufferUsageFlagBits::TransferDstBit | BalVulkan::EBufferUsageFlagBits::VertexBufferBit ),
            BalVulkan::EMemoryPropertyFlagBits::DeviceLocalBit
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
