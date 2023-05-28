//
// Created by joran on 10/09/2022.
//

#pragma once
#include <filesystem>
#include <Common.h>
#include "glm/glm.hpp"

namespace FawnForge
{
    class CTextureFileImporter
    {
    public:
        CTextureFileImporter() noexcept;
        ~CTextureFileImporter() = default;
        CTextureFileImporter( const CTextureFileImporter& ) = delete;
        CTextureFileImporter( CTextureFileImporter&& ) = delete;
        CTextureFileImporter& operator=( const CTextureFileImporter& ) = delete;
        CTextureFileImporter operator=( CTextureFileImporter&& ) = delete;
        
        void SetVisible( const std::filesystem::path& path, const std::string& destinationDirection = "../Data" );
        [[nodiscard]] bool IsVisible() const;
        bool DrawImportSettings();
        float GetImportPercentage();
    
    private:
        enum sample_level : uint8_t
        {
            sample_level_one,
            sample_level_two,
            sample_level_four,
            sample_level_eight,
            sample_level_sixteen,
            sample_level_thirty_two,
            sample_level_sixty_four,
            sample_level_auto
        };
        enum texture_type : uint8_t
        {
            texture_type_default,
            texture_type_normal,
            texture_type_single_channel
        };
        enum texture_shape : uint8_t
        {
            texture_shape_2D,      //Import the texture as a 2D texture (default).
            texture_shape_cube,    //Import the texture as a Cubemap.
            texture_shape_2D_array,    //Import the texture as a 2D Array texture.
            texture_shape_3D,        //Import the texture as a 3D texture.
        };
        
        bool m_isVisible;
        
        bool m_generateMips;
        bool m_generateNormalFromGray;  //only for normal map
        bool m_useSrgb;          //only for single channel textures
        
        int m_anisotropyLevel;
        
        FawnVision::EFilter::Enum             m_mipmapMode;
        FawnVision::EFilter::Enum             m_filterMode;
        FawnVision::ESamplerAddressMode::Enum m_wrapModeU;
        FawnVision::ESamplerAddressMode::Enum m_wrapModeV;
        FawnVision::ESamplerAddressMode::Enum m_wrapModeW;
        sample_level                          m_sampleLevel;
        texture_type                          m_type;
        texture_shape m_shape;
        
        float m_importPercentage;
        
        std::filesystem::path m_path;
        std::string           m_destinationDirection;
        
        static const char* ToString( FawnVision::ESamplerAddressMode::Enum mode );
        static const char* ToString( FawnVision::EFilter::Enum mode );
        static const char* ToString( sample_level mode );
        static const char* ToString( texture_type mode );
        static const char* ToString( texture_shape mode );
        
        bool LoadTexture();
        // based on https://wickedengine.net/2019/09/22/improved-normal-reconstruction-from-depth/
        bool GenerateNormalMap( const char* pIn, char*& pOut, int w, int h );
        static glm::vec3 ReconstructPosition( const glm::vec2& uv, float z, const glm::mat4x4& invVp );
        
    };
} // FawnForge


