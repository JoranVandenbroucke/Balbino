#pragma once
#include "Base.h"
#include "Common.h"

#include <vulkan/vulkan.hpp>
namespace BalVulkan
{
	class CImageResource;
	class CShaderPipeline;
	class CDescriptorSet final : public CDeviceObject
	{
	public:
		explicit CDescriptorSet( const CDevice* const device );

		void Initialize( const CShaderPipeline* pShaderPipeline, const std::vector<SDescriptorSet>& descriptorSetsInfo );
		const VkDescriptorSet& GetDescriptorSet()const;

		static CDescriptorSet* CreateNew( const CDevice* pDevice );
	private:
		VkDescriptorSet m_descriptorSet;
		VkDescriptorPool m_descriptorPool;
	};

}
