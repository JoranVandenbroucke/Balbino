//
// Created by joran on 20/08/2022.
//

#pragma once
#include <UUID.h>
#include <FawnVision.h>
#include <string_view>
#include <vector>

// todo expose for in code use

namespace FawnForge::Exporter
{
    bool ExportShader( std::string_view assetName, std::string_view assetPath, const std::vector<FawnVision::SShaderCreateInfo>& shaderInfo, int cullMode, const CUuid& id = CUuid {}, const std::string& editorData = {} );

    bool ExportMesh( std::string_view assetName, std::string_view assetPath, const FawnVision::SMeshCreateInfo& meshInfo, const CUuid& id = CUuid {}, const std::string& editorData = {} );

    bool ExportMaterial( std::string_view assetName, std::string_view assetPath, const FawnVision::SMaterialCreateInfo& materialInfo, const CUuid& id = CUuid {}, const std::string& editorData = {} );

    bool ExportImage( std::string_view assetName, std::string_view assetPath, const FawnVision::STextureCreateInfo& imageInfo, const CUuid& id = CUuid {}, const std::string& editorData = {} );
}// namespace FawnForge::Exporter
