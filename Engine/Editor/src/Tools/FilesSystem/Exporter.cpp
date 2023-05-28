//
// Created by joran on 20/08/2022.
//

#include "Exporter.h"
#include <filesystem>
#include <fstream>
#include <FileParcer.h>

bool FawnForge::Exporter::ExportShader( const std::string& assetName, const std::string& assetPath, const std::vector<std::vector<uint32_t>>& compiledShaders, int32_t types, int32_t cullmode, CUuid id, const std::string& editorData )
{
    std::filesystem::path path{ SanitizePath( assetPath ) + assetName };
    path.replace_extension( ".basset" );
    std::ofstream file{ path, std::ios::out | std::ios::binary };
    
    if ( !file.is_open())
    {
        return false;
    }
    
    Serialization::Write( file, (uint64_t) id );
    Serialization::Write( file, (uint8_t) file_type::file_type_shader );
    Serialization::Write( file, cullmode );
    Serialization::Write( file, types );
    Serialization::Write( file, (uint8_t) compiledShaders.size());
    for ( uint8_t i{}; i < (uint8_t) compiledShaders.size(); ++i )
    {
        Serialization::Write( file, (uint64_t) compiledShaders[i].size());
    }
    for ( uint8_t i{}; i < (uint8_t) compiledShaders.size(); ++i )
    {
        Serialization::Write(
                file, compiledShaders[i].data(), (uint64_t) compiledShaders[i].size() * sizeof( uint32_t ));
    }
    Serialization::Write( file, editorData );
    file.close();
    return true;
}

bool FawnForge::Exporter::ExportMesh( const std::string& assetName, const std::string& assetPath, const std::vector<uint32_t>& indices, const std::vector<FawnVision::SVertex>& vertices, const std::vector<SMeshMetadata>& pMetadata, CUuid id )
{
    std::filesystem::path path{ SanitizePath( assetPath ) + assetName };
    path.replace_extension( ".basset" );
    std::ofstream file{ path, std::ios::out | std::ios::binary };
    
    if ( !file.is_open())
    {
        return false;
    }
    
    Serialization::Write( file, (uint64_t) id );
    Serialization::Write( file, (uint8_t) file_type::file_type_model );
    Serialization::Write( file, (uint64_t) indices.size());
    Serialization::Write( file, (uint64_t) vertices.size());
    Serialization::Write( file, (uint64_t) pMetadata.size());
    Serialization::Write( file, indices.data(), sizeof( uint32_t ) * indices.size());
    Serialization::Write( file, vertices.data(), sizeof( FawnVision::SVertex ) * vertices.size());
    for ( const auto& metadata : pMetadata )
    {
        Serialization::Write( file, metadata );
    }
    
    file.close();
    return true;
}

bool FawnForge::Exporter::ExportMaterial( const std::string& assetName, const std::string& assetPath, CUuid shaderId, const std::vector<FawnVision::SShaderResource>& resources, CUuid id )
{
    std::filesystem::path path{ SanitizePath( assetPath ) + assetName };
    path.replace_extension( ".basset" );
    std::ofstream file{ path, std::ios::out | std::ios::binary };
    
    if ( !file.is_open())
    {
        return false;
    }
    
    Serialization::Write( file, (uint64_t) id );
    Serialization::Write( file, (uint8_t) file_type::file_type_material );
    Serialization::Write( file, (uint64_t) shaderId );
    Serialization::Write( file, resources.size());
    Serialization::Write( file, resources.data(), resources.size() * sizeof( FawnVision::SShaderResource ));
    
    file.close();
    return true;
}

bool FawnForge::Exporter::ExportImage( const std::string& assetName, const std::string& assetPath, uint8_t imageType, uint32_t imageFormat, uint8_t mips, uint8_t layers, uint32_t width, uint32_t height, uint32_t depth, uint8_t pitch, const void* const pData, int anisotropy, int sampleLevel, int mipmapMode, int filterMode, int wrapModeU, int wrapModeV, int wrapModeW, CUuid id )
{
    std::filesystem::path path{ SanitizePath( assetPath ) + assetName };
    path.replace_extension( ".basset" );
    std::ofstream file{ path, std::ios::out | std::ios::binary };
    
    if ( !file.is_open())
    {
        return false;
    }
    
    Serialization::Write( file, (uint64_t) id );
    Serialization::Write( file, (uint8_t) file_type::file_type_image );
    Serialization::Write( file, imageType );
    Serialization::Write( file, imageFormat );
    Serialization::Write( file, mips );
    Serialization::Write( file, layers );
    Serialization::Write( file, anisotropy );
    Serialization::Write( file, sampleLevel );
    Serialization::Write( file, mipmapMode );
    Serialization::Write( file, filterMode );
    Serialization::Write( file, wrapModeU );
    Serialization::Write( file, wrapModeV );
    Serialization::Write( file, wrapModeW );
    Serialization::Write( file, width );
    Serialization::Write( file, height );
    Serialization::Write( file, depth );
    Serialization::Write( file, pitch );
    Serialization::Write( file, pData, width * height * depth * pitch );
    
    file.close();
    return true;
}
