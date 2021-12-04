#pragma once
#include "../Renderer/Material.h"

namespace Balbino
{
	class CMaterialManager
	{
	public:
		CMaterialManager() = default;
		~CMaterialManager();
		CMaterialManager(const CMaterialManager&) = delete;
		CMaterialManager(CMaterialManager&&) = delete;
		CMaterialManager& operator=(const CMaterialManager&) = delete;
		CMaterialManager& operator=(CMaterialManager&&) = delete;

		void Initialize();
		void Cleanup();

		void AddReserve(uint32_t size);
		void BindMaterial( uint32_t id ) const;
		CMaterial* AddMaterial( BalVulkan::CShaderPipeline* shaderPipeline, BalVulkan::CCommandPool* pPool, const std::vector<BalVulkan::SDescriptorSet>& descriptorSetsInfo, const BalVulkan::CDevice* pDevice );
	private:
		std::vector<CMaterial> m_materials;
	};
}
