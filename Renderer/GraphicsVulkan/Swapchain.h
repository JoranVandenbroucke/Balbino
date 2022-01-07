#pragma once
#include "Base.h"
#include "Semaphore.h"

namespace BalVulkan
{
	class CImageResource;
	class CSemaphore;

	class CSwapchain final : public CDeviceObject
	{
	public:
		CSwapchain( const CDevice* device, const VkSurfaceKHR& surface );
		~CSwapchain() override;

		void Initialize( uint32_t width, uint32_t height );
		bool AcquireNextImage( CSemaphore* presentCompleteSemaphore, uint32_t* imageIndex ) const;
		VkSurfaceFormatKHR GetSurfaceFormat() const;
		VkExtent2D GetExtend()const;
		void GetImages( std::vector<CImageResource*>& swapChainImages, uint32_t& count) const;
		const VkSwapchainKHR& GetVkSwapchain() const;
		uint32_t GetMinImage()const;
		uint32_t GetImageCount() const;

		static CSwapchain* CreateNew( const CDevice* device, const VkSurfaceKHR& surface );
	private:
		std::vector<VkQueueFamilyProperties> m_queueFamilies;
		std::vector<VkSurfaceFormatKHR> m_surfaceFormats;
		std::vector<VkPresentModeKHR> m_presentModes;
		std::vector<VkImage> m_images;

		VkSurfaceCapabilitiesKHR m_surfaceCapabilities;
		VkSwapchainKHR m_swapchain;
		VkSurfaceKHR m_surfaceKhr;
		VkExtent2D m_swapchainExtent;
		uint32_t m_minImageCount;
		uint32_t m_imageCount;

		VkSurfaceFormatKHR m_swapSurfaceFormat;
		VkSwapchainCreateInfoKHR m_swapChainCreateInfo;
		VkPresentModeKHR m_presentMode;

		void GetSwapExtent(uint32_t w, uint32_t h);
		void GetSwapSurfaceFormat();
		void GetQueueFamiliesProperties();
	};
}

