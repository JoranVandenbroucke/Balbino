#pragma once
#include <unordered_map>
#include "../Core.h"
#include "../Renderer/Mesh.h"

namespace Balbino
{
	class CMesh;
	class CMeshManager final
	{
	public:
		CMeshManager();
		~CMeshManager();
		CMeshManager(const CMeshManager&) = delete;
		CMeshManager(CMeshManager&&) = delete;
		CMeshManager& operator=(const CMeshManager&) = delete;
		CMeshManager& operator=(CMeshManager&&) = delete;

		void Initialize( const BalVulkan::CDevice* pDevice, const BalVulkan::CCommandPool* pCommandPool, const BalVulkan::CQueue* pQueue );
		void Cleanup();
		void Draw( BalVulkan::CCommandPool* pCommandPool );
		BALBINO_API CMesh* AddMesh(const char* filePath);
	private:
		std::unordered_map<uint32_t, CMesh> m_meshes;
	};
}
