//
// Copyright (c) 2024.
// Author: Joran
//

#pragma once
#include <cstdint>
#include <iostream>
#include <source_location>

#include <vulkan/vulkan.hpp>

namespace DeerVulkan
{
constexpr auto CheckVkResult(const VkResult err, const std::string_view failMessage = "", const VkResult exclusions = VK_SUCCESS, const std::source_location& location = std::source_location::current()) -> bool
{
    if (err == VK_SUCCESS || err == exclusions)
    {
        return true;
    }
    const std::string_view file = location.file_name();
    const std::string_view func = location.function_name();
    const uint32_t line = location.line();

    std::cerr << "Vulkan Error: " << err << " at " << file << ": " << line << " (" << func << ")" << '\n';
    if (!failMessage.empty())
    {
        // todo use the logging library
        std::cerr << "Message: " << failMessage << '\n';
    }
    std::cerr.flush();
    return false;
}
} // namespace DeerVulkan