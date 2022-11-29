//
// Created by joran on 20/08/2022.
//

#ifndef GAME_EXPORTER_H
#define GAME_EXPORTER_H

#include <string>
#include <unordered_map>
#include <Vertex.h>
#include <UUID.h>
#include <Shader.h>

namespace Balbino
{
    struct SMeshMetadata;
}

namespace BalEditor
{
    namespace Exporter
    {
        /*
        * Layout
        *
        * UUID
        * Type ID
        * Shader Combination Type
        * ...
        * Shader Length 1
        * Shader Length 2
        * ...
        * Shader Data 1
        * Shader Data 2
        * ...
        * Editor Data (not yet implemented)
        */
        bool ExportShader( const std::string& assetName, const std::string& assetPath, uint16_t type, const std::vector<std::vector<uint32_t>>& compiledShaders, CUuid id = CUuid{}, const std::string& editorData ={} );
        
        /*
        * Layout
        *
        * UUID
        * Type ID
        * Mesh Indices Size
        * Mesh Vertices Size
        * Indices Raw Data
        * Vertices Raw Data
        * Metadata data
        *
        * Editor Data (not yet implemented)
        */
        bool ExportMesh( const std::string& assetName, const std::string& assetPath, const std::vector<uint32_t>& indices, const std::vector<BalVulkan::SVertex>& vertices, const std::vector<Balbino::SMeshMetadata>& metadata, CUuid id = CUuid{} );
        
        /*
        * Layout
        *
        * UUID;
        * Type ID
        * Shader File UUID;
        * Shader Resources Size;
        * Shader Resources Data
        *
        * Editor Data (not yet implemented)
        */
        bool ExportMaterial( const std::string& assetName, const std::string& assetPath, CUuid shaderID, const std::vector<BalVulkan::SShaderResource>& resources, CUuid id = CUuid{} );
        
        /*
        * Layout
        *
        * UUID
        * Type ID
        * ImageType
        * Format
        * Mip Levels
        * Array Layers
        * Dimensions
        * Pitch
        * Image Data
        *
        * Editor Data (not yet implemented)
        */
        bool ExportImage( const std::string& assetName, const std::string& assetPath, uint8_t imageType, uint32_t imageFormat, uint8_t mips, uint8_t layers, uint32_t width, uint32_t height, uint32_t depth, uint8_t pitch, const void*const pData, int anisotropy, int sampleLevel, int mipmapMode, int filterMode, int wrapModeU, int wrapModeV, int wrapModeW, CUuid id = CUuid{} );
    }
}
#endif //GAME_EXPORTER_H
