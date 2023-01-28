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
        CMesh( std::vector<FawnVision::SVertex> vertices, std::vector<uint32_t> indices, std::vector<Balbino::SMeshMetadata> metadatas, const CUuid& uuid );
        ~CMesh() override;
        CMesh( const CMesh& ) = delete;
        CMesh( CMesh&& ) = delete;
        CMesh& operator=( const CMesh& ) = delete;
        CMesh& operator=( CMesh&& ) = delete;
        
        void Initialize( const FawnVision::CDevice* pDevice, const FawnVision::CCommandPool* pCommandPool, const FawnVision::CQueue* pQueue ) override;
        
        void Cleanup() override;
        
        void Bind() const override;
        
        [[nodiscard]] CUuid GetUuid() const override;
        [[nodiscard]] const std::vector<Balbino::SMeshMetadata>& GetMetaData() const override;
        [[nodiscard]] uint32_t GetMaterialCount() const override;
        static CMesh* CreateNew( std::vector<FawnVision::SVertex>& vertices, std::vector<uint32_t>& indices, const std::vector<Balbino::SMeshMetadata>& metaData, uint64_t uuid );
    
    private:
        std::vector<FawnVision::SVertex>     m_vertices;
        std::vector<uint32_t>               m_indices;
        std::vector<Balbino::SMeshMetadata> m_metadatas;
        
        CIndexBuffer  m_index;
        CVertexBuffer m_vertex;
        CUuid         m_uuid;
        int           m_materialCount;
    };
}
