// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

#pragma comment(lib, "vulkan-1.lib")
//#pragma comment(lib, "spirv-cross-c-shared.lib")
#pragma comment(lib, "shaderc_shared.lib")

//#include <vulkan/vulkan.h>
#define NOMINMAX
#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <format>
#include <iostream>
#include <unordered_map>
#include <vector>

#include <glm.hpp>

#endif //PCH_H
