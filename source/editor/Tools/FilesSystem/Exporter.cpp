//
// Created by joran on 20/08/2022.
//

#include "Exporter.h"
#include <Serializer.h>
#include <filesystem>
#include <fstream>

bool FawnForge::Exporter::ExportShader( std::string_view assetName, std::string_view assetPath, const std::vector<FawnVision::SShaderCreateInfo>& shaderInfo, int cullMode, const CUuid& id, const std::string& editorData )
{
    std::filesystem::path path { SanitizePath( assetPath.data() ) + assetName.data() };
    path.replace_extension( ".basset" );
    std::ofstream file { path, std::ios::out | std::ios::binary };

    if ( !file.is_open() )
    {
        return false;
    }

    Serialization::Write( file, id.Data() );
    Serialization::Write( file, (uint8_t)file_type::shader );
    Serialization::Write( file, shaderInfo );
    Serialization::Write( file, cullMode );
    Serialization::Write( file, editorData );
    file.close();
    return true;
}

bool FawnForge::Exporter::ExportMesh( std::string_view assetName, std::string_view assetPath, const FawnVision::SMeshCreateInfo& meshInfo, const CUuid& id, const std::string& editorData )
{
    std::filesystem::path path { SanitizePath( assetPath.data() ) + assetName.data() };
    path.replace_extension( ".basset" );
    std::ofstream file { path, std::ios::out | std::ios::binary };

    if ( !file.is_open() )
    {
        return false;
    }

    Serialization::Write( file, id.Data() );
    Serialization::Write( file, (uint8_t)file_type::model );
    Serialization::Write( file, meshInfo );
    Serialization::Write( file, editorData );

    file.close();
    return true;
}

bool FawnForge::Exporter::ExportMaterial( std::string_view assetName, std::string_view assetPath, const FawnVision::SMaterialCreateInfo& materialInfo, const CUuid& id, const std::string& editorData )
{
    std::filesystem::path path { SanitizePath( assetPath.data() ) + assetName.data() };
    path.replace_extension( ".basset" );
    std::ofstream file { path, std::ios::out | std::ios::binary };

    if ( !file.is_open() )
    {
        return false;
    }

    Serialization::Write( file, id.Data() );
    Serialization::Write( file, (uint8_t)file_type::material );
    Serialization::Write( file, materialInfo );
    Serialization::Write( file, editorData );

    file.close();
    return true;
}

bool FawnForge::Exporter::ExportImage( std::string_view assetName, std::string_view assetPath, const FawnVision::STextureCreateInfo& imageInfo, const CUuid& id, const std::string& editorData )
{
    std::filesystem::path path { SanitizePath( assetPath.data() ) + assetName.data() };
    path.replace_extension( ".basset" );
    std::ofstream file { path, std::ios::out | std::ios::binary };

    if ( !file.is_open() )
    {
        return false;
    }

    Serialization::Write( file, id.Data() );
    Serialization::Write( file, (uint8_t)file_type::image );
    Serialization::Write( file, imageInfo );
    Serialization::Write( file, editorData );

    file.close();
    return true;
}
