#pragma once
#include "Common.h"

namespace BalVulkan
{
	class CDevice;
	class CCommandPool;
	class CShaderPipeline;
	class CDescriptorSet;
}

namespace Balbino
{
	class CShader;
	class CTexture;

	class CMaterial
	{
	public:
		CMaterial( BalVulkan::CShaderPipeline* shaderPipeline, BalVulkan::CCommandPool* pPool );
		~CMaterial() = default;
		CMaterial(const CMaterial&) = delete;
		CMaterial(CMaterial&&) = default;
		CMaterial& operator=(const CMaterial&) = delete;
		CMaterial& operator=(CMaterial&&) = delete;
		void Initialize( const std::vector<BalVulkan::SDescriptorSet>& descriptorSetsInfo, const BalVulkan::CDevice* pDevice );
		void Cleanup() const;
		void Bind() const;
	private:
		BalVulkan::CDescriptorSet* m_descriptorSet;
		BalVulkan::CShaderPipeline* m_shaderPipeline;
		BalVulkan::CCommandPool* m_command;
	};
}
