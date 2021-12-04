#pragma once
#include "IndexBuffer.h"
#include "MeshMetadata.h"
#include "Vertex.h"
#include "VertexBuffer.h"

namespace BalVulkan
{
	class CDevice;
	class CCommandPool;
	class CQueue;
}

namespace Balbino
{
	class CMesh
	{
	public:
		CMesh() = default;
		CMesh( std::vector<BalVulkan::SVertex> vertices, std::vector<uint32_t> indices,
		       std::vector<SMeshMetadata> metadatas);
		~CMesh();
		CMesh(const CMesh&) = delete;
		CMesh(CMesh&&) = delete;
		CMesh& operator=(const CMesh&) = delete;
		CMesh& operator=(CMesh&&) = delete;

		void Initialize( const BalVulkan::CDevice* pDevice, const BalVulkan::CCommandPool* pCommandPool, const BalVulkan::CQueue* pQueue );
		void Cleanup();
		void Draw( const BalVulkan::CCommandPool* pCommandBuffer ) const;
	private:
		std::vector<BalVulkan::SVertex> m_vertices;
		std::vector<uint32_t> m_indices;
		std::vector<SMeshMetadata> m_metadatas;

		CVertexBuffer m_vertexBuffer;
		CIndexBuffer m_indexBuffer;
	};
}
