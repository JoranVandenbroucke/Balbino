#include "ShaderGraph.h"

#include <FileParcer.h>
#include <filesystem>
#include <fstream>
#include <imgui.h>
#include <imnodes.h>
#include <SDL.h>
#include <Shader.h>
#include <set>
#include <shaderc/shaderc.hpp>

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

BalEditor::CShaderGraph::CShaderGraph()
        : m_isVisible{ false }
          , m_wantsToSave{ false }
{
    AddNode( EUiNodeType::VertexOutput, glm::vec2{ 420, 128 } );
    AddNode( EUiNodeType::FragmentOutput, glm::vec2{ 420, 256 } );
}

BalEditor::CShaderGraph::~CShaderGraph()
{
    for( const auto& element : m_nodes )
    {
        delete element;
    }
    m_nodes.clear();
    m_links.clear();
}

void BalEditor::CShaderGraph::Draw()
{
    if( m_isVisible )
    {
        if( ImGui::Begin( "Shader Graph", &m_isVisible, ImGuiWindowFlags_MenuBar ))
        {
            if( ImGui::BeginMenuBar())
            {
                if( ImGui::Button( "Save" ))
                {
                    m_wantsToSave = true;
                }
                ImGui::EndMenuBar();
            }

            ImNodes::BeginNodeEditor();
            if( ImGui::BeginPopupContextWindow( "Add Node", 1, false ))
            {
                ImGui::Text( "Add Node" );
                ImGui::Separator();
                for( int n = 2; n < static_cast<int>( EUiNodeType::MaxIndex ); n++ )
                {
                    if( ImGui::MenuItem( ToString( static_cast<EUiNodeType>( n ))))
                    {
                        const ImVec2 mousePos = ImGui::GetMousePos();
                        AddNode( static_cast<EUiNodeType>( n ), glm::vec2{ mousePos.x, mousePos.y } );
                    }
                }
                ImGui::EndPopup();
            }

            for( INode* node : m_nodes )
            {
                node->Draw();
            }

            for( const SLink& link : m_links )
            {
                ImNodes::Link( link.id, link.startAttr, link.endAttr );
            }

            ImNodes::EndNodeEditor();

            {
                SLink link{};
                if( ImNodes::IsLinkCreated( &link.startNodeId, &link.startAttr, &link.endNodeId, &link.endAttr ))
                {
                    int nodeId{ link.endNodeId };
                    const auto it = std::ranges::find_if( m_nodes, [&nodeId](const INode* node)
                    {
                        return node->GetId() == nodeId;
                    } );
                    if( it != m_nodes.end() && ( *it )->HasFreeAttachment( link.endAttr ))
                    {
                        ( *it )->Attach( link.endAttr );
                        link.id = m_currentId++;
                        m_links.push_back( link );
                    }
                }
            }

            {
                int linkId;
                if( ImNodes::IsLinkDestroyed( &linkId ))
                {
                    const auto iterator = std::ranges::find_if( m_links, [linkId](const SLink& link) -> bool
                    {
                        return link.id == linkId;
                    } );
                    assert( iterator != m_links.end());
                    int nodeId{ iterator->endNodeId };
                    const auto it = std::ranges::find_if( m_nodes, [&nodeId](const INode* node)
                    {
                        return node->GetId() == nodeId;
                    } );
                    ( *it )->Detach( iterator->endAttr );
                    m_links.erase( iterator );
                }
            }
            if( m_wantsToSave )
            {
                Evaluate();
            }
        }
        ImGui::End();
    }
}

void BalEditor::CShaderGraph::ShowWindow()
{
    m_isVisible = true;
    ImGui::SetWindowFocus( "Shader Graph" );
}

void BalEditor::CShaderGraph::OpenShader(const std::filesystem::path& path) const
{
    std::ifstream file( path.string().c_str(), std::ios::in | std::ios::binary );
    if( !file.is_open())
    {
        return;
    }
    uint64_t uuid;
    uint8_t type;
    uint64_t graphSize;
    uint64_t vertexSPRV;
    uint64_t fragmentSPRV;
    BinaryReadWrite::Read( file, uuid );
    BinaryReadWrite::Read( file, type );
    BinaryReadWrite::Read( file, vertexSPRV );
    BinaryReadWrite::Read( file, fragmentSPRV );
    BinaryReadWrite::MoveCursor(file, (int)(vertexSPRV* sizeof( uint32_t )));
    BinaryReadWrite::MoveCursor(file, (int)(fragmentSPRV* sizeof( uint32_t )));
    BinaryReadWrite::Read( file, graphSize );
    auto pGraphData = static_cast<char*>( malloc( graphSize ));
    BinaryReadWrite::Read( file, pGraphData, graphSize );
    ImNodes::LoadCurrentEditorStateFromIniString( pGraphData, graphSize );
    free( pGraphData );
    file.close();
}

std::vector< BalEditor::CShaderGraph::SLink > BalEditor::CShaderGraph::GetNeighbors(const int currentNode)
{
    std::vector< SLink > links;
    auto it = m_links.begin();

    while(( it = std::find_if( it, m_links.end(), [&currentNode](const SLink& link)
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
    if( m_currentName.empty())
    {
        ImGui::OpenPopup( "Enter Name" );

        // Always center this window when appearing
        ImVec2 center = ImGui::GetMainViewport()->GetCenter();
        ImGui::SetNextWindowPos( center, ImGuiCond_Appearing, ImVec2( 0.5f, 0.5f ));
        if( ImGui::BeginPopupModal( "Enter Name", nullptr, ImGuiWindowFlags_AlwaysAutoResize ))
        {
            char name[64]{};
            if( ImGui::InputText( "##shader name", name, 64, ImGuiInputTextFlags_AlwaysInsertMode | ImGuiInputTextFlags_EnterReturnsTrue ))
            {
                m_currentName.append( name, std::find_if( name, name + 64, [](char c)
                {
                    return c == '\0';
                } ));
            }
            ImGui::EndPopup();
        }
    }

    if( !m_currentName.empty())
    {
        std::vector< INode* > postOrder;
        std::vector< INode* > stack;

        stack.push_back( m_nodes[0] );

        //todo: save nodes that are used multiple times to separate variable
        while( !stack.empty())
        {
            INode* pCurrentNode = stack.back();
            stack.pop_back();

            postOrder.push_back( pCurrentNode );

            std::vector connected{ GetNeighbors( pCurrentNode->GetId()) };
            for( const SLink& neighbor : connected )
            {
                int nodeId = neighbor.startNodeId;
                auto nodeIterator = std::ranges::find_if( m_nodes, [nodeId](const INode* pNode)
                {
                    return pNode->GetId() == nodeId;
                } );
                stack.push_back( *nodeIterator );
            }
        }

        std::vector< INode* >::iterator nextNode{};
        if( postOrder.empty())
        {
            nextNode = postOrder.end();
        }
        else
        {
            nextNode = postOrder.begin() + 1;
        }

        std::set< std::string > includesVertex{};
        std::set< std::string > bindingsVertex{};
        std::string includes{};
        std::string bindings{};
        std::string vertexShader{};
        int bindingId{ 6 };
        vertexShader = m_nodes[0]->Evaluate( nextNode, bindingsVertex, includesVertex, EAttributeType::None );
        for( const std::string& include : includesVertex )
        {
            includes += "#include \"" + include + "\"\n";
        }
        for( const std::string& binding : bindingsVertex )
        {
            bindings += std::format( "layout(set=0, binding={}) uniform {};\n", bindingId++, binding );
        }
        if( !bindings.empty())
        {
            vertexShader.insert( vertexShader.find( "#version 450\n" ) + 13, bindings );
        }
        if( !includes.empty())
        {
            vertexShader.insert( vertexShader.find( "#version 450\n" ) + 13, includes );
        }

        //add fragment shader
        postOrder.clear();
        stack.clear();

        stack.push_back( m_nodes[1] );

        //todo: save nodes that are used multiple times to separate variable
        while( !stack.empty())
        {
            INode* pCurrentNode = stack.back();
            stack.pop_back();
            postOrder.push_back( pCurrentNode );

            std::vector connected{ GetNeighbors( pCurrentNode->GetId()) };
            for( const SLink& neighbor : connected )
            {
                int nodeId = neighbor.startNodeId;
                auto nodeIterator = std::ranges::find_if( m_nodes, [nodeId](const INode* pNode)
                {
                    return pNode->GetId() == nodeId;
                } );
                stack.push_back( *nodeIterator );
            }
        }

        if( postOrder.empty())
        {
            nextNode = postOrder.end();
        }
        else
        {
            nextNode = postOrder.begin() + 1;
        }

        std::string fragmentShader{};
        std::set< std::string > includesFragment{};
        std::set< std::string > bindingsFragment{};
        fragmentShader = m_nodes[1]->Evaluate( nextNode, bindingsFragment, includesFragment, EAttributeType::None );
        includes = {};
        bindings = {};
        for( const std::string& include : includesFragment )
        {
            includes += "#include \"" + include + "\"\n";
        }
        for( const std::string& binding : bindingsFragment )
        {
            bindings += std::format( "layout(set=0, binding={}) uniform {};\n", bindingId++, binding );
        }
        if( !bindings.empty())
        {
            fragmentShader.insert( fragmentShader.find( "#version 450\n" ) + 13, bindings );
        }
        if( !includes.empty())
        {
            fragmentShader.insert( fragmentShader.find( "#version 450\n" ) + 13, includes );
        }

        //todo: save nodes graph
        uint64_t graphSize{};
        const char* pGraphData{ ImNodes::SaveCurrentEditorStateToIniString( &graphSize ) };

        const shaderc::Compiler compiler;
        shaderc::CompileOptions options;
        options.SetTargetEnvironment( shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_2 );
        options.SetOptimizationLevel( shaderc_optimization_level_performance );
        options.SetIncluder( std::make_unique< BalVulkan::CFileIncluder >());
        shaderc::SpvCompilationResult result = compiler.CompileGlslToSpv( vertexShader.c_str(), vertexShader.size(), shaderc_vertex_shader, ( "../Data/Editor/Shaders/" + m_currentName + ".vert" ).c_str(), options );
        if( result.GetCompilationStatus() != shaderc_compilation_status_success )
        {
            //handle errors
            std::cout << result.GetErrorMessage();
            assert( false );
        }
        const std::vector vertexSPRV( result.cbegin(), result.cend());
        std::cout << "fragment shader\n" << fragmentShader;
        result = compiler.CompileGlslToSpv( fragmentShader.c_str(), fragmentShader.size(), shaderc_fragment_shader, ( "../Data/Editor/Shaders/" + m_currentName + ".frag" ).c_str(), options );
        if( result.GetCompilationStatus() != shaderc_compilation_status_success )
        {
            //handle errors
            std::cout << result.GetErrorMessage();
            assert( false );
        }
        const std::vector fragmentSPRV( result.cbegin(), result.cend());

        std::filesystem::path p{ std::string( "../Data/" ) + m_currentName };
        p.replace_extension( ".basset" );
        std::ofstream file( p.string().c_str(), std::ios::out | std::ios::binary );
        if( !file.is_open())
        {
            return;
        }

        BinaryReadWrite::Write( file, (uint64_t) CUuid());
        BinaryReadWrite::Write( file, static_cast<uint8_t>( EFileTypes::Shader ));
        BinaryReadWrite::Write( file, vertexSPRV.size());
        BinaryReadWrite::Write( file, fragmentSPRV.size());
        BinaryReadWrite::Write( file, vertexSPRV.data(), vertexSPRV.size() * sizeof( uint32_t ));
        BinaryReadWrite::Write( file, fragmentSPRV.data(), fragmentSPRV.size() * sizeof( uint32_t ));
        BinaryReadWrite::Write( file, graphSize );
        BinaryReadWrite::Write( file, pGraphData, graphSize );
        file.close();
        m_wantsToSave = false;
    }
}

void BalEditor::CShaderGraph::AddNode(const EUiNodeType type, const glm::vec2& position)
{
    INode* pNode{ nullptr };
    const int id = m_currentId++;
    switch( type )
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
        default:;
    }
    ImNodes::SetNodeScreenSpacePos( id, { position.x, position.y } );
    m_nodes.push_back( pNode );
}

const char* BalEditor::CShaderGraph::ToString(const EUiNodeType type)
{
    switch( type )
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
        case EUiNodeType::MaxIndex:
            break;
        case EUiNodeType::ImageTexture:
            return "Image Texture";
    }
    return "nullptr";
}
