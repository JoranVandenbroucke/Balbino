#pragma once
#include <vulkan/vulkan.h>

namespace Balbino
{
	class CTextureSampler
	{
	public:
		CTextureSampler();
		~CTextureSampler();
		CTextureSampler(const CTextureSampler&) = delete;
		CTextureSampler(CTextureSampler&&) = delete;
		CTextureSampler& operator=(const CTextureSampler&) = delete;
		CTextureSampler& operator=(CTextureSampler&&) = delete;

		void Initialize( const VkDevice& device, const VkAllocationCallbacks* pCallbacks, uint32_t mipLevels );
		void Cleanup( const VkDevice& device, const VkAllocationCallbacks* pCallbacks );

		const VkDescriptorSetLayoutBinding& GetDescriptorLayoutBinding();
		const VkDescriptorPoolSize& GetDescriptorPoolSize(uint32_t size);
		const VkSampler& GetSampler();
	private:
		VkImageView m_textureImageView;
		VkSampler m_textureSampler;

		VkDescriptorSetLayoutBinding m_descriptorSetLayout;
		VkDescriptorPoolSize m_descriptorPoolSize;
	};
}
