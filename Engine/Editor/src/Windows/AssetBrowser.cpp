#include "AssetBrowser.h"

#include "ISystem.h"
#include "IResourceManager.h"
#include "MaterialEditor.h"
#include "ShaderEditor.h"
#include "ShaderPipeline.h"

#include "PropertyPanel.h"

#include "../Renderer/EditorGui.h"
#include "../Renderer/Imgui/backends/imgui_impl_vulkan.h"
#include "../Tools/FilesSystem/Exporter.h"

FawnForge::CAssetBrowser::~CAssetBrowser() = default;

void FawnForge::CAssetBrowser::Initialize( const ISystem* pSystem )
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
    
    m_pVkDescriptorSetUnknownIcon  = ImGui_ImplVulkan_AddTexture(
            m_pUnknownIcon->GetSampler(), m_pUnknownIcon->GetImageView(), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
    );
    m_pVkDescriptorSetFolderIcon   = ImGui_ImplVulkan_AddTexture(
            m_pFolderIcon->GetSampler(), m_pFolderIcon->GetImageView(), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
    );
    m_pVkDescriptorSetBalbinoIcon  = ImGui_ImplVulkan_AddTexture(
            m_pBalbinoIcon->GetSampler(), m_pBalbinoIcon->GetImageView(), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
    );
    m_pVkDescriptorSetImageIcon    = ImGui_ImplVulkan_AddTexture(
            m_pImageIcon->GetSampler(), m_pImageIcon->GetImageView(), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
    );
    m_pVkDescriptorSetAudioIcon    = ImGui_ImplVulkan_AddTexture(
            m_pAudioIcon->GetSampler(), m_pAudioIcon->GetImageView(), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
    );
    m_pVkDescriptorSetModelIcon    = ImGui_ImplVulkan_AddTexture(
            m_pModelIcon->GetSampler(), m_pModelIcon->GetImageView(), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
    );
    m_pVkDescriptorSetPresetIcon   = ImGui_ImplVulkan_AddTexture(
            m_pPresetIcon->GetSampler(), m_pPresetIcon->GetImageView(), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
    );
    m_pVkDescriptorSetCodeIcon     = ImGui_ImplVulkan_AddTexture(
            m_pCodeIcon->GetSampler(), m_pCodeIcon->GetImageView(), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
    );
    m_pVkDescriptorSetFontIcon     = ImGui_ImplVulkan_AddTexture(
            m_pFontIcon->GetSampler(), m_pFontIcon->GetImageView(), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
    );
    m_pVkDescriptorSetMaterialIcon = ImGui_ImplVulkan_AddTexture(
            m_pMaterialIcon->GetSampler(), m_pMaterialIcon->GetImageView(), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
    );
    m_pVkDescriptorSetShaderIcon   = ImGui_ImplVulkan_AddTexture(
            m_pShaderIcon->GetSampler(), m_pShaderIcon->GetImageView(), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
    );
    
    m_pSystem              = pSystem;
    m_currentDirectoryName = "..\\Data";
}

void FawnForge::CAssetBrowser::Draw()
{
    FindAllFiles();
    if ( m_updateCurrentDirectory )
    {
        GetAllFilesInSelectedPath( m_currentDirectoryName, m_currentDirectory );
        m_updateCurrentDirectory = false;
        std::ranges::sort(
                m_currentDirectory, []( const SFile& left, const SFile& right )
                {
                    return left.fileName < right.fileName;
                }
        );
        std::ranges::sort(
                m_currentDirectory, []( const SFile& left, const SFile& right )
                {
                    return (int)left.isFolder > (int)right.isFolder;
                }
        );
    }
    if ( Gui::Begin( "Asset Browser", m_isVisible, 0 ))
    {
        if ( Gui::BeginChild( "Asset Tree", { 128, 0 }, true, 1 << 11 ))
        {
            uint32_t nodeIdx{};
            DrawTree( "..\\Data", nodeIdx );
        }
        Gui::EndChild();
        Gui::SameLine();
        if ( Gui::BeginChild( "Asset File", { 0, 0 }, true, 1 << 6 ))
        {
            if ( Gui::BeginChild( "Icon Size Slider", { 0, Gui::GetContentRegionAvail().y - 32 }, false, 1 << 6 ))
            {
                int  id{};
                bool wasOneSelected{};
                for ( const auto& currentFile : m_currentDirectory )
                {
                    bool isSelected{};
                    Gui::DrawSelectable( id++, isSelected, m_size * 1.5f );
                    HandelSelected( currentFile, isSelected );
                    switch ( currentFile.type )
                    {
                        case file_type::file_type_folder:
                        {
                            Gui::DrawResourceItem( currentFile, m_pVkDescriptorSetFolderIcon, m_size );
                            break;
                        }
                        case file_type::file_type_scene:
                        {
                            Gui::DrawResourceItem( currentFile, m_pVkDescriptorSetBalbinoIcon, m_size );
                            break;
                        }
                        case file_type::file_type_image:
                        {
                            //todo:: get image data
                            Gui::DrawResourceItem( currentFile, m_pVkDescriptorSetImageIcon, m_size );
                            break;
                        }
                        case file_type::file_type_audio:
                        {
                            Gui::DrawResourceItem( currentFile, m_pVkDescriptorSetAudioIcon, m_size );
                            break;
                        }
                        case file_type::file_typec_code:
                        {
                            Gui::DrawResourceItem( currentFile, m_pVkDescriptorSetCodeIcon, m_size );
                            break;
                        }
                        case file_type::file_type_unknown:
                        {
                            Gui::DrawResourceItem( currentFile, m_pVkDescriptorSetUnknownIcon, m_size );
                            break;
                        }
                        case file_type::file_type_font: //todo add font icon
                        {
                            Gui::DrawResourceItem( currentFile, m_pVkDescriptorSetFontIcon, m_size );
                            break;
                        }
                        case file_type::file_type_model:
                        {
                            Gui::DrawResourceItem( currentFile, m_pVkDescriptorSetModelIcon, m_size );
                            break;
                        }
                        case file_type::file_type_preset:
                        {
                            Gui::DrawResourceItem( currentFile, m_pVkDescriptorSetPresetIcon, m_size );
                            break;
                        }
                        case file_type::file_type_shader:
                        {
                            Gui::DrawResourceItem( currentFile, m_pVkDescriptorSetShaderIcon, m_size );
                            break;
                        }
                        case file_type::file_type_material:
                            Gui::DrawResourceItem( currentFile, m_pVkDescriptorSetMaterialIcon, m_size );
                            break;
                        default:;
                    }
                    wasOneSelected |= isSelected;
                }
                DrawContextMenu( wasOneSelected );
            }
            Gui::EndChild();
            Gui::DrawFloatSlider( "Icon Size", m_size, 32.0f, 128.f, false );
        }
        Gui::EndChild();
        if ( !m_itemToBeCreated.isFolder )
        {
            CreateItem();
        }
    }
    if(m_isVisible)
        Gui::End();
}

void FawnForge::CAssetBrowser::Cleanup()
{
    m_currentDirectory.clear();
    m_files.clear();
}

void FawnForge::CAssetBrowser::ShowWindow()
{
    m_isVisible = true;
    Gui::SetWindowFocus( "Asset Browser" );
}

void FawnForge::CAssetBrowser::SetShaderGraphReference( CShaderEditor* pShaderGraph, CMaterialEditor* pMaterialEditor, CPropertyPanel* pPropertyPanel )
{
    m_pShaderGraph    = pShaderGraph;
    m_pMaterialEditor = pMaterialEditor;
    m_pPropertyPanel  = pPropertyPanel;
}

void FawnForge::CAssetBrowser::FindAllFiles()
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
            "..\\Data"
    ); file != std::filesystem::recursive_directory_iterator(); ++file )
    {
        
        auto currentFileLastWriteTime = std::filesystem::last_write_time( *file );
        
        // File creation
        auto filetIt = std::ranges::find_if(
                m_files, [ &file ]( const SFile& filePair )
                {
                    return filePair.path == file->path();
                }
        );
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

void FawnForge::CAssetBrowser::DrawTree( const std::string& path, uint32_t& nodeIdx )
{
    const auto& fileIter = std::ranges::find_if(
            std::as_const( m_files ), [ &path ]( const SFile& f )
            {
                return f.path == path;
            }
    );
    if ( fileIter == m_files.cend())
    {
        return;
    }
    
    std::vector<std::string>     files;
    constexpr ImGuiTreeNodeFlags baseFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
    ImGuiTreeNodeFlags           nodeFlags = baseFlags;
    for ( const auto& f : m_files )
    {
        if ( f.path != path && f.path.find(
                path
        ) != std::string::npos && f.isFolder && f.depth - fileIter->depth <= 1 && std::ranges::find_if(
                files, [ &f ]( std::string_view string )
                {
                    return string.find( f.path ) != std::string::npos;
                }
        ) == files.cend())
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
        Gui::SetNextItemOpen( true );
    }
    if ( Gui::TreeNodeEx((uint64_t) fileIter->fileName.c_str(), nodeFlags, fileIter->fileName.c_str()))
    {
        if ( Gui::IsItemClicked())
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
        if ( Gui::IsItemClicked())
        {
            m_updateCurrentDirectory = true;
            m_currentDirectoryName   = std::filesystem::relative( fileIter->path ).string();
        }
    }
    for ( int i = 0; i < (int) file_type::file_type_max; ++i )
    {
        if ( void* pData = Gui::ReceiveDragDrop( ToString( file_type( i ))))
        {
            const SFile* pFile{ static_cast<SFile*>(pData) };
            MoveFile( pFile, path.c_str());
            return;
        }
    }
}

void FawnForge::CAssetBrowser::GetAllFilesInSelectedPath( std::string path, std::vector<SFile>& filesInDirectory )
{
    const auto& fileIter = std::ranges::find_if(
            std::as_const( m_files ), [ &path ]( const SFile& f )
            {
                return f.path == path;
            }
    );
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
        case file_type::file_type_folder:
            if (( Gui::IsMouseDoubleClicked( 0 ) && isSelected ) || m_openItem )
            {
                const std::string currentPath{ currentFile.path };
                m_updateCurrentDirectory = true;
                m_currentDirectoryName   = std::filesystem::relative( currentPath ).string();
                return;
            }
            for ( int i = 0; i < (int) file_type::file_type_max; ++i )
            {
                if ( void* pData = Gui::ReceiveDragDrop( ToString( file_type( i ))))
                {
                    const SFile* pFile{ static_cast<SFile*>(pData) };
                    MoveFile( pFile, currentFile.path + "/" );
                    return;
                }
            }
        case file_type::file_type_shader:
            if (( Gui::IsMouseDoubleClicked( 0 ) && isSelected ) || m_openItem )
            {
                m_pShaderGraph->ShowWindow( currentFile );
            }
            break;
        case file_type::file_type_material:
            if (( Gui::IsMouseDoubleClicked( ImGuiMouseButton_Left ) && isSelected ) || m_openItem )
            {
                m_pMaterialEditor->ShowWindow( currentFile );
            }
            break;
        case file_type::file_type_scene:
        case file_type::file_type_image:
        case file_type::file_type_audio:
        case file_type::file_type_font:
        case file_type::file_type_model:
        case file_type::file_typec_code:
        case file_type::file_type_preset:
        case file_type::file_type_unknown:
        case file_type::file_type_max:
            break;
    }
}

const char* FawnForge::CAssetBrowser::GetCurrentDirectory()const
{
    return m_currentDirectoryName.c_str();
}

void FawnForge::CAssetBrowser::MoveFile( const SFile* pFile, const std::filesystem::path& destination )const
{
    Rename( pFile->path, destination.string() + "\\" + pFile->fileName );
}

void FawnForge::CAssetBrowser::Rename( const std::filesystem::path& oldName, const std::filesystem::path& newName )const
{
    if ( std::filesystem::exists( oldName ))
    {
        std::filesystem::rename( oldName, newName );
    }
}
void FawnForge::CAssetBrowser::DrawContextMenu( bool isSelected )
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
    m_isContextMenuOpen = false;
    if ( Gui::BeginPopupContextWindow( 1 ))
    {
        m_isContextMenuOpen = true;
        if ( !m_wasContextMenuOpen )
        {
            m_isItemSelected = isSelected;
        }
        if ( Gui::BeginMenu( "Create" ))
        {
            if ( Gui::MenuItem( "Shader" ))
            {
                m_itemToBeCreated.isFolder = false;
                m_itemToBeCreated.path     = m_currentDirectoryName;
                m_itemToBeCreated.fileName = "";
                m_itemToBeCreated.type     = file_type::file_type_shader;
            }
            if ( Gui::MenuItem( "Material" ))
            {
                m_itemToBeCreated.isFolder = false;
                m_itemToBeCreated.path     = m_currentDirectoryName;
                m_itemToBeCreated.fileName = "";
                m_itemToBeCreated.type     = file_type::file_type_material;
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
        
        (void) showInExplorer;
        (void) rename;
        (void) shouldDelete;
        (void) copyPath;
        (void) importFile;
        (void) reimport;
        (void) reimportAll;
        if ( properties )
        {
            m_pPropertyPanel->ShowWindow();
        }
    }
    m_wasContextMenuOpen = m_isContextMenuOpen;
}
void FawnForge::CAssetBrowser::CreateItem()
{
    bool saved{};
    bool cancel{};
    if ( Gui::StartPopup( "Create New Item", true, { 256, -1 } ))
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
        inputChanged = Gui::DrawInputText( "new name", name, 64 );
        saved        = Gui::DrawButton( "Save" );
        Gui::SameLine();
        cancel = Gui::DrawButton( "Cancel" );
        Gui::EndPopup();
        if ( inputChanged )
        {
            m_itemToBeCreated.fileName = "";
            m_itemToBeCreated.fileName.append(
                    name, std::find_if(
                            name, name + 64, []( char c )
                            {
                                return c == '\0';
                            }
                    ));
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
        case file_type::file_type_shader:
        {
            Exporter::ExportShader(
                    m_itemToBeCreated.fileName, m_currentDirectoryName, {}, {}, 1, CUuid(), ""
            );
            break;
        }
            //material
        case file_type::file_type_material:
        {
            Exporter::ExportMaterial(
                    m_itemToBeCreated.fileName,
                    m_currentDirectoryName,
                    m_pPropertyPanel->GetDefaultShader(),
                    m_pSystem->GetResourceManager()->GetShader(
                            m_pPropertyPanel->GetDefaultShader(), true
                    )->GetShaderResources(),
                    CUuid());
            break;
        }
        case file_type::file_type_folder:    //todo
        case file_type::file_type_scene:
        case file_type::file_type_image:
        case file_type::file_type_audio:
        case file_type::file_type_font:
        case file_type::file_type_model:
        case file_type::file_typec_code:
        case file_type::file_type_preset:
        case file_type::file_type_unknown:
        case file_type::file_type_max:
            break;
    }
}

