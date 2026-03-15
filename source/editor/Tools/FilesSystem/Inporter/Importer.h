#pragma once

#include <filesystem>

namespace FawnForge
{
    bool ImportFont( const std::filesystem::path& path, const char* pDestinationDirection = "../Data" );

    bool ImportAudio( const std::filesystem::path& path, const char* pDestinationDirection = "../Data" );
}// namespace FawnForge
