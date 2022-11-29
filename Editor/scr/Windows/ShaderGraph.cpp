#include "ShaderGraph.h"

#include <FileParcer.h>
#include <filesystem>
#include <fstream>
#include <imnodes.h>
#include <SDL.h>
#include <set>

#include "UUID.h"
#include "Nodes/Bump.h"
#include "Nodes/Clamp.h"
#include "Nodes/ColorNodes.h"
#include "Nodes/Displacement.h"
#include "Nodes/ImageTexture.h"
#include "Nodes/FragmentOutput.h"
#include "Nodes/Mapping.h"
#include "Nodes/MathNode.h"
#include "Nodes/NormalMap.h"
#include "Nodes/RGBtoBW.h"
#include "Nodes/VectorMath.h"
#include "Nodes/VertexOutput.h"
#include "Nodes/ShaderNode.h"
#include "../EditorGUI/EditorGui.h"
#include "../Tools/FilesSystem/Exporter.h"

BalEditor::CShaderGraph::CShaderGraph()
        : m_isVisible{ false },
          m_wantsToSave{ false }
{
    for ( int n{ 4 }; n < (uint32_t) EUiNodeType::MaxIndex; ++n )
    {
        m_allNodeNames.emplace_back( ToString((EUiNodeType) n ));
    }
}

BalEditor::CShaderGraph::~CShaderGraph()
{
    for ( const auto& element : m_nodes )
    {
        delete element.second;
    }
    m_nodes.clear();
    m_links.clear();
}

void BalEditor::CShaderGraph::Draw()
{
    if ( GUI::Begin( "Shader Graph", m_isVisible, 1 << 10 ))
    {
        if ( GUI::BeginMenuBar())
        {
            m_wantsToSave |= GUI::DrawButton( "Save" );
            if ( GUI::DrawButton( "Save as ..." ))
            {
                m_currentShaderFile.fileName = "";
                m_currentShaderFile.isFolder = true;
                m_wantsToSave = true;
            }
            GUI::EndMenuBar();
        }
        
        ImNodes::BeginNodeEditor();
        if ( int idx = GUI::DrawPopupContextWindow( "Add Node", m_allNodeNames ) != -1 )
        {
            glm::vec2 mousePos = GUI::GetMousePos();
            AddNode( static_cast<EUiNodeType>( idx + 4 ), mousePos );
        }
        
        for ( const std::pair<int, INode*>& node : m_nodes )
        {
            node.second->Draw();
        }
        
        for ( const SLink& link : m_links )
        {
            ImNodes::Link( link.id, link.startAttr, link.endAttr );
        }
        
        ImNodes::EndNodeEditor();
        
        {
            SLink link{};
            if ( ImNodes::IsLinkCreated( &link.startNodeId, &link.startAttr, &link.endNodeId, &link.endAttr ))
            {
                int        nodeId{ link.endNodeId };
                const auto it = std::ranges::find_if( m_nodes, [ &nodeId ]( const std::pair<int, INode*>& node )
                {
                    return node.second->GetId() == nodeId;
                } );
                if ( it != m_nodes.end() && ( *it ).second->HasFreeAttachment( link.endAttr ))
                {
                    ( *it ).second->Attach( link.endAttr );
                    link.id = m_currentId++;
                    m_links.push_back( link );
                }
            }
        }
        
        {
            int linkId;
            if ( ImNodes::IsLinkDestroyed( &linkId ))
            {
                const auto iterator = std::ranges::find_if( m_links, [ linkId ]( const SLink& link ) -> bool
                {
                    return link.id == linkId;
                } );
                assert( iterator != m_links.end());
                int        nodeId{ iterator->endNodeId };
                const auto it = std::ranges::find_if( m_nodes, [ &nodeId ]( const std::pair<int, INode*>& node )
                {
                    return node.second->GetId() == nodeId;
                } );
                ( *it ).second->Detach( iterator->endAttr );
                m_links.erase( iterator );
            }
        }
        if ( m_wantsToSave )
        {
            Evaluate();
        }
        GUI::End();
    }
}

void BalEditor::CShaderGraph::ShowWindow( const SFile& shader )
{
    m_isVisible = true;
    
    GUI::SetWindowFocus( "Shader Graph" );
    
    for ( const auto& element : m_nodes )
    {
        delete element.second;
    }
    m_nodes.clear();
    m_links.clear();
    m_currentAttributeId = 0;
    m_currentId          = 0;
    m_currentShaderFile.uuid = 0;
    if ( shader.isFolder || shader.type != EFileTypes::Shader )
    {
        LoadDefaultEditor();
    }
    else
    {
        LoadEditorFromData( shader );
    }
}

std::vector<BalEditor::CShaderGraph::SLink> BalEditor::CShaderGraph::GetNeighbors( const int currentNode )
{
    std::vector<SLink> links;
    auto               it = m_links.begin();
    
    while (( it = std::find_if( it, m_links.end(), [ &currentNode ]( const SLink& link )
    {
        return link.endNodeId == currentNode;
    } )) != m_links.end())
    {
        links.push_back( *it );
        ++it;
    }
    std::ranges::reverse( links );
    return links;
}

void BalEditor::CShaderGraph::Evaluate()
{
    bool canTraverseGraph{};
    if ( m_currentShaderFile.isFolder )
    {
        GUI::PushId( "SaveShader" );
        if ( GUI::StartPopup( "Save Shader", true, { 256, -1 } ))
        {
            char      name[64]{};
            bool      inputChanged;
            bool      saved;
            bool      cancel;
            for ( int i{}; i < 64; ++i )
            {
                if ( i > m_currentShaderFile.fileName.size())
                {
                    break;
                }
                name[i] = m_currentShaderFile.fileName[i];
            }
            inputChanged = GUI::DrawInputText( "new name", name, 64 );
            saved        = GUI::DrawButton( "Save" );
            GUI::SameLine();
            cancel = GUI::DrawButton( "Cancel" );
            GUI::EndPopup();
            if ( inputChanged )
            {
                m_currentShaderFile.fileName = "";
                m_currentShaderFile.fileName.append( name, std::find_if( name, name + 64, []( char c )
                {
                    return c == '\0';
                } ));
            }
            if ( saved && !m_currentShaderFile.fileName.empty())
            {
                m_currentShaderFile.isFolder = false;
                m_wantsToSave    = false;
                canTraverseGraph = true;
            }
            if ( cancel )
            {
                m_currentShaderFile.isFolder = true;
                m_currentShaderFile.fileName = "";
                m_wantsToSave = false;
            }
        }
        GUI::PopID();
    }
    
    if ( canTraverseGraph )
    {
        std::vector<INode*> postOrder;
        std::vector<INode*> stack;
        
        stack.emplace_back( m_nodes[0].second );
        
        //todo: save nodes that are used multiple times to separate variable
        while ( !stack.empty())
        {
            INode* pCurrentNode = stack.back();
            stack.pop_back();
            postOrder.push_back( pCurrentNode );
            
            std::vector connected{ GetNeighbors( pCurrentNode->GetId()) };
            for ( const SLink& neighbor : connected )
            {
                int  nodeId       = neighbor.startNodeId;
                auto nodeIterator = std::ranges::find_if( m_nodes, [ nodeId ]( const std::pair<int, INode*>& node )
                {
                    return node.second->GetId() == nodeId;
                } );
                stack.push_back( nodeIterator->second );
            }
        }
        
        std::vector<INode*>::iterator nextNode{};
        nextNode = postOrder.begin() + 1;
        std::vector<std::vector<uint32_t>> compiledShaders;
        std::string                        shader;
        uint32_t                           bindingId{ 6 };
        while ( nextNode != postOrder.cend())
        {
            std::set<std::string> bindings;
            std::set<std::string> includes;
            std::string           includeString{};
            std::string           bindingString{};
            shader = m_nodes[0].second->Evaluate( nextNode, bindings, includes, EAttributeType::None );
            for ( const std::string& include : includes )
            {
                includeString += "#include \"" + include + "\"\n";
            }
            for ( const std::string& binding : bindings )
            {
                bindingString += std::format( "layout(set=0, binding={}) uniform {};\n", bindingId++, binding );
            }
            if ( !bindingString.empty())
            {
                shader.insert( shader.find( "#version 450\n" ) + 13, bindingString );
            }
            if ( !includeString.empty())
            {
                shader.insert( shader.find( "#version 450\n" ) + 13, includeString );
            }
            
            const shaderc::Compiler compiler;
            shaderc::CompileOptions options;
            options.SetTargetEnvironment( shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_3 );
            options.SetOptimizationLevel( shaderc_optimization_level_performance );
            options.SetWarningsAsErrors();
            options.SetTargetSpirv( shaderc_spirv_version_1_5 );
            options.SetIncluder( std::make_unique<BalVulkan::CFileIncluder>());
            
            shaderc_shader_kind shaderType{ shaderc_vertex_shader };
            const char* extension{ ".vert" };
            switch ( std::stoi( shader.substr( shader.size() - 4, 3 )))
            {
                case 1u:
                    extension  = ".geom";
                    shaderType = shaderc_geometry_shader;
                    break;
                case 2u:
                    if ( uint32_t( shader[shader.size() - 1] ) == 51u )
                    {
                        extension  = ".frag";
                        shaderType = shaderc_fragment_shader;
                    }
                    break;
                case 3u:
                    if ( uint32_t( shader[shader.size() - 1] ) == 49u )
                    {
                        extension  = ".rgen";
                        shaderType = shaderc_raygen_shader;
                    }
                    else if ( uint32_t( shader[shader.size() - 1] ) == 50u )
                    {
                        extension  = ".rhit";
                        shaderType = shaderc_anyhit_shader;
                    }
                    else if ( uint32_t( shader[shader.size() - 1] ) == 51u )
                    {
                        extension  = ".rhit";
                        shaderType = shaderc_closesthit_shader;
                    }
                    else if ( uint32_t( shader[shader.size() - 1] ) == 52u )
                    {
                        extension  = ".miss";
                        shaderType = shaderc_miss_shader;
                    }
                    else if ( uint32_t( shader[shader.size() - 1] ) == 53u )
                    {
                        extension  = ".inter";
                        shaderType = shaderc_intersection_shader;
                    }
                    break;
            }
            shaderc::SpvCompilationResult result = compiler.CompileGlslToSpv( shader.c_str(), shader.size() - 4,
                                                                              shaderType,
                                                                              ( "../Data/Editor/Shaders/" + m_currentShaderFile.fileName + extension ).c_str(),
                                                                              options );
            if ( result.GetCompilationStatus() != shaderc_compilation_status_success )
            {
                //handle errors
                std::cout << shader << std::endl;
                std::cout << result.GetErrorMessage();
                assert( false );
            }
            const std::vector sprv( result.cbegin(), result.cend());
            compiledShaders.push_back( sprv );
        }
        
        std::string editorData{ SaveEditor() };
        BalEditor::Exporter::ExportShader( m_currentShaderFile.fileName, "../Data/",    //todo get current file path
                                           (uint16_t) std::stoi( shader.substr( shader.size() - 4, 3 )),
                                           compiledShaders, m_currentShaderFile.uuid, editorData );
    }
}

void BalEditor::CShaderGraph::AddNode( const EUiNodeType type, const glm::vec2& position, int id )
{
    INode* pNode{ nullptr };
    int nodeID;
    if ( id == -1 )
    {
        nodeID = m_currentId++;
    }
    else
    {
        nodeID      = id;
        m_currentId = std::max( id + 1, m_currentId );
    }
    switch ( type )
    {
        case EUiNodeType::VertexOutput:
            pNode = new CVertexOutputNode{ nodeID, m_currentAttributeId };
            break;
        case EUiNodeType::FragmentOutput:
            pNode = new CFragmentOutputNode{ nodeID, m_currentAttributeId };
            break;
        case EUiNodeType::Math:
            pNode = new CMathNode{ nodeID, m_currentAttributeId };
            break;
        case EUiNodeType::Clamp:
            pNode = new CClamp{ nodeID, m_currentAttributeId };
            break;
        case EUiNodeType::BrightContrast:
            pNode = new CBrightContrast{ nodeID, m_currentAttributeId };
            break;
        case EUiNodeType::Gamma:
            pNode = new CGamma{ nodeID, m_currentAttributeId };
            break;
        case EUiNodeType::HueSaturationValue:
            pNode = new CHueSaturationValue{ nodeID, m_currentAttributeId };
            break;
        case EUiNodeType::Invert:
            pNode = new CInvert{ nodeID, m_currentAttributeId };
            break;
        case EUiNodeType::Mix:
            pNode = new CMix{ nodeID, m_currentAttributeId };
            break;
        case EUiNodeType::Bump:
            pNode = new CBump{ nodeID, m_currentAttributeId };
            break;
        case EUiNodeType::Displacement:
            pNode = new CDisplacement{ nodeID, m_currentAttributeId };
            break;
        case EUiNodeType::Mapping:
            pNode = new CMapping{ nodeID, m_currentAttributeId };
            break;
        case EUiNodeType::NormalMap:
            pNode = new CNormalMap{ nodeID, m_currentAttributeId };
            break;
        case EUiNodeType::RGBtoBW:
            pNode = new CRGBtoBW{ nodeID, m_currentAttributeId };
            break;
        case EUiNodeType::VectorMath:
            pNode = new CVectorMath{ nodeID, m_currentAttributeId };
            break;
        case EUiNodeType::ImageTexture:
            pNode = new CImageTexture{ nodeID, m_currentAttributeId };
            break;
        case EUiNodeType::ShaderNode:
            pNode = new CShaderNode{ nodeID, m_currentAttributeId };
            break;
        case EUiNodeType::GeometryOutput:
        case EUiNodeType::MaxIndex:
        default:
            break;
    }
    ImNodes::SetNodeScreenSpacePos( nodeID, position );
    m_nodes.emplace_back((int) type, pNode );
}

const char* BalEditor::CShaderGraph::ToString( const EUiNodeType type )
{
    switch ( type )
    {
        case EUiNodeType::VertexOutput:
            return "Vertex Output";
        case EUiNodeType::FragmentOutput:
            return "Fragment Output";
        case EUiNodeType::BrightContrast:
            return "BrightContrast";
        case EUiNodeType::Gamma:
            return "Gamma";
        case EUiNodeType::HueSaturationValue:
            return "Hue Saturation Value";
        case EUiNodeType::Invert:
            return "Invert";
        case EUiNodeType::Mix:
            return "Mix";
        case EUiNodeType::Bump:
            return "Bump";
        case EUiNodeType::Displacement:
            return "Displacement";
        case EUiNodeType::Mapping:
            return "Mapping";
        case EUiNodeType::NormalMap:
            return "NormalMap";
        case EUiNodeType::Clamp:
            return "Clamp";
        case EUiNodeType::Math:
            return "Math";
        case EUiNodeType::RGBtoBW:
            return "RGB To BW";
        case EUiNodeType::VectorMath:
            return "Vector Math";
        case EUiNodeType::ImageTexture:
            return "Image Texture";
        case EUiNodeType::ShaderNode:
            return "Shader Output";
        case EUiNodeType::GeometryOutput:
            return "Geometry Shader";
        case EUiNodeType::MaxIndex:
            break;
    }
    return "nullptr";
}
void BalEditor::CShaderGraph::LoadEditorFromData( const SFile& shader )
{
    std::ifstream file( shader.path, std::ios::in | std::ios::binary );
    if ( !file.is_open())
    {
        file.close();
        return;
    }
    
    uint8_t type;
    BinaryReadWrite::Read( file, m_currentShaderFile.uuid );
    BinaryReadWrite::Read( file, type );
    if ( type != (uint8_t) EFileTypes::Shader )
    {
        return;
    }
    
    m_currentShaderFile.type = (EFileTypes) type;
    
    //TODO RTX
    //skip shader info
    uint16_t shaderComboType;
    BinaryReadWrite::Read( file, shaderComboType );
    switch ( shaderComboType )
    {
        case 0u:
        {
            uint64_t size;
            BinaryReadWrite::Read( file, size );
            BinaryReadWrite::MoveCursor( file, (int) ( size * sizeof( uint32_t )));
            if(size == std::numeric_limits<unsigned int>::max())
            {
                file.close();
                LoadDefaultEditor();
                return;
            }
            break;
        }
        case 1u:
        {
            uint64_t size1;
            uint64_t size2;
            uint64_t size3;
            
            BinaryReadWrite::Read( file, size1 );
            BinaryReadWrite::Read( file, size2 );
            BinaryReadWrite::Read( file, size3 );
            
            BinaryReadWrite::MoveCursor( file, (int) ( size1 * sizeof( uint32_t )));
            BinaryReadWrite::MoveCursor( file, (int) ( size2 * sizeof( uint32_t )));
            BinaryReadWrite::MoveCursor( file, (int) ( size3 * sizeof( uint32_t )));
            break;
        }
        case 2u:
        {
            uint64_t size1;
            uint64_t size2;
            
            BinaryReadWrite::Read( file, size1 );
            BinaryReadWrite::Read( file, size2 );
            
            BinaryReadWrite::MoveCursor( file, (int) ( size1 * sizeof( uint32_t )));
            BinaryReadWrite::MoveCursor( file, (int) ( size2 * sizeof( uint32_t )));
            break;
        }
        case 3u:
        default:
            file.close();
            LoadDefaultEditor();
            return;
    }
    m_currentShaderFile = shader;
    int64_t  leftoverSize;
    uint64_t size;
    
    std::vector<std::pair<int, int>> nodeIdAndType;
    BinaryReadWrite::Read( file, leftoverSize );
    BinaryReadWrite::Read( file, size );
    for ( int i = 0; i < size; ++i )
    {
        int nodeType;
        int id;
        BinaryReadWrite::Read( file, nodeType );
        BinaryReadWrite::Read( file, id );
        AddNode( EUiNodeType( nodeType ), glm::vec2{ 512, 128 }, id );
    }
    leftoverSize -= (int64_t) ( size * sizeof( int ) * 2 );
    BinaryReadWrite::Read( file, size );
    m_links.resize( size );
    for ( int i = 0; i < size; ++i )
    {
        BinaryReadWrite::Read( file, m_links[i].id );
        BinaryReadWrite::Read( file, m_links[i].startAttr );
        BinaryReadWrite::Read( file, m_links[i].endAttr );
        BinaryReadWrite::Read( file, m_links[i].startNodeId );
        BinaryReadWrite::Read( file, m_links[i].endNodeId );
    }
    leftoverSize -= (int64_t) ( size * sizeof( int ) * 5 );
    char* editor{ new char[leftoverSize] };
    BinaryReadWrite::Read( file, editor, leftoverSize );
    file.close();
    ImNodes::LoadCurrentEditorStateFromIniString( editor, leftoverSize );
}
void BalEditor::CShaderGraph::LoadDefaultEditor()
{
    AddNode( EUiNodeType::ShaderNode, glm::vec2{ 512, 128 } );
    AddNode( EUiNodeType::VertexOutput, glm::vec2{ 420, 128 } );
    AddNode( EUiNodeType::FragmentOutput, glm::vec2{ 420, 256 } );
    
    m_links.emplace_back(
            SLink{ .id = m_currentId++, .startNodeId = 1, .endNodeId = 0, .startAttr = 10, .endAttr = 0 } );
    m_links.emplace_back(
            SLink{ .id = m_currentId++, .startNodeId = 2, .endNodeId = 0, .startAttr = 24, .endAttr = 2 } );
    if ( m_currentShaderFile.uuid == 0 )
    {
        m_currentShaderFile.uuid = (uint64_t) CUuid();
    }
}
std::string BalEditor::CShaderGraph::SaveEditor()
{
    uint64_t          size{};
    std::string       editorNodes{};
    std::stringstream stringStream;
    const char* editorState{ ImNodes::SaveCurrentEditorStateToIniString( &size ) };
    BinaryReadWrite::Write( stringStream, (uint64_t) m_nodes.size());
    for ( const auto& node : m_nodes )
    {
        BinaryReadWrite::Write( stringStream, node.first );
        BinaryReadWrite::Write( stringStream, node.second->GetId());
    }
    BinaryReadWrite::Write( stringStream, (uint64_t) m_links.size());
    for ( const auto& link : m_links )
    {
        BinaryReadWrite::Write( stringStream, link.id );
        BinaryReadWrite::Write( stringStream, link.startAttr );
        BinaryReadWrite::Write( stringStream, link.endAttr );
        BinaryReadWrite::Write( stringStream, link.startNodeId );
        BinaryReadWrite::Write( stringStream, link.endNodeId );
    }
    editorNodes += stringStream.str();
    editorNodes.append( editorState, editorState + size );
    stringStream.clear();
    return editorNodes;
}
