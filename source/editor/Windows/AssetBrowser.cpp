#include "AssetBrowser.h"

#include <ErrorHandling.h>
#include <IResourceManager.h>
#include <ISystem.h>
#define IMGUI_DEFINE_MATH_OPERATORS
#include <backends/imgui_impl_vulkan.h>

#include "MaterialEditor.h"
#include "PropertyPanel.h"
#include "ShaderEditor.h"

#include "../Renderer/EditorGui.h"
#include "../Tools/FilesSystem/Exporter.h"

FawnForge::CAssetBrowser::~CAssetBrowser() = default;

void FawnForge::CAssetBrowser::Initialize( ISystem* pSystem )
{
    try
    {

        m_pIcon[ (uint8_t)file_type::unknown ]  = pSystem->GetResourceManager()->GetTexture( "../Data/Editor/Icons/UnknownFile.basset" );
        m_pIcon[ (uint8_t)file_type::folder ]   = pSystem->GetResourceManager()->GetTexture( "../Data/Editor/Icons/Folder.basset" );
        m_pIcon[ (uint8_t)file_type::audio ]    = pSystem->GetResourceManager()->GetTexture( "../Data/Editor/Icons/AudioFile.basset" );
        m_pIcon[ (uint8_t)file_type::code ]     = pSystem->GetResourceManager()->GetTexture( "../Data/Editor/Icons/CodeFile.basset" );
        m_pIcon[ (uint8_t)file_type::font ]     = pSystem->GetResourceManager()->GetTexture( "../Data/Editor/Icons/FontFile.basset" );
        m_pIcon[ (uint8_t)file_type::image ]    = pSystem->GetResourceManager()->GetTexture( "../Data/Editor/Icons/ImageFile.basset" );
        m_pIcon[ (uint8_t)file_type::material ] = pSystem->GetResourceManager()->GetTexture( "../Data/Editor/Icons/MaterialFile.basset" );
        m_pIcon[ (uint8_t)file_type::model ]    = pSystem->GetResourceManager()->GetTexture( "../Data/Editor/Icons/ModelFile.basset" );
        m_pIcon[ (uint8_t)file_type::preset ]   = pSystem->GetResourceManager()->GetTexture( "../Data/Editor/Icons/PresetFile.basset" );
        m_pIcon[ (uint8_t)file_type::scene ]    = pSystem->GetResourceManager()->GetTexture( "../Data/Editor/Icons/BalbinoFile.basset" );
        m_pIcon[ (uint8_t)file_type::shader ]   = pSystem->GetResourceManager()->GetTexture( "../Data/Editor/Icons/ShaderFile.basset" );

        m_descriptorIcon[ (uint8_t)file_type::unknown ]
            = ImGui_ImplVulkan_AddTexture( m_pIcon[ (uint8_t)file_type::unknown ]->GetSamplerObject()->GetSampler(), m_pIcon[ (uint8_t)file_type::unknown ]->GetImageViewObject()->GetImageView(), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL );
        m_descriptorIcon[ (uint8_t)file_type::folder ]
            = ImGui_ImplVulkan_AddTexture( m_pIcon[ (uint8_t)file_type::folder ]->GetSamplerObject()->GetSampler(), m_pIcon[ (uint8_t)file_type::folder ]->GetImageViewObject()->GetImageView(), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL );
        m_descriptorIcon[ (uint8_t)file_type::audio ]
            = ImGui_ImplVulkan_AddTexture( m_pIcon[ (uint8_t)file_type::audio ]->GetSamplerObject()->GetSampler(), m_pIcon[ (uint8_t)file_type::audio ]->GetImageViewObject()->GetImageView(), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL );
        m_descriptorIcon[ (uint8_t)file_type::code ]
            = ImGui_ImplVulkan_AddTexture( m_pIcon[ (uint8_t)file_type::code ]->GetSamplerObject()->GetSampler(), m_pIcon[ (uint8_t)file_type::code ]->GetImageViewObject()->GetImageView(), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL );
        m_descriptorIcon[ (uint8_t)file_type::font ]
            = ImGui_ImplVulkan_AddTexture( m_pIcon[ (uint8_t)file_type::font ]->GetSamplerObject()->GetSampler(), m_pIcon[ (uint8_t)file_type::font ]->GetImageViewObject()->GetImageView(), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL );
        m_descriptorIcon[ (uint8_t)file_type::image ]
            = ImGui_ImplVulkan_AddTexture( m_pIcon[ (uint8_t)file_type::image ]->GetSamplerObject()->GetSampler(), m_pIcon[ (uint8_t)file_type::image ]->GetImageViewObject()->GetImageView(), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL );
        m_descriptorIcon[ (uint8_t)file_type::material ]
            = ImGui_ImplVulkan_AddTexture( m_pIcon[ (uint8_t)file_type::material ]->GetSamplerObject()->GetSampler(), m_pIcon[ (uint8_t)file_type::material ]->GetImageViewObject()->GetImageView(), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL );
        m_descriptorIcon[ (uint8_t)file_type::model ]
            = ImGui_ImplVulkan_AddTexture( m_pIcon[ (uint8_t)file_type::model ]->GetSamplerObject()->GetSampler(), m_pIcon[ (uint8_t)file_type::model ]->GetImageViewObject()->GetImageView(), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL );
        m_descriptorIcon[ (uint8_t)file_type::preset ]
            = ImGui_ImplVulkan_AddTexture( m_pIcon[ (uint8_t)file_type::preset ]->GetSamplerObject()->GetSampler(), m_pIcon[ (uint8_t)file_type::preset ]->GetImageViewObject()->GetImageView(), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL );
        m_descriptorIcon[ (uint8_t)file_type::scene ]
            = ImGui_ImplVulkan_AddTexture( m_pIcon[ (uint8_t)file_type::scene ]->GetSamplerObject()->GetSampler(), m_pIcon[ (uint8_t)file_type::scene ]->GetImageViewObject()->GetImageView(), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL );
        m_descriptorIcon[ (uint8_t)file_type::shader ]
            = ImGui_ImplVulkan_AddTexture( m_pIcon[ (uint8_t)file_type::shader ]->GetSamplerObject()->GetSampler(), m_pIcon[ (uint8_t)file_type::shader ]->GetImageViewObject()->GetImageView(), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL );
    }
    catch ( FawnTrace::Exception& e )
    {
        pSystem->Log.Error( e.what() );
    }
    m_pSystem = pSystem;
}

void FawnForge::CAssetBrowser::Draw() noexcept
{
    FindAllFiles();
    if ( m_updateCurrentDirectory )
    {
        GetAllFilesInSelectedPath( m_currentDirectoryName, m_currentDirectory );
        m_updateCurrentDirectory = false;
        std::ranges::sort( m_currentDirectory, []( const SFile& left, const SFile& right ) {
            return left.fileName < right.fileName;
        } );
        std::ranges::sort( m_currentDirectory, []( const SFile& left, const SFile& right ) {
            return (int)left.isFolder > (int)right.isFolder;
        } );
    }
    if ( Gui::Begin( "Asset Browser", m_isVisible, 0 ) )
    {
        if ( Gui::BeginChild( "Asset Tree", { 128, 0 }, true, 1 << 11 ) )
        {
            uint32_t nodeIdx {};
            DrawTree( "..\\Data", nodeIdx );
        }
        Gui::EndChild();
        Gui::SameLine();
        if ( Gui::BeginChild( "Asset File", { 0, 0 }, true, 1 << 6 ) )
        {
            if ( Gui::BeginChild( "Icon Size Slider", { 0, Gui::GetContentRegionAvail().y - 32 }, false, 1 << 6 ) )
            {
                int id {};
                bool wasOneSelected {};
                std::ranges::for_each( m_currentDirectory, [ & ]( const auto& currentFile ) {
                    bool isSelected {};
                    Gui::Selectable( id, isSelected, m_size * 1.5f );
                    ++id;
                    HandelSelected( currentFile, isSelected );

                    // todo: display audio wave form
                    // todo: show font name in font style
                    // todo: display image pData
                    // todo: display material thumbnail
                    // todo: display model thumbnail
                    Gui::ResourceItem( currentFile, m_descriptorIcon[ (uint8_t)currentFile.type ], m_size );
                    wasOneSelected |= isSelected;
                } );
                DrawContextMenu( wasOneSelected );
            }
            Gui::EndChild();
            Gui::FloatSlider( "Icon Size", m_size, 32.0f, 128.f, false );
        }
        Gui::EndChild();
        if ( !m_itemToBeCreated.isFolder )
        {
            CreateItem();
        }
    }
    if ( m_isVisible )
    {
        Gui::End();
    }
}

void FawnForge::CAssetBrowser::Cleanup()
{
    m_currentDirectory.clear();
    m_files.clear();
}

void FawnForge::CAssetBrowser::ShowWindow() noexcept
{
    m_isVisible = true;
    Gui::SetWindowFocus( "Asset Browser" );
}

void FawnForge::CAssetBrowser::SetShaderGraphReference( CShaderEditor* pShaderGraph, CMaterialEditor* pMaterialEditor, CPropertyPanel* pPropertyPanel ) noexcept
{
    m_pShaderGraph    = pShaderGraph;
    m_pMaterialEditor = pMaterialEditor;
    m_pPropertyPanel  = pPropertyPanel;
}

void FawnForge::CAssetBrowser::FindAllFiles() noexcept
{
    auto it = m_files.begin();
    while ( it != m_files.end() )
    {
        if ( !std::filesystem::exists( it->path ) )
        {
            //Remove
            it                       = m_files.erase( it );
            m_updateCurrentDirectory = true;
        }
        else
        {
            ++it;
        }
    }

    if ( m_files.empty() )
    {
        m_files.push_back( GetData( std::filesystem::relative( "..\\Data" ) ) );
        m_files.back().lastWrittenTime = std::filesystem::last_write_time( "..\\Data" );
    }

    for ( auto file = std::filesystem::recursive_directory_iterator( "..\\Data" ); file != std::filesystem::recursive_directory_iterator(); ++file )
    {
        auto currentFileLastWriteTime = std::filesystem::last_write_time( *file );

        // File creation
        auto filetIt = std::ranges::find_if( m_files, [ &file ]( const SFile& filePair ) {
            return filePair.path == file->path();
        } );
        if ( filetIt == m_files.end() )
        {
            //Create New
            m_files.push_back( GetData( relative( file->path() ) ) );
            m_files.back().lastWrittenTime = currentFileLastWriteTime;
            m_files.back().depth           = file.depth() + 1;
            m_updateCurrentDirectory       = true;
        }
        else if ( filetIt->lastWrittenTime != currentFileLastWriteTime )
        {
            //Changed
            ( *filetIt )             = GetData( relative( file->path() ) );
            filetIt->lastWrittenTime = currentFileLastWriteTime;
            filetIt->depth           = file.depth() + 1;
            m_updateCurrentDirectory = true;
        }
    }
}

void FawnForge::CAssetBrowser::DrawTree( const std::string& path, uint32_t& nodeIdx )
{
    const auto& fileIter = std::ranges::find_if( std::as_const( m_files ), [ &path ]( const SFile& f ) {
        return f.path == path;
    } );
    if ( fileIter == m_files.cend() )
    {
        return;
    }

    std::vector<std::string> files;
    constexpr ImGuiTreeNodeFlags baseFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
    ImGuiTreeNodeFlags nodeFlags           = baseFlags;
    for ( const auto& f : m_files )
    {
        if ( f.path != path && f.path.find( path ) != std::string::npos && f.isFolder && f.depth - fileIter->depth <= 1
             && std::ranges::find_if( files,
                                      [ &f ]( std::string_view string ) {
                                          return string.find( f.path ) != std::string::npos;
                                      } )
                 == files.cend() )
        {
            files.push_back( f.path );
        }
    }
    if ( files.empty() )
    {
        nodeFlags |= 1 << 8;
    }
    if ( !fileIter->depth )
    {
        nodeFlags |= 1 << 5;
        Gui::SetNextItemOpen( true );
    }
    if ( Gui::TreeNodeEx( (uint64_t)fileIter->fileName.c_str(), nodeFlags, fileIter->fileName ) )
    {
        if ( Gui::IsItemClicked() )
        {
            m_updateCurrentDirectory = true;
            m_currentDirectoryName   = std::filesystem::relative( fileIter->path ).string();
        }
        for ( const auto& f : files )
        {
            DrawTree( f, nodeIdx );
        }
        Gui::TreePop();
    }
    else
    {
        if ( Gui::IsItemClicked() )
        {
            m_updateCurrentDirectory = true;
            m_currentDirectoryName   = std::filesystem::relative( fileIter->path ).string();
        }
    }
    for ( int i = 0; i < (int)file_type::max; ++i )
    {
        if ( void* pData = Gui::ReceiveDragDrop( ToString( file_type( i ) ) ) )
        {
            const SFile* pFile { static_cast<SFile*>( pData ) };
            MoveFile( pFile, path.c_str() );
            return;
        }
    }
}

void FawnForge::CAssetBrowser::GetAllFilesInSelectedPath( std::string path, std::vector<SFile>& filesInDirectory ) noexcept
{
    const auto& fileIter = std::ranges::find_if( std::as_const( m_files ), [ &path ]( const SFile& f ) {
        return f.path == path;
    } );
    if ( fileIter == m_files.cend() )
    {
        return;
    }
    for ( char& c : path )
    {
        if ( c == '/' )
        {
            c = '\\';
        }
    }
    filesInDirectory.clear();
    for ( const auto& f : m_files )
    {
        if ( f.path != path && f.path.find( path ) != std::string::npos && f.depth - fileIter->depth == 1 )
        {
            filesInDirectory.push_back( f );
        }
    }
}

void FawnForge::CAssetBrowser::HandelSelected( const SFile& currentFile, bool isSelected )
{
    switch ( currentFile.type )
    {
        case file_type::folder:
            if ( ( Gui::IsMouseDoubleClicked( 0 ) && isSelected ) || m_openItem )
            {
                const std::string currentPath { currentFile.path };
                m_updateCurrentDirectory = true;
                m_currentDirectoryName   = std::filesystem::relative( currentPath ).string();
                return;
            }
            for ( int i = 0; i < (int)file_type::max; ++i )
            {
                if ( void* pData = Gui::ReceiveDragDrop( ToString( file_type( i ) ) ) )
                {
                    const SFile* pFile { static_cast<SFile*>( pData ) };
                    MoveFile( pFile, currentFile.path + "/" );
                    return;
                }
            }
        case file_type::shader:
            if ( ( Gui::IsMouseDoubleClicked( 0 ) && isSelected ) || m_openItem )
            {
                m_pShaderGraph->ShowWindow( currentFile );
            }
            break;
        case file_type::material:
            if ( ( Gui::IsMouseDoubleClicked( ImGuiMouseButton_Left ) && isSelected ) || m_openItem )
            {
                m_pMaterialEditor->ShowWindow( currentFile );
            }
            break;
        case file_type::scene:
        case file_type::image:
        case file_type::audio:
        case file_type::font:
        case file_type::model:
        case file_type::code:
        case file_type::preset:
        case file_type::unknown:
        case file_type::max: break;
    }
}

const char* FawnForge::CAssetBrowser::GetCurrentDirectory() const
{
    return m_currentDirectoryName.c_str();
}

void FawnForge::CAssetBrowser::MoveFile( const SFile* pFile, const std::filesystem::path& destination ) const
{
    Rename( pFile->path, destination.string() + "\\" + pFile->fileName );
}

void FawnForge::CAssetBrowser::Rename( const std::filesystem::path& oldName, const std::filesystem::path& newName ) const
{
    if ( std::filesystem::exists( oldName ) )
    {
        std::filesystem::rename( oldName, newName );
    }
}
void FawnForge::CAssetBrowser::DrawContextMenu( bool isSelected ) noexcept
{
    bool showInExplorer;
    m_openItem = false;
    bool rename;
    bool shouldDelete;
    bool copyPath;
    bool importFile;
    bool reimport;
    bool reimportAll;
    bool properties;
    m_isContextMenuOpen &= 0b10;
    if ( Gui::BeginPopupContextWindow( 1 ) )
    {
        m_isContextMenuOpen |= 0b01;
        if ( m_isContextMenuOpen >= 2 )
        {
            m_isItemSelected = isSelected;
        }
        if ( Gui::BeginMenu( "Create" ) )
        {
            if ( Gui::MenuItem( "Shader" ) )
            {
                m_itemToBeCreated.isFolder = false;
                m_itemToBeCreated.path     = m_currentDirectoryName;
                m_itemToBeCreated.fileName = "";
                m_itemToBeCreated.type     = file_type::shader;
            }
            if ( Gui::MenuItem( "Material" ) )
            {
                m_itemToBeCreated.isFolder = false;
                m_itemToBeCreated.path     = m_currentDirectoryName;
                m_itemToBeCreated.fileName = "";
                m_itemToBeCreated.type     = file_type::material;
            }
            Gui::EndMenu();
        }
        showInExplorer = Gui::MenuItem( "Show in Explorer" );
        m_openItem     = Gui::MenuItem( "Open", m_isItemSelected );
        rename         = Gui::MenuItem( "Rename", m_isItemSelected );
        shouldDelete   = Gui::MenuItem( "Delete", m_isItemSelected );
        copyPath       = Gui::MenuItem( "Copy Path" );
        Gui::Separator();
        importFile               = Gui::MenuItem( "Import new asset" );
        m_updateCurrentDirectory = Gui::MenuItem( "Refresh" );
        reimport                 = Gui::MenuItem( "Reimport" );
        reimportAll              = Gui::MenuItem( "Reimport All" );
        Gui::Separator();
        properties = Gui::MenuItem( "Properties..." );
        Gui::EndPopup();

        (void)showInExplorer;
        (void)rename;
        (void)shouldDelete;
        (void)copyPath;
        (void)importFile;
        (void)reimport;
        (void)reimportAll;
        if ( properties )
        {
            m_pPropertyPanel->ShowWindow();
        }
    }
    m_isContextMenuOpen <<= 1;
}
void FawnForge::CAssetBrowser::CreateItem() noexcept
{
    bool saved {};
    bool cancel {};
    if ( Gui::StartPopup( "Create New Item", true, { 256, -1 } ) )
    {
        char name[ 64 ] {};
        bool inputChanged;
        for ( int i {}; i < 64; ++i )
        {
            if ( i > m_itemToBeCreated.fileName.size() )
            {
                break;
            }
            name[ i ] = m_itemToBeCreated.fileName[ i ];
        }
        inputChanged = Gui::InputText( "new name", name, 64 );
        saved        = Gui::Button( "Save" );
        Gui::SameLine();
        cancel = Gui::Button( "Cancel" );
        Gui::EndPopup();
        if ( inputChanged )
        {
            m_itemToBeCreated.fileName = "";
            m_itemToBeCreated.fileName.append( name, std::find_if( name, name + 64, []( char c ) {
                                                   return c == '\0';
                                               } ) );
        }
        if ( saved && !m_itemToBeCreated.fileName.empty() )
        {
            m_itemToBeCreated.isFolder = true;
        }
        if ( cancel )
        {
            m_itemToBeCreated.isFolder = true;
        }
    }
    if ( cancel || !saved || m_itemToBeCreated.fileName.empty() )
    {
        return;
    }

    switch ( m_itemToBeCreated.type )
    {
        //shader
        case file_type::shader: {
            FawnVision::SShaderCreateInfo createInfo {};
            Exporter::ExportShader( m_itemToBeCreated.fileName, m_currentDirectoryName, { createInfo }, 0 );
            break;
        }//material
        case file_type::material: {
            try
            {
                FawnVision::SMaterialCreateInfo createInfo { m_pPropertyPanel->GetDefaultShader(), m_pSystem->GetResourceManager()->GetShader( m_pPropertyPanel->GetDefaultShader() )->GetShaderResources() };
                Exporter::ExportMaterial( m_itemToBeCreated.fileName, m_currentDirectoryName, createInfo );
            }
            catch ( const FawnTrace::Exception& e )
            {
                m_pSystem->Log.Error( e.what() );
            }
            break;
        }
        case file_type::folder://todo
        case file_type::scene:
        case file_type::image:
        case file_type::audio:
        case file_type::font:
        case file_type::model:
        case file_type::code:
        case file_type::preset:
        case file_type::unknown:
        case file_type::max: break;
    }
}
