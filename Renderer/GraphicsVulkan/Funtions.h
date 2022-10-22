#pragma once

#include <format>
#include <iostream>
#include <source_location>
#include <vulkan/vulkan.h>

namespace BalVulkan
{
    inline bool CheckVkResult( VkResult err, std::string_view failMessage = "", VkResult exclusions = VK_SUCCESS, const std::source_location location = std::source_location::current())
    {
        if ( err == VK_SUCCESS || ( err & exclusions ) != 0 )
        {
            return false;
        }
        //std::cout << "[Vulkan] Error: VkResult = " << err << std::endl << failMessage << std::endl;
        std::cout
                << std::format( R"({}({}:{})[Vulkan] Error: VkResult = {} {} )", location.file_name(), location.line(),
                                location.column(), location.function_name(), static_cast<uint32_t>( err ), failMessage )
                << std::endl;
        if ( err < 0 )
        {
            abort();
        }
        return true;
    }
    
    inline uint32_t FindMemoryType( const VkPhysicalDevice& physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties )
    {
        VkPhysicalDeviceMemoryProperties memProperties;
        vkGetPhysicalDeviceMemoryProperties( physicalDevice, &memProperties );
        
        for ( uint32_t i = 0; i < memProperties.memoryTypeCount; i++ )
        {
            if (( typeFilter & ( 1
                    << i )) && ( memProperties.memoryTypes[i].propertyFlags & properties ) == properties )
            {
                return i;
            }
        }
        
        throw std::runtime_error( "failed to find suitable memory type!" );
    }
}
