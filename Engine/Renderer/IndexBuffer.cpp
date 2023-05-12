#include "IndexBuffer.h"

#include <Buffer.h>
#include <CommandPool.h>
#include <Queue.h>

FawnVision::CIndexBuffer::CIndexBuffer()
        : m_pIndexBuffer{ nullptr }
{
}

FawnVision::CIndexBuffer::~CIndexBuffer()
{
    if ( m_pIndexBuffer != nullptr )
    {
        std::cerr << "index buffer was not destroyed" << std::endl;
    }
}

void FawnVision::CIndexBuffer::Initialize( const std::vector<uint32_t>& indices, const FawnVision::CDevice* pDevice, FawnVision::CCommandPool* pCommandPool, FawnVision::CQueue* pQueue )
{
    pCommandPool->AddRef();
    pQueue->AddRef();
    
    const uint64_t   size{ sizeof( uint32_t ) * indices.size() };
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
    
    pCommandPool->Release();
    pQueue->Release();
}

void FawnVision::CIndexBuffer::Cleanup()
{
    m_pIndexBuffer->Release();
    m_pIndexBuffer = nullptr;
}

void FawnVision::CIndexBuffer::Bind() const
{
    m_pIndexBuffer->Bind( true );
}
