#pragma once
#include <unordered_map>
#include <vulkan/vulkan.h>
#include "../Renderer/Mesh.h"

namespace Balbino
{
	class CRenderer;

	class CMeshManager final
	{
	public:
		CMeshManager();
		~CMeshManager();
		CMeshManager(const CMeshManager&) = delete;
		CMeshManager(CMeshManager&&) = delete;
		CMeshManager& operator=(const CMeshManager&) = delete;
		CMeshManager& operator=(CMeshManager&&) = delete;

		void Initialize(CRenderer* pRenderer);
		void Cleanup(const VkDevice& device, const VkAllocationCallbacks* pAllocator);
		void SetRenderer(CRenderer* pRenderer);
		void Draw(const VkCommandBuffer& commandBuffer, const VkDescriptorSet* descriptorSet);
		CMesh* AddMesh(const char* filePath);
	private:
		CRenderer* m_pRenderer;
		std::unordered_map<uint32_t, CMesh> m_meshes;
	};
}

