#include "AssetBrowser.h"

#include "IManager.h"
#include "IResourceManager.h"
#include "MaterialEditor.h"
#include "ShaderGraph.h"
#include "ShaderPipeline.h"

#include "imgui.h"
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
          m_pSystem{ nullptr },
          m_isVisible{ true },
          m_newFile{ false },
          m_size{ 32.f },
          m_newFileFronID{ 0 },
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
    m_currentDirectoryName = "../Data";
}

void BalEditor::CAssetBrowser::Draw()
{
    FindAllFiles();
    if ( m_updateCurrentDirectory )
    {
        GetAllFilesInSelectedPath( m_currentDirectoryName, m_currentDirectory );
        m_updateCurrentDirectory = false;
    }
    if ( m_isVisible )
    {
        if ( ImGui::Begin( "Asset Browser", &m_isVisible ))
        {
            if ( ImGui::BeginChild( "Asset Tree", ImVec2{ 128, -1 }, true, ImGuiWindowFlags_HorizontalScrollbar ))
            {
                uint32_t nodeIdx{};
                DrawTree( "..\\Data", nodeIdx );
            }
            ImGui::EndChild();
            std::ranges::sort( m_currentDirectory, []( const SFile& left, const SFile& right )
            {
                return left.fileName < right.fileName;
            } );
            std::ranges::sort( m_currentDirectory, []( const SFile& left, const SFile& right )
            {
                return left.isFolder > right.isFolder;
            } );
            
            ImGui::SameLine();
            if ( ImGui::BeginChild( "Asset File", ImVec2{ -1, -1 }, true, ImGuiWindowFlags_AlwaysAutoResize ))
            {
                int  id{};
                bool isSelected{ false };
                for ( const auto& currentFile : m_currentDirectory )
                {
                    isSelected = false;
                    ImGui::Selectable(( "##file" + std::to_string( id++ )).c_str(), &isSelected,
                                      ImGuiSelectableFlags_AllowDoubleClick, ImVec2{ 0, m_size * 1.05f } );
                    HandelSelected( currentFile, isSelected );
                    if ( ImGui::BeginDragDropSource( ImGuiDragDropFlags_None ))
                    {
                        ImGui::SetDragDropPayload( ToString( currentFile.type ), &currentFile, sizeof( SFile ));
                        ImGui::Text( "%s", currentFile.fileName.c_str());
                        ImGui::EndDragDropSource();
                    }
                    ImGui::SameLine();
                    switch ( currentFile.type )
                    {
                        case EFileTypes::Folder:
                        {
                            ImGui::Image( m_pVkDescriptorSetFolderIcon, { m_size, m_size } );
                            break;
                        }
                        case EFileTypes::Scene:
                        {
                            ImGui::Image( m_pVkDescriptorSetBalbinoIcon, { m_size, m_size } );
                            break;
                        }
                        case EFileTypes::Image:
                        {
                            //todo:: get image data
                            ImGui::Image( m_pVkDescriptorSetImageIcon, { m_size, m_size } );
                            break;
                        }
                        case EFileTypes::Audio:
                        {
                            ImGui::Image( m_pVkDescriptorSetAudioIcon, { m_size, m_size } );
                            break;
                        }
                        case EFileTypes::Code:
                        {
                            ImGui::Image( m_pVkDescriptorSetCodeIcon, { m_size, m_size } );
                            break;
                        }
                        case EFileTypes::Unknown:
                        {
                            ImGui::Image( m_pVkDescriptorSetUnknownIcon, { m_size, m_size } );
                            break;
                        }
                        case EFileTypes::Font: //todo add font icon
                        {
                            ImGui::Image( m_pVkDescriptorSetFontIcon, { m_size, m_size } );
                            break;
                        }
                        case EFileTypes::Model:
                        {
                            ImGui::Image( m_pVkDescriptorSetModelIcon, { m_size, m_size } );
                            break;
                        }
                        case EFileTypes::Preset:
                        {
                            ImGui::Image( m_pVkDescriptorSetPresetIcon, { m_size, m_size } );
                            break;
                        }
                        case EFileTypes::Shader:
                        {
                            ImGui::Image( m_pVkDescriptorSetShaderIcon, { m_size, m_size } );
                            break;
                        }
                        case EFileTypes::Material:
                            ImGui::Image( m_pVkDescriptorSetMaterialIcon, { m_size, m_size } );
                            break;
                        default:;
                    }
                    ImGui::SameLine();
                    ImGui::Text( "%s", currentFile.fileName.c_str());
                    ImGui::NewLine();
                }
                ImGui::SliderFloat( "Icon Size", &m_size, 8.0f, 128.f, "%.0f", ImGuiSliderFlags_NoInput );
            }
            ImGui::EndChild();
        }
        ImGui::End();
    }
}

void BalEditor::CAssetBrowser::Cleanup()
{
    m_currentDirectory.clear();
    m_files.clear();
}

void BalEditor::CAssetBrowser::ShowWindow()
{
    m_isVisible = true;
    ImGui::SetWindowFocus( "Asset Browser" );
}

void BalEditor::CAssetBrowser::SetShaderGraphReference( CShaderGraph* pShaderGraph, CMaterialEditor* pMaterialEditor )
{
    m_pShaderGraph    = pShaderGraph;
    m_pMaterialEditor = pMaterialEditor;
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
    
    for ( auto file = std::filesystem::recursive_directory_iterator( "..\\Data" );
          file != std::filesystem::recursive_directory_iterator(); ++file )
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

void BalEditor::CAssetBrowser::CreateMaterial( const SFile& file, std::string_view name ) const
{
    std::filesystem::path path = file.path;
    path.replace_filename( name );
    path.replace_extension( ".basset" );
    const BalVulkan::CShaderPipeline* pResources = m_pSystem->GetResourceManager()->LoadShader( file.path );
    std::ofstream materialFile( path, std::ios::out | std::ios::binary );
    if ( !materialFile.is_open() && pResources )
    {
        return;
    }
    
    const std::unordered_map<std::string, BalVulkan::SShaderResource>& resources = pResources->GetShaderResources();
    std::vector<BalVulkan::SShaderResource> shaderResource;
    shaderResource.reserve( resources.size());
    /*** Copy all value fields from map to a vector using transform() & Lambda function ***/
    std::ranges::transform( resources, std::back_inserter( shaderResource ),
                            []( const std::pair<std::string, BalVulkan::SShaderResource>& pair )
                            {
                                return pair.second;
                            } );
    BinaryReadWrite::Write( materialFile, (uint64_t) CUuid());
    BinaryReadWrite::Write( materialFile, (uint8_t) EFileTypes::Material );
    BinaryReadWrite::Write( materialFile, file.uuid );
    BinaryReadWrite::Write( materialFile, shaderResource );
    materialFile.close();
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
    constexpr ImGuiTreeNodeFlags baseFlags =
                                         ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
    ImGuiTreeNodeFlags           nodeFlags = baseFlags;
    for ( auto& f : m_files )
    {
        if ( f.path != path && f.path.find( path ) != std::string::npos && f.isFolder &&
             f.depth - fileIter->depth <= 1 && std::ranges::find_if( files, [ &f ]( const std::string& string )
        {
            return string.find( f.path ) != std::string::npos;
        } ) == files.cend())
        {
            files.push_back( f.path );
        }
    }
    if ( files.empty())
    {
        nodeFlags |= ImGuiTreeNodeFlags_Leaf /*| ImGuiTreeNodeFlags_NoTreePushOnOpen*/;
    }
    if ( !fileIter->depth )
    {
        nodeFlags |= ImGuiTreeNodeFlags_DefaultOpen;
        ImGui::SetNextItemOpen( true );
    }
    if ( ImGui::TreeNodeEx( fileIter->fileName.c_str(), nodeFlags ))
    {
        if ( ImGui::IsItemClicked( ImGuiMouseButton_Left ))
        {
            GetAllFilesInSelectedPath( std::filesystem::relative( fileIter->path ).string(), m_currentDirectory );
            m_currentDirectoryName = std::filesystem::relative( fileIter->path ).string();
        }
        for ( const auto& f : files )
        {
            DrawTree( f, nodeIdx );
        }
        ImGui::TreePop();
    }
    else
    {
        if ( ImGui::IsItemClicked( ImGuiMouseButton_Left ))
        {
            GetAllFilesInSelectedPath( std::filesystem::relative( fileIter->path ).string(), m_currentDirectory );
            m_currentDirectoryName = std::filesystem::relative( fileIter->path ).string();
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
        if ( f.path != path && f.path.find( path ) != std::string::npos &&
             f.depth - fileIter->depth == 1 &&
             std::find_if( filesInDirectory.cbegin(), filesInDirectory.cend(), [ &f ]( const SFile& string )
             {
                 return string.path.find( f.path ) != std::string::npos;
             } ) == filesInDirectory.cend())
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
            if ( ImGui::IsMouseDoubleClicked( ImGuiMouseButton_Left ) && isSelected )
            {
                const std::string currentPath{ currentFile.path };
                m_updateCurrentDirectory = true;
                m_currentDirectoryName   = std::filesystem::relative( currentPath ).string();
            }
            if ( ImGui::BeginDragDropTarget())
            {
                if ( const ImGuiPayload* payload = ImGui::AcceptDragDropPayload( ToString( EFileTypes::Shader )))
                {
                    SFile* pFile{ static_cast<SFile*>(payload->Data) };
                    MoveFile( pFile, currentFile.path );
                }
                ImGui::EndDragDropTarget();
            }
            break;
        case EFileTypes::Shader:
            if ( ImGui::IsMouseDoubleClicked( ImGuiMouseButton_Left ) && isSelected )
            {
                m_pShaderGraph->SetShader( currentFile );
                m_pShaderGraph->ShowWindow();
            }
            else if ( ImGui::BeginPopupContextItem(( "##ShaderOptions" + std::to_string( currentFile.uuid )).c_str()))
            {
                if ( ImGui::MenuItem( "Open Shader Editor" ))
                {
                    m_pShaderGraph->SetShader( currentFile );
                    m_pShaderGraph->ShowWindow();
                }
                else if ( ImGui::MenuItem( "Create New Material" ))
                {
                    m_newFile       = true;
                    m_currentName   = "";
                    m_newFileFronID = currentFile.uuid;
                }
                ImGui::EndPopup();
            }
            else if ( m_newFile && m_currentName.empty() && m_newFileFronID == currentFile.uuid )
            {
                ImGui::OpenPopup( "Enter Name" );
                
                // Always center this window when appearing
                ImVec2 center = ImGui::GetMainViewport()->GetCenter();
                ImGui::SetNextWindowPos( center, ImGuiCond_Appearing, ImVec2( 0.5f, 0.5f ));
                if ( ImGui::BeginPopupModal( "Enter Name", nullptr, ImGuiWindowFlags_AlwaysAutoResize ))
                {
                    char name[64]{};
                    if ( ImGui::InputText(( "##material name" + std::to_string( currentFile.uuid )).c_str(), name, 64,
                                          ImGuiInputTextFlags_AlwaysInsertMode | ImGuiInputTextFlags_EnterReturnsTrue ))
                    {
                        m_currentName.append( name, std::find_if( name, name + 64, []( char c )
                        {
                            return c == '\0';
                        } ));
                        //TODO CHANGE
                        CreateMaterial( currentFile, m_currentName );
                        m_newFile = false;
                    }
                }
                ImGui::EndPopup();
            }
            break;
        case EFileTypes::Material:
            if ( ImGui::IsMouseDoubleClicked( ImGuiMouseButton_Left ) && isSelected )
            {
                m_pMaterialEditor->SetMaterial( currentFile );
                m_pMaterialEditor->ShowWindow();
            }
            else if ( ImGui::BeginPopupContextItem(( "##MaterialOptions" + std::to_string( currentFile.uuid )).c_str()))
            {
                if ( ImGui::MenuItem( "Open Material Editor" ))
                {
                    m_pMaterialEditor->SetMaterial( currentFile );
                    m_pMaterialEditor->ShowWindow();
                }
                ImGui::EndPopup();
            }
            break;
        case EFileTypes::Scene:
        case EFileTypes::Image:
        case EFileTypes::Audio:
        case EFileTypes::Font:
        case EFileTypes::Model:
        case EFileTypes::Code:
        case EFileTypes::Preset:
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
    pFile->path = destination.string() + "\\" + pFile->fileName;
}

void BalEditor::CAssetBrowser::Rename( const std::filesystem::path& oldName, std::filesystem::path newName )
{
    std::filesystem::rename( oldName, newName );
}

