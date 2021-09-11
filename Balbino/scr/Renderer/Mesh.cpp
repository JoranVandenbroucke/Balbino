#include "pch.h"
#include "Mesh.h"
#include "../Managers/MeshManager.h"
#include "../Managers/ShaderManager.h"

Balbino::CMesh::CMesh()
	: m_indixCount{m_indixCount}
{
}

void* Balbino::CMesh::operator new(size_t size, const char* filePath)
{
	if (CMesh* pMesh = static_cast<CMesh*>(::operator new(size)))
	{
		CMesh* pActualMesh = CMeshManager::AddMesh(pMesh, filePath);

		if (pMesh != pActualMesh)
		{
			delete pMesh;
			return pActualMesh;
		}
		return pMesh;
	}
	return nullptr;
}

void* Balbino::CMesh::operator new(size_t size, int b, const char* f, int l, const char* filePath)
{
	if (CMesh* pMesh = static_cast<CMesh*>(::operator new(size, b, f, l)))
	{
		CMesh* pActualMesh = CMeshManager::AddMesh(pMesh, filePath);

		if (pMesh != pActualMesh)
		{
			delete pMesh;
			return pActualMesh;
		}
		return pMesh;
	}
	return nullptr;
}

void Balbino::CMesh::operator delete(void* ptr)
{
	if (ptr)
		::operator delete(ptr);
	ptr = nullptr;
}

void Balbino::CMesh::operator delete(void* ptr, const char* filePath)
{
	(void) filePath;
	if (ptr)
		::operator delete(ptr);
	ptr = nullptr;
}

void Balbino::CMesh::operator delete(void* ptr, int b, const char* f, int l, const char* filePath)
{
	(void) b;
	(void) f;
	(void) l;
	(void) filePath;
	if (ptr)
		::operator delete(ptr);
	ptr = nullptr;
}

void Balbino::CMesh::Draw(const VkCommandBuffer& commandBuffer) const
{
	m_indexBuffer.Bind(commandBuffer);
	m_vertexBuffer.Bind(commandBuffer);
	vkCmdDrawIndexed(commandBuffer, m_indixCount, 1, 0, 0, 0);
}

void Balbino::CMesh::Initialize(const VkDevice& device, const VkCommandPool& commandPool, const VkQueue& queue, const VkPhysicalDevice& physicalDevice, const VkAllocationCallbacks* callbacks, const std::vector<SVertex>& vertices, const std::vector<uint16_t>& indices)
{
	//m_indices.resize(indices.size());
	m_indixCount = (uint32_t) indices.size();
	m_vertexBuffer.Initialize(device, commandPool, queue, physicalDevice, vertices, callbacks);
	m_indexBuffer.Initialize(device, commandPool, queue, physicalDevice, indices, callbacks);
}

void Balbino::CMesh::Cleanup(const VkDevice& device, const VkAllocationCallbacks* callbacks)
{
	m_vertexBuffer.Cleanup(device, callbacks);
	m_indexBuffer.Cleanup(device, callbacks);
}
