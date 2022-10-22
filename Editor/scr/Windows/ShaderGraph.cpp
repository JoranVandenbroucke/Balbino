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
    AddNode( EUiNodeType::ShaderNode, glm::vec2{ 480, 128 } );
    AddNode( EUiNodeType::VertexOutput, glm::vec2{ 420, 128 } );
    AddNode( EUiNodeType::FragmentOutput, glm::vec2{ 420, 256 } );
}

BalEditor::CShaderGraph::~CShaderGraph()
{
    for ( const auto& element : m_nodes )
    {
        delete element;
    }
    m_nodes.clear();
    m_links.clear();
}

void BalEditor::CShaderGraph::Draw()
{
    if ( BalEditor::EditorGUI::Begin( "Shader Graph", m_isVisible, 1 << 10 ))
    {
        if ( BalEditor::EditorGUI::BeginMenuBar())
        {
            if ( BalEditor::EditorGUI::DrawButton( "Save" ))
            {
                m_wantsToSave = true;
            }
            BalEditor::EditorGUI::EndMenuBar();
        }
        
        ImNodes::BeginNodeEditor();
        if ( int idx = BalEditor::EditorGUI::DrawPopupContextWindow( "Add Node", m_allNodeNames ) != -1 )
        {
            glm::vec2 mousePos = BalEditor::EditorGUI::GetMousePos();
            AddNode( static_cast<EUiNodeType>( idx + 4 ), mousePos );
        }
        
        for ( INode* node : m_nodes )
        {
            node->Draw();
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
                const auto it = std::ranges::find_if( m_nodes, [ &nodeId ]( const INode* node )
                {
                    return node->GetId() == nodeId;
                } );
                if ( it != m_nodes.end() && ( *it )->HasFreeAttachment( link.endAttr ))
                {
                    ( *it )->Attach( link.endAttr );
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
                const auto it = std::ranges::find_if( m_nodes, [ &nodeId ]( const INode* node )
                {
                    return node->GetId() == nodeId;
                } );
                ( *it )->Detach( iterator->endAttr );
                m_links.erase( iterator );
            }
        }
        if ( m_wantsToSave )
        {
            Evaluate();
        }
        BalEditor::EditorGUI::End();
    }
}

void BalEditor::CShaderGraph::ShowWindow()
{
    m_isVisible = true;
    
    BalEditor::EditorGUI::SetWindowFocus( "Shader Graph" );
    if ( m_links.empty())
    {
        m_links.emplace_back(
                SLink{ .id = m_currentId++, .startNodeId = 1, .endNodeId = 0, .startAttr = 10, .endAttr = 0 } );
        m_links.emplace_back(
                SLink{ .id = m_currentId++, .startNodeId = 2, .endNodeId = 0, .startAttr = 24, .endAttr = 2 } );
    }
}

void BalEditor::CShaderGraph::SetShader( const SFile& shaderFile )
{
    std::ifstream file( shaderFile.path, std::ios::in | std::ios::binary );
    if ( !file.is_open())
    {
        return;
    }
    
    //todo: Get Shader Graph Debug Infor
    file.close();
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
    if ( m_currentShaderFile.isFolder )
    {
        if ( BalEditor::EditorGUI::StartPopup( "Save Shader", true, { 256, -1 } ))
        {
            char name[64]{};
            bool inputChanged;
            bool saved;
            bool cancel;
            inputChanged = BalEditor::EditorGUI::DrawInputText( "new name", name, 64, 100.f, 1 << 5 );
            saved        = BalEditor::EditorGUI::DrawButton( "Save" );
            BalEditor::EditorGUI::SameLine();
            cancel = BalEditor::EditorGUI::DrawButton( "Cancel" );
            BalEditor::EditorGUI::EndPopup();
            
            if ( inputChanged )
            {
                m_currentShaderFile.fileName.append( name, std::find_if( name, name + 64, []( char c )
                {
                    return c == '\0';
                } ));
                m_currentShaderFile.isFolder = false;
            }
            if ( saved && name[0] )
            {
                m_currentShaderFile.fileName.append( name, std::find_if( name, name + 64, []( char c )
                {
                    return c == '\0';
                } ));
                m_currentShaderFile.isFolder = false;
            }
            if ( cancel )
            {
                m_currentShaderFile.isFolder = true;
                m_currentShaderFile.fileName = "";
                m_wantsToSave = false;
                return;
            }
        }
    }
    
    if ( !m_currentShaderFile.isFolder )
    {
        std::vector<INode*> postOrder;
        std::vector<INode*> stack;
        
        stack.push_back( m_nodes[0] );
        
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
                auto nodeIterator = std::ranges::find_if( m_nodes, [ nodeId ]( const INode* pNode )
                {
                    return pNode->GetId() == nodeId;
                } );
                stack.push_back( *nodeIterator );
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
            shader = m_nodes[0]->Evaluate( nextNode, bindings, includes, EAttributeType::None );
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
            const char* extention{ ".vert" };
            switch ( std::stoi( shader.substr( shader.size() - 4, 3 )))
            {
                case 1u:
                    extention  = ".geom";
                    shaderType = shaderc_geometry_shader;
                    break;
                case 2u:
                    if ( uint32_t( shader[shader.size() - 1] ) == 51u )
                    {
                        extention  = ".frag";
                        shaderType = shaderc_fragment_shader;
                    }
                    break;
                case 3u:
                    if ( uint32_t( shader[shader.size() - 1] ) == 49u )
                    {
                        extention  = ".rgen";
                        shaderType = shaderc_raygen_shader;
                    }
                    else if ( uint32_t( shader[shader.size() - 1] ) == 50u )
                    {
                        extention  = ".rhit";
                        shaderType = shaderc_anyhit_shader;
                    }
                    else if ( uint32_t( shader[shader.size() - 1] ) == 51u )
                    {
                        extention  = ".rhit";
                        shaderType = shaderc_closesthit_shader;
                    }
                    else if ( uint32_t( shader[shader.size() - 1] ) == 52u )
                    {
                        extention  = ".miss";
                        shaderType = shaderc_miss_shader;
                    }
                    else if ( uint32_t( shader[shader.size() - 1] ) == 53u )
                    {
                        extention  = ".inter";
                        shaderType = shaderc_intersection_shader;
                    }
                    break;
            }
            shaderc::SpvCompilationResult result = compiler.CompileGlslToSpv( shader.c_str(), shader.size() - 4,
                                                                              shaderType,
                                                                              ( "../Data/Editor/Shaders/" + m_currentShaderFile.fileName + extention ).c_str(),
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
        BalEditor::Exporter::ExportShader( m_currentShaderFile.fileName, "../Data/",
                                           (uint16_t) std::stoi( shader.substr( shader.size() - 4, 3 )),
                                           compiledShaders );
        m_wantsToSave = false;
    }
}

void BalEditor::CShaderGraph::AddNode( const EUiNodeType type, const glm::vec2& position )
{
    INode* pNode{ nullptr };
    const int id = m_currentId++;
    switch ( type )
    {
        case EUiNodeType::VertexOutput:
            pNode = new CVertexOutputNode{ id, m_currentAttributeId };
            break;
        case EUiNodeType::FragmentOutput:
            pNode = new CFragmentOutputNode{ id, m_currentAttributeId };
            break;
        case EUiNodeType::Math:
            pNode = new CMathNode{ id, m_currentAttributeId };
            break;
        case EUiNodeType::Clamp:
            pNode = new CClamp{ id, m_currentAttributeId };
            break;
        case EUiNodeType::BrightContrast:
            pNode = new CBrightContrast{ id, m_currentAttributeId };
            break;
        case EUiNodeType::Gamma:
            pNode = new CGamma{ id, m_currentAttributeId };
            break;
        case EUiNodeType::HueSaturationValue:
            pNode = new CHueSaturationValue{ id, m_currentAttributeId };
            break;
        case EUiNodeType::Invert:
            pNode = new CInvert{ id, m_currentAttributeId };
            break;
        case EUiNodeType::Mix:
            pNode = new CMix{ id, m_currentAttributeId };
            break;
        case EUiNodeType::Bump:
            pNode = new CBump{ id, m_currentAttributeId };
            break;
        case EUiNodeType::Displacement:
            pNode = new CDisplacement{ id, m_currentAttributeId };
            break;
        case EUiNodeType::Mapping:
            pNode = new CMapping{ id, m_currentAttributeId };
            break;
        case EUiNodeType::NormalMap:
            pNode = new CNormalMap{ id, m_currentAttributeId };
            break;
        case EUiNodeType::RGBtoBW:
            pNode = new CRGBtoBW{ id, m_currentAttributeId };
            break;
        case EUiNodeType::VectorMath:
            pNode = new CVectorMath{ id, m_currentAttributeId };
            break;
        case EUiNodeType::ImageTexture:
            pNode = new CImageTexture{ id, m_currentAttributeId };
            break;
        case EUiNodeType::ShaderNode:
            pNode = new CShaderNode{ id, m_currentAttributeId };
            break;
        case EUiNodeType::GeometryOutput:
        case EUiNodeType::MaxIndex:
        default:
            break;
    }
    ImNodes::SetNodeScreenSpacePos( id, position );
    m_nodes.push_back( pNode );
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
