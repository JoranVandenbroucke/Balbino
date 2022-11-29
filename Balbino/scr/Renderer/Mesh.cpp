#include "Mesh.h"

#include <utility>

Balbino::CMesh::CMesh( std::vector<BalVulkan::SVertex> vertices, std::vector<uint32_t> indices, std::vector<Balbino::SMeshMetadata> metadatas, const CUuid& uuid )
        : m_vertices{ std::move( vertices ) },
          m_indices{ std::move( indices ) },
          m_metadatas{ std::move( metadatas ) },
          m_uuid{ uuid },
          m_materialCount{ 0 }
{
    m_materialCount = (int) m_metadatas.size();
}

Balbino::CMesh::~CMesh()
{
    m_vertices.clear();
    m_indices.clear();
    m_metadatas.clear();
}

void Balbino::CMesh::Initialize( const BalVulkan::CDevice* pDevice, const BalVulkan::CCommandPool* pCommandPool, const BalVulkan::CQueue* pQueue )
{
    m_vertex.Initialize( m_vertices, pDevice, pCommandPool, pQueue );
    m_index.Initialize( m_indices, pDevice, pCommandPool, pQueue );
}

void Balbino::CMesh::Cleanup()
{
    m_vertex.Cleanup();
    m_index.Cleanup();
}

void Balbino::CMesh::Bind() const
{
    m_index.Bind();
    m_vertex.Bind();
}

CUuid Balbino::CMesh::GetUuid() const
{
    return m_uuid;
}

const std::vector<Balbino::SMeshMetadata>& Balbino::CMesh::GetMetaData() const
{
    return m_metadatas;
}

Balbino::CMesh* Balbino::CMesh::CreateNew( std::vector<BalVulkan::SVertex>& vertices, std::vector<uint32_t>& indices, const std::vector<Balbino::SMeshMetadata>& metaData, uint64_t uuid )
{
    return new CMesh{ vertices, indices, metaData, uuid };
}

uint32_t Balbino::CMesh::GetMaterialCount() const
{
    return m_materialCount;
}
