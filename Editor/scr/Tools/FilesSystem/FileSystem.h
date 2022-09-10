#pragma once
#include <array>
#include <vector>
#include <Vertex.h>
#include <MeshMetadata.h>

#include "FileParcer.h"

namespace BalEditor
{
    class CMeshFileImporter;
    
	const std::array supportedImageFormat{ ".BMP", ".GIF", ".JPEG", ".JPG", ".LBM", ".PCX", ".PNG", ".PNM", ".SVG", ".TGA", ".TIFF", ".WEBP", ".XCF", ".XPM", ".XV" };
	const std::array supportedAudioFormat{ ".OGG", ".MP3", ".WAV", ".MID", ".FLAC", ".MOD"/*, ".OPUS"*/ };
	const std::array supportedFontFormat{ ".TTF", ".TTC", ".CFF", ".WOFF", ".OTF", ".OTC", ".PFB", ".PFA", ".CID", ".SFNT", ".PCF", ".FNT", ".BDF", ".PFR" };
	const std::array supportedScriptFormat{ ".C", ".CPP", ".CXX", ".H", ".HPP", ".HXX", ".GLSL", ".VERT", ".FRAG" };
	const std::array supportedMeshFormat{ ".FBX", ".DAE", ".GLTF", " .GLB", ".BLEND", ".3DS", ".ASE", ".OBJ", ".IFC", ".XGL", ".ZGL", ".PLY", ".DXF", ".LWO", ".LWS", ".LXO", ".STL", ".X", ".AC", ".MS3D", ".COB", ".SCN", ".BVH", ".CSM", ".XML", ".IRRMESH", ".IRR", ".MDL", ".MD2", ".MD3", ".PK3", ".MDC", ".MD5*", ".SMD", ".VTA", ".OGEX", ".3D", ".B3D", ".Q3D", ".Q3S", ".NFF", ".NFF", ".OFF", ".RAW", ".TER", ".MDL", ".HMP", ".NDO" };

	bool ImportFile( const char* pPath, const char* pDestinationDirection, BalEditor::CMeshFileImporter* pMeshImporter);
	SFile GetData( const std::filesystem::path& path );
}
