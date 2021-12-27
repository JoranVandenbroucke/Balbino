#pragma once
#include <array>
#include <vector>

#include "FileParcer.h"

namespace BalEditor
{

	const std::array supportedImageFormat{ ".BMP", ".GIF", ".JPEG", ".JPG", ".LBM", ".PCX", ".PNG", ".PNM", ".SVG", ".TGA", ".TIFF", ".WEBP", ".XCF", ".XPM", ".XV" };
	const std::array supportedAudioFormat{ ".OGG", ".MP3", ".WAV", ".MID", ".FLAC", ".MOD"/*, ".OPUS"*/ };
	const std::array supportedFontFormat{ ".TTF", ".TTC", ".CFF", ".WOFF", ".OTF", ".OTC", ".PFB", ".PFA", ".CID", ".SFNT", ".PCF", ".FNT", ".BDF", ".PFR" };
	const std::array supportedScriptFormat{ ".C", ".CPP", ".CXX", ".H", ".HPP", ".HXX", ".GLSL", ".VERT", ".FRAG" };
	const std::array supportedMeshFormat{ ".fbx", ".dae", ".gltf", " .glb", ".blend", ".3ds", ".ase", ".obj", ".ifc", ".xgl", ".zgl", ".ply", ".dxf", ".lwo", ".lws", ".lxo", ".stl", ".x", ".ac", ".ms3d", ".cob", ".scn", ".bvh", ".csm", ".xml", ".irrmesh", ".irr", ".mdl", ".md2", ".md3", ".pk3", ".mdc", ".md5*", ".smd", ".vta", ".ogex", ".3d", ".b3d", ".q3d", ".q3s", ".nff", ".nff", ".off", ".raw", ".ter", ".mdl", ".hmp", ".ndo" };

	bool ImportFile( const char* pPath );
	std::vector<SFile> GetFilesInPath( const std::filesystem::path& path );

}
