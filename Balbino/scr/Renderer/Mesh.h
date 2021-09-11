#pragma once
#include "IndexBuffer.h"
#include "Vertex.h"
#include "VertexBuffer.h"

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

		void* operator new(size_t size, const char* filePath);
		void* operator new(size_t size, int b, const char* f, int l, const char* filePath);
		void* operator new(size_t size) = delete;
		void* operator new[](size_t size) = delete;

		void operator delete(void* ptr);
		void operator delete(void* ptr, const char* filePath);
		void operator delete(void* ptr, int b, const char* f, int l, const char* filePath);
		void operator delete[](void* ptr) = delete;

		void Initialize( const VkDevice& device, const VkCommandPool& commandPool, const VkQueue& queue, const VkPhysicalDevice& physicalDevice, const VkAllocationCallbacks* callbacks, const std::vector<SVertex>& vertices, const std::vector<uint16_t>& indices );
		void Cleanup(const VkDevice& device, const VkAllocationCallbacks* callbacks);
		void Draw(const VkCommandBuffer& commandBuffer) const;
	private:
		CVertexBuffer m_vertexBuffer{};
		CIndexBuffer m_indexBuffer;
		uint32_t m_indixCount;
	};

}