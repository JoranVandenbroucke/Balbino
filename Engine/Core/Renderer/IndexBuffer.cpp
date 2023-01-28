#include "IndexBuffer.h"

#include <Buffer.h>

Balbino::CIndexBuffer::CIndexBuffer()
        : m_pIndexBuffer{ nullptr }
{
}

Balbino::CIndexBuffer::~CIndexBuffer()
{
    if ( m_pIndexBuffer != nullptr )
    {
        std::cerr << "index buffer was not destroyed" << std::endl;
    }
}

void Balbino::CIndexBuffer::Initialize( const std::vector<uint32_t>& indices, const FawnVision::CDevice* pDevice, const FawnVision::CCommandPool* pCommandPool, const FawnVision::CQueue* pQueue )
{
    const uint64_t     size{ sizeof( uint32_t ) * indices.size() };
    FawnVision::CBuffer stagingBuffer{ pDevice, pCommandPool, pQueue };
    stagingBuffer.Initialize(
            size,
            FawnVision::EBufferUsageFlagBits::TransferSrcBit,
            FawnVision::EMemoryPropertyFlagBits::Enum( FawnVision::EMemoryPropertyFlagBits::HostVisibleBit | FawnVision::EMemoryPropertyFlagBits::HostCoherentBit ));
    stagingBuffer.UpdateData( indices.data(), size );
    
    m_pIndexBuffer = new FawnVision::CBuffer{ pDevice, pCommandPool, pQueue };
    m_pIndexBuffer->Initialize(
            size,
            FawnVision::EBufferUsageFlagBits::Enum( FawnVision::EBufferUsageFlagBits::TransferDstBit | FawnVision::EBufferUsageFlagBits::IndexBufferBit ),
            FawnVision::EMemoryPropertyFlagBits::DeviceLocalBit
    );
    
    stagingBuffer.CopyBuffer( *m_pIndexBuffer, size );
}

void Balbino::CIndexBuffer::Cleanup()
{
    m_pIndexBuffer->Release();
    m_pIndexBuffer = nullptr;
}

void Balbino::CIndexBuffer::Bind() const
{
    m_pIndexBuffer->Bind( true );
}
