#include "AssetBrowser.h"

#include "ISystem.h"
#include "IResourceManager.h"
#include "MaterialEditor.h"
#include "ShaderGraph.h"
#include "ShaderPipeline.h"

#include "PropertyPanel.h"

#include "../EditorGUI/EditorGui.h"
#include "backends/imgui_impl_vulkan.h"
#include "../Tools/FilesSystem/Exporter.h"

BalEditor::CAssetBrowser::CAssetBrowser()
        : m_pUnknownIcon{ nullptr }
          , m_pFolderIcon{ nullptr }
          , m_pBalbinoIcon{ nullptr }
          , m_pImageIcon{ nullptr }
          , m_pAudioIcon{ nullptr }
          , m_pModelIcon{ nullptr }
          , m_pPresetIcon{ nullptr }
          , m_pCodeIcon{ nullptr }
          , m_pFontIcon{ nullptr }
          , m_pMaterialIcon{ nullptr }
          , m_pShaderIcon{ nullptr }
          , m_pVkDescriptorSetUnknownIcon{ VK_NULL_HANDLE }
          , m_pVkDescriptorSetFolderIcon{ VK_NULL_HANDLE }
          , m_pVkDescriptorSetBalbinoIcon{ VK_NULL_HANDLE }
          , m_pVkDescriptorSetImageIcon{ VK_NULL_HANDLE }
          , m_pVkDescriptorSetAudioIcon{ VK_NULL_HANDLE }
          , m_pVkDescriptorSetModelIcon{ VK_NULL_HANDLE }
          , m_pVkDescriptorSetPresetIcon{ VK_NULL_HANDLE }
          , m_pVkDescriptorSetCodeIcon{ VK_NULL_HANDLE }
          , m_pVkDescriptorSetFontIcon{ VK_NULL_HANDLE }
          , m_pVkDescriptorSetMaterialIcon{ VK_NULL_HANDLE }
          , m_pVkDescriptorSetShaderIcon{ VK_NULL_HANDLE }
          , m_pShaderGraph{ nullptr }
          , m_pMaterialEditor{ nullptr }
          , m_pPropertyPanel{ nullptr }
          , m_pSystem{ nullptr }
          , m_isVisible{ true }
          , m_size{ 32.f }
          , m_isItemSelected{}
          , m_isContextMenuOpen{}
          , m_wasContextMenuOpen{}
          , m_openItem{}
          , m_updateCurrentDirectory{ true }
          , m_itemToBeCreated{ true, EFileTypes::Folder, 0, "", "", {}, 0  }
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
    if ( GUI::Begin( "Asset Browser", m_isVisible, 0 ))
    {
        if ( GUI::BeginChild( "Asset Tree", { 128, 0 }, true, 1 << 11 ))
        {
            uint32_t nodeIdx{};
            DrawTree( "..\\Data", nodeIdx );
        }
        GUI::EndChild();
        GUI::SameLine();
        if ( GUI::BeginChild( "Asset File", { 0, 0 }, true, 1 << 6 ))
        {
            if ( GUI::BeginChild( "Icon Size Slider", { 0, GUI::GetContentRegionAvail().y - 32 }, false, 1 << 6 ))
            {
                int  id{};
                bool wasOneSelected{};
                for ( const auto& currentFile : m_currentDirectory )
                {
                    bool isSelected{};
                    GUI::DrawSelectable( id++, isSelected, m_size * 1.5f );
                    HandelSelected( currentFile, isSelected );
                    switch ( currentFile.type )
                    {
                        case EFileTypes::Folder:
                        {
                            GUI::DrawResourceItem( currentFile, m_pVkDescriptorSetFolderIcon, m_size );
                            break;
                        }
                        case EFileTypes::Scene:
                        {
                            GUI::DrawResourceItem( currentFile, m_pVkDescriptorSetBalbinoIcon, m_size );
                            break;
                        }
                        case EFileTypes::Image:
                        {
                            //todo:: get image data
                            GUI::DrawResourceItem( currentFile, m_pVkDescriptorSetImageIcon, m_size );
                            break;
                        }
                        case EFileTypes::Audio:
                        {
                            GUI::DrawResourceItem( currentFile, m_pVkDescriptorSetAudioIcon, m_size );
                            break;
                        }
                        case EFileTypes::Code:
                        {
                            GUI::DrawResourceItem( currentFile, m_pVkDescriptorSetCodeIcon, m_size );
                            break;
                        }
                        case EFileTypes::Unknown:
                        {
                            GUI::DrawResourceItem( currentFile, m_pVkDescriptorSetUnknownIcon, m_size );
                            break;
                        }
                        case EFileTypes::Font: //todo add font icon
                        {
                            GUI::DrawResourceItem( currentFile, m_pVkDescriptorSetFontIcon, m_size );
                            break;
                        }
                        case EFileTypes::Model:
                        {
                            GUI::DrawResourceItem( currentFile, m_pVkDescriptorSetModelIcon, m_size );
                            break;
                        }
                        case EFileTypes::Preset:
                        {
                            GUI::DrawResourceItem( currentFile, m_pVkDescriptorSetPresetIcon, m_size );
                            break;
                        }
                        case EFileTypes::Shader:
                        {
                            GUI::DrawResourceItem( currentFile, m_pVkDescriptorSetShaderIcon, m_size );
                            break;
                        }
                        case EFileTypes::Material:
                            GUI::DrawResourceItem( currentFile, m_pVkDescriptorSetMaterialIcon, m_size );
                            break;
                        default:;
                    }
                    wasOneSelected |= isSelected;
                }
                DrawContextMenu( wasOneSelected );
            }
            GUI::EndChild();
            GUI::DrawFloatSlider( "Icon Size", m_size, 32.0f, 128.f, false );
        }
        GUI::EndChild();
        if ( !m_itemToBeCreated.isFolder )
        {
            CreateItem();
        }
    }
    GUI::End();
}

void BalEditor::CAssetBrowser::Cleanup()
{
    m_currentDirectory.clear();
    m_files.clear();
}

void BalEditor::CAssetBrowser::ShowWindow()
{
    m_isVisible = true;
    GUI::SetWindowFocus( "Asset Browser" );
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
        GUI::SetNextItemOpen( true );
    }
    if ( GUI::TreeNodeEx((uint64_t) fileIter->fileName.c_str(), nodeFlags, fileIter->fileName.c_str()))
    {
        if ( GUI::IsItemClicked())
        {
            m_updateCurrentDirectory = true;
            m_currentDirectoryName   = std::filesystem::relative( fileIter->path ).string();
        }
        for ( const auto& f : files )
        {
            DrawTree( f, nodeIdx );
        }
        GUI::TreePop();
    }
    else
    {
        if ( GUI::IsItemClicked())
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
            if (( GUI::IsMouseDoubleClicked( 0 ) && isSelected ) || m_openItem )
            {
                const std::string currentPath{ currentFile.path };
                m_updateCurrentDirectory = true;
                m_currentDirectoryName   = std::filesystem::relative( currentPath ).string();
                return;
            }
            for ( int i = 0; i < (int) EFileTypes::MaxFileTypes; ++i )
            {
                if ( void* pData = GUI::ReceiveDragDrop( ToString( EFileTypes( i ))))
                {
                    SFile* pFile{ static_cast<SFile*>(pData) };
                    MoveFile( pFile, currentFile.path + "/" );
                    return;
                }
            }
        case EFileTypes::Shader:
            if (( GUI::IsMouseDoubleClicked( 0 ) && isSelected ) || m_openItem )
            {
                m_pShaderGraph->ShowWindow( currentFile );
            }
            break;
        case EFileTypes::Material:
            if (( GUI::IsMouseDoubleClicked( ImGuiMouseButton_Left ) && isSelected ) || m_openItem )
            {
                m_pMaterialEditor->ShowWindow( currentFile );
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
    if ( GUI::BeginPopupContextWindow( 1 ))
    {
        m_isContextMenuOpen = true;
        if ( !m_wasContextMenuOpen )
        {
            m_isItemSelected = isSelected;
        }
        if ( GUI::BeginMenu( "Create" ))
        {
            if ( GUI::MenuItem( "Shader" ))
            {
                m_itemToBeCreated.isFolder = false;
                m_itemToBeCreated.path     = m_currentDirectoryName;
                m_itemToBeCreated.fileName = "";
                m_itemToBeCreated.type     = EFileTypes::Shader;
            }
            if ( GUI::MenuItem( "Material" ))
            {
                m_itemToBeCreated.isFolder = false;
                m_itemToBeCreated.path     = m_currentDirectoryName;
                m_itemToBeCreated.fileName = "";
                m_itemToBeCreated.type     = EFileTypes::Material;
            }
            GUI::EndMenu();
        }
        showInExplorer = GUI::MenuItem( "Show in Explorer" );
        m_openItem     = GUI::MenuItem( "Open", m_isItemSelected );
        rename         = GUI::MenuItem( "Rename", m_isItemSelected );
        shouldDelete   = GUI::MenuItem( "Delete", m_isItemSelected );
        copyPath       = GUI::MenuItem( "Copy Path" );
        GUI::Separator();
        import                   = GUI::MenuItem( "Import new asset" );
        m_updateCurrentDirectory = GUI::MenuItem( "Refresh" );
        reimport                 = GUI::MenuItem( "Reimport" );
        reimportAll              = GUI::MenuItem( "Reimport All" );
        GUI::Separator();
        properties = GUI::MenuItem( "Properties..." );
        GUI::EndPopup();
        
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
void BalEditor::CAssetBrowser::CreateItem()
{
    bool saved{};
    bool cancel{};
    if ( GUI::StartPopup( "Create New Item", true, { 256, -1 } ))
    {
        char      name[64]{};
        bool      inputChanged;
        for ( int i{}; i < 64; ++i )
        {
            if ( i > m_itemToBeCreated.fileName.size())
            {
                break;
            }
            name[i] = m_itemToBeCreated.fileName[i];
        }
        inputChanged = GUI::DrawInputText( "new name", name, 64 );
        saved        = GUI::DrawButton( "Save" );
        GUI::SameLine();
        cancel = GUI::DrawButton( "Cancel" );
        GUI::EndPopup();
        if ( inputChanged )
        {
            m_itemToBeCreated.fileName = "";
            m_itemToBeCreated.fileName.append( name, std::find_if( name, name + 64, []( char c )
            {
                return c == '\0';
            } ));
        }
        if ( saved && !m_itemToBeCreated.fileName.empty())
        {
            m_itemToBeCreated.isFolder = true;
        }
        if ( cancel )
        {
            m_itemToBeCreated.isFolder = true;
        }
    }
    if ( cancel || !saved || m_itemToBeCreated.fileName.empty())
    {
        return;
    }
    
    switch ( m_itemToBeCreated.type )
    {
        //shader
        case EFileTypes::Shader:
        {
            Exporter::ExportShader( m_itemToBeCreated.fileName, m_currentDirectoryName + "/", 0,
                                    {{ std::numeric_limits<uint32_t>().max() }}, CUuid(), "" );
            break;
        }
            //material
        case EFileTypes::Material:
        {
            Exporter::ExportMaterial( m_itemToBeCreated.fileName, m_currentDirectoryName + "/",
                                      m_pPropertyPanel->GetDefaultShader(),
                                      m_pSystem->GetResourceManager()->GetShader( m_pPropertyPanel->GetDefaultShader(),
                                                                                  true )->GetShaderResources(),
                                      CUuid());
            break;
        }
        case EFileTypes::Folder:    //todo
        case EFileTypes::Scene:     //todo
        case EFileTypes::Image:
        case EFileTypes::Audio:
        case EFileTypes::Font:
        case EFileTypes::Model:
        case EFileTypes::Code:
        case EFileTypes::Preset:
        case EFileTypes::Unknown:
        case EFileTypes::MaxFileTypes:
            break;
    }
}

