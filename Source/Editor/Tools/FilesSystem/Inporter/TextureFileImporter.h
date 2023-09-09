//
// Created by joran on 10/09/2022.
//

#pragma once
#include <FawnVision.h>
#include <filesystem>

namespace FawnForge
{
    class CTextureFileImporter final
    {
    public:
        CTextureFileImporter()                                         = default;
        ~CTextureFileImporter()                                        = default;
        CTextureFileImporter( const CTextureFileImporter& )            = delete;
        CTextureFileImporter( CTextureFileImporter&& )                 = delete;
        CTextureFileImporter& operator=( const CTextureFileImporter& ) = delete;
        CTextureFileImporter operator=( CTextureFileImporter&& )       = delete;

        void SetVisible( const std::filesystem::path& path, const std::string_view destinationDirection = "../Data" );
        [[nodiscard]] bool IsVisible() const;
        bool DrawImportSettings() noexcept;

    private:
        enum class sample_level : uint8_t
        {
            one,
            two,
            four,
            eight,
            sixteen,
            thirty_two,
            sixty_four,
            auto_level
        };
        enum class texture_type : uint8_t
        {
            default_type,
            normal_type,
            single_channel_type
        };
        enum class texture_shape : uint8_t
        {
            shape1D,//Import the texture as a 1D texture (default).
            shape2D,//Import the texture as a 2D texture (default).
            shape3D,//Import the texture as a 3D texture.
        };

        FawnVision::STextureCreateInfo m_createInfo;
        std::filesystem::path m_path {};
        std::string m_destinationDirection {};
        FawnVision::SComponentMapping m_componentMapping {};
        float m_maxLod {};
        float m_minLod {};
        float m_mipLodBias {};
        int32_t m_anisotropyLevel { 1 };

        FawnVision::border_color m_borderColor { FawnVision::border_color::float_opaque_black };
        FawnVision::image_filter m_filterMode { FawnVision::image_filter::nearest };
        FawnVision::image_filter m_mipmapMode { FawnVision::image_filter::nearest };
        FawnVision::sampler_address_mode m_wrapModeU { FawnVision::sampler_address_mode::repeat };
        FawnVision::sampler_address_mode m_wrapModeV { FawnVision::sampler_address_mode::repeat };
        FawnVision::sampler_address_mode m_wrapModeW { FawnVision::sampler_address_mode::repeat };
        FawnVision::image_view_type m_type { FawnVision::image_view_type::view_2d };
        sample_level m_sampleLevel { sample_level::auto_level };
        texture_shape m_shape { texture_shape::shape2D };

        bool m_compareOperator {};
        bool m_compareEnable {};
        bool m_useMips { true };
        bool m_generateMips { true };
        bool m_generateNormal {};
        bool m_unNormalizedCoordinates {};
        bool m_useSrgb { true };
        bool m_isVisible {};

        static const char* ToString( FawnVision::sampler_address_mode mode );
        static const char* ToString( FawnVision::image_filter mode );
        static const char* ToString( sample_level mode );
        static const char* ToString( FawnVision::image_view_type mode );
        static const char* ToString( texture_shape mode );
        static const char* ToString( FawnVision::component_swizzle mode );
        static const char* ToString( FawnVision::border_color mode );

        bool LoadTexture() noexcept;
        // based on https://wickedengine.net/2019/09/22/improved-normal-reconstruction-from-depth/
        bool GenerateNormalMapFromRed( const char* pIn, char*& pOut, int w, int h );
        static BambiMath::Vector3 ReconstructPosition( const BambiMath::Vector2& uv, float z, const BambiMath::Matrix4x4& invVp );

        bool LoadTextureWithSDL();
        bool LoadTextureWithHDR();
        bool LoadTextureWithGLI();
    };
}// namespace FawnForge
