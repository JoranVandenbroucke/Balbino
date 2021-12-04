#pragma once
#include <Vertex.h>

namespace BalVulkan
{
	class CDevice;
	class CCommandPool;
	class CQueue;
	class CBuffer;
}

namespace Balbino
{
	class CVertexBuffer
	{
	public:
		CVertexBuffer();
		~CVertexBuffer();
		CVertexBuffer( const CVertexBuffer& ) = delete;
		CVertexBuffer( CVertexBuffer&& ) = delete;
		CVertexBuffer& operator=( const CVertexBuffer& ) = delete;
		CVertexBuffer& operator=( CVertexBuffer&& ) = delete;

		void Initialize( const std::vector<BalVulkan::SVertex>& vertices, const BalVulkan::CDevice* pDevice, const BalVulkan::CCommandPool* pCommandPool, const BalVulkan::CQueue* pQueue );
		void Cleanup();
		void Bind() const;
	private:
		BalVulkan::CBuffer* m_vertexBuffer;
	};
}
