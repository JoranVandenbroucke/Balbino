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
        bool m_isVisible;

        bool m_useMips;
        bool m_generateMips;
        bool m_useAnisotropy;

        int m_anisotropyLevel;

        BalVulkan::EFilter             m_mipmapMode;
        BalVulkan::EFilter             m_filterMode;
        BalVulkan::ESamplerAddressMode m_wrapModeU;
        BalVulkan::ESamplerAddressMode m_wrapModeV;
        BalVulkan::ESamplerAddressMode m_wrapModeW;
        ESampleLevel                   m_sampleLevel;

        float m_importPercentage;

        std::filesystem::path m_path;
        std::string           m_destinationDirection;

        static const char* ToString( BalVulkan::EFilter mode );

        static const char* ToString( ESampleLevel mode );

        static const char* ToString( BalVulkan::ESamplerAddressMode mode );

        bool LoadTexture();
    };
} // BalEditor

#endif //GAME_TEXTUREFILEIMPORTER_H
