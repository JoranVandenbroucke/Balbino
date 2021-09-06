#pragma once
#include <vulkan/vulkan.h>

namespace Balbino
{
	class CTexture;
	class CTextureSampler
	{
	public:
		CTextureSampler();
		~CTextureSampler();
		CTextureSampler(const CTextureSampler&) = delete;
		CTextureSampler(CTextureSampler&&) = delete;
		CTextureSampler& operator=(const CTextureSampler&) = delete;
		CTextureSampler& operator=(CTextureSampler&&) = delete;

		void Initialize(const CTexture* pTexture, const VkDevice& device);
		void Cleanup();
	private:
		VkImageView m_textureImageView;
	};
}
