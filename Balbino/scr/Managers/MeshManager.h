#pragma once
#include "Singleton.h"

#include <unordered_map>
#include <vulkan/vulkan.h>

namespace Balbino
{
	class CRenderer;
	class CMesh;

	class CMeshManager final : public CSingleton<CMeshManager>
	{
	public:
		static CMesh* AddMesh(CMesh* pTexture, const std::string& filePath);
		static CMesh* AddMesh(CMesh* pTexture, const char* filePath);

		void Initialize(CRenderer* pRenderer);
		void Cleanup(const VkDevice& device, const VkAllocationCallbacks* pAllocator);
		void SetRenderer(CRenderer* pRenderer);

	private:

		friend CSingleton<CMeshManager>;

		CMeshManager();
		virtual ~CMeshManager() override;
		CMeshManager(const CMeshManager&) = delete;
		CMeshManager(CMeshManager&&) = delete;
		CMeshManager& operator=(const CMeshManager&) = delete;
		CMeshManager& operator=(CMeshManager&&) = delete;

		CMesh* IAddMesh(CMesh* pMesh, const char* filePath);

		CRenderer* m_pRenderer;
		std::unordered_map<uint32_t, CMesh*> m_meshes;
	};
}

