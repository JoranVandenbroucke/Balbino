#include "pch.h"
#include "IndexBuffer.h"

#include <Buffer.h>

Balbino::CIndexBuffer::CIndexBuffer()
	: m_pIndexBuffer{nullptr}
{
}

Balbino::CIndexBuffer::~CIndexBuffer()
{
	if (m_pIndexBuffer != nullptr )
		std::cerr << "index buffer was not destroyed" << std::endl;
}

void Balbino::CIndexBuffer::Initialize( const std::vector<uint32_t>& indices, const BalVulkan::CDevice* pDevice, const BalVulkan::CCommandPool* pCommandPool, const BalVulkan::CQueue* pQueue )
{
	const uint64_t size{ sizeof( uint32_t ) * indices.size() };
	BalVulkan::CBuffer stagingBuffer{ pDevice, pCommandPool, pQueue };
	stagingBuffer.Initialize( size, BalVulkan::EBufferUsageFlagBits::TransferSrcBit, BalVulkan::EMemoryPropertyFlagBits::HostVisibleBit | BalVulkan::EMemoryPropertyFlagBits::HostCoherentBit );
	stagingBuffer.UpdateData( indices.data(), size );

	m_pIndexBuffer = DBG_NEW BalVulkan::CBuffer{ pDevice, pCommandPool, pQueue };
	m_pIndexBuffer->Initialize( size, BalVulkan::EBufferUsageFlagBits::TransferDstBit | BalVulkan::EBufferUsageFlagBits::IndexBufferBit, BalVulkan::EMemoryPropertyFlagBits::DeviceLocalBit );

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