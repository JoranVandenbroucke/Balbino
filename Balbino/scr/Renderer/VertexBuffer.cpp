#include "pch.h"
#include "VertexBuffer.h"

#include <Buffer.h>

Balbino::CVertexBuffer::CVertexBuffer()
	: m_vertexBuffer{nullptr}
{
}

Balbino::CVertexBuffer::~CVertexBuffer()
{
	if (m_vertexBuffer != nullptr )
		std::cerr << "index buffer was not destroyed" << std::endl;
}

void Balbino::CVertexBuffer::Initialize( const std::vector<BalVulkan::SVertex>& vertices, const BalVulkan::CDevice* pDevice, const BalVulkan::CCommandPool* pCommandPool, const BalVulkan::CQueue* pQueue )
{
	const uint64_t size{ sizeof( BalVulkan::SVertex ) * vertices.size() };
	BalVulkan::CBuffer stagingBuffer{ pDevice, pCommandPool, pQueue };
	stagingBuffer.Initialize( size, BalVulkan::EBufferType::StagingBuffer );
	stagingBuffer.UpdateData( vertices.data(), size );

	m_vertexBuffer = DBG_NEW BalVulkan::CBuffer{ pDevice, pCommandPool, pQueue };
	m_vertexBuffer->Initialize( size, BalVulkan::EBufferType::VertexBuffer );

	stagingBuffer.CopyBuffer( *m_vertexBuffer, size );
}

void Balbino::CVertexBuffer::Cleanup()
{
	if ( !m_vertexBuffer )
		return;
	m_vertexBuffer->Release();
	m_vertexBuffer = nullptr;
}

void Balbino::CVertexBuffer::Bind() const
{
	m_vertexBuffer->Bind( false );
}