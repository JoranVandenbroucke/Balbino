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
            : m_importPercentage{},
              m_isVisible{},
              m_useMips{ true },
              m_generateMips{ true },
              m_useAnisotropy{ true },
              m_wrapModeU{ BalVulkan::ESamplerAddressMode::Repeat },
              m_wrapModeV{ BalVulkan::ESamplerAddressMode::Repeat },
              m_wrapModeW{ BalVulkan::ESamplerAddressMode::Repeat },
              m_mipmapMode{ BalVulkan::EFilter::Nearest },
              m_filterMode{ BalVulkan::EFilter::Nearest },
              m_sampleLevel{ ESampleLevel::Auto },
              m_anisotropyLevel{ 1 },
              m_path{},
              m_destinationDirection{}
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
        if ( BalEditor::EditorGUI::StartPopup( "Import Texture" ))
        {
            std::string mipmapMode{ ToString( m_mipmapMode ) };
            std::string filterMode{ ToString( m_filterMode ) };
            std::string wrapU{ ToString( m_wrapModeU ) };
            std::string wrapV{ ToString( m_wrapModeV ) };
            std::string wrapW{ ToString( m_wrapModeW ) };
            std::string sampleLevel{ ToString( m_sampleLevel ) };
            
            const std::vector<std::string> wrapModes{
                    ToString( BalVulkan::ESamplerAddressMode::Repeat ),
                    ToString( BalVulkan::ESamplerAddressMode::MirroredRepeat ),
                    ToString( BalVulkan::ESamplerAddressMode::ClampToEdge ),
                    ToString( BalVulkan::ESamplerAddressMode::ClampToBorder ),
                    ToString( BalVulkan::ESamplerAddressMode::MirrorClampToEdge )
            };
            const std::vector<std::string> sampleModes{
                    ToString( BalVulkan::EFilter::Nearest ),
                    ToString( BalVulkan::EFilter::Linear )
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
            
            BalEditor::EditorGUI::DrawText( std::format( "Importing from {}", m_path.string()).c_str());
            BalEditor::EditorGUI::DrawToggle( "Use Mips", m_useMips, 200 );
            BalEditor::EditorGUI::DrawToggle( "Generate Mips", m_generateMips, 200 );
            BalEditor::EditorGUI::DrawComboBox( "Mipmap Mode", mipmapMode, sampleModes, 200 );
            BalEditor::EditorGUI::DrawComboBox( "Filter Mode", filterMode, sampleModes, 200 );
            
            BalEditor::EditorGUI::DrawToggle( "Use Anisotropy", m_useAnisotropy, 200 );
            BalEditor::EditorGUI::DrawInt( "Anisotropy Level", m_anisotropyLevel, 1, 1, 16, 200 );
            
            BalEditor::EditorGUI::DrawComboBox( "Sample Level", sampleLevel, sampleLevels, 200 );
            BalEditor::EditorGUI::DrawComboBox( "Wrap Mode U", wrapU, wrapModes, 200 );
            BalEditor::EditorGUI::DrawComboBox( "Wrap Mode V", wrapV, wrapModes, 200 );
            BalEditor::EditorGUI::DrawComboBox( "Wrap Mode W", wrapW, wrapModes, 200 );
            
            if ( BalEditor::EditorGUI::DrawButton( "Import" ))
            {
                LoadTexture(); //TODO create a task
                BalEditor::EditorGUI::EndPopup();
                m_isVisible = false;
                return true;
            }
            if ( BalEditor::EditorGUI::DrawButton( "Cancel" ))
            {
                m_isVisible = false;
            }
            
            if ( mipmapMode == ToString( BalVulkan::EFilter::Nearest ))
            {
                m_mipmapMode = BalVulkan::EFilter::Nearest;
            }
            else
            {
                m_mipmapMode = BalVulkan::EFilter::Linear;
            }
            
            if ( filterMode == ToString( BalVulkan::EFilter::Nearest ))
            {
                m_filterMode = BalVulkan::EFilter::Nearest;
            }
            else
            {
                m_filterMode = BalVulkan::EFilter::Linear;
            }
            
            if ( sampleLevel == ToString( ESampleLevel::One ))
            {
                m_sampleLevel = ESampleLevel::One;
            }
            else if ( sampleLevel == ToString( ESampleLevel::Two ))
            {
                m_sampleLevel = ESampleLevel::Two;
            }
            else if ( sampleLevel == ToString( ESampleLevel::Four ))
            {
                m_sampleLevel = ESampleLevel::Four;
            }
            else if ( sampleLevel == ToString( ESampleLevel::Eight ))
            {
                m_sampleLevel = ESampleLevel::Eight;
            }
            else if ( sampleLevel == ToString( ESampleLevel::Sixteen ))
            {
                m_sampleLevel = ESampleLevel::Sixteen;
            }
            else if ( sampleLevel == ToString( ESampleLevel::ThirtyTwo ))
            {
                m_sampleLevel = ESampleLevel::ThirtyTwo;
            }
            else if ( sampleLevel == ToString( ESampleLevel::SixtyFour ))
            {
                m_sampleLevel = ESampleLevel::SixtyFour;
            }
            else
            {
                m_sampleLevel = ESampleLevel::Auto;
            }
            
            if ( wrapU == ToString( BalVulkan::ESamplerAddressMode::Repeat ))
            {
                m_wrapModeU = BalVulkan::ESamplerAddressMode::Repeat;
            }
            else if ( wrapU == ToString( BalVulkan::ESamplerAddressMode::MirroredRepeat ))
            {
                m_wrapModeU = BalVulkan::ESamplerAddressMode::MirroredRepeat;
            }
            else if ( wrapU == ToString( BalVulkan::ESamplerAddressMode::ClampToEdge ))
            {
                m_wrapModeU = BalVulkan::ESamplerAddressMode::ClampToEdge;
            }
            else if ( wrapU == ToString( BalVulkan::ESamplerAddressMode::ClampToBorder ))
            {
                m_wrapModeU = BalVulkan::ESamplerAddressMode::ClampToBorder;
            }
            else
            {
                m_wrapModeU = BalVulkan::ESamplerAddressMode::MirrorClampToEdge;
            }
            
            if ( wrapV == ToString( BalVulkan::ESamplerAddressMode::Repeat ))
            {
                m_wrapModeV = BalVulkan::ESamplerAddressMode::Repeat;
            }
            else if ( wrapV == ToString( BalVulkan::ESamplerAddressMode::MirroredRepeat ))
            {
                m_wrapModeV = BalVulkan::ESamplerAddressMode::MirroredRepeat;
            }
            else if ( wrapV == ToString( BalVulkan::ESamplerAddressMode::ClampToEdge ))
            {
                m_wrapModeV = BalVulkan::ESamplerAddressMode::ClampToEdge;
            }
            else if ( wrapV == ToString( BalVulkan::ESamplerAddressMode::ClampToBorder ))
            {
                m_wrapModeV = BalVulkan::ESamplerAddressMode::ClampToBorder;
            }
            else
            {
                m_wrapModeV = BalVulkan::ESamplerAddressMode::MirrorClampToEdge;
            }
            
            if ( wrapW == ToString( BalVulkan::ESamplerAddressMode::Repeat ))
            {
                m_wrapModeW = BalVulkan::ESamplerAddressMode::Repeat;
            }
            else if ( wrapW == ToString( BalVulkan::ESamplerAddressMode::MirroredRepeat ))
            {
                m_wrapModeW = BalVulkan::ESamplerAddressMode::MirroredRepeat;
            }
            else if ( wrapW == ToString( BalVulkan::ESamplerAddressMode::ClampToEdge ))
            {
                m_wrapModeW = BalVulkan::ESamplerAddressMode::ClampToEdge;
            }
            else if ( wrapW == ToString( BalVulkan::ESamplerAddressMode::ClampToBorder ))
            {
                m_wrapModeW = BalVulkan::ESamplerAddressMode::ClampToBorder;
            }
            else
            {
                m_wrapModeW = BalVulkan::ESamplerAddressMode::MirrorClampToEdge;
            }
            
            BalEditor::EditorGUI::EndPopup();
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
        std::ranges::for_each( extension, []( char& c )
        {
            c = static_cast< char >( toupper( c ));
        } );
        
        int anisotropy{ m_useAnisotropy ? m_anisotropyLevel : 1 };
        int sampleLevel{ (int) m_sampleLevel };
        int mipmapMode{ (int) m_mipmapMode };
        int filterMode{ (int) m_filterMode };
        int wrapModeU{ (int) m_wrapModeU };
        int wrapModeV{ (int) m_wrapModeV };
        int wrapModeW{ (int) m_wrapModeW };
        
        if ( extension == ".KTX" || extension == ".DDS" )
        {
            gli::texture_cube texture{ gli::load( m_path.string().c_str()) };
            if ( texture.empty())
            {
                return false;
            }
            
            Exporter::ExportImage( m_path.filename().string(),
                                   m_destinationDirection + "\\",
                                   (uint8_t) texture.target(),
                                   (uint32_t) texture.format(),
                                   (uint8_t) (( m_useMips && m_generateMips ) ? -1 : texture.max_level()),
                                   (uint8_t) texture.max_face(),
                                   (uint32_t) texture.extent().x,
                                   (uint32_t) texture.extent().y,
                                   (uint32_t) texture.max_layer(),
                                   (uint8_t) (( texture.extent().x * texture.extent().y * texture.max_layer()) / texture.size()),
                                   texture.data(), anisotropy, sampleLevel, mipmapMode, filterMode, wrapModeU,
                                   wrapModeV, wrapModeW );
            
            texture.clear();
        }
        else if ( extension == ".hdr" )
        {
            HDRLoaderResult hdrLoaderResult{};
            if ( !HDRLoader::load( m_path.string().c_str(), hdrLoaderResult ))
            {
                return false;
            }
            
            Exporter::ExportImage( m_path.filename().string(), m_destinationDirection + "\\",
                                   (uint8_t) BalVulkan::EImageViewType::Cube,
                                   static_cast< uint32_t >( BalVulkan::EFormat::R16G16B16Sfloat),
                                   ( m_useMips && m_generateMips ) ? -1 : 1, 6u, hdrLoaderResult.width,
                                   hdrLoaderResult.height, 1, 3, hdrLoaderResult.cols, anisotropy, sampleLevel,
                                   mipmapMode, filterMode, wrapModeU, wrapModeV, wrapModeW );
        }
        else
        {
            SDL_Surface* pSurface{ IMG_Load( m_path.string().c_str()) };
            if ( pSurface == nullptr )
            {
                std::cerr << "IMG_Load: " << IMG_GetError() << std::endl;
                return false;
            }
            uint32_t imageFormat{};
            switch ( SDL_PixelFormatEnum( pSurface->format->format ))
            {
                
                case SDL_PIXELFORMAT_UNKNOWN:
                case SDL_PIXELFORMAT_INDEX1LSB:
                case SDL_PIXELFORMAT_INDEX1MSB:
                case SDL_PIXELFORMAT_INDEX4LSB:
                case SDL_PIXELFORMAT_INDEX4MSB:
                    imageFormat = (uint32_t) VK_FORMAT_UNDEFINED;
                    break;
                case SDL_PIXELFORMAT_INDEX8:
                    imageFormat = (uint32_t) VK_FORMAT_R8_SRGB;
                    break;
                case SDL_PIXELFORMAT_RGB332:
                    imageFormat = (uint32_t) VK_FORMAT_R32G32B32_SFLOAT;
                    break;
                case SDL_PIXELFORMAT_XRGB4444:
                    imageFormat = (uint32_t) VK_FORMAT_A4R4G4B4_UNORM_PACK16;
                    break;
                case SDL_PIXELFORMAT_XBGR4444:
                    imageFormat = (uint32_t) VK_FORMAT_A4B4G4R4_UNORM_PACK16;
                    break;
                case SDL_PIXELFORMAT_XRGB1555:
                    imageFormat = (uint32_t) VK_FORMAT_A1R5G5B5_UNORM_PACK16;
                    break;
                case SDL_PIXELFORMAT_XBGR1555:
                    imageFormat = (uint32_t) VK_FORMAT_A1R5G5B5_UNORM_PACK16;  //TODO SWAP RGB TO BGR
                    break;
                case SDL_PIXELFORMAT_ARGB4444:
                    imageFormat = (uint32_t) VK_FORMAT_A4R4G4B4_UNORM_PACK16;
                    break;
                case SDL_PIXELFORMAT_RGBA4444:
                    imageFormat = (uint32_t) VK_FORMAT_R4G4B4A4_UNORM_PACK16;
                    break;
                case SDL_PIXELFORMAT_ABGR4444:
                    imageFormat = (uint32_t) VK_FORMAT_A4B4G4R4_UNORM_PACK16;
                    break;
                case SDL_PIXELFORMAT_BGRA4444:
                    imageFormat = (uint32_t) VK_FORMAT_B4G4R4A4_UNORM_PACK16;
                    break;
                case SDL_PIXELFORMAT_ARGB1555:
                    imageFormat = (uint32_t) VK_FORMAT_A1R5G5B5_UNORM_PACK16;
                    break;
                case SDL_PIXELFORMAT_RGBA5551:
                    imageFormat = (uint32_t) VK_FORMAT_R5G5B5A1_UNORM_PACK16;
                    break;
                case SDL_PIXELFORMAT_ABGR1555:
                    imageFormat = (uint32_t) VK_FORMAT_B5G6R5_UNORM_PACK16;    //TODO SWAP RGB TO BGR
                    break;
                case SDL_PIXELFORMAT_BGRA5551:
                    imageFormat = (uint32_t) VK_FORMAT_B5G5R5A1_UNORM_PACK16;
                    break;
                case SDL_PIXELFORMAT_RGB565:
                    imageFormat = (uint32_t) VK_FORMAT_B5G6R5_UNORM_PACK16;    //TODO SWAP BGR TO RGB
                    break;
                case SDL_PIXELFORMAT_BGR565:
                    imageFormat = (uint32_t) VK_FORMAT_B5G6R5_UNORM_PACK16;
                    break;
                case SDL_PIXELFORMAT_RGB24:
                {
                    SDL_PixelFormat* format = SDL_AllocFormat( SDL_PIXELFORMAT_RGBA32 );
                    pSurface = SDL_ConvertSurface( pSurface, format, 0 );
                    SDL_FreeFormat( format );
                    if ( !pSurface )
                    {
                        std::cout << "Something went wrong while converting formats: " << SDL_GetError() << std::endl;
                    }
                    imageFormat = (uint32_t) VK_FORMAT_R8G8B8A8_SRGB;
                    break;
                }
                case SDL_PIXELFORMAT_BGR24:
                {
                    SDL_PixelFormat* format = SDL_AllocFormat( SDL_PIXELFORMAT_RGBA32 );
                    pSurface                = SDL_ConvertSurface( pSurface, format, 0 );
                    SDL_FreeFormat( format );
                    if ( !pSurface )
                    {
                        std::cout << "Something went wrong while converting formats: " << SDL_GetError() << std::endl;
                    }
                    imageFormat = (uint32_t) VK_FORMAT_R8G8B8A8_SRGB; //TODO SWAR RGD TO BGR
                    break;
                }
                case SDL_PIXELFORMAT_XRGB8888:
                    imageFormat = (uint32_t) VK_FORMAT_A8B8G8R8_SRGB_PACK32;   //TODO SWAR BGR TO RGB
                    break;
                case SDL_PIXELFORMAT_RGBX8888:
                    imageFormat = (uint32_t) VK_FORMAT_R8G8B8A8_SRGB;
                    break;
                case SDL_PIXELFORMAT_XBGR8888:
                    imageFormat = (uint32_t) VK_FORMAT_A8B8G8R8_SRGB_PACK32;
                    break;
                case SDL_PIXELFORMAT_BGRX8888:
                    imageFormat = (uint32_t) VK_FORMAT_B8G8R8A8_SRGB;
                    break;
                case SDL_PIXELFORMAT_ARGB8888:
                    imageFormat = (uint32_t) VK_FORMAT_A8B8G8R8_SRGB_PACK32;   //TODO SWAR BGR TO RGB
                    break;
                case SDL_PIXELFORMAT_RGBA8888:
                    imageFormat = (uint32_t) VK_FORMAT_R8G8B8A8_SRGB;
                    break;
                case SDL_PIXELFORMAT_ABGR8888:
                    imageFormat = (uint32_t) VK_FORMAT_A8B8G8R8_SRGB_PACK32;
                    break;
                case SDL_PIXELFORMAT_BGRA8888:
                    imageFormat = (uint32_t) VK_FORMAT_B8G8R8A8_SRGB;
                    break;
                case SDL_PIXELFORMAT_ARGB2101010:
                    imageFormat = (uint32_t) VK_FORMAT_A2R10G10B10_UNORM_PACK32;
                    break;
                case SDL_PIXELFORMAT_YV12:
                    imageFormat = (uint32_t) VK_FORMAT_UNDEFINED;
                    break;
                case SDL_PIXELFORMAT_IYUV:
                    imageFormat = (uint32_t) VK_FORMAT_UNDEFINED;
                    break;
                case SDL_PIXELFORMAT_YUY2:
                    imageFormat = (uint32_t) VK_FORMAT_UNDEFINED;
                    break;
                case SDL_PIXELFORMAT_UYVY:
                    imageFormat = (uint32_t) VK_FORMAT_UNDEFINED;
                    break;
                case SDL_PIXELFORMAT_YVYU:
                    imageFormat = (uint32_t) VK_FORMAT_UNDEFINED;
                    break;
                case SDL_PIXELFORMAT_NV12:
                    imageFormat = (uint32_t) VK_FORMAT_UNDEFINED;
                    break;
                case SDL_PIXELFORMAT_NV21:
                    imageFormat = (uint32_t) VK_FORMAT_UNDEFINED;
                    break;
                case SDL_PIXELFORMAT_EXTERNAL_OES:
                    imageFormat = (uint32_t) VK_FORMAT_UNDEFINED;
                    break;
            }
            
            Exporter::ExportImage( m_path.filename().string(),
                                   m_destinationDirection + "\\",
                                   2u,
                                   imageFormat,
                                   (uint8_t) (( m_useMips && m_generateMips ) ? -1 : 1 ),
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
                                   wrapModeW );
            SDL_FreeSurface( pSurface );
        }
        return true;
    }
    
    const char* CTextureFileImporter::ToString( BalVulkan::ESamplerAddressMode mode )
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
        }
        return nullptr;
    }
    
    const char* CTextureFileImporter::ToString( BalVulkan::EFilter mode )
    {
        switch ( mode )
        {
            
            case BalVulkan::EFilter::Nearest:
                return "Nearest";
            case BalVulkan::EFilter::Linear:
                return "Linear";
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
} // BalEditor