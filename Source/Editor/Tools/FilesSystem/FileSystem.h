#pragma once

#include <filesystem>

// todo expose for in code use

namespace FawnForge
{
    class CMeshFileImporter;
    class CTextureFileImporter;

    constexpr const char* g_supportedImageFormat[] { ".HDR", ".BMP", ".GIF", ".JPEG", ".JPG", ".LBM", ".PCX", ".PNG", ".PNM", ".SVG", ".TGA", ".TIFF", ".WEBP", ".XCF", ".XPM", ".XV" };
    constexpr const char* g_supportedAudioFormat[] { ".OGG", ".MP3", ".WAV", ".MID", ".FLAC", ".MOD" /*, ".OPUS"*/ };
    constexpr const char* g_supportedFontFormat[] { ".TTF", ".TTC", ".CFF", ".WOFF", ".OTF", ".OTC", ".PFB", ".PFA", ".CID", ".SFNT", ".PCF", ".FNT", ".BDF", ".PFR" };
    constexpr const char* g_supportedScriptFormat[] { ".C", ".CPP", ".CXX", ".H", ".HPP", ".HXX", ".GLSL", ".VERT", ".FRAG" };
    constexpr const char* g_supportedMeshFormat[] { ".FBX", ".DAE",     ".GLTF", ".GLB", ".BLEND", ".3DS", ".ASE", ".OBJ", ".IFC",  ".XGL", ".ZGL", ".PLY",  ".DXF", ".LWO", ".LWS", ".LXO", ".STL", ".X",   ".AC",  ".MS3D", ".COB", ".SCN", ".BVH", ".CSM",
                                                    ".XML", ".IRRMESH", ".IRR",  ".MDL", ".MD2",   ".MD3", ".PK3", ".MDC", ".MD5*", ".SMD", ".VTA", ".OGEX", ".3D",  ".B3D", ".Q3D", ".Q3S", ".NFF", ".NFF", ".OFF", ".RAW",  ".TER", ".MDL", ".HMP", ".NDO" };

    bool ImportFile( const char* pPath, const char* pDestinationDirection, CMeshFileImporter* pMeshImporter, CTextureFileImporter* pTextureImporter );
}// namespace FawnForge
