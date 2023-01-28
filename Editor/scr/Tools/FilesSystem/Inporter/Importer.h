#pragma once

#include <filesystem>

namespace BalEditor
{
    bool ImportFont( const std::filesystem::path& path, const char* pDestinationDirection = "../Data" );
    
    bool ImportAudio( const std::filesystem::path& path, const char* pDestinationDirection = "../Data" );
}
