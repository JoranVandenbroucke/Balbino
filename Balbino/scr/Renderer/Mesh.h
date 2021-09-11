#pragma once
#include "IndexBuffer.h"
#include "Vertex.h"
#include "VertexBuffer.h"
#include "MeshMaterialData.h"

#include <vector>


namespace Balbino
{
	class CMesh
	{
	public:
		CMesh();
		~CMesh() = default;
		CMesh(const CMesh&) = delete;
		CMesh(CMesh&&) = delete;
		CMesh& operator&(const CMesh&) = delete;
		CMesh& operator&(CMesh&&) = delete;

		void Initialize( const VkDevice& device, const VkCommandPool& commandPool, const VkQueue& queue, const VkPhysicalDevice& physicalDevice, const VkAllocationCallbacks* callbacks );
		void Cleanup(const VkDevice& device, const VkAllocationCallbacks* callbacks);
		void Draw(const VkCommandBuffer& commandBuffer) const;
	private:
		CVertexBuffer m_vertexBuffer;
		CIndexBuffer m_indexBuffer;
		const std::vector<SVertex> m_vertices;
		const std::vector<uint16_t> m_indices;
		const std::vector<SMeshMaterialData> m_materialData;
	};

}