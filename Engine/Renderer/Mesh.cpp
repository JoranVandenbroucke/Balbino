#include "Mesh.h"

#include <utility>

FawnVision::CMesh::CMesh( std::vector<FawnVision::SVertex> vertices, std::vector<uint32_t> indices, std::vector<SMeshMetadata> metadatas, const CUuid& uuid )
        : m_vertices{ std::move( vertices ) }
          , m_indices{ std::move( indices ) }
          , m_metadatas{ std::move( metadatas ) }
          , m_uuid{ uuid }
          , m_materialCount{ 0 }
{
    m_materialCount = (int) m_metadatas.size();
}

FawnVision::CMesh::~CMesh()
{
    m_vertices.clear();
    m_indices.clear();
    m_metadatas.clear();
}

void FawnVision::CMesh::Initialize( const FawnVision::CDevice* pDevice, FawnVision::CCommandPool* pCommandPool, FawnVision::CQueue* pQueue )
{
    m_vertex.Initialize( m_vertices, pDevice, pCommandPool, pQueue );
    m_index.Initialize( m_indices, pDevice, pCommandPool, pQueue );
}

void FawnVision::CMesh::Cleanup()
{
    m_vertex.Cleanup();
    m_index.Cleanup();
}

void FawnVision::CMesh::Bind() const
{
    m_index.Bind();
    m_vertex.Bind();
}

CUuid FawnVision::CMesh::GetUuid() const
{
    return m_uuid;
}

const std::vector<SMeshMetadata>& FawnVision::CMesh::GetMetaData() const
{
    return m_metadatas;
}

uint32_t FawnVision::CMesh::GetMaterialCount() const
{
    return m_materialCount;
}
