#include "Mesh.h"

FawnVision::CMesh::CMesh ( const FawnVision::SMeshCreateInfo & createInfo, const CUuid& uuid )
        : m_vertices{ createInfo.vertices }
          , m_indices{ createInfo.indices }
          , m_metadatas{ createInfo.metadata }
          , m_uuid{ uuid }
{
    m_materialCount = (int) m_metadatas.size();
}

void FawnVision::CMesh::Initialize( FawnVision::Device device, FawnVision::CommandPool commandPool, FawnVision::Queue queue )
{
    m_vertex.Initialize( m_vertices, device, commandPool, queue );
    m_index.Initialize( m_indices, device, commandPool, queue );
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
