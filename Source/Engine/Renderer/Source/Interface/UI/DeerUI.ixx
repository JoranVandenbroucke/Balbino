//
// Copyright (c) 2024.
// Author: Joran.
//

module;
#include <compiler.hpp>
#include <enum_helpers.hpp>

#include <functional>
#include <vector>

#include "API/Vulkan/DeerVulkan.hpp"

export module DeerUI;
import FawnAlgebra.Arithmetics;
import FawnVision;
using namespace FawnAlgebra;

namespace DeerUI
{
export using CanvasHandle    = uint16;
export using UIElementHandle = uint16;

constexpr uint32 UI_INSTANCE_COUNT{64U};
const std::vector<uint8> g_uiVert{
#if defined(BALBINO_EMBED_HAS_BUILTIN_EMBED) && (BALBINO_EMBED_HAS_BUILTIN_EMBED != 0)
#    embed "Embedded/SPV/ui.vert.spv"
#else
    0x03, 0x02, 0x23, 0x07, 0x00, 0x00, 0x01, 0x00, 0x0b, 0x00, 0x08, 0x00, 0x26, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x11, 0x00, 0x02, 0x00, 0x01, 0x00, 0x00, 0x00, 0x0b, 0x00, 0x06, 0x00, 0x01, 0x00, 0x00, 0x00, 0x47, 0x4c,
    0x53, 0x4c, 0x2e, 0x73, 0x74, 0x64, 0x2e, 0x34, 0x35, 0x30, 0x00, 0x00, 0x00, 0x00, 0x0e, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x0d, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00,
    0x6d, 0x61, 0x69, 0x6e, 0x00, 0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00, 0x0b, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x11, 0x00, 0x00, 0x00, 0x15, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x1a, 0x00, 0x00, 0x00, 0x1d, 0x00,
    0x00, 0x00, 0x03, 0x00, 0x03, 0x00, 0x02, 0x00, 0x00, 0x00, 0xc2, 0x01, 0x00, 0x00, 0x05, 0x00, 0x04, 0x00, 0x04, 0x00, 0x00, 0x00, 0x6d, 0x61, 0x69, 0x6e, 0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0x04, 0x00, 0x09, 0x00, 0x00, 0x00,
    0x6f, 0x75, 0x74, 0x55, 0x56, 0x00, 0x00, 0x00, 0x05, 0x00, 0x03, 0x00, 0x0b, 0x00, 0x00, 0x00, 0x61, 0x55, 0x56, 0x00, 0x05, 0x00, 0x05, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x6f, 0x75, 0x74, 0x43, 0x6f, 0x6c, 0x6f, 0x72, 0x00, 0x00,
    0x00, 0x00, 0x05, 0x00, 0x04, 0x00, 0x11, 0x00, 0x00, 0x00, 0x61, 0x43, 0x6f, 0x6c, 0x6f, 0x72, 0x00, 0x00, 0x05, 0x00, 0x06, 0x00, 0x13, 0x00, 0x00, 0x00, 0x67, 0x6c, 0x5f, 0x50, 0x65, 0x72, 0x56, 0x65, 0x72, 0x74, 0x65, 0x78,
    0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x06, 0x00, 0x13, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x67, 0x6c, 0x5f, 0x50, 0x6f, 0x73, 0x69, 0x74, 0x69, 0x6f, 0x6e, 0x00, 0x05, 0x00, 0x03, 0x00, 0x15, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x05, 0x00, 0x04, 0x00, 0x18, 0x00, 0x00, 0x00, 0x61, 0x50, 0x6f, 0x73, 0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0x04, 0x00, 0x1a, 0x00, 0x00, 0x00, 0x75, 0x53, 0x63, 0x61, 0x6c, 0x65, 0x00, 0x00, 0x05, 0x00, 0x05, 0x00,
    0x1d, 0x00, 0x00, 0x00, 0x75, 0x54, 0x72, 0x61, 0x6e, 0x73, 0x6c, 0x61, 0x74, 0x65, 0x00, 0x00, 0x47, 0x00, 0x04, 0x00, 0x09, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x47, 0x00, 0x04, 0x00, 0x0b, 0x00,
    0x00, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x47, 0x00, 0x04, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x47, 0x00, 0x04, 0x00, 0x11, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x00,
    0x04, 0x00, 0x00, 0x00, 0x48, 0x00, 0x05, 0x00, 0x13, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0b, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x47, 0x00, 0x03, 0x00, 0x13, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x47, 0x00,
    0x04, 0x00, 0x18, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x47, 0x00, 0x04, 0x00, 0x1a, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x47, 0x00, 0x04, 0x00, 0x1d, 0x00, 0x00, 0x00,
    0x1e, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x13, 0x00, 0x02, 0x00, 0x02, 0x00, 0x00, 0x00, 0x21, 0x00, 0x03, 0x00, 0x03, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x16, 0x00, 0x03, 0x00, 0x06, 0x00, 0x00, 0x00, 0x20, 0x00,
    0x00, 0x00, 0x17, 0x00, 0x04, 0x00, 0x07, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x20, 0x00, 0x04, 0x00, 0x08, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x3b, 0x00, 0x04, 0x00,
    0x08, 0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x20, 0x00, 0x04, 0x00, 0x0a, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x3b, 0x00, 0x04, 0x00, 0x0a, 0x00, 0x00, 0x00, 0x0b, 0x00,
    0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x17, 0x00, 0x04, 0x00, 0x0d, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x20, 0x00, 0x04, 0x00, 0x0e, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x0d, 0x00, 0x00, 0x00,
    0x3b, 0x00, 0x04, 0x00, 0x0e, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x20, 0x00, 0x04, 0x00, 0x10, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x0d, 0x00, 0x00, 0x00, 0x3b, 0x00, 0x04, 0x00, 0x10, 0x00,
    0x00, 0x00, 0x11, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x03, 0x00, 0x13, 0x00, 0x00, 0x00, 0x0d, 0x00, 0x00, 0x00, 0x20, 0x00, 0x04, 0x00, 0x14, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x13, 0x00, 0x00, 0x00,
    0x3b, 0x00, 0x04, 0x00, 0x14, 0x00, 0x00, 0x00, 0x15, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x15, 0x00, 0x04, 0x00, 0x16, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x2b, 0x00, 0x04, 0x00, 0x16, 0x00,
    0x00, 0x00, 0x17, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3b, 0x00, 0x04, 0x00, 0x0a, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x3b, 0x00, 0x04, 0x00, 0x0a, 0x00, 0x00, 0x00, 0x1a, 0x00, 0x00, 0x00,
    0x01, 0x00, 0x00, 0x00, 0x3b, 0x00, 0x04, 0x00, 0x0a, 0x00, 0x00, 0x00, 0x1d, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x2b, 0x00, 0x04, 0x00, 0x06, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2b, 0x00,
    0x04, 0x00, 0x06, 0x00, 0x00, 0x00, 0x21, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x3f, 0x36, 0x00, 0x05, 0x00, 0x02, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0xf8, 0x00, 0x02, 0x00,
    0x05, 0x00, 0x00, 0x00, 0x3d, 0x00, 0x04, 0x00, 0x07, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x0b, 0x00, 0x00, 0x00, 0x3e, 0x00, 0x03, 0x00, 0x09, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x3d, 0x00, 0x04, 0x00, 0x0d, 0x00,
    0x00, 0x00, 0x12, 0x00, 0x00, 0x00, 0x11, 0x00, 0x00, 0x00, 0x3e, 0x00, 0x03, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x12, 0x00, 0x00, 0x00, 0x3d, 0x00, 0x04, 0x00, 0x07, 0x00, 0x00, 0x00, 0x19, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00,
    0x3d, 0x00, 0x04, 0x00, 0x07, 0x00, 0x00, 0x00, 0x1b, 0x00, 0x00, 0x00, 0x1a, 0x00, 0x00, 0x00, 0x85, 0x00, 0x05, 0x00, 0x07, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x19, 0x00, 0x00, 0x00, 0x1b, 0x00, 0x00, 0x00, 0x3d, 0x00,
    0x04, 0x00, 0x07, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x1d, 0x00, 0x00, 0x00, 0x81, 0x00, 0x05, 0x00, 0x07, 0x00, 0x00, 0x00, 0x1f, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x51, 0x00, 0x05, 0x00,
    0x06, 0x00, 0x00, 0x00, 0x22, 0x00, 0x00, 0x00, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x51, 0x00, 0x05, 0x00, 0x06, 0x00, 0x00, 0x00, 0x23, 0x00, 0x00, 0x00, 0x1f, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x50, 0x00,
    0x07, 0x00, 0x0d, 0x00, 0x00, 0x00, 0x24, 0x00, 0x00, 0x00, 0x22, 0x00, 0x00, 0x00, 0x23, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x21, 0x00, 0x00, 0x00, 0x41, 0x00, 0x05, 0x00, 0x0e, 0x00, 0x00, 0x00, 0x25, 0x00, 0x00, 0x00,
    0x15, 0x00, 0x00, 0x00, 0x17, 0x00, 0x00, 0x00, 0x3e, 0x00, 0x03, 0x00, 0x25, 0x00, 0x00, 0x00, 0x24, 0x00, 0x00, 0x00, 0xfd, 0x00, 0x01, 0x00, 0x38, 0x00, 0x01, 0x00
#endif
};
const std::vector<uint8> g_uiFrag{
#if defined(BALBINO_EMBED_HAS_BUILTIN_EMBED) && (BALBINO_EMBED_HAS_BUILTIN_EMBED != 0)
#    embed "Embedded/SPV/ui.frag.spv"
#else
    0x03, 0x02, 0x23, 0x07, 0x00, 0x00, 0x01, 0x00, 0x0b, 0x00, 0x08, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x11, 0x00, 0x02, 0x00, 0x01, 0x00, 0x00, 0x00, 0x0b, 0x00, 0x06, 0x00, 0x01, 0x00, 0x00, 0x00, 0x47, 0x4c, 0x53,
    0x4c, 0x2e, 0x73, 0x74, 0x64, 0x2e, 0x34, 0x35, 0x30, 0x00, 0x00, 0x00, 0x00, 0x0e, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x08, 0x00, 0x04, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x6d, 0x61,
    0x69, 0x6e, 0x00, 0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00, 0x0b, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x10, 0x00, 0x03, 0x00, 0x04, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x03, 0x00, 0x03, 0x00, 0x02, 0x00, 0x00, 0x00, 0xc2,
    0x01, 0x00, 0x00, 0x05, 0x00, 0x04, 0x00, 0x04, 0x00, 0x00, 0x00, 0x6d, 0x61, 0x69, 0x6e, 0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0x05, 0x00, 0x09, 0x00, 0x00, 0x00, 0x6f, 0x75, 0x74, 0x43, 0x6f, 0x6c, 0x6f, 0x72, 0x00, 0x00, 0x00, 0x00,
    0x05, 0x00, 0x04, 0x00, 0x0b, 0x00, 0x00, 0x00, 0x69, 0x6e, 0x43, 0x6f, 0x6c, 0x6f, 0x72, 0x00, 0x05, 0x00, 0x04, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x69, 0x6e, 0x55, 0x56, 0x00, 0x00, 0x00, 0x00, 0x47, 0x00, 0x04, 0x00, 0x09, 0x00, 0x00,
    0x00, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x47, 0x00, 0x04, 0x00, 0x0b, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x47, 0x00, 0x04, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x13, 0x00, 0x02, 0x00, 0x02, 0x00, 0x00, 0x00, 0x21, 0x00, 0x03, 0x00, 0x03, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x16, 0x00, 0x03, 0x00, 0x06, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x17, 0x00, 0x04, 0x00, 0x07,
    0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x20, 0x00, 0x04, 0x00, 0x08, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x3b, 0x00, 0x04, 0x00, 0x08, 0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00,
    0x03, 0x00, 0x00, 0x00, 0x20, 0x00, 0x04, 0x00, 0x0a, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x3b, 0x00, 0x04, 0x00, 0x0a, 0x00, 0x00, 0x00, 0x0b, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x17, 0x00, 0x04,
    0x00, 0x0d, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x20, 0x00, 0x04, 0x00, 0x0e, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x0d, 0x00, 0x00, 0x00, 0x3b, 0x00, 0x04, 0x00, 0x0e, 0x00, 0x00, 0x00, 0x0f, 0x00,
    0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x36, 0x00, 0x05, 0x00, 0x02, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0xf8, 0x00, 0x02, 0x00, 0x05, 0x00, 0x00, 0x00, 0x3d, 0x00, 0x04, 0x00, 0x07,
    0x00, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x0b, 0x00, 0x00, 0x00, 0x3d, 0x00, 0x04, 0x00, 0x0d, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x3d, 0x00, 0x04, 0x00, 0x0d, 0x00, 0x00, 0x00, 0x11, 0x00, 0x00, 0x00,
    0x0f, 0x00, 0x00, 0x00, 0x51, 0x00, 0x05, 0x00, 0x06, 0x00, 0x00, 0x00, 0x12, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x51, 0x00, 0x05, 0x00, 0x06, 0x00, 0x00, 0x00, 0x13, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00,
    0x00, 0x01, 0x00, 0x00, 0x00, 0x51, 0x00, 0x05, 0x00, 0x06, 0x00, 0x00, 0x00, 0x14, 0x00, 0x00, 0x00, 0x11, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x51, 0x00, 0x05, 0x00, 0x06, 0x00, 0x00, 0x00, 0x15, 0x00, 0x00, 0x00, 0x11, 0x00,
    0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x50, 0x00, 0x07, 0x00, 0x07, 0x00, 0x00, 0x00, 0x16, 0x00, 0x00, 0x00, 0x12, 0x00, 0x00, 0x00, 0x13, 0x00, 0x00, 0x00, 0x14, 0x00, 0x00, 0x00, 0x15, 0x00, 0x00, 0x00, 0x85, 0x00, 0x05, 0x00, 0x07,
    0x00, 0x00, 0x00, 0x17, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x16, 0x00, 0x00, 0x00, 0x3e, 0x00, 0x03, 0x00, 0x09, 0x00, 0x00, 0x00, 0x17, 0x00, 0x00, 0x00, 0xfd, 0x00, 0x01, 0x00, 0x38, 0x00, 0x01, 0x00
#endif
};

export enum class canvas_flag : uint16 {
    none                 = 0,
    fullscreen           = 1 << 0,
    hidden               = 1 << 1,
    menu_bar             = 1 << 3,
    background           = 1 << 4,
    resizable            = 1 << 3,
    movable              = 1 << 4,
    accepts_inputs       = 1 << 5,
    modal                = 1 << 6,
    always_on_top        = 1 << 7,
    focus_on_appearing   = 1 << 8,
    vertical_scrollbar   = 1 << 9,
    horizontal_scrollbar = 1 << 10,
    max
};
} // namespace DeerUI
ENABLE_ENUM_BITWISE_OPERATIONS(DeerUI::canvas_flag);
namespace DeerUI
{
struct SVertex
{
    float2 position{};
    float2 uv{};
};

struct SUiInstanceData
{
    float2 scale{1.0f};
    float2 transform{0.0f};
    float4 colour{1.0f};
};

struct PassData
{
    FawnVision::SShader shader;
    FawnVision::STexture renderTarget;
    FawnVision::SMesh mesh;
    FawnVision::SBuffer instanceBuffer;
};

export struct LayoutProperties
{
    ushort2 padding{};
    ushort2 margin{};
    ushort2 size{};
    uchar2 border{};
    uchar2 flex{};
};

struct InteractionCallback
{
    std::function<void()> callable;

    explicit InteractionCallback(std::function<void()>&& func) noexcept
        : callable(std::move(func))
    {
    }

    void operator()() const noexcept
    {
        callable();
    }
};

struct Interactions
{
    std::vector<InteractionCallback> onHover, onPress, onRelease, onDrag, onDoubleClick;

    template <typename Callable>
    constexpr void AddSubscriber(std::vector<InteractionCallback>& subscribers, Callable&& callback) noexcept
    {
        subscribers.emplace_back(std::forward<Callable>(callback));
    }

    constexpr void AddOnHoverSubscriber(auto&& callback) noexcept
    {
        AddSubscriber(onHover, std::forward<decltype(callback)>(callback));
    }
    constexpr void AddOnPressSubscriber(auto&& callback) noexcept
    {
        AddSubscriber(onPress, std::forward<decltype(callback)>(callback));
    }
    constexpr void AddOnReleaseSubscriber(auto&& callback) noexcept
    {
        AddSubscriber(onRelease, std::forward<decltype(callback)>(callback));
    }
    constexpr void AddOnDragSubscriber(auto&& callback) noexcept
    {
        AddSubscriber(onDrag, std::forward<decltype(callback)>(callback));
    }
    constexpr void AddOnDoubleClickSubscriber(auto&& callback) noexcept
    {
        AddSubscriber(onDoubleClick, std::forward<decltype(callback)>(callback));
    }
};

struct UIElement
{
    Interactions interactions;
    LayoutProperties layoutProperties;
    ushort4 boundingBox;
};

struct Canvas
{
    std::vector<UIElement> elements;
    LayoutProperties layoutProperties;
    ushort4 boundingBox;
    uint8 layer;
    canvas_flag flags;
};

struct CanvasManager
{
    std::vector<Canvas> canvases;
    static constexpr ushort2 maxDimensions{1920, 1080};
};

export struct SUIRenderer
{
    CanvasManager canvasManager;
    PassData* renderData{};
    bool dirty;
};

constexpr std::array quad{SVertex{float2{0.0f, 0.0f}, float2{0.0f, 0.0f}}, SVertex{float2{0.0f, 1.0f}, float2{1.0f, 0.0f}}, SVertex{float2{1.0f, 1.0f}, float2{1.0f, 1.0f}}, SVertex{float2{1.0f, 0.0f}, float2{0.0f, 1.0f}}};
constexpr std::array indices{0U, 1U, 2U, 2U, 3U, 0U};

constexpr ushort4 CalculateBoundingBox(const ushort2& canvasSize, const LayoutProperties& previousLayout, const ushort4& previousBoundingBox, const LayoutProperties& layout)
{
    ushort4 boundingBox{ 0, 0, layout.size.x(), layout.size.y()};
    if (!boundingBox.z())
        boundingBox.z() = canvasSize.x() - (layout.margin.x() + layout.padding.x()) * 2;
    if (!boundingBox.w())
        boundingBox.w() = canvasSize.y() - (layout.margin.y() + layout.padding.y()) * 2;

    boundingBox.x() = previousLayout.margin.x() + previousLayout.padding.x() + layout.margin.x() + layout.padding.x();
    boundingBox.y() = previousBoundingBox.y() + previousBoundingBox.w() + previousLayout.margin.y() + previousLayout.padding.y() + layout.margin.y() + layout.padding.y();

    return boundingBox;
}

export constexpr void CreateCanvas(SUIRenderer& renderer, const ushort2& position, const ushort2& size, const canvas_flag flags, CanvasHandle& canvasHandle)
{
    CanvasManager& canvasManager{renderer.canvasManager};
    const ushort2 actualSize{(size.x() != 0U) ? size.x() : static_cast<uint16>(std::max(0, canvasManager.maxDimensions.x() - position.x())),
        (size.y() != 0U) ? size.y() : static_cast<uint16>(std::max(0, canvasManager.maxDimensions.y() - position.y()))};
    canvasManager.canvases.push_back(Canvas{{}, LayoutProperties{ushort2{0}, ushort2(0), actualSize, uchar2(0), uchar2(1)}, ushort4{position.x(), position.y(), size.x(), size.y()}, 0, flags});
    canvasHandle = static_cast<CanvasHandle>(canvasManager.canvases.size() - 1);
}

export constexpr void AddElement(SUIRenderer& renderer, const CanvasHandle handle, const LayoutProperties& layout, UIElementHandle& elementHandle)
{
    auto& [canvases]{renderer.canvasManager};
    auto& [elements, layoutProperties, boundingBox, layer, flags]{canvases[handle]};
    const LayoutProperties& previousLayout{elements.empty() ? layoutProperties : elements.back().layoutProperties};
    const ushort4& previousBounding{elements.empty() ? ushort4{boundingBox.x(), boundingBox.y(), 0, 0} : elements.back().boundingBox};
    elements.emplace_back(UIElement{{}, layout, CalculateBoundingBox(layoutProperties.size, previousLayout, previousBounding, layout)});
    elementHandle = static_cast<UIElementHandle>(elements.size() - 1);
}

template <typename Callable>
constexpr void SubscribeOnHover(UIElement& element, Callable&& callback) noexcept
{
    element.interactions.AddOnHoverSubscriber(std::forward<Callable>(callback));
}
template <typename Callable>
constexpr void SubscribeOnPress(UIElement& element, Callable&& callback) noexcept
{
    element.interactions.AddOnPressSubscriber(std::forward<Callable>(callback));
}

inline void Render(SUIRenderer& uiRenderer, const PassData* pPassData, const FawnVision::SRenderPassContext& renderContext)
{
    BindShader(renderContext, pPassData->shader);

    SetViewport(renderContext, 0.0f, 0.0f, static_cast<float>(renderContext.swapChain.extent.width), static_cast<float>(renderContext.swapChain.extent.height));
    SetScissor(renderContext, renderContext.swapChain.extent.width, renderContext.swapChain.extent.height, 0, 0);

    SetAlphaToCoverageEnable(renderContext, false);

    SetColorBlendEnable(renderContext, true);
    SetColorBlendEquation(renderContext, FawnVision::blend_factor::src_alpha, FawnVision::blend_factor::one_minus_src_alpha, FawnVision::blend_operator::add, FawnVision::blend_factor::one, FawnVision::blend_factor::one_minus_src_alpha,
                          FawnVision::blend_operator::add);
    SetColorWriteMask(renderContext, static_cast<FawnVision::color_component>(15));

    SetPolygonMode(renderContext, FawnVision::polygon_mode::fill);
    SetCullMode(renderContext, FawnVision::cull_mode::none);
    SetFrontFace(renderContext, false);
    SetLineWidth(renderContext, 1.0f);

    SetDepthBiasEnable(renderContext, false);
    SetDepthCompareOperator(renderContext, FawnVision::compare_operator::less_or_equal);
    SetDepthTestEnable(renderContext, false);
    SetDepthWriteEnable(renderContext, false);

    SetPrimitiveRestartEnable(renderContext, false);
    SetPrimitiveTopology(renderContext, FawnVision::primitive_topology::triangle_list);

    SetRasterizationSamples(renderContext, 1U);
    SetRasterizerDiscardEnable(renderContext, false);

    SetStencilTestEnable(renderContext, false);
    constexpr std::array attributes{FawnVision::SVertexAttributes{
                                        .location   = 0,
                                        .binding    = 0,
                                        .format     = FawnVision::format::r32g32_sfloat,
                                        .offset     = offsetof(SVertex, position),
                                        .isInstance = false,
                                    },
                                    FawnVision::SVertexAttributes{
                                        .location   = 1,
                                        .binding    = 0,
                                        .format     = FawnVision::format::r32g32_sfloat,
                                        .offset     = offsetof(SVertex, uv),
                                        .isInstance = false,
                                    },
                                    FawnVision::SVertexAttributes{
                                        .location   = 2,
                                        .binding    = 1,
                                        .format     = FawnVision::format::r32g32_sfloat,
                                        .offset     = offsetof(SUiInstanceData, scale),
                                        .isInstance = true,
                                    },
                                    FawnVision::SVertexAttributes{
                                        .location   = 3,
                                        .binding    = 1,
                                        .format     = FawnVision::format::r32g32_sfloat,
                                        .offset     = offsetof(SUiInstanceData, transform),
                                        .isInstance = true,
                                    },
                                    FawnVision::SVertexAttributes{
                                        .location   = 4,
                                        .binding    = 1,
                                        .format     = FawnVision::format::r32g32b32a32_sfloat,
                                        .offset     = offsetof(SUiInstanceData, colour),
                                        .isInstance = true,
                                    }};
    SetVertexInput<std::array<FawnVision::SVertexAttributes, std::size(attributes)>>(renderContext, attributes, sizeof(SVertex), sizeof(SUiInstanceData));
    BindMesh(renderContext, pPassData->mesh);
    std::vector uiElements = uiRenderer.canvasManager.canvases;
    std::ranges::sort(uiElements,
                      [](const Canvas& a, const Canvas& b)
                      {
                          return a.layer < b.layer;
                      });

    // Rendering logic
    const float sx{2.0f / static_cast<float>(renderContext.swapChain.extent.width)};
    const float sy{2.0f / static_cast<float>(renderContext.swapChain.extent.height)};
    std::vector<SUiInstanceData> instanceDatas;
    for (const auto& [elements, canvasLayout, canvasBox, layer, flags] : uiElements)
    {
        if ((flags & canvas_flag::hidden) == canvas_flag::hidden)
            continue;
        for (const auto& [interactions, layoutProperties, boundingBox] : elements)
        {
            instanceDatas.emplace_back(float2{sx * boundingBox.z(), sy * boundingBox.w()}, float2{-1 + boundingBox.x() * sx, -1 + boundingBox.y() * sy}, float4{1.f, 1.f, 1.f, 1.f});
        }
    }

    for (std::size_t i{}; i < instanceDatas.size(); i += UI_INSTANCE_COUNT)
    {
        const uint32 size{std::min(UI_INSTANCE_COUNT, static_cast<uint32>(instanceDatas.size() - i))};
        CopyData(renderContext, pPassData->instanceBuffer, size * sizeof(SUiInstanceData), std::data(instanceDatas));
        BindInstanceBufferBuffer(renderContext.commandBuffer, pPassData->instanceBuffer.buffer);
        Draw(renderContext, static_cast<uint32>(indices.size()), size, 0, 0);
    }
    uiRenderer.dirty = false;
}

export inline int32 Initialize(const FawnVision::SRenderer& renderer, FawnVision::SRenderGraph& renderGraph, SUIRenderer& uiRenderer)
{
    FawnVision::IRenderPassHandle pass{FawnVision::AddRasterRenderPass<PassData>(renderGraph, uiRenderer.renderData)};

    if (const FawnVision::ShaderCreateInfo shaderCreateInfo{{FawnVision::shader_stage::vertex, g_uiVert}, {FawnVision::shader_stage::fragment, g_uiFrag}}; CreateShader(renderer, shaderCreateInfo, uiRenderer.renderData->shader) != 0)
    {
        return -1;
    }
    // if (Initialize(renderer,
    //                FawnVision::SRenderTextureCreateInfo{
    //                    .format = FawnVision::format::r8g8b8a8_srgb,
    //                    .aspect = FawnVision::image_aspect::color,
    //                    .width  = renderer.swapChain.extent.width,
    //                    .height = renderer.swapChain.extent.height,
    //                },
    //                uiRenderer.renderData->renderTarget)
    //     != 0)
    // {
    //     return -1;
    // }
    if (Initialize<SVertex, std::array<uint32, indices.size()>, std::array<SVertex, quad.size()>>(renderer, uiRenderer.renderData->mesh, indices, quad) != 0)
    {
        return -1;
    }
    if (Initialize(renderer, uiRenderer.renderData->instanceBuffer, UI_INSTANCE_COUNT * sizeof(SUiInstanceData), static_cast<uint32>(FawnVision::buffer_usage::vertex_buffer) | static_cast<uint32>(FawnVision::buffer_usage::transfer_dst),
                   static_cast<uint32>(FawnVision::memory_property::host_visible) | static_cast<uint32>(FawnVision::memory_property::host_coherent))
        != 0)
    {
        return -1;
    }

    // SetRenderColorTarget(renderGraph, pass, uiRenderer.renderData->renderTarget);
    FawnVision::SetRenderFunc<PassData>(renderGraph, pass,
                                        [&uiRenderer](const PassData* pPassData, const FawnVision::SRenderPassContext& renderContext)
                                        {
                                            Render(uiRenderer, pPassData, renderContext);
                                        });
    return 0;
}
export inline void Cleanup(const FawnVision::SRenderer& renderer, SUIRenderer& uiRenderer)
{
    Cleanup(renderer, uiRenderer.renderData->renderTarget);
    Cleanup(renderer, uiRenderer.renderData->instanceBuffer);
    Cleanup(renderer, uiRenderer.renderData->mesh);
    Cleanup(renderer, uiRenderer.renderData->shader);
    uiRenderer.renderData = nullptr;
    for (auto& [elements, layoutProperties, boundingBox, layer, flags] : uiRenderer.canvasManager.canvases)
    {
        elements.clear();
    }
    uiRenderer.canvasManager.canvases.clear();
}
} // namespace DeerUI
