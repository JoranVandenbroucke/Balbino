//
// Created by joran on 10/09/2022.
//

#include "TextureFileImporter.h"

#include <hdrloader.h>
#include "../../../Renderer/EditorGui.h"
#include "../Exporter.h"
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#include <algorithm>
#include <array>

#pragma warning( push )
#pragma warning( disable : 4100 )
#pragma warning( disable : 4201 )
#pragma warning( disable : 4244 )
#pragma warning( disable : 4458 )
#pragma warning( disable : 4723 )
#pragma warning( disable : 5054 )

#include <gli/gli.hpp>

#pragma warning( pop )

namespace FawnForge
{

    void CTextureFileImporter::SetVisible( const std::filesystem::path& path, const std::string_view destinationDirection )
    {
        m_isVisible            = true;
        m_path                 = path;
        m_destinationDirection = destinationDirection;
    }

    bool CTextureFileImporter::IsVisible() const
    {
        return m_isVisible;
    }

    bool CTextureFileImporter::DrawImportSettings() noexcept
    {
        if ( Gui::StartPopup( "Import Texture", true, { 500, -1 } ) )
        {
            uint64_t mipmapMode { (uint64_t)m_mipmapMode };
            uint64_t filterMode { (uint64_t)m_filterMode };
            uint64_t wrapU { (uint64_t)m_wrapModeU };
            uint64_t wrapV { (uint64_t)m_wrapModeV };
            uint64_t wrapW { (uint64_t)m_wrapModeW };
            uint64_t sampleLevel { (uint64_t)m_sampleLevel };
            uint64_t textureType { (uint64_t)m_type };
            uint64_t textureShape { (uint64_t)m_shape };
            uint64_t componentMapR { (uint64_t)m_componentMapping.r };
            uint64_t componentMapG { (uint64_t)m_componentMapping.g };
            uint64_t componentMapB { (uint64_t)m_componentMapping.b };
            uint64_t componentMapA { (uint64_t)m_componentMapping.a };
            uint64_t borderColor { (uint64_t)m_borderColor };

            const std::vector<std::string> textureTypes { ToString( FawnVision::image_view_type::view_1d ),         ToString( FawnVision::image_view_type::view_1d_array ), ToString( FawnVision::image_view_type::view_2d ),
                                                          ToString( FawnVision::image_view_type::view_2d_array ),   ToString( FawnVision::image_view_type::view_3d ),       ToString( FawnVision::image_view_type::view_rect ),
                                                          ToString( FawnVision::image_view_type::view_rect_array ), ToString( FawnVision::image_view_type::view_cube ),     ToString( FawnVision::image_view_type::view_cube_array ) };
            const std::vector<std::string> textureShapes { ToString( texture_shape::shape1D ), ToString( texture_shape::shape2D ), ToString( texture_shape::shape3D ) };
            const std::vector<std::string> wrapModes { ToString( FawnVision::sampler_address_mode::repeat ), ToString( FawnVision::sampler_address_mode::mirrored_repeat ), ToString( FawnVision::sampler_address_mode::clamp_to_edge ),
                                                       ToString( FawnVision::sampler_address_mode::clamp_to_border ), ToString( FawnVision::sampler_address_mode::mirror_clamp_to_edge ) };
            const std::vector<std::string> sampleModes { ToString( FawnVision::image_filter::nearest ), ToString( FawnVision::image_filter::linear ) };
            const std::vector<std::string> sampleLevels { ToString( sample_level::one ),     ToString( sample_level::two ),        ToString( sample_level::four ),       ToString( sample_level::eight ),
                                                          ToString( sample_level::sixteen ), ToString( sample_level::thirty_two ), ToString( sample_level::sixty_four ), ToString( sample_level::auto_level ) };
            const std::vector<std::string> componentMapping { ToString( FawnVision::component_swizzle::swizzle_identity ), ToString( FawnVision::component_swizzle::swizzle_zero ), ToString( FawnVision::component_swizzle::swizzle_one ),
                                                              ToString( FawnVision::component_swizzle::swizzle_r ),        ToString( FawnVision::component_swizzle::swizzle_g ),    ToString( FawnVision::component_swizzle::swizzle_b ),
                                                              ToString( FawnVision::component_swizzle::swizzle_a ) };
            const std::vector<std::string> borderColors { ToString( FawnVision::border_color::float_transparent_black ), ToString( FawnVision::border_color::int_transparent_black ), ToString( FawnVision::border_color::float_opaque_black ),
                                                          ToString( FawnVision::border_color::int_opaque_black ),        ToString( FawnVision::border_color::float_opaque_white ),    ToString( FawnVision::border_color::int_opaque_white ) };

            //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // Path input
            Gui::Text( "Source Direction: " + m_path.string(), 400 );
            Gui::Text( "Destination Direction: " + m_destinationDirection, 400 );

            Gui::Separator();

            // LOD Settings
            Gui::Toggle( "Use Mips", m_useMips, 150.f );
            if ( !m_useMips )
            {
                Gui::Disable();
            }
            Gui::Toggle( "Generate Mips", m_generateMips, 150.f );
            Gui::Float( "Max LOD", m_maxLod, 0.1f, 150.f );
            Gui::Float( "Min LOD", m_minLod, 0.1f, 150.f );
            Gui::Float( "Mip LOD Bias", m_mipLodBias, 0.01f, 150.f );
            Gui::ComboBox( "Mipmap Mode", mipmapMode, sampleModes, {}, 150.0f );
            if ( !m_useMips )
            {
                Gui::Enable();
            }

            Gui::Separator();

            // Sampler Settings
            Gui::Text( "Filter Mode" );
            Gui::ComboBox( "Mode", filterMode, sampleModes, {}, 150.0f );// Example enum values

            Gui::Text( "Wrap Mode" );
            Gui::ComboBox( "U", wrapU, wrapModes, {}, 150.0f );
            Gui::ComboBox( "V", wrapV, wrapModes, {}, 150.0f );
            Gui::ComboBox( "W", wrapW, wrapModes, {}, 150.0f );
            Gui::Separator();

            // Other Settings

            Gui::Text( "Component Mapping" );
            Gui::ComboBox( "R", componentMapR, componentMapping, {}, 150.0f );
            Gui::ComboBox( "G", componentMapG, componentMapping, {}, 150.0f );
            Gui::ComboBox( "B", componentMapB, componentMapping, {}, 150.0f );
            Gui::ComboBox( "A", componentMapA, componentMapping, {}, 150.0f );

            Gui::Int( "Anisotropy Level", m_anisotropyLevel, 1, 1, 7, 150.0f );
            Gui::ComboBox( "Shape", textureShape, textureShapes, {}, 150.0f );
            Gui::ComboBox( "Type", textureType, textureTypes, {}, 150.0f );
            Gui::ComboBox( "Border Color", borderColor, borderColors, {}, 150.0f );

            Gui::Separator();

            // Optional Settings
            Gui::Toggle( "Compare Enable", m_compareEnable, 150.f );
            Gui::Toggle( "Compare Operator", m_compareOperator, 150.f );
            Gui::Toggle( "Generate Normal", m_generateNormal, 150.f );
            Gui::Toggle( "Use Denormalized Coordinates", m_unNormalizedCoordinates, 150.f );
            Gui::Toggle( "Use sRGB", m_useSrgb, 150.f );

            if ( Gui::Button( "Import" ) )
            {
                LoadTexture();//TODO create a task
                Gui::EndPopup();
                m_isVisible = false;
                return true;
            }
            Gui::SameLine( 128 );
            if ( Gui::Button( "Cancel" ) )
            {
                m_isVisible = false;
                Gui::EndPopup();
                return true;
            }

            Gui::EndPopup();

            m_borderColor        = FawnVision::border_color( borderColor );
            m_componentMapping.a = (FawnVision::component_swizzle)componentMapA;
            m_componentMapping.b = (FawnVision::component_swizzle)componentMapB;
            m_componentMapping.g = (FawnVision::component_swizzle)componentMapG;
            m_componentMapping.r = (FawnVision::component_swizzle)componentMapR;
            m_filterMode         = FawnVision::image_filter( filterMode );
            m_mipmapMode         = FawnVision::image_filter( mipmapMode );
            m_sampleLevel        = sample_level( sampleLevel );
            m_shape              = texture_shape( textureShape );
            m_type               = FawnVision::image_view_type( textureType );
            m_wrapModeU          = FawnVision::sampler_address_mode( wrapU );
            m_wrapModeV          = FawnVision::sampler_address_mode( wrapV );
            m_wrapModeW          = FawnVision::sampler_address_mode( wrapW );
        }
        return false;
    }

    bool CTextureFileImporter::LoadTexture() noexcept
    {
        std::string extension = m_path.extension().string();
        std::ranges::for_each( extension, []( char& c ) {
            c = static_cast<char>( toupper( c ) );
        } );

        m_createInfo = FawnVision::STextureCreateInfo {
            .imageCreateInfo={
                .initialLayout = (uint32_t)FawnVision::image_layout::undefined, .samples = (uint8_t)m_sampleLevel, .usage = FawnVision::image_usage_flag_transfer_dst | FawnVision::image_usage_flag_sampled, .imageType = (uint8_t)m_type,
            }, .imageViewCreateInfo{
                .componentMapping = (uint32_t)m_componentMapping.r | (uint32_t)m_componentMapping.g << 8u | (uint32_t)m_componentMapping.b << 16u | (uint32_t)m_componentMapping.a << 24u, .firstLayer = 0, .firstMip = 0, .imageViewType = (uint8_t)m_type,
            }, .samplerCreateInfo{
                .maxAnisotropy = (float)m_anisotropyLevel, .addressModeU = (uint8_t)m_wrapModeU, .addressModeV = (uint8_t)m_wrapModeV, .addressModeW = (uint8_t)m_wrapModeW, .minFilter = (uint8_t)m_filterMode, .mipMapMode = (uint8_t)m_mipmapMode, .magFilter = (uint8_t)m_filterMode,
            },
        };

        if ( extension == ".HDR" )
        {
            if ( !LoadTextureWithHDR() )
            {
                return false;
            }
        }
        else if ( extension == ".KTX" || extension == ".DDS" )
        {
            if ( !LoadTextureWithGLI() )
            {
                return false;
            }
        }
        else
        {
            if ( !LoadTextureWithSDL() )
            {
                return false;
            }
        }

        // todo convert single/triple/quadruple channel to (double channel) normal map
        // todo convert to single channel
        // todo convert shapes

        Exporter::ExportImage( m_path.filename().string(), m_destinationDirection, m_createInfo );
        free( m_createInfo.pImageData );
        return true;
    }
    bool CTextureFileImporter::LoadTextureWithGLI()
    {
        gli::texture texture = gli::load( "path_to_your_texture" );
        if ( texture.empty() )
        {
            std::cerr << "Texture not found or failed to load" << std::endl;
            return false;
        }
        m_createInfo.imageCreateInfo.imageType = m_createInfo.imageViewCreateInfo.imageViewType = (uint8_t)texture.target();
        m_createInfo.imageCreateInfo.format                                                     = (uint8_t)texture.format();
        m_createInfo.imageCreateInfo.mipLevels                                                  = m_generateMips ? ~0u : (uint32_t)texture.max_level();
        m_createInfo.imageCreateInfo.arrayLayers                                                = (uint32_t)texture.max_face();
        m_createInfo.imageViewCreateInfo.layerCount                                             = (uint32_t)texture.max_face();
        m_createInfo.imageCreateInfo.width                                                      = (uint32_t)texture.extent().x;
        m_createInfo.imageCreateInfo.height                                                     = (uint32_t)texture.extent().y;
        m_createInfo.imageCreateInfo.depth                                                      = (uint32_t)texture.max_layer();
        m_createInfo.imageDataSize                                                              = (uint32_t)texture.size() * sizeof( uint32_t );
        m_createInfo.pImageData                                                                 = malloc( m_createInfo.imageDataSize );
        memcpy_s( m_createInfo.pImageData, m_createInfo.imageDataSize, texture.data(), m_createInfo.imageDataSize );
        texture.clear();
        return true;
    }
    bool CTextureFileImporter::LoadTextureWithHDR()
    {
        HDRLoaderResult hdrLoaderResult {};
        if ( !HDRLoader::Load( m_path.string().c_str(), hdrLoaderResult ) )
        {
            return false;
        }
        m_createInfo.imageCreateInfo.imageType = m_createInfo.imageViewCreateInfo.imageViewType = (uint8_t)FawnVision::image_view_type::view_2d;
        m_createInfo.imageCreateInfo.format                                                     = (uint32_t)FawnVision::image_format::r16_g16_b16;
        m_createInfo.imageCreateInfo.mipLevels                                                  = m_generateMips ? ~0u : 1u;
        m_createInfo.imageCreateInfo.arrayLayers                                                = 1u;
        m_createInfo.imageViewCreateInfo.layerCount                                             = 1u;
        m_createInfo.imageCreateInfo.width                                                      = hdrLoaderResult.width;
        m_createInfo.imageCreateInfo.height                                                     = hdrLoaderResult.height;
        m_createInfo.imageCreateInfo.depth                                                      = 1;
        m_createInfo.imageDataSize                                                              = (uint32_t)hdrLoaderResult.width * hdrLoaderResult.height * sizeof( float );
        m_createInfo.pImageData                                                                 = malloc( m_createInfo.imageDataSize );
        memcpy_s( m_createInfo.pImageData, m_createInfo.imageDataSize, hdrLoaderResult.cols, m_createInfo.imageDataSize );
        delete hdrLoaderResult.cols;
        return true;
    }
    bool CTextureFileImporter::LoadTextureWithSDL()
    {
        SDL_Surface* pSurface { IMG_Load( m_path.string().c_str() ) };
        if ( !pSurface )
        {
            std::cerr << "IMG_Load: " << IMG_GetError() << std::endl;
            return false;
        }
        FawnVision::image_format format {};
        switch ( SDL_PixelFormatEnum( pSurface->format->format ) )
        {
            case SDL_PIXELFORMAT_ABGR8888:
            case SDL_PIXELFORMAT_XBGR8888: format = FawnVision::image_format::r8_g8_b8_a8; break;
            case SDL_PIXELFORMAT_ABGR1555:
            case SDL_PIXELFORMAT_XBGR1555: format = FawnVision::image_format::r5_g5_b5_a1; break;
            case SDL_PIXELFORMAT_ABGR4444:
            case SDL_PIXELFORMAT_XBGR4444: format = FawnVision::image_format::r4_g4_b4_a4; break;
            case SDL_PIXELFORMAT_ARGB8888:
            case SDL_PIXELFORMAT_XRGB8888: format = FawnVision::image_format::r8_g8_b8_a8; break;
            case SDL_PIXELFORMAT_ARGB1555:
            case SDL_PIXELFORMAT_XRGB1555: format = FawnVision::image_format::r5_g5_b5_a1; break;
            case SDL_PIXELFORMAT_ARGB4444:
            case SDL_PIXELFORMAT_XRGB4444: format = FawnVision::image_format::r4_g4_b4_a4; break;
            case SDL_PIXELFORMAT_BGRA8888:
            case SDL_PIXELFORMAT_BGRX8888: format = FawnVision::image_format::r8_g8_b8_a8; break;
            case SDL_PIXELFORMAT_BGRA5551: format = FawnVision::image_format::r5_g6_b5; break;
            case SDL_PIXELFORMAT_BGRA4444: format = FawnVision::image_format::r4_g4_b4_a4; break;
            case SDL_PIXELFORMAT_RGBA8888:
            case SDL_PIXELFORMAT_RGBX8888: format = FawnVision::image_format::r8_g8_b8_a8; break;
            case SDL_PIXELFORMAT_RGBA5551: format = FawnVision::image_format::r5_g6_b5; break;
            case SDL_PIXELFORMAT_RGBA4444: format = FawnVision::image_format::r4_g4_b4_a4; break;
            case SDL_PIXELFORMAT_BGR24:
                format = FawnVision::image_format::r8_g8_b8;

                //todo: check if format is supported
                {
                    pSurface = SDL_ConvertSurfaceFormat( pSurface, SDL_PIXELFORMAT_RGBA32 );
                    format   = FawnVision::image_format::r8_g8_b8_a8;
                }
                break;
            case SDL_PIXELFORMAT_BGR565: format = FawnVision::image_format::r5_g6_b5; break;
            case SDL_PIXELFORMAT_RGB24:
                format = FawnVision::image_format::r8_g8_b8;

                //todo: check if format is supported
                {
                    pSurface = SDL_ConvertSurfaceFormat( pSurface, SDL_PIXELFORMAT_RGBA32 );
                    format   = FawnVision::image_format::r8_g8_b8_a8;
                }
                break;
            case SDL_PIXELFORMAT_RGB565: format = FawnVision::image_format::r5_g6_b5; break;
            case SDL_PIXELFORMAT_ARGB2101010:
            case SDL_PIXELFORMAT_EXTERNAL_OES:
            case SDL_PIXELFORMAT_INDEX1LSB:
            case SDL_PIXELFORMAT_INDEX1MSB:
            case SDL_PIXELFORMAT_INDEX4LSB:
            case SDL_PIXELFORMAT_INDEX4MSB:
            case SDL_PIXELFORMAT_INDEX8:
            case SDL_PIXELFORMAT_IYUV:
            case SDL_PIXELFORMAT_NV12:
            case SDL_PIXELFORMAT_NV21:
            case SDL_PIXELFORMAT_RGB332:
            case SDL_PIXELFORMAT_UNKNOWN:
            case SDL_PIXELFORMAT_UYVY:
            case SDL_PIXELFORMAT_YUY2:
            case SDL_PIXELFORMAT_YV12:
            case SDL_PIXELFORMAT_YVYU: format = FawnVision::image_format::undefined;
        }
        m_createInfo.imageViewCreateInfo.componentMapping = ( (uint32_t)m_componentMapping.r ) | ( ( (uint32_t)m_componentMapping.g ) << 8 ) | ( ( (uint32_t)m_componentMapping.b ) << 16 ) | ( ( (uint32_t)m_componentMapping.a ) << 24 );
        m_createInfo.imageCreateInfo.format               = (uint8_t)format;
        m_createInfo.imageCreateInfo.width                = pSurface->w;
        m_createInfo.imageCreateInfo.width                = pSurface->w;
        m_createInfo.imageCreateInfo.height               = pSurface->h;
        m_createInfo.imageDataSize                        = pSurface->h * pSurface->pitch;
        m_createInfo.pImageData                           = malloc( m_createInfo.imageDataSize );
        memcpy_s( m_createInfo.pImageData, m_createInfo.imageDataSize, pSurface->pixels, m_createInfo.imageDataSize );

        SDL_DestroySurface( pSurface );
        return true;
    }

    const char* CTextureFileImporter::ToString( FawnVision::sampler_address_mode mode )
    {
        switch ( mode )
        {
            case FawnVision::sampler_address_mode::repeat: return "Repeat";
            case FawnVision::sampler_address_mode::mirrored_repeat: return "Mirror";
            case FawnVision::sampler_address_mode::clamp_to_edge: return "Clamp To Edge";
            case FawnVision::sampler_address_mode::clamp_to_border: return "Clamp To Border";
            case FawnVision::sampler_address_mode::mirror_clamp_to_edge: return "Mirror Once";
            case FawnVision::sampler_address_mode::max: break;
        }
        return nullptr;
    }
    const char* CTextureFileImporter::ToString( FawnVision::image_filter mode )
    {
        switch ( mode )
        {
            case FawnVision::image_filter::nearest: return "Nearest";
            case FawnVision::image_filter::linear: return "Linear";
            case FawnVision::image_filter::max: break;
        }
        return nullptr;
    }
    const char* CTextureFileImporter::ToString( CTextureFileImporter::sample_level mode )
    {
        switch ( mode )
        {
            case sample_level::one: return "1x";
            case sample_level::two: return "2x";
            case sample_level::four: return "4x";
            case sample_level::eight: return "8x";
            case sample_level::sixteen: return "16x";
            case sample_level::thirty_two: return "32x";
            case sample_level::sixty_four: return "64x";
            case sample_level::auto_level: return "Auto";
        }
        return nullptr;
    }
    const char* CTextureFileImporter::ToString( FawnVision::image_view_type mode )
    {
        switch ( mode )
        {
            case FawnVision::image_view_type::view_1d: return "1D";
            case FawnVision::image_view_type::view_1d_array: return "1D Array";
            case FawnVision::image_view_type::view_2d: return "2D";
            case FawnVision::image_view_type::view_2d_array: return "2D Array";
            case FawnVision::image_view_type::view_3d: return "3D";
            case FawnVision::image_view_type::view_rect: return "Rectangle";
            case FawnVision::image_view_type::view_rect_array: return "Rectangle Array";
            case FawnVision::image_view_type::view_cube: return "Cube Map";
            case FawnVision::image_view_type::view_cube_array: return "Cube Map Array";
        }
        return nullptr;
    }
    const char* CTextureFileImporter::ToString( CTextureFileImporter::texture_shape mode )
    {
        switch ( mode )
        {
            case texture_shape::shape1D: return "1D";
            case texture_shape::shape2D: return "2D";
            case texture_shape::shape3D: return "3D";
        }
        return nullptr;
    }
    const char* CTextureFileImporter::ToString( FawnVision::component_swizzle mode )
    {
        switch ( mode )
        {
            case FawnVision::component_swizzle::swizzle_identity: return "Identity";
            case FawnVision::component_swizzle::swizzle_zero: return "Zero";
            case FawnVision::component_swizzle::swizzle_one: return "One";
            case FawnVision::component_swizzle::swizzle_r: return "R";
            case FawnVision::component_swizzle::swizzle_g: return "G";
            case FawnVision::component_swizzle::swizzle_b: return "B";
            case FawnVision::component_swizzle::swizzle_a: return "A";
            case FawnVision::component_swizzle::max: break;
        }
        return nullptr;
    }

    //  based on https://wickedengine.net/2019/09/22/improved-normal-reconstruction-from-depth/
    bool CTextureFileImporter::GenerateNormalMapFromRed( const char* pIn, char*& pOut, int w, int h )
    {
        pOut                                    = new char[ w * h * 2 ] {};
        const BambiMath::Matrix4x4 inverseViewProjection = BambiMath::Transpose( BambiMath::OrthographicMatrix( 1.0f, -1.0f, 1.0f, 1.0f ) );
        for ( int y {}; y < h; ++y )
        {
            for ( int x {}; x < h; ++x )
            {
                BambiMath::IVector2 uv { x, y };                    // center
                BambiMath::IVector2 uv0 { uv };                     // center
                BambiMath::IVector2 uv1 { uv + BambiMath::IVector2( 1, 0 ) };// right
                BambiMath::IVector2 uv2 { uv + BambiMath::IVector2( 0, 1 ) };// constexpr top

                if ( m_wrapModeU == FawnVision::sampler_address_mode::repeat )
                {
                    uv0.x = uv0.x % w;
                    uv1.x = uv1.x % w;
                    uv2.x = uv2.x % w;
                }
                else if ( m_wrapModeU == FawnVision::sampler_address_mode::mirrored_repeat )
                {
                    if ( uv0.x > w )
                    {
                        uv0.x -= w;
                    }
                    if ( uv1.x > w )
                    {
                        uv1.x -= w;
                    }
                    if ( uv2.x > w )
                    {
                        uv2.x -= w;
                    }
                }
                else if ( m_wrapModeU == FawnVision::sampler_address_mode::mirror_clamp_to_edge )
                {
                    if ( uv0.x > w )
                    {
                        uv0.x -= w;
                        if ( uv0.x > w )
                        {
                            uv0.x = 0;
                        }
                    }
                    if ( uv1.x > w )
                    {
                        uv1.x -= w;
                        if ( uv1.x > w )
                        {
                            uv1.x = 0;
                        }
                    }
                    if ( uv2.x > w )
                    {
                        uv2.x -= w;
                        if ( uv2.x > w )
                        {
                            uv2.x = 0;
                        }
                    }
                }
                else
                {
                    uv0.x = std::min( uv0.x, w );
                    uv1.x = std::min( uv1.x, w );
                    uv2.x = std::min( uv2.x, w );
                }
                if ( m_wrapModeV == FawnVision::sampler_address_mode::repeat )
                {
                    uv0.y = uv0.y % h;
                    uv1.y = uv1.y % h;
                    uv2.y = uv2.y % h;
                }
                else if ( m_wrapModeV == FawnVision::sampler_address_mode::mirrored_repeat )
                {
                    if ( uv0.y > h )
                    {
                        uv0.y -= h;
                    }
                    if ( uv1.y > h )
                    {
                        uv1.y -= h;
                    }
                    if ( uv2.y > h )
                    {
                        uv2.y -= h;
                    }
                }
                else if ( m_wrapModeV == FawnVision::sampler_address_mode::mirror_clamp_to_edge )
                {
                    if ( uv0.y > h )
                    {
                        uv0.y -= h;
                        if ( uv0.y > h )
                        {
                            uv0.y = 0;
                        }
                    }
                    if ( uv1.y > h )
                    {
                        uv1.y -= h;
                        if ( uv1.y > h )
                        {
                            uv1.y = 0;
                        }
                    }
                    if ( uv2.y > h )
                    {
                        uv2.y -= h;
                        if ( uv2.y > h )
                        {
                            uv2.y = 0;
                        }
                    }
                }
                else
                {
                    uv0.y = std::min( uv0.y, h );
                    uv1.y = std::min( uv1.y, h );
                    uv2.y = std::min( uv2.y, h );
                }

                float depth0              = (float)pIn[ uv0.x * 4 + uv0.y * w ] / 255.f;
                float depth1              = (float)pIn[ uv1.x * 4 + uv1.y * w ] / 255.f;
                float depth2              = (float)pIn[ uv2.x * 4 + uv2.y * w ] / 255.f;
                BambiMath::Vector3 p0              = ReconstructPosition( uv0, depth0, inverseViewProjection );
                BambiMath::Vector3 p1              = ReconstructPosition( uv1, depth1, inverseViewProjection );
                BambiMath::Vector3 p2              = ReconstructPosition( uv2, depth2, inverseViewProjection );
                BambiMath::Vector3 normal          = normalize( cross( p2 - p0, p1 - p0 ) );
                pOut[ x * 2 + y * w ]     = char( normal.x * 255.f );
                pOut[ x * 2 + y * w + 1 ] = char( normal.y * 255.f );
            }
        }
        return true;
    }
    BambiMath::Vector3 CTextureFileImporter::ReconstructPosition( const BambiMath::Vector2& uv, float z, const BambiMath::Matrix4x4& invVp )
    {
        float x { uv.x * 2.0f - 1.0f };
        float y { ( 1.0f - uv.y ) * 2.0f - 1.0f };
        BambiMath::Vector4 positionS { x, y, z, 1.0f };
        BambiMath::Vector4 positionV { invVp * positionS };
        float s { 1.0f / positionV.w };
        return BambiMath::Vector3 { positionV.x, positionV.y, positionV.z } * s;
    }
    const char* CTextureFileImporter::ToString( FawnVision::border_color mode )
    {
        switch ( mode )
        {
            case FawnVision::border_color::float_transparent_black: return "Transparent Black (float)";
            case FawnVision::border_color::int_transparent_black: return "Transparent Black (int)";
            case FawnVision::border_color::float_opaque_black: return "Opaque Black (float)";
            case FawnVision::border_color::int_opaque_black: return "Opaque Black (int)";
            case FawnVision::border_color::float_opaque_white: return "Opaque White (float)";
            case FawnVision::border_color::int_opaque_white: return "Opaque White (int)";
            default: break;
        }
        return nullptr;
    }
}// namespace FawnForge