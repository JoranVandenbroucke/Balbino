//
// Created by joran on 10/09/2022.
//

#ifndef GAME_TEXTUREFILEIMPORTER_H
#define GAME_TEXTUREFILEIMPORTER_H

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
        
        bool IsVisible() const;
        
        bool DrawImportSettings();
        
        float GetImportPercentage();
    
    private:
        
        enum class ESampleLevel : uint8_t
        {
            One,
            Two,
            Four,
            Eight,
            Sixteen,
            ThirtyTwo,
            SixtyFour,
            Auto
        };
        
        enum class ETextureType : uint8_t
        {
            Default,
            Normal,
            SingleChannel
        };
        enum class ETextureShape : uint8_t
        {
            Texture2D,      //Import the texture as a 2D texture (default).
            TextureCube,    //Import the texture as a Cubemap.
            Texture2DArray,    //Import the texture as a 2D Array texture.
            Texture3D,        //Import the texture as a 3D texture.
        };
        
        bool m_isVisible;
        
        bool m_generateMips;
        bool m_generateNormalFromGray;  //only for normal map
        bool m_useSRGB;          //only for single channel textures
        
        int m_anisotropyLevel;
        
        FawnVision::EFilter::Enum             m_mipmapMode;
        FawnVision::EFilter::Enum             m_filterMode;
        FawnVision::ESamplerAddressMode::Enum m_wrapModeU;
        FawnVision::ESamplerAddressMode::Enum m_wrapModeV;
        FawnVision::ESamplerAddressMode::Enum m_wrapModeW;
        ESampleLevel                         m_sampleLevel;
        ETextureType                         m_type;
        ETextureShape                        m_shape;
        
        float m_importPercentage;
        
        std::filesystem::path m_path;
        std::string           m_destinationDirection;
        
        static const char* ToString( FawnVision::ESamplerAddressMode::Enum mode );
        static const char* ToString( FawnVision::EFilter::Enum mode );
        static const char* ToString( ESampleLevel mode );
        static const char* ToString( ETextureType mode );
        static const char* ToString( ETextureShape mode );
        
        bool LoadTexture();
        
        //based on https://wickedengine.net/2019/09/22/improved-normal-reconstruction-from-depth/
        bool GenerateNormalMap( const char* pIn, char*& pOut, int w, int h );
        static glm::vec3 ReconstructPosition( const glm::vec2& uv, float z, const glm::mat4x4& InvVP );
        
    };
} // FawnForge

#endif //GAME_TEXTUREFILEIMPORTER_H
