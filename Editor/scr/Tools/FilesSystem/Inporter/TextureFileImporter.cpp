//
// Created by joran on 10/09/2022.
//

#include "TextureFileImporter.h"

#include "../../../EditorGUI/EditorGui.h"
#include "../Exporter.h"
#include "hdrloader.h"
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <algorithm>

#pragma warning (push)
#pragma warning (disable : 4100)
#pragma warning (disable : 4201)
#pragma warning (disable : 4244)
#pragma warning (disable : 4458)
#pragma warning (disable : 5054)

#include <gli/gli.hpp>

#pragma warning (pop)

namespace BalEditor
{
    BalEditor::CTextureFileImporter::CTextureFileImporter() noexcept
            : m_importPercentage{}
              , m_isVisible{}
              , m_generateNormalFromGray{}
              , m_generateMips{ true }
              , m_useSRGB{ true }
              , m_wrapModeU{ BalVulkan::ESamplerAddressMode::Repeat }
              , m_wrapModeV{ BalVulkan::ESamplerAddressMode::Repeat }
              , m_wrapModeW{ BalVulkan::ESamplerAddressMode::Repeat }
              , m_mipmapMode{ BalVulkan::EFilter::Nearest }
              , m_filterMode{ BalVulkan::EFilter::Nearest }
              , m_sampleLevel{ ESampleLevel::Auto }
              , m_anisotropyLevel{ 1 }
              , m_path{}
              , m_type{ ETextureType::Default }
              , m_shape{ ETextureShape::Texture2D }
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
        if ( GUI::StartPopup( "Import Texture" ))
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
                    ToString( ETextureType::Default ),
                    ToString( ETextureType::Normal ),
                    ToString( ETextureType::SingleChannel )
            };
            const std::vector<std::string> textureShapes{
                    ToString( ETextureShape::Texture2D ),
                    ToString( ETextureShape::TextureCube ),
                    ToString( ETextureShape::Texture2DArray ),
                    ToString( ETextureShape::Texture3D )
            };
            const std::vector<std::string> wrapModes{
                    ToString( BalVulkan::ESamplerAddressMode::Repeat ),
                    ToString( BalVulkan::ESamplerAddressMode::MirroredRepeat ),
                    ToString( BalVulkan::ESamplerAddressMode::ClampToEdge ),
                    ToString( BalVulkan::ESamplerAddressMode::ClampToBorder ),
                    ToString( BalVulkan::ESamplerAddressMode::MirrorClampToEdge )
            };
            const std::vector<std::string> sampleModes{
                    ToString( BalVulkan::EFilter::Nearest ), ToString( BalVulkan::EFilter::Linear )
            };
            const std::vector<std::string> sampleLevels{
                    ToString( ESampleLevel::One ),
                    ToString( ESampleLevel::Two ),
                    ToString( ESampleLevel::Four ),
                    ToString( ESampleLevel::Eight ),
                    ToString( ESampleLevel::Sixteen ),
                    ToString( ESampleLevel::ThirtyTwo ),
                    ToString( ESampleLevel::SixtyFour ),
                    ToString( ESampleLevel::Auto )
            };
            
            //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            GUI::DrawText( std::format( "Importing from {}", m_path.string()).c_str());
            
            GUI::DrawComboBox( "Texture Type", textureType, textureTypes, {}, 150.0f );
            GUI::DrawComboBox( "Texture Shape", textureShape, textureShapes, {}, 150.0f );
            
            if ( textureType == (uint64_t) ETextureType::Default )
            {
                GUI::DrawToggle( "Use sRGB", m_useSRGB, 150.0f );
            }
            else if ( textureType == (uint64_t) ETextureType::Normal )
            {
                GUI::DrawToggle( "Generate from gray scale", m_generateNormalFromGray, 150.0f );
            }
            
            GUI::DrawToggle( "Generate Mips", m_generateMips, 150.0f );
            GUI::DrawComboBox( "Mipmap Mode", mipmapMode, sampleModes, {}, 150.0f );
            GUI::DrawComboBox( "Filter Mode", filterMode, sampleModes, {}, 150.0f );
            
            GUI::DrawInt( "Anisotropy Level", m_anisotropyLevel, 1, 1, 16, 150.0f );
            
            GUI::DrawComboBox( "Sample Level", sampleLevel, sampleLevels, {}, 150.0f );
            GUI::DrawComboBox( "Wrap Mode U", wrapU, wrapModes, {}, 150.0f );
            GUI::DrawComboBox( "Wrap Mode V", wrapV, wrapModes, {}, 150.0f );
            GUI::DrawComboBox( "Wrap Mode W", wrapW, wrapModes, {}, 150.0f );
            
            //todo swizzle?
            
            if ( GUI::DrawButton( "Import" ))
            {
                LoadTexture(); //TODO create a task
                GUI::EndPopup();
                m_isVisible = false;
                return true;
            }
            if ( GUI::DrawButton( "Cancel" ))
            {
                m_isVisible = false;
            }
            GUI::EndPopup();
            
            m_mipmapMode  = BalVulkan::EFilter::Enum( mipmapMode );
            m_filterMode  = BalVulkan::EFilter::Enum( filterMode );
            m_sampleLevel = ESampleLevel( sampleLevel );
            m_wrapModeU   = BalVulkan::ESamplerAddressMode::Enum( wrapU );
            m_wrapModeV   = BalVulkan::ESamplerAddressMode::Enum( wrapV );
            m_wrapModeW   = BalVulkan::ESamplerAddressMode::Enum( wrapW );
            m_type        = ETextureType( textureType );
            m_shape       = ETextureShape( textureShape );
            
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
        uint8_t type{ BalVulkan::EImageViewType::View2D };
        
        if ( m_shape == ETextureShape::Texture2DArray )
        {
            type = BalVulkan::EImageViewType::View2DArray;
        }
        else if ( m_shape == ETextureShape::TextureCube )
        {
            type = BalVulkan::EImageViewType::Cube;
        }
        else if ( m_shape == ETextureShape::Texture3D )
        {
            type = BalVulkan::EImageViewType::View3D;
        }
        
        if ( extension == ".KTX" || extension == ".DDS" )
        {
            if ( m_shape == ETextureShape::Texture2D )
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
            if ( m_shape == ETextureShape::TextureCube )
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
            if ( m_shape == ETextureShape::Texture2DArray )
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
            if ( m_shape == ETextureShape::Texture3D )
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
                    static_cast< uint32_t >( BalVulkan::EFormat::R16G16B16Sfloat),
                    ( m_generateMips ) ? -1 : 1,
                    6u,
                    hdrLoaderResult.width,
                    hdrLoaderResult.height,
                    1,
                    3,
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
            uint32_t imageFormat{};
            switch ( SDL_PixelFormatEnum( pSurface->format->format ))
            {
                case SDL_PIXELFORMAT_BGR24:
                case SDL_PIXELFORMAT_RGB24:
                case SDL_PIXELFORMAT_ABGR8888:
                {
                    pSurface = SDL_ConvertSurfaceFormat( pSurface, SDL_PIXELFORMAT_RGBA8888, 0 );
                    if ( !pSurface )
                    {
                        std::cerr << "IMG_Load: " << IMG_GetError() << std::endl;
                        return false;
                    }
                }
                case SDL_PIXELFORMAT_RGBA8888:
                {
                    if ( m_type == ETextureType::Default )
                    {
                        if ( m_useSRGB )
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
                        SDL_FreeSurface( pSurface );
                        return true;
                    }
                    if ( m_type == ETextureType::Normal )
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
                            SDL_FreeSurface( pSurface );
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
                        SDL_FreeSurface( pSurface );
                        return true;
                    }
                    if ( m_type == ETextureType::SingleChannel )
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
                    imageFormat = (uint32_t) VK_FORMAT_UNDEFINED;
                    std::cerr
                            << "format "
                            << SDL_GetPixelFormatName( pSurface->format->format )
                            << " is currently not supported. Please contact the developers trough GitHub."
                            << std::endl;
                    break;
                }
                
            }
        }
        return false;
    }
    
    const char* CTextureFileImporter::ToString( BalVulkan::ESamplerAddressMode::Enum mode )
    {
        switch ( mode )
        {
            case BalVulkan::ESamplerAddressMode::Repeat:
                return "Repeat";
            case BalVulkan::ESamplerAddressMode::MirroredRepeat:
                return "Mirror";
            case BalVulkan::ESamplerAddressMode::ClampToEdge:
                return "Clamp To Edge";
            case BalVulkan::ESamplerAddressMode::ClampToBorder:
                return "Clamp To Border";
            case BalVulkan::ESamplerAddressMode::MirrorClampToEdge:
                return "Mirror Once";
            case BalVulkan::ESamplerAddressMode::MaxEnum:
                break;
        }
        return nullptr;
    }
    
    const char* CTextureFileImporter::ToString( BalVulkan::EFilter::Enum mode )
    {
        switch ( mode )
        {
            
            case BalVulkan::EFilter::Nearest:
                return "Nearest";
            case BalVulkan::EFilter::Linear:
                return "Linear";
            case BalVulkan::EFilter::MaxEnum:
                break;
        }
        return nullptr;
    }
    
    const char* CTextureFileImporter::ToString( CTextureFileImporter::ESampleLevel mode )
    {
        switch ( mode )
        {
            case ESampleLevel::One:
                return "1x";
            case ESampleLevel::Two:
                return "2x";
            case ESampleLevel::Four:
                return "4x";
            case ESampleLevel::Eight:
                return "8x";
            case ESampleLevel::Sixteen:
                return "16x";
            case ESampleLevel::ThirtyTwo:
                return "32x";
            case ESampleLevel::SixtyFour:
                return "64x";
            case ESampleLevel::Auto:
                return "Auto";
        }
        return nullptr;
    }
    const char* CTextureFileImporter::ToString( CTextureFileImporter::ETextureType mode )
    {
        switch ( mode )
        {
            case ETextureType::Default:
                return "Default";
            case ETextureType::Normal:
                return "Normal";
            case ETextureType::SingleChannel:
                return "Single Channel";
        }
        return nullptr;
    }
    const char* CTextureFileImporter::ToString( CTextureFileImporter::ETextureShape mode )
    {
        switch ( mode )
        {
            case ETextureShape::Texture2D:
                return "2D";
            case ETextureShape::TextureCube:
                return "Cube";
            case ETextureShape::Texture2DArray:
                return "2D Array";
            case ETextureShape::Texture3D:
                return "3D";
        }
        return nullptr;
    }

//    based on https://wickedengine.net/2019/09/22/improved-normal-reconstruction-from-depth/
    bool CTextureFileImporter::GenerateNormalMap( const char* pIn, char*& pOut, int w, int h )
    {
        pOut = new char[w * h * 2]{};
        const glm::mat4x4 InverseViewProjection = glm::transpose( glm::ortho( 1.0f, -1.0f, 1.0f, 1.0f ));
        for ( int         y{}; y < h; ++y )
        {
            for ( int x{}; x < h; ++x )
            {
                glm::ivec2 uv{ x, y }; // center
                glm::ivec2 uv0{ uv }; // center
                glm::ivec2 uv1{ uv + glm::ivec2( 1, 0 ) }; // right
                glm::ivec2 uv2{ uv + glm::ivec2( 0, 1 ) }; // top
                
                if ( m_wrapModeU == BalVulkan::ESamplerAddressMode::Repeat )
                {
                    uv0.x = uv0.x % w;
                    uv1.x = uv1.x % w;
                    uv2.x = uv2.x % w;
                }
                else if ( m_wrapModeU == BalVulkan::ESamplerAddressMode::MirroredRepeat )
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
                else if ( m_wrapModeU == BalVulkan::ESamplerAddressMode::MirrorClampToEdge )
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
                if ( m_wrapModeV == BalVulkan::ESamplerAddressMode::Repeat )
                {
                    uv0.y = uv0.y % h;
                    uv1.y = uv1.y % h;
                    uv2.y = uv2.y % h;
                }
                else if ( m_wrapModeV == BalVulkan::ESamplerAddressMode::MirroredRepeat )
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
                else if ( m_wrapModeV == BalVulkan::ESamplerAddressMode::MirrorClampToEdge )
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
                
                float     depth0 = pIn[uv0.x * 4 + uv0.y * w] / 255.f;
                float     depth1 = pIn[uv1.x * 4 + uv1.y * w] / 255.f;
                float     depth2 = pIn[uv2.x * 4 + uv2.y * w] / 255.f;
                glm::vec3 P0     = ReconstructPosition( uv0, depth0, InverseViewProjection );
                glm::vec3 P1     = ReconstructPosition( uv1, depth1, InverseViewProjection );
                glm::vec3 P2     = ReconstructPosition( uv2, depth2, InverseViewProjection );
                glm::vec3 normal = normalize( cross( P2 - P0, P1 - P0 ));
                pOut[x * 2 + y * w]     = char( normal.x * 255.f );
                pOut[x * 2 + y * w + 1] = char( normal.y * 255.f );
            }
        }
        return true;
    }
    
    glm::vec3 CTextureFileImporter::ReconstructPosition( const glm::vec2& uv, float z, const glm::mat4x4& InvVP )
    {
        float     x          = uv.x * 2.0f - 1.0f;
        float     y          = ( 1.0f - uv.y ) * 2.0f - 1.0f;
        glm::vec4 position_s = glm::vec4( x, y, z, 1.0f );
        glm::vec4 position_v = InvVP * position_s;
        float     s          = 1.0f / position_v.w;
        return glm::vec3{ position_v.x, position_v.y, position_v.z } * s;
    }
} // BalEditor