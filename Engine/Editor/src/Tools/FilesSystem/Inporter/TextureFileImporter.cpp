//
// Created by joran on 10/09/2022.
//

#include "TextureFileImporter.h"

#include "../../../Renderer/EditorGui.h"
#include "../Exporter.h"
#include "hdrloader.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_image.h>
#include <iostream>
#include <algorithm>
#include <array>

#pragma warning (push)
#pragma warning (disable : 4100)
#pragma warning (disable : 4201)
#pragma warning (disable : 4244)
#pragma warning (disable : 4458)
#pragma warning (disable : 4723)
#pragma warning (disable : 5054)

#include <gli/gli.hpp>

#pragma warning (pop)

namespace FawnForge
{
    FawnForge::CTextureFileImporter::CTextureFileImporter() noexcept
            : m_importPercentage{}
            , m_isVisible{}
            , m_generateNormalFromGray{}
            , m_generateMips{ true }
            , m_useSrgb{ true }
            , m_wrapModeU{ FawnVision::ESamplerAddressMode::Repeat }
            , m_wrapModeV{ FawnVision::ESamplerAddressMode::Repeat }
            , m_wrapModeW{ FawnVision::ESamplerAddressMode::Repeat }
            , m_mipmapMode{ FawnVision::EFilter::Nearest }
            , m_filterMode{ FawnVision::EFilter::Nearest }
            , m_sampleLevel{ sample_level::sample_level_auto }
            , m_anisotropyLevel{ 1 }
            , m_path{}
            , m_type{ texture_type_default }
            , m_shape{ texture_shape::texture_shape_2D }
            , m_destinationDirection{}
    {
    }
    
    void CTextureFileImporter::SetVisible( const std::filesystem::path& path, const std::string& destinationDirection )
    {
        m_isVisible            = true;
        m_path                 = path;
        m_destinationDirection = destinationDirection;
    }
    
    bool CTextureFileImporter::IsVisible() const
    {
        return m_isVisible;
    }
    
    bool CTextureFileImporter::DrawImportSettings()
    {
        if ( Gui::StartPopup( "Import Texture" ))
        {
            uint64_t mipmapMode{ (uint64_t) m_mipmapMode };
            uint64_t filterMode{ (uint64_t) m_filterMode };
            uint64_t wrapU{ (uint64_t) m_wrapModeU };
            uint64_t wrapV{ (uint64_t) m_wrapModeV };
            uint64_t wrapW{ (uint64_t) m_wrapModeW };
            uint64_t sampleLevel{ (uint64_t) m_sampleLevel };
            uint64_t textureType{ (uint64_t) m_type };
            uint64_t textureShape{ (uint64_t) m_shape };
            
            const std::vector<std::string> textureTypes{
                    ToString( texture_type_default ),
                    ToString( texture_type_normal ),
                    ToString( texture_type_single_channel )
            };
            const std::vector<std::string> textureShapes{
                    ToString( texture_shape::texture_shape_2D ),
                    ToString( texture_shape::texture_shape_cube ),
                    ToString( texture_shape::texture_shape_2D_array ),
                    ToString( texture_shape::texture_shape_3D )
            };
            const std::vector<std::string> wrapModes{
                    ToString( FawnVision::ESamplerAddressMode::Repeat ),
                    ToString( FawnVision::ESamplerAddressMode::MirroredRepeat ),
                    ToString( FawnVision::ESamplerAddressMode::ClampToEdge ),
                    ToString( FawnVision::ESamplerAddressMode::ClampToBorder ),
                    ToString( FawnVision::ESamplerAddressMode::MirrorClampToEdge )
            };
            const std::vector<std::string> sampleModes{
                    ToString( FawnVision::EFilter::Nearest ), ToString( FawnVision::EFilter::Linear )
            };
            const std::vector<std::string> sampleLevels{
                    ToString( sample_level::sample_level_one ),
                    ToString( sample_level::sample_level_two ),
                    ToString( sample_level::sample_level_four ),
                    ToString( sample_level::sample_level_eight ),
                    ToString( sample_level::sample_level_sixteen ),
                    ToString( sample_level::sample_level_thirty_two ),
                    ToString( sample_level::sample_level_sixty_four ),
                    ToString( sample_level::sample_level_auto )
            };
            
            //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            Gui::DrawText( std::format( "Importing from {}", m_path.string()));
            
            Gui::DrawComboBox( "Texture GetType", textureType, textureTypes, {}, 150.0f );
            Gui::DrawComboBox( "Texture Shape", textureShape, textureShapes, {}, 150.0f );
            
            if ( textureType == (uint64_t) texture_type_default )
            {
                Gui::DrawToggle( "Use sRGB", m_useSrgb, 150.0f );
            }
            else if ( textureType == (uint64_t) texture_type_normal )
            {
                Gui::DrawToggle( "Generate from gray scale", m_generateNormalFromGray, 150.0f );
            }
            
            Gui::DrawToggle( "Generate Mips", m_generateMips, 150.0f );
            Gui::DrawComboBox( "Mipmap Mode", mipmapMode, sampleModes, {}, 150.0f );
            Gui::DrawComboBox( "Filter Mode", filterMode, sampleModes, {}, 150.0f );
            
            Gui::DrawInt( "Anisotropy Level", m_anisotropyLevel, 1, 1, 16, 150.0f );
            
            Gui::DrawComboBox( "Sample Level", sampleLevel, sampleLevels, {}, 150.0f );
            Gui::DrawComboBox( "Wrap Mode U", wrapU, wrapModes, {}, 150.0f );
            Gui::DrawComboBox( "Wrap Mode V", wrapV, wrapModes, {}, 150.0f );
            Gui::DrawComboBox( "Wrap Mode W", wrapW, wrapModes, {}, 150.0f );
            
            //todo swizzle?
            
            if ( Gui::DrawButton( "Import" ))
            {
                LoadTexture(); //TODO create a task
                Gui::EndPopup();
                m_isVisible = false;
                return true;
            }
            if ( Gui::DrawButton( "Cancel" ))
            {
                m_isVisible = false;
            }
            Gui::EndPopup();
            
            m_mipmapMode  = FawnVision::EFilter::Enum( mipmapMode );
            m_filterMode  = FawnVision::EFilter::Enum( filterMode );
            m_sampleLevel = sample_level( sampleLevel );
            m_wrapModeU   = FawnVision::ESamplerAddressMode::Enum( wrapU );
            m_wrapModeV   = FawnVision::ESamplerAddressMode::Enum( wrapV );
            m_wrapModeW   = FawnVision::ESamplerAddressMode::Enum( wrapW );
            m_type        = texture_type( textureType );
            m_shape       = texture_shape( textureShape );
            
        }
        return false;
    }
    
    float CTextureFileImporter::GetImportPercentage()
    {
        return m_importPercentage;
    }
    
    bool CTextureFileImporter::LoadTexture()
    {
        std::string extension = m_path.extension().string();
        std::ranges::for_each(
                extension, []( char& c )
                {
                    c = static_cast< char >( toupper( c ));
                }
        );
        
        int     anisotropy{ m_anisotropyLevel };
        int     sampleLevel{ (int) m_sampleLevel };
        int     mipmapMode{ (int) m_mipmapMode };
        int     filterMode{ (int) m_filterMode };
        int     wrapModeU{ (int) m_wrapModeU };
        int     wrapModeV{ (int) m_wrapModeV };
        int     wrapModeW{ (int) m_wrapModeW };
        uint8_t type{ FawnVision::EImageViewType::View2D };
        
        if ( m_shape == texture_shape::texture_shape_2D_array )
        {
            type = FawnVision::EImageViewType::View2DArray;
        }
        else if ( m_shape == texture_shape::texture_shape_cube )
        {
            type = FawnVision::EImageViewType::Cube;
        }
        else if ( m_shape == texture_shape::texture_shape_3D )
        {
            type = FawnVision::EImageViewType::View3D;
        }
        
        if ( extension == ".KTX" || extension == ".DDS" )
        {
            if ( m_shape == texture_shape::texture_shape_2D )
            {
                gli::texture2d texture{ gli::load( m_path.string().c_str()) };
                if ( texture.empty())
                {
                    return false;
                }
                Exporter::ExportImage(
                        m_path.filename().string(),
                        m_destinationDirection,
                        (uint8_t) texture.target(),
                        (uint32_t) texture.format(),
                        (uint8_t) (( m_generateMips ) ? -1 : texture.max_level()),
                        (uint8_t) texture.max_face(),
                        (uint32_t) texture.extent().x,
                        (uint32_t) texture.extent().y,
                        (uint32_t) texture.max_layer(),
                        (uint8_t) (( texture.extent().x * texture.extent().y * texture.max_layer()) / texture.size()),
                        texture.data(),
                        anisotropy,
                        sampleLevel,
                        mipmapMode,
                        filterMode,
                        wrapModeU,
                        wrapModeV,
                        wrapModeW
                );
                texture.clear();
                return true;
            }
            if ( m_shape == texture_shape::texture_shape_cube )
            {
                gli::texture_cube texture{ gli::load( m_path.string().c_str()) };
                if ( texture.empty())
                {
                    return false;
                }
                Exporter::ExportImage(
                        m_path.filename().string(),
                        m_destinationDirection,
                        (uint8_t) texture.target(),
                        (uint32_t) texture.format(),
                        (uint8_t) (( m_generateMips ) ? -1 : texture.max_level()),
                        (uint8_t) texture.max_face(),
                        (uint32_t) texture.extent().x,
                        (uint32_t) texture.extent().y,
                        (uint32_t) texture.max_layer(),
                        (uint8_t) (( texture.extent().x * texture.extent().y * texture.max_layer()) / texture.size()),
                        texture.data(),
                        anisotropy,
                        sampleLevel,
                        mipmapMode,
                        filterMode,
                        wrapModeU,
                        wrapModeV,
                        wrapModeW
                );
                texture.clear();
                return true;
            }
            if ( m_shape == texture_shape::texture_shape_2D_array )
            {
                gli::texture2d_array texture{ gli::load( m_path.string().c_str()) };
                if ( texture.empty())
                {
                    return false;
                }
                Exporter::ExportImage(
                        m_path.filename().string(),
                        m_destinationDirection,
                        (uint8_t) texture.target(),
                        (uint32_t) texture.format(),
                        (uint8_t) (( m_generateMips ) ? -1 : texture.max_level()),
                        (uint8_t) texture.max_face(),
                        (uint32_t) texture.extent().x,
                        (uint32_t) texture.extent().y,
                        (uint32_t) texture.max_layer(),
                        (uint8_t) (( texture.extent().x * texture.extent().y * texture.max_layer()) / texture.size()),
                        texture.data(),
                        anisotropy,
                        sampleLevel,
                        mipmapMode,
                        filterMode,
                        wrapModeU,
                        wrapModeV,
                        wrapModeW
                );
                texture.clear();
                return true;
            }
            if ( m_shape == texture_shape::texture_shape_3D )
            {
                gli::texture3d texture{ gli::load( m_path.string().c_str()) };
                if ( texture.empty())
                {
                    return false;
                }
                Exporter::ExportImage(
                        m_path.filename().string(),
                        m_destinationDirection,
                        (uint8_t) texture.target(),
                        (uint32_t) texture.format(),
                        (uint8_t) (( m_generateMips ) ? -1 : texture.max_level()),
                        (uint8_t) texture.max_face(),
                        (uint32_t) texture.extent().x,
                        (uint32_t) texture.extent().y,
                        (uint32_t) texture.max_layer(),
                        (uint8_t) (( texture.extent().x * texture.extent().y * texture.max_layer()) / texture.size()),
                        texture.data(),
                        anisotropy,
                        sampleLevel,
                        mipmapMode,
                        filterMode,
                        wrapModeU,
                        wrapModeV,
                        wrapModeW
                );
                texture.clear();
                return true;
            }
        }
        else if ( extension == ".HDR" )
        {
            HDRLoaderResult hdrLoaderResult{};
            if ( !HDRLoader::load( m_path.string().c_str(), hdrLoaderResult ))
            {
                return false;
            }
            Exporter::ExportImage(
                    m_path.filename().string(),
                    m_destinationDirection,
                    type,
                    static_cast<uint32_t>( FawnVision::EFormat::R16G16B16Sfloat),
                    ( m_generateMips ) ? -1 : 1,
                    6u,
                    hdrLoaderResult.width,
                    hdrLoaderResult.height,
                    1,
                    3 * sizeof( float ),
                    hdrLoaderResult.cols,
                    anisotropy,
                    sampleLevel,
                    mipmapMode,
                    filterMode,
                    wrapModeU,
                    wrapModeV,
                    wrapModeW
            );
        }
        else
        {
            SDL_Surface* pSurface{ IMG_Load( m_path.string().c_str()) };
            if ( !pSurface )
            {
                std::cerr << "IMG_Load: " << IMG_GetError() << std::endl;
                return false;
            }
            uint32_t imageFormat;
            switch ( SDL_PixelFormatEnum( pSurface->format->format ))
            {
                case SDL_PIXELFORMAT_BGR24:
                case SDL_PIXELFORMAT_RGB24:
                case SDL_PIXELFORMAT_ABGR8888:
                {
                    pSurface = SDL_ConvertSurfaceFormat( pSurface, SDL_PIXELFORMAT_RGBA8888 );
                    if ( !pSurface )
                    {
                        std::cerr << "IMG_Load: " << IMG_GetError() << std::endl;
                        return false;
                    }
                }
                case SDL_PIXELFORMAT_RGBA8888:
                {
                    if ( m_type == texture_type_default )
                    {
                        if ( m_useSrgb )
                        {
                            imageFormat = VK_FORMAT_R8G8B8A8_SRGB;
                        }
                        else
                        {
                            imageFormat = VK_FORMAT_R8G8B8A8_UNORM;
                        }
                        Exporter::ExportImage(
                                m_path.filename().string(),
                                m_destinationDirection,
                                type,
                                imageFormat,
                                (uint8_t) (( m_generateMips ) ? -1 : 1 ),
                                1u,
                                (uint32_t) pSurface->w,
                                (uint32_t) pSurface->h,
                                1u,
                                (uint8_t) pSurface->format->BytesPerPixel,
                                pSurface->pixels,
                                anisotropy,
                                sampleLevel,
                                mipmapMode,
                                filterMode,
                                wrapModeU,
                                wrapModeV,
                                wrapModeW
                        );
                        SDL_DestroySurface( pSurface );
                        return true;
                    }
                    if ( m_type == texture_type_normal )
                    {
                        imageFormat = VK_FORMAT_R8G8_UNORM;
                        if ( m_generateNormalFromGray )
                        {
                            char* pData{ nullptr };
                            GenerateNormalMap((char*) pSurface->pixels, pData, pSurface->w, pSurface->h );
                            
                            Exporter::ExportImage(
                                    m_path.filename().string(),
                                    m_destinationDirection,
                                    2u,
                                    imageFormat,
                                    (uint8_t) (( m_generateMips ) ? -1 : 1 ),
                                    1u,
                                    (uint32_t) pSurface->w,
                                    (uint32_t) pSurface->h,
                                    1u,
                                    2u,
                                    pData,
                                    anisotropy,
                                    sampleLevel,
                                    mipmapMode,
                                    filterMode,
                                    wrapModeU,
                                    wrapModeV,
                                    wrapModeW
                            );
                            SDL_DestroySurface( pSurface );
                            delete[] pData;
                            return true;
                        }
                        Exporter::ExportImage(
                                m_path.filename().string(),
                                m_destinationDirection,
                                2u,
                                imageFormat,
                                (uint8_t) (( m_generateMips ) ? -1 : 1 ),
                                1u,
                                (uint32_t) pSurface->w,
                                (uint32_t) pSurface->h,
                                1u,
                                2u,
                                pSurface->pixels,
                                anisotropy,
                                sampleLevel,
                                mipmapMode,
                                filterMode,
                                wrapModeU,
                                wrapModeV,
                                wrapModeW
                        );
                        SDL_DestroySurface( pSurface );
                        return true;
                    }
                    if ( m_type == texture_type_single_channel )
                    {
                        imageFormat = VK_FORMAT_R8_UNORM;
                        Exporter::ExportImage(
                                m_path.filename().string(),
                                m_destinationDirection,
                                2u,
                                imageFormat,
                                (uint8_t) (( m_generateMips ) ? -1 : 1 ),
                                1u,
                                (uint32_t) pSurface->w,
                                (uint32_t) pSurface->h,
                                1u,
                                1u,
                                pSurface->pixels,
                                anisotropy,
                                sampleLevel,
                                mipmapMode,
                                filterMode,
                                wrapModeU,
                                wrapModeV,
                                wrapModeW
                        );
                    }
                    break;
                }
                
                default:
                {
//                    imageFormat = (uint32_t) VK_FORMAT_UNDEFINED;
                    std::cerr << "format " << SDL_GetPixelFormatName( pSurface->format->format ) << " is currently not supported. Please contact the developers trough GitHub." << std::endl;
                    break;
                }
                
            }
        }
        return false;
    }
    
    const char* CTextureFileImporter::ToString( FawnVision::ESamplerAddressMode::Enum mode )
    {
        switch ( mode )
        {
            case FawnVision::ESamplerAddressMode::Repeat:
                return "Repeat";
            case FawnVision::ESamplerAddressMode::MirroredRepeat:
                return "Mirror";
            case FawnVision::ESamplerAddressMode::ClampToEdge:
                return "Clamp To Edge";
            case FawnVision::ESamplerAddressMode::ClampToBorder:
                return "Clamp To Border";
            case FawnVision::ESamplerAddressMode::MirrorClampToEdge:
                return "Mirror Once";
            case FawnVision::ESamplerAddressMode::MaxEnum:
                break;
        }
        return nullptr;
    }
    
    const char* CTextureFileImporter::ToString( FawnVision::EFilter::Enum mode )
    {
        switch ( mode )
        {
            case FawnVision::EFilter::Nearest:
                return "Nearest";
            case FawnVision::EFilter::Linear:
                return "Linear";
            case FawnVision::EFilter::MaxEnum:
                break;
        }
        return nullptr;
    }
    
    const char* CTextureFileImporter::ToString( CTextureFileImporter::sample_level mode )
    {
        switch ( mode )
        {
            case sample_level::sample_level_one:
                return "1x";
            case sample_level::sample_level_two:
                return "2x";
            case sample_level::sample_level_four:
                return "4x";
            case sample_level::sample_level_eight:
                return "8x";
            case sample_level::sample_level_sixteen:
                return "16x";
            case sample_level::sample_level_thirty_two:
                return "32x";
            case sample_level::sample_level_sixty_four:
                return "64x";
            case sample_level::sample_level_auto:
                return "Auto";
        }
        return nullptr;
    }
    const char* CTextureFileImporter::ToString( texture_type mode )
    {
        switch ( mode )
        {
            case texture_type_default:
                return "Default";
            case texture_type_normal:
                return "Normal";
            case texture_type_single_channel:
                return "Single Channel";
        }
        return nullptr;
    }
    const char* CTextureFileImporter::ToString( CTextureFileImporter::texture_shape mode )
    {
        switch ( mode )
        {
            case texture_shape::texture_shape_2D:
                return "2D";
            case texture_shape::texture_shape_cube:
                return "Cube";
            case texture_shape::texture_shape_2D_array:
                return "2D Array";
            case texture_shape::texture_shape_3D:
                return "3D";
        }
        return nullptr;
    }

//  based on https://wickedengine.net/2019/09/22/improved-normal-reconstruction-from-depth/
    bool CTextureFileImporter::GenerateNormalMap( const char* pIn, char*& pOut, int w, int h )
    {
        pOut = new char[w * h * 2]{};
        const glm::mat4x4 inverseViewProjection = glm::transpose( glm::ortho( 1.0f, -1.0f, 1.0f, 1.0f ));
        for ( int         y{}; y < h; ++y )
        {
            for ( int x{}; x < h; ++x )
            {
                glm::ivec2 uv{ x, y }; // center
                glm::ivec2 uv0{ uv }; // center
                glm::ivec2 uv1{ uv + glm::ivec2( 1, 0 ) }; // right
                glm::ivec2 uv2{ uv + glm::ivec2( 0, 1 ) }; // top
                
                if ( m_wrapModeU == FawnVision::ESamplerAddressMode::Repeat )
                {
                    uv0.x = uv0.x % w;
                    uv1.x = uv1.x % w;
                    uv2.x = uv2.x % w;
                }
                else if ( m_wrapModeU == FawnVision::ESamplerAddressMode::MirroredRepeat )
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
                else if ( m_wrapModeU == FawnVision::ESamplerAddressMode::MirrorClampToEdge )
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
                if ( m_wrapModeV == FawnVision::ESamplerAddressMode::Repeat )
                {
                    uv0.y = uv0.y % h;
                    uv1.y = uv1.y % h;
                    uv2.y = uv2.y % h;
                }
                else if ( m_wrapModeV == FawnVision::ESamplerAddressMode::MirroredRepeat )
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
                else if ( m_wrapModeV == FawnVision::ESamplerAddressMode::MirrorClampToEdge )
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
                
                float     depth0 = (float)pIn[uv0.x * 4 + uv0.y * w] / 255.f;
                float     depth1 = (float)pIn[uv1.x * 4 + uv1.y * w] / 255.f;
                float     depth2 = (float)pIn[uv2.x * 4 + uv2.y * w] / 255.f;
                glm::vec3 p0     = ReconstructPosition( uv0, depth0, inverseViewProjection );
                glm::vec3 p1     = ReconstructPosition( uv1, depth1, inverseViewProjection );
                glm::vec3 p2     = ReconstructPosition( uv2, depth2, inverseViewProjection );
                glm::vec3 normal = normalize( cross( p2 - p0, p1 - p0 ));
                pOut[x * 2 + y * w]     = char( normal.x * 255.f );
                pOut[x * 2 + y * w + 1] = char( normal.y * 255.f );
            }
        }
        return true;
    }
    
    glm::vec3 CTextureFileImporter::ReconstructPosition( const glm::vec2& uv, float z, const glm::mat4x4& invVp )
    {
        float     x          = uv.x * 2.0f - 1.0f;
        float     y          = ( 1.0f - uv.y ) * 2.0f - 1.0f;
        glm::vec4 positionS = glm::vec4( x, y, z, 1.0f );
        glm::vec4 positionV = invVp * positionS;
        float     s         = 1.0f / positionV.w;
        return glm::vec3{ positionV.x, positionV.y, positionV.z } * s;
    }
} // FawnForge