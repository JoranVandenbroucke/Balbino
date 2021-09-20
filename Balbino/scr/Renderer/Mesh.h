#pragma once
#include "IndexBuffer.h"
#include "Vertex.h"
#include "VertexBuffer.h"
#include "MeshMetadata.h"

#include <vector>


namespace Balbino
{
	class CMesh
	{
	public:
		CMesh() = default;
		~CMesh() = default;
		CMesh(const CMesh&) = delete;
		CMesh(CMesh&&) = delete;
		CMesh& operator=(const CMesh&) = delete;
		CMesh& operator=(CMesh&&) = delete;

		void Initialize(const VkDevice& device, const VkCommandPool& commandPool, const VkQueue& queue, const VkPhysicalDevice& physicalDevice, const
		                VkAllocationCallbacks* callbacks, const std::vector<SVertex>& vertices, const std::vector<uint32_t>& indices, const std
		                ::vector<SMeshMetadata>& metadatas);
		void Cleanup(const VkDevice& device, const VkAllocationCallbacks* callbacks);
		void Draw(const VkCommandBuffer& commandBuffer, const VkDescriptorSet* descriptorSet) const;
	private:
		CVertexBuffer m_vertexBuffer;
		CIndexBuffer m_indexBuffer;
		std::vector<SMeshMetadata> m_metadatas;
	};

}