#pragma once
#include <string>
#include <vulkan/vulkan.h>

struct SDL_Surface;
namespace Balbino
{
	class CTexture
	{
	public:
		CTexture();
		~CTexture();
		CTexture(const CTexture&) = delete;
		CTexture(CTexture&&) = delete;
		CTexture& operator=(const CTexture&) = delete;
		CTexture& operator=(CTexture&&) = delete;

		void* operator new(size_t size, const char* filePath);
		void* operator new(size_t size, int b, const char* f, int l, const char* filePath);
		void* operator new(size_t size) = delete;
		void* operator new[](size_t size) = delete;
		
		void operator delete(void* ptr);
		void operator delete(void* ptr, const char* filePath);
		void operator delete(void* ptr, int b, const char* f, int l, const char* filePath);
		void operator delete[](void* ptr) = delete;

		void Initialize(SDL_Surface* pSurface, const VkDevice& device, const VkAllocationCallbacks* pCallbacks, const VkPhysicalDevice& physicalDevice, const VkQueue& queue, const VkCommandPool& commandPool);
		void Cleanup(const VkDevice& device, const VkAllocationCallbacks* pCallbacks);

		const VkImage& GetImage()const;
		const VkImageView& GetImageView()const;
	private:
		VkImage m_textureImage;
		VkDeviceMemory m_textureImageMemory;
		VkImageView m_textureImageView;

		uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties, const VkPhysicalDevice& physicalDevice);
		void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size, const VkDevice& device, const VkQueue& queue, const VkCommandPool& commandPool);
		VkCommandBuffer BeginSingleTimeCommands(const VkDevice& device, const VkCommandPool& commandPool);
		void EndSingleTimeCommands(const VkCommandBuffer& commandBuffer, const VkDevice& device, const VkQueue& queue, const VkCommandPool& commandPool);
		void TransitionImageLayout(const VkImage& image, const VkImageLayout& oldLayout, const VkImageLayout& newLayout, const VkDevice& device, const VkQueue& queue, const VkCommandPool& commandPool);
		void CopyBufferToImage( const VkBuffer& buffer, const VkImage& image, const uint32_t width, const uint32_t height, const VkDevice& device, const VkQueue& queue, const VkCommandPool& commandPool );
		void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory, const VkDevice& device, const VkPhysicalDevice& physicalDevice);
	};
}
