//
// Copyright (c) 2023.
// Author: Joran
//

module;
#include <bit>

#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>

#include "compiler.hpp"

#include <type_traits>
export module FawnVision.Window;
import FawnAlgebra.Arithmetics;
using namespace FawnAlgebra;

namespace FawnVision
{
#pragma region defenition
export struct SWindowCreateInfo;
export struct SWindow;

export enum class window_flags : uint32;

export constexpr window_flags operator|(window_flags lhs, window_flags rhs) noexcept;

export inline auto InitializeSDL() noexcept -> int32;

export inline auto CreateWindow(const SWindowCreateInfo& createInfo, SWindow& window) noexcept -> int32;
export inline void ReleaseWindow(const SWindow& window) noexcept;

export inline auto SetWindowFlags(SWindow& window, uint32 flags) noexcept -> int32;
export inline auto ToggleWindowFlags(SWindow& window, uint32 flags) noexcept -> int32;
export inline auto SetWindowPosition(const SWindow& window, int xPosition = SDL_WINDOWPOS_CENTERED, int yPosition = SDL_WINDOWPOS_CENTERED) noexcept -> int32;
export inline auto GetWindowPosition(const SWindow& window, int& xPosition, int& yPosition) noexcept -> int32;
export inline auto SetWindowSize(SWindow& window, int width, int height) noexcept -> int32;
export inline auto GetWindowSize(SWindow& window) noexcept -> int32;
export constexpr auto GetWindowSize(const SWindow& window, int& width, int& height) noexcept -> int32;
#pragma endregion

#pragma region implementation
struct SWindowCreateInfo
{
    const char8_t* const name{};
    uint8 screen{0};
    int32 width{0};
    int32 height{0};
    uint32 flags{0};
};
struct SWindow
{
    SDL_Window* pWindow{nullptr};
    int32 width{0};
    int32 height{0};
    uint32 flags{0};
    uint32 extensionCount{0};
    char const* const* extensions{nullptr};
};

enum class window_flags : uint32
{
    fullscreen         = 0x00000001U,
    openGL             = 0x00000002U,
    occluded           = 0x00000004U,
    hidden             = 0x00000008U,
    borderless         = 0x00000010U,
    resizable          = 0x00000020U,
    minimized          = 0x00000040U,
    maximized          = 0x00000080U,
    mouse_grabbed      = 0x00000100U,
    input_focus        = 0x00000200U,
    mouse_focus        = 0x00000400U,
    external           = 0x00000800U,
    high_pixel_density = 0x00002000U,
    mouseCapture       = 0x00004000U,
    always_on_top      = 0x00008000U,
    utility            = 0x00020000U,
    tooltip            = 0x00040000U,
    popup_menu         = 0x00080000U,
    keyboard_grabbed   = 0x00100000U,
    vulkan             = 0x10000000U,
    metal              = 0x20000000U,
    transparent        = 0x40000000U,
    not_focusable      = 0x80000000U,
};

BALBINO_CONSTEXPR_SINCE_CXX17 auto operator|(const window_flags lhs, const window_flags rhs) noexcept -> window_flags
{
    using UnderlyingType = std::underlying_type_t<window_flags>;
    return static_cast<window_flags>(static_cast<UnderlyingType>(lhs) | static_cast<UnderlyingType>(rhs));
}
inline auto InitializeSDL() noexcept -> int32
{
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        return -1;
    }
    return 0;
}
inline auto CreateWindow(const SWindowCreateInfo& createInfo, SWindow& window) noexcept -> int32
{
    int screenCount{};
    const SDL_DisplayID* const pDisplayList{SDL_GetDisplays(&screenCount)};
    const SDL_DisplayMode* pCurrent{nullptr};

    if (createInfo.screen == 0)
    {
        for (const SDL_DisplayID* pDisplayId{pDisplayList}; pDisplayId != nullptr; ++pDisplayId)
        {
            pCurrent = SDL_GetDesktopDisplayMode(*pDisplayId);
            if (pCurrent != nullptr)
            {
                break;
            }
        }
    }
    else
    {
        if (createInfo.screen >= screenCount)
        {
            return -1;
        }
        pCurrent = SDL_GetDesktopDisplayMode(createInfo.screen);
    }

    if (pCurrent == nullptr || createInfo.width > pCurrent->w || createInfo.height > pCurrent->h)
    {
        return -1;
    }

    SDL_Window* const pWindow{SDL_CreateWindow(std::bit_cast<const char*>(createInfo.name), createInfo.width, createInfo.height, createInfo.flags)};
    if (pWindow == nullptr)
    {
        return -1;
    }

    window            = SWindow{pWindow, createInfo.width, createInfo.height, createInfo.flags};
    window.extensions = SDL_Vulkan_GetInstanceExtensions(&window.extensionCount);
    if (window.extensions == nullptr)
    {
        return -1;
    }

    return 0;
}
inline void ReleaseWindow(const SWindow& window) noexcept
{
    SDL_DestroyWindow(window.pWindow);
}
inline auto SetWindowFlags(SWindow& window, const uint32 flags) noexcept -> int32
{
    if (!SDL_SetWindowBordered(window.pWindow, !(flags & SDL_WINDOW_BORDERLESS)) || !SDL_SetWindowResizable(window.pWindow, flags & SDL_WINDOW_RESIZABLE)
        || !SDL_SetWindowFullscreen(window.pWindow, flags & SDL_WINDOW_FULLSCREEN) || !SDL_SetWindowFocusable(window.pWindow, !(flags & SDL_WINDOW_NOT_FOCUSABLE)))
    {
        return -1;
    }

    if ((flags & SDL_WINDOW_MAXIMIZED) != 0U && !SDL_MaximizeWindow(window.pWindow))
    {
        return -1;
    }

    if ((flags & SDL_WINDOW_MINIMIZED) != 0U && !SDL_MinimizeWindow(window.pWindow))
    {
        return -1;
    }

    window.flags = flags;
    return 0;
}
inline auto ToggleWindowFlags(SWindow& window, const uint32 flags) noexcept -> int32
{
    return SetWindowFlags(window, window.flags ^ flags);
}
inline auto SetWindowPosition(const SWindow& window, const int xPosition, const int yPosition) noexcept -> int32
{
    SDL_SetWindowPosition(window.pWindow, xPosition, yPosition);
    return 0;
}
inline auto GetWindowPosition(const SWindow& window, int& xPosition, int& yPosition) noexcept -> int32
{
    SDL_GetWindowPosition(window.pWindow, &xPosition, &yPosition);
    return 0;
}
inline auto SetWindowSize(SWindow& window, const int width, const int height) noexcept -> int32
{
    window.width  = width;
    window.height = height;
    SDL_SetWindowSize(window.pWindow, width, height);
    SDL_SetWindowPosition(window.pWindow, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    return 0;
}
inline auto GetWindowSize(SWindow& window) noexcept -> int32
{
    SDL_GetWindowSize(window.pWindow, &window.width, &window.height);
    return 0;
}
BALBINO_CONSTEXPR_SINCE_CXX11 auto GetWindowSize(const SWindow& window, int& width, int& height) noexcept -> int32
{
    width  = window.width;
    height = window.height;
    return 0;
}
#pragma endregion
} // namespace FawnVision
