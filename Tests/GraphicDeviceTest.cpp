//
// Created by joran on 03/08/2023.
//
#include "AntlerTestCore.h"

// Enable the WSI extensions
#if defined( __ANDROID__ )
#define VK_USE_PLATFORM_ANDROID_KHR
#elif defined( __linux__ )
#define VK_USE_PLATFORM_XLIB_KHR
#elif defined( _WIN32 )
#define VK_USE_PLATFORM_WIN32_KHR
#endif

// Tell SDL not to mess with main()
#define SDL_MAIN_HANDLED
#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>

#include <API/Vulkan/Instance.h>
#include <Device.h>
#include <FawnVision.h>

FawnVision::Instance g_instance { BALBINO_NULL };
FawnVision::Device g_device { BALBINO_NULL };
FawnVision::Queue g_queue { BALBINO_NULL };
FawnVision::Swapchain g_swapchain { BALBINO_NULL };

START( FawnVision )
{
    // Create an SDL window that supports Vulkan rendering.
    if ( SDL_Init( SDL_INIT_VIDEO ) != 0 )
    {
        throw "Could not initialize SDL.";
    }
    SDL_Window* window = SDL_CreateWindow( "Vulkan Window", 1280, 720, SDL_WINDOW_VULKAN );
    if ( window == NULL )
    {
        throw "Could not create SDL window.";
    }

    // Get WSI extensions from SDL (we can add more if we like - we just can't remove these)
    unsigned extension_count;
    if ( !SDL_Vulkan_GetInstanceExtensions( &extension_count, NULL ) )
    {
        throw "Could not get the number of required instance extensions from SDL.";
    }
    std::vector<const char*> extensions( extension_count );
    if ( !SDL_Vulkan_GetInstanceExtensions( &extension_count, extensions.data() ) )
    {
        throw "Could not get the names of required instance extensions from SDL.";
    }

    g_instance = FawnVision::CreateInstance( (const char**)extensions.data(), extension_count );// Create a Vulkan surface for rendering
    VkSurfaceKHR c_surface;
    if ( !SDL_Vulkan_CreateSurface( window, static_cast<VkInstance>( g_instance->GetHandle() ), &c_surface ) )
    {
        throw "Could not create a Vulkan surface.";
    }
    g_instance->SetSurface( c_surface );
    g_instance->InitializeDevice( &g_device, g_instance->FindBestPhysicalDeviceIndex( c_surface ) );
}

STOP( FawnVision )
{
    FawnVision::FreeSwapchain( g_swapchain );
    FawnVision::FreeQueue( g_queue );
    FawnVision::FreeDevice( g_device );
    FawnVision::FreeInstance( g_instance );
}

TEST_CASE( "CreateInstance returns a valid instance" )
{
    std::vector<const char*> enabledExtensions = { "VK_KHR_surface" };
    FawnVision::Instance pInstance { FawnVision::CreateInstance( (const char**)enabledExtensions.data(), (uint32_t)enabledExtensions.size() ) };
    REQUIRE( pInstance != nullptr );
    FawnVision::FreeInstance( pInstance );
    REQUIRE( pInstance == nullptr );
}

TEST_CASE( "FindSupportedFormat returns correct format" )
{
    const FawnVision::SPhysicalDeviceInfo& info { g_device->GetPhysicalDeviceInfo() };

    // Candidates and features for the test
    std::vector<FawnVision::image_format> candidates = { FawnVision::image_format::d32_sfloat_s8_uint, FawnVision::image_format::d32_sfloat };
    VkImageTiling tiling                             = VK_IMAGE_TILING_OPTIMAL;
    VkFormatFeatureFlags features                    = VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT;
    // Call the function and check the result
    VkFormat result = info.FindSupportedFormat( candidates, tiling, features );
    REQUIRE( result == VK_FORMAT_D32_SFLOAT_S8_UINT );// Modify the expected result as per your implementation
}

TEST_CASE( "GetDepthFormat returns a valid format", "[GetDepthFormat]" )
{
    const FawnVision::SPhysicalDeviceInfo& info { g_device->GetPhysicalDeviceInfo() };

    // Call the function and check the result
    VkFormat result = info.GetDepthFormat();
    bool possibleFormat { ( result == VK_FORMAT_D32_SFLOAT_S8_UINT ) || ( result == VK_FORMAT_D32_SFLOAT ) || ( result == VK_FORMAT_D24_UNORM_S8_UINT ) || ( result == VK_FORMAT_D16_UNORM_S8_UINT ) || ( result == VK_FORMAT_D16_UNORM ) };
    REQUIRE( possibleFormat );// Modify the expected results as per your implementation
}

TEST_CASE( "GetMaxUsableSampleCount returns a valid sample count" )
{
    const FawnVision::SPhysicalDeviceInfo& info { g_device->GetPhysicalDeviceInfo() };

    // Call the function and check the result
    VkSampleCountFlagBits result = info.GetMaxUsableSampleCount();
    bool possibleCount           = ( result == VK_SAMPLE_COUNT_64_BIT || result == VK_SAMPLE_COUNT_32_BIT || result == VK_SAMPLE_COUNT_16_BIT || result == VK_SAMPLE_COUNT_8_BIT || result == VK_SAMPLE_COUNT_4_BIT || result == VK_SAMPLE_COUNT_2_BIT
                           || result == VK_SAMPLE_COUNT_1_BIT );// Modify the expected results as per your implementation
    REQUIRE( possibleCount );
}