#include "pch.h"
#include "Texture.h"
#include "../Managers/TextureManager.h"

#include  <SDL.h>

Balbino::CTexture::CTexture()
	: m_textureImage{m_textureImage}
	, m_textureImageMemory{m_textureImageMemory}
	, m_textureImageView{m_textureImageView} {}

Balbino::CTexture::~CTexture()
{
	if (m_textureImageMemory || m_textureImage || m_textureImageView)
		std::cerr << "didn't clear Texture" << std::endl;
}

void Balbino::CTexture::operator delete(void* ptr)
{
	if (ptr)
		::operator delete(ptr);
	ptr = nullptr;
}

void Balbino::CTexture::operator delete(void* ptr, const char* filePath)
{
	(void) filePath;
	operator delete(ptr);
}
void Balbino::CTexture::operator delete(void* ptr, int b, const char* f, int l, const char* filePath)
{
	(void) b;
	(void) f;
	(void) l;
	(void) filePath;
	operator delete(ptr);
}

void Balbino::CTexture::Initialize(SDL_Surface* pSurface, const VkDevice& device, const VkAllocationCallbacks* pCallbacks, const VkPhysicalDevice& physicalDevice, const VkQueue& queue, const VkCommandPool& commandPool)
{
	const uint32_t imageSize{static_cast<uint32_t>(pSurface->w * pSurface->h * 4)};

	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	CreateBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory, device, physicalDevice);

	void* data;
	vkMapMemory(device, stagingBufferMemory, 0, imageSize, 0, &data);
	memcpy(data, pSurface->pixels, static_cast<size_t>(imageSize));
	vkUnmapMemory(device, stagingBufferMemory);

	const VkImageCreateInfo imageInfo{
		.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
		.flags = 0, // Optional
		.imageType = VK_IMAGE_TYPE_2D,
		.format = VK_FORMAT_R8G8B8A8_SRGB,
		.extent = { static_cast<uint32_t>(pSurface->w), static_cast<uint32_t>(pSurface->w), 1 },
		.mipLevels = 1,
		.arrayLayers = 1,
		.samples = VK_SAMPLE_COUNT_1_BIT,
		.tiling = VK_IMAGE_TILING_OPTIMAL,
		.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
		.sharingMode = VK_SHARING_MODE_EXCLUSIVE,
		.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
	};
	if (vkCreateImage(device, &imageInfo, pCallbacks, &m_textureImage) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create image!");
	}

	VkMemoryRequirements memRequirements;
	vkGetImageMemoryRequirements(device, m_textureImage, &memRequirements);

	VkMemoryAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = FindMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, physicalDevice);

	if (vkAllocateMemory(device, &allocInfo, pCallbacks, &m_textureImageMemory) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to allocate image memory!");
	}

	vkBindImageMemory(device, m_textureImage, m_textureImageMemory, 0);
	TransitionImageLayout(m_textureImage, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, device, queue, commandPool);
	CopyBufferToImage(stagingBuffer, m_textureImage, static_cast<uint32_t>(pSurface->w), static_cast<uint32_t>(pSurface->h), device, queue, commandPool);
	TransitionImageLayout(m_textureImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, device, queue, commandPool);

	vkDestroyBuffer(device, stagingBuffer, nullptr);
	vkFreeMemory(device, stagingBufferMemory, nullptr);

	const VkImageViewCreateInfo viewInfo{
	.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
	.image = m_textureImage,
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

	if (vkCreateImageView(device, &viewInfo, pCallbacks, &m_textureImageView) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create texture image view!");
	}
}

void Balbino::CTexture::Cleanup(const VkDevice& device, const VkAllocationCallbacks* pCallbacks)
{
	vkDestroyImageView(device, m_textureImageView, pCallbacks);
	vkDestroyImage(device, m_textureImage, pCallbacks);
	vkFreeMemory(device, m_textureImageMemory, pCallbacks);
	m_textureImage = nullptr;
	m_textureImageMemory = nullptr;
	m_textureImageView = nullptr;
}

const VkImage& Balbino::CTexture::GetImage() const
{
	return m_textureImage;
}

const VkImageView& Balbino::CTexture::GetImageView() const
{
	return m_textureImageView;
}

void* Balbino::CTexture::operator new(size_t size, const char* filePath)
{
	if (CTexture* pTexture = static_cast<CTexture*>(::operator new(size)))
	{
		CTexture* pActualTexture = CTextureManager::AddTexture(pTexture, filePath);

		if (pTexture != pActualTexture)
		{
			delete pTexture;
			return pActualTexture;
		}
		return pTexture;
	}
	return nullptr;
}

void* Balbino::CTexture::operator new(size_t size, int b, const char* f, int l, const char* filePath)
{
	if (CTexture* pTexture = static_cast<CTexture*>(::operator new(size, b, f, l)))
	{
		CTexture* pActualTexture = CTextureManager::AddTexture(pTexture, filePath);

		if (pTexture != pActualTexture)
		{
			delete pTexture;
			return pActualTexture;
		}
		return pTexture;
	}
	return nullptr;
}

uint32_t Balbino::CTexture::FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties, const VkPhysicalDevice& physicalDevice)
{
	VkPhysicalDeviceMemoryProperties memProperties;
	vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

	for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
	{
		if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
		{
			return i;
		}
	}

	throw std::runtime_error("failed to find suitable memory type!");
}

void Balbino::CTexture::CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size, const VkDevice& device, const VkQueue& queue, const VkCommandPool& commandPool)
{
	const VkCommandBuffer& commandBuffer{BeginSingleTimeCommands(device, commandPool)};

	VkBufferCopy copyRegion{};
	copyRegion.size = size;
	vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

	EndSingleTimeCommands(commandBuffer, device, queue, commandPool);
}

VkCommandBuffer Balbino::CTexture::BeginSingleTimeCommands(const VkDevice& device, const VkCommandPool& commandPool)
{
	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandPool = commandPool;
	allocInfo.commandBufferCount = 1;

	VkCommandBuffer commandBuffer;
	vkAllocateCommandBuffers(device, &allocInfo, &commandBuffer);

	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	vkBeginCommandBuffer(commandBuffer, &beginInfo);

	return commandBuffer;
}

void Balbino::CTexture::EndSingleTimeCommands(const VkCommandBuffer& commandBuffer, const VkDevice& device, const VkQueue& queue, const VkCommandPool& commandPool)
{
	vkEndCommandBuffer(commandBuffer);

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffer;

	vkQueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE);
	vkQueueWaitIdle(queue);

	vkFreeCommandBuffers(device, commandPool, 1, &commandBuffer);
}

void Balbino::CTexture::TransitionImageLayout(const VkImage& image, const VkImageLayout& oldLayout, const VkImageLayout& newLayout, const VkDevice& device, const VkQueue& queue, const VkCommandPool& commandPool)
{
	const VkCommandBuffer& commandBuffer{BeginSingleTimeCommands(device, commandPool)};

	VkImageMemoryBarrier barrier{
		.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
		.oldLayout = oldLayout,
		.newLayout = newLayout,
		.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
		.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
		.image = image,
		.subresourceRange = {
			.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
			.baseMipLevel = 0,
			.levelCount = 1,
			.baseArrayLayer = 0,
			.layerCount = 1,},
	};
	VkPipelineStageFlags sourceStage;
	VkPipelineStageFlags destinationStage;

	if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
	{
		barrier.srcAccessMask = 0;
		barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

		sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
		destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
	}
	else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
	{
		barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

		sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
		destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
	}
	else
	{
		throw std::invalid_argument("unsupported layout transition!");
	}

	vkCmdPipelineBarrier(
		commandBuffer,
		sourceStage, destinationStage,
		0,
		0, nullptr,
		0, nullptr,
		1, &barrier
	);

	EndSingleTimeCommands(commandBuffer, device, queue, commandPool);
}

void Balbino::CTexture::CopyBufferToImage(const VkBuffer& buffer, const VkImage& image, const uint32_t width, const uint32_t height, const VkDevice& device, const VkQueue& queue, const VkCommandPool& commandPool)
{
	const VkCommandBuffer& commandBuffer{BeginSingleTimeCommands(device, commandPool)};

	const VkBufferImageCopy region{
		.bufferOffset = 0,
		.bufferRowLength = 0,
		.bufferImageHeight = 0,
		.imageSubresource = {
			.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
			.mipLevel = 0,
			.baseArrayLayer = 0,
			.layerCount = 1,
			},
		.imageOffset = {0, 0, 0},
		.imageExtent = { width, height, 1 },
	};
	vkCmdCopyBufferToImage(commandBuffer, buffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

	EndSingleTimeCommands(commandBuffer, device, queue, commandPool);
}

void Balbino::CTexture::CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory, const VkDevice& device, const VkPhysicalDevice& physicalDevice)
{
	VkBufferCreateInfo bufferInfo{};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size = size;
	bufferInfo.usage = usage;
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	if (vkCreateBuffer(device, &bufferInfo, nullptr, &buffer) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create buffer!");
	}

	VkMemoryRequirements memRequirements;
	vkGetBufferMemoryRequirements(device, buffer, &memRequirements);

	VkMemoryAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = FindMemoryType(memRequirements.memoryTypeBits, properties, physicalDevice);

	if (vkAllocateMemory(device, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to allocate buffer memory!");
	}

	vkBindBufferMemory(device, buffer, bufferMemory, 0);
}