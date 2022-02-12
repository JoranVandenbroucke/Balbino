#pragma once
#include "IMesh.h"

#include "IndexBuffer.h"
#include "MeshMetadata.h"
#include "VertexBuffer.h"
#include "Vertex.h"

#include <vector>

namespace Balbino
{
	class CMesh final : public IMesh
	{
	public:
		CMesh( std::vector<BalVulkan::SVertex> vertices, std::vector<uint32_t> indices,
			   std::vector<SMeshMetadata> metadatas, const CUuid& uuid );
		~CMesh();
		CMesh( const CMesh& ) = delete;
		CMesh( CMesh&& ) = delete;
		CMesh& operator=( const CMesh& ) = delete;
		CMesh& operator=( CMesh&& ) = delete;

		void Initialize( const BalVulkan::CDevice* pDevice, const BalVulkan::CCommandPool* pCommandPool, const BalVulkan::CQueue* pQueue ) override;

		void Cleanup() override;

		void Bind() const override;

		[[nodiscard]] CUuid GetUuid() const override;

		[[nodiscard]] uint64_t GetMaterialCount() const override;

		[[nodiscard]] const std::vector<SMeshMetadata>& GetMetaData()const override;
	private:
		std::vector<BalVulkan::SVertex> m_vertices;
		std::vector<uint32_t> m_indices;
		std::vector<SMeshMetadata> m_metadatas;

		CIndexBuffer m_index;
		CVertexBuffer m_vertex;
		CUuid m_uuid;
	};
}
