//
// Created by joran on 10/09/2022.
//

#ifndef GAME_TEXTUREFILEIMPORTER_H
#define GAME_TEXTUREFILEIMPORTER_H

#include <filesystem>
#include <Common.h>

namespace BalEditor
{
    class CTextureFileImporter
    {
    public:
        CTextureFileImporter() noexcept;

        ~CTextureFileImporter() = default;

        CTextureFileImporter( const CTextureFileImporter& ) = delete;

        CTextureFileImporter( CTextureFileImporter&& ) = delete;

        CTextureFileImporter& operator =( const CTextureFileImporter& ) = delete;

        CTextureFileImporter operator =( CTextureFileImporter&& ) = delete;
    
        void SetVisible( const std::filesystem::path& path, const std::string& destinationDirection = "../Data" );
    
        bool IsVisible() const;
    
        bool DrawImportSettings();
    
        float GetImportPercentage();

    private:
        struct ESampleLevel
        {
            enum Enum : uint8_t
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
        };
        bool m_isVisible;
    
        bool m_useMips;
        bool m_generateMips;
        bool m_useAnisotropy;
    
        int m_anisotropyLevel;
    
        BalVulkan::EFilter::Enum             m_mipmapMode;
        BalVulkan::EFilter::Enum             m_filterMode;
        BalVulkan::ESamplerAddressMode::Enum m_wrapModeU;
        BalVulkan::ESamplerAddressMode::Enum m_wrapModeV;
        BalVulkan::ESamplerAddressMode::Enum m_wrapModeW;
        ESampleLevel::Enum                   m_sampleLevel;
    
        float m_importPercentage;
    
        std::filesystem::path m_path;
        std::string           m_destinationDirection;
    
        static const char* ToString( BalVulkan::EFilter::Enum mode );
    
        static const char* ToString( ESampleLevel::Enum mode );
    
        static const char* ToString( BalVulkan::ESamplerAddressMode::Enum mode );
    
        bool LoadTexture();
    };
} // BalEditor

#endif //GAME_TEXTUREFILEIMPORTER_H
