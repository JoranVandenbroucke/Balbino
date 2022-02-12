#pragma once

#include <vulkan/vulkan.hpp>

namespace BalVulkan
{
	class CDevice;

	struct SPhysicalDeviceInfo
	{
		VkPhysicalDevice device;
		VkPhysicalDeviceProperties deviceProperties;
		VkPhysicalDeviceMemoryProperties deviceMemoryProperties;
		VkPhysicalDeviceFeatures deviceFeatures;
		std::vector<VkQueueFamilyProperties> queueFamilyProperties;
		std::array<VkFormatProperties, VK_FORMAT_ASTC_12x12_SRGB_BLOCK - VK_FORMAT_UNDEFINED + 1> formatProperties;
		VkFormat FindSupportedFormat( const std::vector<VkFormat>& candidates, const VkImageTiling& tiling, const VkFormatFeatureFlags& features ) const;
	};

	class CInstanceHolder
	{
	public:
		CInstanceHolder();
		CInstanceHolder( const CInstanceHolder& ) = delete;
		CInstanceHolder( CInstanceHolder&& ) = delete;
		CInstanceHolder& operator=( const CInstanceHolder& ) = delete;
		CInstanceHolder& operator=( CInstanceHolder&& ) = delete;
		~CInstanceHolder();

		const VkInstance& GetHandle() const;
	protected:
		VkInstance m_instanceHandle;
	};

	class CInstance : public CInstanceHolder
	{
	public:
		CInstance();
		CInstance( const CInstance& ) = delete;
		CInstance( CInstance&& ) = delete;
		CInstance& operator=( const CInstance& ) = delete;
		CInstance& operator=( CInstance&& ) = delete;
		~CInstance();

		bool Initialize( const char** extensions, uint32_t extensionsCount );
		void SetSurface( const VkSurfaceKHR& newSurface );
		uint8_t DeviceCount() const;
		CDevice* CreateDevice( uint32_t physicalDeviceIndex );
		uint32_t FindBestPhysicalDeviceIndex( const VkSurfaceKHR& surf );
		VkDebugReportCallbackEXT GetReportCallbackExt() const;
		VkSurfaceKHR GetSurface() const;
		static CInstance* CreateNew();

	private:
		VkDebugReportCallbackEXT m_debugReport;
		VkAllocationCallbacks* m_pCallbacks;
		VkSurfaceKHR m_surfaceKhr;

		std::vector<SPhysicalDeviceInfo> m_physicalDevices;

#ifdef _DEBUG
		PFN_vkCreateDebugReportCallbackEXT vkpfn_CreateDebugReportCallbackEXT = nullptr;
		PFN_vkDestroyDebugReportCallbackEXT vkpfn_DestroyDebugReportCallbackEXT = nullptr;
#endif
	};
}
