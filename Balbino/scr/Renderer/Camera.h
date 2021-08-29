#pragma once
#include <vulkan/vulkan.hpp>
#define MAX_PRESENT_MODE_COUNT 3
#define PRESENT_MODE_MAILBOX_IMAGE_COUNT 3
#define PRESENT_MODE_DEFAULT_IMAGE_COUNT 3

namespace Balbino
{
	class CCamera
	{
	public:
		CCamera() = default;
		~CCamera() = default;
		CCamera( const CCamera& ) = delete;
		CCamera( CCamera&& ) = delete;
		CCamera& operator=( const CCamera& ) = delete;
		CCamera& operator=( CCamera&& ) = delete;

		void* operator new( size_t size );
		void* operator new( size_t size, int, const char*, int );
		void* operator new[]( size_t size ) = delete;

		void operator delete( void* ptr )noexcept;
		void operator delete[]( void* ptr ) = delete;

		void SetDrawIndex(int index);
		int GetDrawIndex() const;
		void Setup( const VkDevice& device, const VkPhysicalDevice& physicalDevice, const VkSurfaceKHR& surf, uint32_t minImageCount );
		void Destroy( const VkDevice& device ) const;

		const VkSwapchainKHR& GetSwapchainKhr() const
		{
			return m_swapchain;
		}
	private:
		int m_index;

		VkSwapchainKHR m_swapchain;
		VkSurfaceFormatKHR m_surfaceFormat;
		VkPresentModeKHR m_presentMode;
		VkExtent2D  m_swapchainExtent;
		VkSurfaceCapabilitiesKHR m_SurfaceCapabilities;
		VkImage m_Images[MAX_PRESENT_MODE_COUNT];
		
		uint32_t m_Width;
		uint32_t m_Height;
		uint32_t m_ImageCount;

		static void CheckVkResult(const VkResult err);
	};
}