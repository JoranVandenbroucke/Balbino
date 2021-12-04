#include "pch.h"
#include "Mesh.h"

#include "CommandPool.h"
#include "Common.h"
#include "../Managers/Manager.h"
#include "../Managers/MaterialManager.h"

Balbino::CMesh::CMesh( std::vector<BalVulkan::SVertex> vertices, std::vector<uint32_t> indices,
                       std::vector<SMeshMetadata> metadatas)
	: m_vertices{ std::move( vertices ) }
	, m_indices{ std::move( indices ) }
	, m_metadatas{ std::move( metadatas ) }
{
}

Balbino::CMesh::~CMesh()
{
	if (!m_vertices.empty() || !m_indices.empty() || !m_metadatas.empty())
		std::cout << "mesh was not cleaned up" << std::endl;
}

void Balbino::CMesh::Draw( const BalVulkan::CCommandPool* pCommandBuffer ) const
{
	m_vertexBuffer.Bind();
	m_indexBuffer.Bind();
	for ( const auto& [materialId, indexCount, indexStart] : m_metadatas )
	{
		CManager::GetMaterialManager()->BindMaterial( materialId );
		BalVulkan::DrawMesh( pCommandBuffer, indexCount, indexCount );
	}
}

void Balbino::CMesh::Initialize( const BalVulkan::CDevice* pDevice, const BalVulkan::CCommandPool* pCommandPool, const BalVulkan::CQueue* pQueue )
{
	m_vertexBuffer.Initialize( m_vertices, pDevice, pCommandPool, pQueue );
	m_indexBuffer.Initialize( m_indices, pDevice, pCommandPool, pQueue );
}

void Balbino::CMesh::Cleanup()
{
	m_vertexBuffer.Cleanup();
	m_indexBuffer.Cleanup();
	m_vertices.clear();
	m_indices.clear();
	m_metadatas.clear();
}
