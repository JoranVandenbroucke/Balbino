#pragma once
#include <filesystem>

namespace BalEditor
{
	bool ImportImage( const std::filesystem::path& path );
	bool ImportFont( const std::filesystem::path& path );
	bool ImportAudio( const std::filesystem::path& path );
	bool ImportMesh( const std::filesystem::path& path );
}
