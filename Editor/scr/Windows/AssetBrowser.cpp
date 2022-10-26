#include "AssetBrowser.h"

#include "ISystem.h"
#include "IResourceManager.h"
#include "MaterialEditor.h"
#include "ShaderGraph.h"
#include "ShaderPipeline.h"

#include "PropertyPanel.h"

#include "../EditorGUI/EditorGui.h"
#include "backends/imgui_impl_vulkan.h"

BalEditor::CAssetBrowser::CAssetBrowser()
        : m_pUnknownIcon{ nullptr },
          m_pFolderIcon{ nullptr },
          m_pBalbinoIcon{ nullptr },
          m_pImageIcon{ nullptr },
          m_pAudioIcon{ nullptr },
          m_pModelIcon{ nullptr },
          m_pPresetIcon{ nullptr },
          m_pCodeIcon{ nullptr },
          m_pFontIcon{ nullptr },
          m_pMaterialIcon{ nullptr },
          m_pShaderIcon{ nullptr },
          m_pVkDescriptorSetUnknownIcon{ VK_NULL_HANDLE },
          m_pVkDescriptorSetFolderIcon{ VK_NULL_HANDLE },
          m_pVkDescriptorSetBalbinoIcon{ VK_NULL_HANDLE },
          m_pVkDescriptorSetImageIcon{ VK_NULL_HANDLE },
          m_pVkDescriptorSetAudioIcon{ VK_NULL_HANDLE },
          m_pVkDescriptorSetModelIcon{ VK_NULL_HANDLE },
          m_pVkDescriptorSetPresetIcon{ VK_NULL_HANDLE },
          m_pVkDescriptorSetCodeIcon{ VK_NULL_HANDLE },
          m_pVkDescriptorSetFontIcon{ VK_NULL_HANDLE },
          m_pVkDescriptorSetMaterialIcon{ VK_NULL_HANDLE },
          m_pVkDescriptorSetShaderIcon{ VK_NULL_HANDLE },
          m_pShaderGraph{ nullptr },
          m_pMaterialEditor{ nullptr },
          m_pPropertyPanel{ nullptr },
          m_pSystem{ nullptr },
          m_isVisible{ true },
          m_size{ 32.f },
          m_isItemSelected{},
          m_isContextMenuOpen{},
          m_wasContextMenuOpen{},
          m_openItem{},
          m_updateCurrentDirectory{ true }
{
}

BalEditor::CAssetBrowser::~CAssetBrowser() = default;

void BalEditor::CAssetBrowser::Initialize( const ISystem* pSystem )
{
    m_pUnknownIcon  = pSystem->GetResourceManager()->LoadTexture( "../Data/Editor/Icons/UnknownFile.basset" );
    m_pFolderIcon   = pSystem->GetResourceManager()->LoadTexture( "../Data/Editor/Icons/Folder.basset" );
    m_pBalbinoIcon  = pSystem->GetResourceManager()->LoadTexture( "../Data/Editor/Icons/BalbinoFile.basset" );
    m_pImageIcon    = pSystem->GetResourceManager()->LoadTexture( "../Data/Editor/Icons/ImageFile.basset" );
    m_pAudioIcon    = pSystem->GetResourceManager()->LoadTexture( "../Data/Editor/Icons/AudioFile.basset" );
    m_pModelIcon    = pSystem->GetResourceManager()->LoadTexture( "../Data/Editor/Icons/ModelFile.basset" );
    m_pPresetIcon   = pSystem->GetResourceManager()->LoadTexture( "../Data/Editor/Icons/PresetFile.basset" );
    m_pCodeIcon     = pSystem->GetResourceManager()->LoadTexture( "../Data/Editor/Icons/CodeFile.basset" );
    m_pFontIcon     = pSystem->GetResourceManager()->LoadTexture( "../Data/Editor/Icons/FontFile.basset" );
    m_pMaterialIcon = pSystem->GetResourceManager()->LoadTexture( "../Data/Editor/Icons/MaterialFile.basset" );
    m_pShaderIcon   = pSystem->GetResourceManager()->LoadTexture( "../Data/Editor/Icons/ShaderFile.basset" );
    
    m_pVkDescriptorSetUnknownIcon  = static_cast< VkDescriptorSet >( ImGui_ImplVulkan_AddTexture(
            m_pUnknownIcon->GetSampler(), m_pUnknownIcon->GetImageView(), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL ));
    m_pVkDescriptorSetFolderIcon   = static_cast< VkDescriptorSet >( ImGui_ImplVulkan_AddTexture(
            m_pFolderIcon->GetSampler(), m_pFolderIcon->GetImageView(), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL ));
    m_pVkDescriptorSetBalbinoIcon  = static_cast< VkDescriptorSet >( ImGui_ImplVulkan_AddTexture(
            m_pBalbinoIcon->GetSampler(), m_pBalbinoIcon->GetImageView(), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL ));
    m_pVkDescriptorSetImageIcon    = static_cast< VkDescriptorSet >( ImGui_ImplVulkan_AddTexture(
            m_pImageIcon->GetSampler(), m_pImageIcon->GetImageView(), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL ));
    m_pVkDescriptorSetAudioIcon    = static_cast< VkDescriptorSet >( ImGui_ImplVulkan_AddTexture(
            m_pAudioIcon->GetSampler(), m_pAudioIcon->GetImageView(), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL ));
    m_pVkDescriptorSetModelIcon    = static_cast< VkDescriptorSet >( ImGui_ImplVulkan_AddTexture(
            m_pModelIcon->GetSampler(), m_pModelIcon->GetImageView(), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL ));
    m_pVkDescriptorSetPresetIcon   = static_cast< VkDescriptorSet >( ImGui_ImplVulkan_AddTexture(
            m_pPresetIcon->GetSampler(), m_pPresetIcon->GetImageView(), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL ));
    m_pVkDescriptorSetCodeIcon     = static_cast< VkDescriptorSet >( ImGui_ImplVulkan_AddTexture(
            m_pCodeIcon->GetSampler(), m_pCodeIcon->GetImageView(), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL ));
    m_pVkDescriptorSetFontIcon     = static_cast< VkDescriptorSet >( ImGui_ImplVulkan_AddTexture(
            m_pFontIcon->GetSampler(), m_pFontIcon->GetImageView(), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL ));
    m_pVkDescriptorSetMaterialIcon = static_cast< VkDescriptorSet >( ImGui_ImplVulkan_AddTexture(
            m_pMaterialIcon->GetSampler(), m_pMaterialIcon->GetImageView(), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL ));
    m_pVkDescriptorSetShaderIcon   = static_cast< VkDescriptorSet >( ImGui_ImplVulkan_AddTexture(
            m_pShaderIcon->GetSampler(), m_pShaderIcon->GetImageView(), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL ));
    
    m_pSystem              = pSystem;
    m_currentDirectoryName = "..\\Data";
}

void BalEditor::CAssetBrowser::Draw()
{
    FindAllFiles();
    if ( m_updateCurrentDirectory )
    {
        GetAllFilesInSelectedPath( m_currentDirectoryName, m_currentDirectory );
        m_updateCurrentDirectory = false;
        std::ranges::sort( m_currentDirectory, []( const SFile& left, const SFile& right )
        {
            return left.fileName < right.fileName;
        } );
        std::ranges::sort( m_currentDirectory, []( const SFile& left, const SFile& right )
        {
            return left.isFolder > right.isFolder;
        } );
    }
    if ( BalEditor::EditorGUI::Begin( "Asset Browser", m_isVisible, 0 ))
    {
        if ( BalEditor::EditorGUI::BeginChild( "Asset Tree", { 128, 0 }, true, 1 << 11 ))
        {
            uint32_t nodeIdx{};
            DrawTree( "..\\Data", nodeIdx );
        }
        BalEditor::EditorGUI::EndChild();
        BalEditor::EditorGUI::SameLine();
        if ( BalEditor::EditorGUI::BeginChild( "Asset File", { 0, 0 }, true, 1 << 6 ))
        {
            if ( BalEditor::EditorGUI::BeginChild( "Icon Size Slider",
                                                   { 0, BalEditor::EditorGUI::GetContentRegionAvail().y - 32 }, false,
                                                   1 << 6 ))
            {
                int  id{};
                bool wasOneSelected{};
                for ( const auto& currentFile : m_currentDirectory )
                {
                    bool isSelected{};
                    BalEditor::EditorGUI::DrawSelectable( id++, isSelected, m_size * 1.5f );
                    HandelSelected( currentFile, isSelected );
                    switch ( currentFile.type )
                    {
                        case EFileTypes::Folder:
                        {
                            BalEditor::EditorGUI::DrawResourceItem( currentFile, m_pVkDescriptorSetFolderIcon, m_size );
                            break;
                        }
                        case EFileTypes::Scene:
                        {
                            BalEditor::EditorGUI::DrawResourceItem( currentFile, m_pVkDescriptorSetBalbinoIcon,
                                                                    m_size );
                            break;
                        }
                        case EFileTypes::Image:
                        {
                            //todo:: get image data
                            BalEditor::EditorGUI::DrawResourceItem( currentFile, m_pVkDescriptorSetImageIcon, m_size );
                            break;
                        }
                        case EFileTypes::Audio:
                        {
                            BalEditor::EditorGUI::DrawResourceItem( currentFile, m_pVkDescriptorSetAudioIcon, m_size );
                            break;
                        }
                        case EFileTypes::Code:
                        {
                            BalEditor::EditorGUI::DrawResourceItem( currentFile, m_pVkDescriptorSetCodeIcon, m_size );
                            break;
                        }
                        case EFileTypes::Unknown:
                        {
                            BalEditor::EditorGUI::DrawResourceItem( currentFile, m_pVkDescriptorSetUnknownIcon,
                                                                    m_size );
                            break;
                        }
                        case EFileTypes::Font: //todo add font icon
                        {
                            BalEditor::EditorGUI::DrawResourceItem( currentFile, m_pVkDescriptorSetFontIcon, m_size );
                            break;
                        }
                        case EFileTypes::Model:
                        {
                            BalEditor::EditorGUI::DrawResourceItem( currentFile, m_pVkDescriptorSetModelIcon, m_size );
                            break;
                        }
                        case EFileTypes::Preset:
                        {
                            BalEditor::EditorGUI::DrawResourceItem( currentFile, m_pVkDescriptorSetPresetIcon, m_size );
                            break;
                        }
                        case EFileTypes::Shader:
                        {
                            BalEditor::EditorGUI::DrawResourceItem( currentFile, m_pVkDescriptorSetShaderIcon, m_size );
                            break;
                        }
                        case EFileTypes::Material:
                            BalEditor::EditorGUI::DrawResourceItem( currentFile, m_pVkDescriptorSetMaterialIcon,
                                                                    m_size );
                            break;
                        default:;
                    }
                    wasOneSelected |= isSelected;
                }
                DrawContextMenu( wasOneSelected );
            }
            BalEditor::EditorGUI::EndChild();
            BalEditor::EditorGUI::DrawFloatSlider( "Icon Size", m_size, 32.0f, 128.f, false );
        }
        BalEditor::EditorGUI::EndChild();
    }
    BalEditor::EditorGUI::End();
}

void BalEditor::CAssetBrowser::Cleanup()
{
    m_currentDirectory.clear();
    m_files.clear();
}

void BalEditor::CAssetBrowser::ShowWindow()
{
    m_isVisible = true;
    BalEditor::EditorGUI::SetWindowFocus( "Asset Browser" );
}

void BalEditor::CAssetBrowser::SetShaderGraphReference( CShaderGraph* pShaderGraph, CMaterialEditor* pMaterialEditor, CPropertyPanel* pPropertyPanel )
{
    m_pShaderGraph    = pShaderGraph;
    m_pMaterialEditor = pMaterialEditor;
    m_pPropertyPanel  = pPropertyPanel;
}

void BalEditor::CAssetBrowser::FindAllFiles()
{
    auto it = m_files.begin();
    while ( it != m_files.end())
    {
        if ( !std::filesystem::exists( it->path ))
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
    
    if ( m_files.empty())
    {
        m_files.push_back( GetData( std::filesystem::relative( "..\\Data" )));
        m_files.back().lastWrittenTime = std::filesystem::last_write_time( "..\\Data" );
    }
    
    for ( auto file = std::filesystem::recursive_directory_iterator(
            "..\\Data" ); file != std::filesystem::recursive_directory_iterator(); ++file )
    {
        
        auto currentFileLastWriteTime = std::filesystem::last_write_time( *file );
        
        // File creation
        auto filetIt = std::ranges::find_if( m_files, [ &file ]( const SFile& filePair ) -> bool
        {
            return filePair.path == file->path();
        } );
        if ( filetIt == m_files.end())
        {
            //CreateNew
            m_files.push_back( GetData( relative( file->path())));
            m_files.back().lastWrittenTime = currentFileLastWriteTime;
            m_files.back().depth           = file.depth() + 1;
            m_updateCurrentDirectory = true;
        }
        else if ( filetIt->lastWrittenTime != currentFileLastWriteTime )
        {
            //Changed
            ( *filetIt ) = GetData( relative( file->path()));
            filetIt->lastWrittenTime = currentFileLastWriteTime;
            filetIt->depth           = file.depth() + 1;
            m_updateCurrentDirectory = true;
            
        }
    }
}

void BalEditor::CAssetBrowser::DrawTree( const std::string& path, uint32_t& nodeIdx )
{
    const auto& fileIter = std::ranges::find_if( std::as_const( m_files ), [ &path ]( const SFile& f ) -> bool
    {
        return f.path == path;
    } );
    if ( fileIter == m_files.cend())
    {
        return;
    }
    
    std::vector<std::string>     files;
    constexpr ImGuiTreeNodeFlags baseFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
    ImGuiTreeNodeFlags           nodeFlags = baseFlags;
    for ( auto& f : m_files )
    {
        if ( f.path != path && f.path.find(
                path ) != std::string::npos && f.isFolder && f.depth - fileIter->depth <= 1 && std::ranges::find_if(
                files, [ &f ]( const std::string& string )
                {
                    return string.find( f.path ) != std::string::npos;
                } ) == files.cend())
        {
            files.push_back( f.path );
        }
    }
    if ( files.empty())
    {
        nodeFlags |= 1 << 8;
    }
    if ( !fileIter->depth )
    {
        nodeFlags |= 1 << 5;
        BalEditor::EditorGUI::SetNextItemOpen( true );
    }
    if ( BalEditor::EditorGUI::TreeNodeEx((uint64_t) fileIter->fileName.c_str(), nodeFlags, fileIter->fileName.c_str()))
    {
        if ( BalEditor::EditorGUI::IsItemClicked())
        {
            m_updateCurrentDirectory = true;
            m_currentDirectoryName   = std::filesystem::relative( fileIter->path ).string();
        }
        for ( const auto& f : files )
        {
            DrawTree( f, nodeIdx );
        }
        BalEditor::EditorGUI::TreePop();
    }
    else
    {
        if ( BalEditor::EditorGUI::IsItemClicked())
        {
            m_updateCurrentDirectory = true;
            m_currentDirectoryName   = std::filesystem::relative( fileIter->path ).string();
        }
    }
}

void BalEditor::CAssetBrowser::GetAllFilesInSelectedPath( std::string path, std::vector<SFile>& filesInDirectory )
{
    const auto& fileIter = std::ranges::find_if( std::as_const( m_files ), [ &path ]( const SFile& f ) -> bool
    {
        return f.path == path;
    } );
    if ( fileIter == m_files.cend())
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
    for ( auto& f : m_files )
    {
        if ( f.path != path && f.path.find( path ) != std::string::npos && f.depth - fileIter->depth == 1 )
        {
            filesInDirectory.push_back( f );
        }
    }
}

void BalEditor::CAssetBrowser::HandelSelected( const SFile& currentFile, bool isSelected )
{
    switch ( currentFile.type )
    {
        case EFileTypes::Unknown:
            break;
        case EFileTypes::Folder:
            if (( BalEditor::EditorGUI::IsMouseDoubleClicked( 0 ) && isSelected ) || m_openItem )
            {
                const std::string currentPath{ currentFile.path };
                m_updateCurrentDirectory = true;
                m_currentDirectoryName   = std::filesystem::relative( currentPath ).string();
                return;
            }
            for ( int i = 0; i < (int) EFileTypes::MaxFileTypes; ++i )
            {
                if ( void* pData = BalEditor::EditorGUI::ReceiveDragDrop( ToString( EFileTypes(i) )))
                {
                    SFile* pFile{ static_cast<SFile*>(pData) };
                    MoveFile( pFile, currentFile.path + "/" );
                    return;
                }
            }
        case EFileTypes::Shader:
            if (( BalEditor::EditorGUI::IsMouseDoubleClicked( 0 ) && isSelected ) || m_openItem )
            {
                m_pShaderGraph->SetShader( currentFile );
                m_pShaderGraph->ShowWindow();
            }
            break;
        case EFileTypes::Material:
            if (( BalEditor::EditorGUI::IsMouseDoubleClicked( ImGuiMouseButton_Left ) && isSelected ) || m_openItem )
            {
                m_pMaterialEditor->SetMaterial( currentFile );
                m_pMaterialEditor->ShowWindow();
            }
            break;
        case EFileTypes::Scene:
        case EFileTypes::Image:
        case EFileTypes::Audio:
        case EFileTypes::Font:
        case EFileTypes::Model:
        case EFileTypes::Code:
        case EFileTypes::Preset:
        case EFileTypes::MaxFileTypes:
            break;
    }
}

const char* BalEditor::CAssetBrowser::GetCurrentDirectory()
{
    return m_currentDirectoryName.c_str();
}

void BalEditor::CAssetBrowser::MoveFile( SFile* pFile, const std::filesystem::path& destination )
{
    Rename( pFile->path, destination.string() + "\\" + pFile->fileName );
}

void BalEditor::CAssetBrowser::Rename( const std::filesystem::path& oldName, const std::filesystem::path& newName )
{
    std::filesystem::rename( oldName, newName );
}
void BalEditor::CAssetBrowser::DrawContextMenu( bool isSelected )
{
    bool create;
    bool showInExplorer;
    m_openItem = false;
    bool rename;
    bool shouldDelete;
    bool copyPath;
    bool import;
    bool reimport;
    bool reimportAll;
    bool properties;
    m_isContextMenuOpen = false;
    if ( BalEditor::EditorGUI::BeginPopupContextWindow( 1 ))
    {
        m_isContextMenuOpen = true;
        if ( !m_wasContextMenuOpen )
        {
            m_isItemSelected = isSelected;
        }
        create              = BalEditor::EditorGUI::MenuItem( "Create" );
        showInExplorer      = BalEditor::EditorGUI::MenuItem( "Show in Explorer" );
        m_openItem          = BalEditor::EditorGUI::MenuItem( "Open", m_isItemSelected );
        rename              = BalEditor::EditorGUI::MenuItem( "Rename", m_isItemSelected );
        shouldDelete        = BalEditor::EditorGUI::MenuItem( "Delete", m_isItemSelected );
        copyPath            = BalEditor::EditorGUI::MenuItem( "Copy Path" );
        BalEditor::EditorGUI::Separator();
        import                   = BalEditor::EditorGUI::MenuItem( "Import new asset" );
        m_updateCurrentDirectory = BalEditor::EditorGUI::MenuItem( "Refresh" );
        reimport                 = BalEditor::EditorGUI::MenuItem( "Reimport" );
        reimportAll              = BalEditor::EditorGUI::MenuItem( "Reimport All" );
        BalEditor::EditorGUI::Separator();
        properties = BalEditor::EditorGUI::MenuItem( "Properties..." );
        BalEditor::EditorGUI::EndPopup();
        
        (void) create;
        (void) showInExplorer;
        (void) rename;
        (void) shouldDelete;
        (void) copyPath;
        (void) import;
        (void) reimport;
        (void) reimportAll;
        if ( properties )
        {
            m_pPropertyPanel->ShowWindow();
        }
    }
    m_wasContextMenuOpen = m_isContextMenuOpen;
    
}

