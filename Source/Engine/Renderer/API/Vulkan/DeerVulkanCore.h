//
// Created by joran on 28/07/2023.
//

#pragma once
#include "../../FawnVisionCore.h"
#include <Core.h>
#include <iostream>
#include <source_location>
#include <string_view>
#include <vulkan/vulkan.hpp>

namespace DeerVulkan
{
    /**
    * @brief Checks the Vulkan result value and logs a failure message if the result is not successful.
    *
    * @param err           The Vulkan result value to check.
    * @param failMessage   The failure message to log (optional).
    * @param exclusions    The Vulkan result values to exclude from logging failures (optional).
    * @param location      The source code location where this function is invoked (optional).
    **/
    constexpr bool CheckVkResult( VkResult err, std::string_view failMessage = "", VkResult exclusions = VK_SUCCESS, std::source_location location = std::source_location::current() )
    {
        if ( err == VK_SUCCESS || err == exclusions )
        {
            return true;
        }
        std::string_view file = location.file_name();
        std::string_view func = location.function_name();
        uint32_t line         = location.line();

        std::cerr << "Vulkan Error: " << static_cast<int>( err ) << " at " << file << ":" << line << " (" << func << ")\n";
        if ( !failMessage.empty() )
        {
#pragma todo("Use the logging library") // todo use the logging library
            std::cerr << "    Message: " << failMessage << "\n";
        }

        return false;
    }
    /**
    * @brief FindMemoryType function is used to find memory type with specified properties that supports the specified type filter.
    *
    * @param physicalDevice The VkPhysicalDevice object representing the physical device.
    * @param typeFilter The type filter used to specify the required memory type.
    * @param properties The memory properties flags specifying the required properties.
    * @return The memory type index that supports the specified type filter and properties.
    */
    inline uint32_t FindMemoryType( const VkPhysicalDevice& physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties )
    {
        VkPhysicalDeviceMemoryProperties memProperties;
        vkGetPhysicalDeviceMemoryProperties( physicalDevice, &memProperties );

        for ( uint32_t i = 0; i < memProperties.memoryTypeCount; ++i )
        {
            if ( ( typeFilter & ( 1 << i ) ) && ( memProperties.memoryTypes[ i ].propertyFlags & properties ) == properties )
            {
                return i;
            }
        }

        throw std::runtime_error( "Failed to find a suitable memory type!" );
    }
}// namespace DeerVulkan
namespace FawnVision
{

    /**
    * @brief Converts the image format of an image to the specified format.
    *
    * This function takes an image format enum as input and converts the image format of the specified image to the specified format.
    * The supported image formats are defined in the image_format enumeration.
    * If the conversion is successful, the image format of the image will be updated to the specified format.
    *
    * @param format The target image format to convert to.
    * @return uint32_t The corresponding VkFormat
    */
    constexpr uint32_t ConvertImageFormat( FawnVision::image_format format )
    {
        switch ( format )
        {
            case FawnVision::image_format::r16_g16_b16_a16: return VK_FORMAT_R16G16B16A16_SFLOAT;
            case FawnVision::image_format::r8_g8_b8_a8: return VK_FORMAT_R8G8B8A8_SRGB;
            case FawnVision::image_format::b8_g8_r8_a8: return VK_FORMAT_B8G8R8A8_SRGB;
            case FawnVision::image_format::r4_g4_b4_a4: return VK_FORMAT_R4G4B4A4_UNORM_PACK16;
            case FawnVision::image_format::r16_g16_b16: return VK_FORMAT_R16G16B16_SFLOAT;
            case FawnVision::image_format::r5_g5_b5_a1: return VK_FORMAT_R5G5B5A1_UNORM_PACK16;
            case FawnVision::image_format::r8_g8_b8: return VK_FORMAT_R8G8B8_SRGB;
            case FawnVision::image_format::r5_g6_b5: return VK_FORMAT_R5G6B5_UNORM_PACK16;
            case FawnVision::image_format::r32_g32: return VK_FORMAT_R32G32_SFLOAT;
            case FawnVision::image_format::r16_g16: return VK_FORMAT_R16G16_SFLOAT;
            case FawnVision::image_format::r8_g8: return VK_FORMAT_R8G8_SRGB;
            case FawnVision::image_format::r8: return VK_FORMAT_R8_SRGB;
            case FawnVision::image_format::r16: return VK_FORMAT_R16_SFLOAT;
            case FawnVision::image_format::a8: return VK_FORMAT_R8_SRGB;//todo: implement/show alpha only
            case FawnVision::image_format::d16_unorm: return VK_FORMAT_D16_UNORM;
            case FawnVision::image_format::d32_sfloat: return VK_FORMAT_D32_SFLOAT;
            case FawnVision::image_format::d16_unorm_s8_uint: return VK_FORMAT_D16_UNORM_S8_UINT;
            case FawnVision::image_format::d24_unorm_s8_uint: return VK_FORMAT_D24_UNORM_S8_UINT;
            case FawnVision::image_format::d32_sfloat_s8_uint: return VK_FORMAT_D32_SFLOAT_S8_UINT;
            case FawnVision::image_format::undefined:
            case FawnVision::image_format::max_enum: return VK_FORMAT_UNDEFINED;
        }
        return VK_FORMAT_UNDEFINED;
    }
    constexpr FawnVision::image_format ConvertImageFormat( uint32_t format )
    {
        switch ( (VkFormat)format )
        {
            case VK_FORMAT_R16G16B16A16_SFLOAT: return FawnVision::image_format::r16_g16_b16_a16;
            case VK_FORMAT_R8G8B8A8_SRGB: return FawnVision::image_format::r8_g8_b8_a8;
            case VK_FORMAT_B8G8R8A8_SRGB: return FawnVision::image_format::b8_g8_r8_a8;
            case VK_FORMAT_R4G4B4A4_UNORM_PACK16: return FawnVision::image_format::r4_g4_b4_a4;
            case VK_FORMAT_R16G16B16_SFLOAT: return FawnVision::image_format::r16_g16_b16;
            case VK_FORMAT_R5G5B5A1_UNORM_PACK16: return FawnVision::image_format::r5_g5_b5_a1;
            case VK_FORMAT_R8G8B8_SRGB: return FawnVision::image_format::r8_g8_b8;
            case VK_FORMAT_R5G6B5_UNORM_PACK16: return FawnVision::image_format::r5_g6_b5;
            case VK_FORMAT_R32G32_SFLOAT: return FawnVision::image_format::r32_g32;
            case VK_FORMAT_R16G16_SFLOAT: return FawnVision::image_format::r16_g16;
            case VK_FORMAT_R8G8_SRGB: return FawnVision::image_format::r8_g8;
            case VK_FORMAT_R8_SRGB: return FawnVision::image_format::r8;
            case VK_FORMAT_R16_SFLOAT: return FawnVision::image_format::r16;
            case VK_FORMAT_D16_UNORM: return FawnVision::image_format::d16_unorm;
            case VK_FORMAT_D32_SFLOAT: return FawnVision::image_format::d32_sfloat;
            case VK_FORMAT_D16_UNORM_S8_UINT: return FawnVision::image_format::d16_unorm_s8_uint;
            case VK_FORMAT_D24_UNORM_S8_UINT: return FawnVision::image_format::d24_unorm_s8_uint;
            case VK_FORMAT_D32_SFLOAT_S8_UINT: return FawnVision::image_format::d32_sfloat_s8_uint;
            case VK_FORMAT_UNDEFINED: return FawnVision::image_format::undefined;
            default: static_assert( "missed a format" ); assert( "missed a format" );
        }
        return FawnVision::image_format::undefined;
    }
    constexpr uint32_t ConvertImageFormat( uint8_t format )
    {
        return ConvertImageFormat( (FawnVision::image_format)format );
    }
    template<>
    inline VkVertexInputBindingDescription GetBindingDescription<FawnVision::SVertex>()
    {
        constexpr VkVertexInputBindingDescription bindingDescription {
            .binding   = 0,
            .stride    = sizeof( FawnVision::SVertex ),
            .inputRate = VK_VERTEX_INPUT_RATE_VERTEX,
        };
        return bindingDescription;
    }
    template<>
    inline VkVertexInputBindingDescription GetBindingDescription<FawnVision::SInstanceBatch>()
    {
        constexpr VkVertexInputBindingDescription bindingDescription {
            .binding   = 1,
            .stride    = sizeof( FawnVision::SVertex ),
            .inputRate = VK_VERTEX_INPUT_RATE_VERTEX,
        };
        return bindingDescription;
    }
    template<>
    inline std::vector<VkVertexInputAttributeDescription> GetAttributeDescriptions<FawnVision::SVertex>( uint32_t binding, FawnVision::vertex_component_flags components )
    {
        std::vector<VkVertexInputAttributeDescription> attributeDescriptions {};

        if ( components & FawnVision::vertex_component_flag_position )
            attributeDescriptions.emplace_back( 0, binding, VK_FORMAT_R32G32B32_SFLOAT, static_cast<uint32_t>( offsetof( FawnVision::SVertex, position ) ) );
        if ( components & FawnVision::vertex_component_flag_uv )
            attributeDescriptions.emplace_back( 1, binding, VK_FORMAT_R32G32_SFLOAT, static_cast<uint32_t>( offsetof( FawnVision::SVertex, uv ) ) );
        if ( components & FawnVision::vertex_component_flag_color )
            attributeDescriptions.emplace_back( 2, binding, VK_FORMAT_R32G32B32A32_SFLOAT, static_cast<uint32_t>( offsetof( FawnVision::SVertex, color ) ) );
        if ( components & FawnVision::vertex_component_flag_normal )
            attributeDescriptions.emplace_back( 3, binding, VK_FORMAT_R32G32B32_SFLOAT, static_cast<uint32_t>( offsetof( FawnVision::SVertex, normal ) ) );
        if ( components & FawnVision::vertex_component_flag_tangent )
            attributeDescriptions.emplace_back( 4, binding, VK_FORMAT_R32G32B32_SFLOAT, static_cast<uint32_t>( offsetof( FawnVision::SVertex, tangent ) ) );

        return attributeDescriptions;
    }
    template<>
    inline std::vector<VkVertexInputAttributeDescription> GetAttributeDescriptions<FawnVision::SInstanceBatch>()
    {
        std::vector<VkVertexInputAttributeDescription> attributeDescriptions {};
        uint32_t location { 5 };

        attributeDescriptions.emplace_back( location, 1, VK_FORMAT_R32G32B32A32_SFLOAT, 0u );
        ++location;
        attributeDescriptions.emplace_back( location, 1, VK_FORMAT_R32G32B32A32_SFLOAT, (uint32_t)sizeof( BambiMath::Vector4 ) );
        ++location;
        attributeDescriptions.emplace_back( location, 1, VK_FORMAT_R32G32B32A32_SFLOAT, (uint32_t)sizeof( BambiMath::Vector4 ) * 2u );
        ++location;
        attributeDescriptions.emplace_back( location, 1, VK_FORMAT_R32G32B32A32_SFLOAT, (uint32_t)sizeof( BambiMath::Vector4 ) * 3u );
        return attributeDescriptions;
    }
}// namespace FawnVision
