#pragma once
#include <IMesh.h>

#include "IndexBuffer.h"
#include "MeshMetadata.h"
#include "VertexBuffer.h"
#include "API/Vulkan/Vertex.h"

#include <vector>

namespace FawnVision
{
    class CMesh final : public IMesh
    {
    public:
        CMesh( std::vector<FawnVision::SVertex> vertices, std::vector<uint32_t> indices, std::vector<SMeshMetadata> metadatas, const CUuid& uuid );
        ~CMesh() override;
        CMesh( const CMesh& ) = delete;
        CMesh( CMesh&& ) = delete;
        CMesh& operator=( const CMesh& ) = delete;
        CMesh& operator=( CMesh&& ) = delete;
        
        void Initialize( const FawnVision::CDevice* pDevice, FawnVision::CCommandPool* pCommandPool, FawnVision::CQueue* pQueue ) override;
        
        void Cleanup() override;
        
        void Bind() const override;
        
        [[nodiscard]] CUuid GetUuid() const override;
        [[nodiscard]] const std::vector<SMeshMetadata>& GetMetaData() const override;
        [[nodiscard]] uint32_t GetMaterialCount() const override;
    
    private:
        std::vector<SVertex>       m_vertices;
        std::vector<uint32_t>      m_indices;
        std::vector<SMeshMetadata> m_metadatas;
        
        CIndexBuffer  m_index;
        CVertexBuffer m_vertex;
        CUuid         m_uuid;
        int           m_materialCount;
    };
}
