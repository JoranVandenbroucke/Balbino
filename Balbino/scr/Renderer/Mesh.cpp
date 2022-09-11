#include "Mesh.h"

#include <utility>

Balbino::CMesh::CMesh( std::vector<BalVulkan::SVertex> vertices, std::vector<uint32_t> indices, SMeshMetadata metadatas, const CUuid& uuid )
        : m_vertices{ std::move( vertices ) },
          m_indices{ std::move( indices ) },
          m_metadatas{ metadatas },
          m_uuid{ uuid },
          m_materialCount{ 0 }
{
    SMeshMetadata* temp{ &m_metadatas };
    while ( temp )
    {
        temp = temp->pNext;
        ++m_materialCount;
    }
}

Balbino::CMesh::~CMesh()
{
    m_vertices.clear();
    m_indices.clear();
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

const Balbino::SMeshMetadata* Balbino::CMesh::GetMetaData() const
{
    return &m_metadatas;
}

Balbino::CMesh* Balbino::CMesh::CreateNew( std::vector<BalVulkan::SVertex>& vertices, std::vector<uint32_t>& indices, SMeshMetadata metaData, uint64_t uuid )
{
    return new CMesh{ vertices, indices, metaData, uuid };
}

const uint32_t Balbino::CMesh::GetMaterialCount() const
{
    return m_materialCount;
}
