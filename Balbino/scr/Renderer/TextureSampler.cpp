#include "pch.h"
#include "TextureSampler.h"

#include "Texture.h"

Balbino::CTextureSampler::CTextureSampler() {}
Balbino::CTextureSampler::~CTextureSampler() {}
void Balbino::CTextureSampler::Initialize(const CTexture* pTexture, const VkDevice& device)
{
	const VkImageViewCreateInfo viewInfo{
		.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
		.image = pTexture->GetImage(),
		.viewType = VK_IMAGE_VIEW_TYPE_2D,
		.format = VK_FORMAT_R8G8B8A8_SRGB,
		.subresourceRange = {
			.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
			.baseMipLevel = 0,
			.levelCount = 1,
			.baseArrayLayer = 0,
			.layerCount = 1,
		},
	};

	if (vkCreateImageView(device, &viewInfo, nullptr, &m_textureImageView) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create texture image view!");
	}
}
void Balbino::CTextureSampler::Cleanup() {}
