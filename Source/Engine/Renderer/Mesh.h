#pragma once
#include <vector>

#include <UUID.h>
#include "IndexBuffer.h"
#include "MeshMetadata.h"
#include "VertexBuffer.h"

namespace FawnVision
{
    class CMesh final
    {
    public:
        CMesh( const FawnVision::SMeshCreateInfo& createInfo, const CUuid& uuid );
        constexpr ~CMesh()
        {
            m_vertices.clear();
            m_indices.clear();
            m_metadatas.clear();
        }
        constexpr CMesh( const CMesh& )            = delete;
        constexpr CMesh( CMesh&& )                 = delete;
        constexpr CMesh& operator=( const CMesh& ) = delete;
        constexpr CMesh& operator=( CMesh&& )      = delete;

        void Initialize( FawnVision::Device device, FawnVision::CommandPool commandPool, FawnVision::Queue queue );
        void Cleanup();
        void Bind() const;

        [[nodiscard]] constexpr const std::vector<SMeshMetadata>& GetMetaData() const {return m_metadatas;}
        [[nodiscard]] constexpr uint32_t GetMaterialCount() const{return m_materialCount;}
        [[nodiscard]] inline const CUuid& GetUuid() const{return m_uuid;}
        [[nodiscard]] inline CUuid GetUuid(){return m_uuid;}

    private:
        std::vector<SVertex> m_vertices;
        std::vector<uint32_t> m_indices;
        std::vector<SMeshMetadata> m_metadatas;

        CIndexBuffer m_index;
        CVertexBuffer m_vertex;
        CUuid m_uuid;
        int m_materialCount { 0 };
    };
}// namespace FawnVision
