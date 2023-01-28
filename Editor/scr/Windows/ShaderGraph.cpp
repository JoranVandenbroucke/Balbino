#include "ShaderGraph.h"

#include <filesystem>
#include <fstream>
#include <imnodes.h>
#include <SDL.h>
#include <set>

#include "Nodes/Shaders/ShaderNode.h"
#include "Nodes/Shaders/VertexOutput.h"
#include "Nodes/Shaders/FragmentOutput.h"
#include "Nodes/Colour/ColorNodes.h"
#include "Nodes/Colour/RGBtoBW.h"
#include "Nodes/Math/Clamp.h"
#include "Nodes/Math/MathNode.h"
#include "Nodes/Math/VectorMath.h"
#include "Nodes/Textures/Bump.h"
#include "Nodes/Textures/Displacement.h"
#include "Nodes/Textures/ImageTexture.h"
#include "Nodes/Textures/Mapping.h"
#include "Nodes/Textures/NormalMap.h"
#include "../EditorGUI/EditorGui.h"
#include "../Tools/FilesSystem/Exporter.h"

BalEditor::CShaderGraph::CShaderGraph()
        : m_isVisible{ false }
        , m_wantsToSave{ false }
{
    for ( int n{ 1 }; n < (uint32_t) EUiNodeType::MaxIndex; ++n )
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
        uint8_t flags{};
        CVertexOutputNode  * pVertex{};
        CFragmentOutputNode* pFragment{};
        SLink link{};
        int   linkId;
        
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
        uint64_t idx = GUI::DrawPopupContextWindow( "Add Node", m_allNodeNames );
        if ( idx != -1 )
        {
            glm::vec2 mousePos = GUI::GetMousePos();
            AddNode( static_cast<EUiNodeType>( idx + 1 ), mousePos );
        }
        
        for ( std::pair<int, INode*>& node : m_nodes )
        {
            if ( node.first == (int) EUiNodeType::VertexOutput )
            {
                pVertex = (CVertexOutputNode*) node.second;
            }
            if ( node.first == (int) EUiNodeType::FragmentOutput )
            {
                pFragment = (CFragmentOutputNode*) node.second;
            }
            flags |= node.second->GetVertexFlags();
            node.second->Draw();
        }
        if ( pVertex )
        {
            pVertex->SetVertexFlags( flags );
        }
        if ( pFragment )
        {
            pFragment->SetVertexFlags( flags );
        }
        
        for ( const SLink& linkToDraw : m_links )
        {
            ImNodes::Link( linkToDraw.id, linkToDraw.startAttr, linkToDraw.endAttr );
        }
        
        ImNodes::MiniMap();
        ImNodes::EndNodeEditor();
        
        if ( ImNodes::IsLinkCreated( &link.startNodeId, &link.startAttr, &link.endNodeId, &link.endAttr ))
        {
            AddLink( link );
        }
        
        if ( ImNodes::IsLinkDestroyed( &linkId ))
        {
            const auto iterator = std::ranges::find_if(
                    m_links, [ linkId ]( const SLink& link ) -> bool
                    {
                        return link.id == linkId;
                    }
            );
            assert( iterator != m_links.end());
            int        nodeId{ iterator->endNodeId };
            const auto it = std::ranges::find_if(
                    m_nodes, [ &nodeId ]( const std::pair<int, INode*>& node )
                    {
                        return node.second->GetId() == nodeId;
                    }
            );
            ( *it ).second->Detach( iterator->endAttr );
            m_links.erase( iterator );
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
    
    while (( it = std::find_if(
            it, m_links.end(), [ &currentNode ]( const SLink& link )
            {
                return link.endNodeId == currentNode;
            }
    )) != m_links.end())
    {
        links.push_back( *it );
        ++it;
    }
    std::ranges::reverse( links );
    return links;
}
void BalEditor::CShaderGraph::Evaluate()
{
    bool canTraverseGraph{ true };
    if ( m_currentShaderFile.isFolder )
    {
        canTraverseGraph = false;
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
                m_currentShaderFile.fileName.append(
                        name, std::find_if(
                                name, name + 64, []( char c )
                                {
                                    return c == '\0';
                                }
                        ));
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
        
        //todo: save nodes that are used multiple times to separate variable
        stack.emplace_back( m_nodes[0].second );
        
        while ( !stack.empty())
        {
            INode* pCurrentNode = stack.back();
            stack.pop_back();
            postOrder.push_back( pCurrentNode );
            
            std::vector connected{ GetNeighbors( pCurrentNode->GetId()) };
            for ( const SLink& neighbor : connected )
            {
                int  nodeId       = neighbor.startNodeId;
                auto nodeIterator = std::ranges::find_if(
                        m_nodes, [ nodeId ]( const std::pair<int, INode*>& node )
                        {
                            return node.second->GetId() == nodeId;
                        }
                );
                stack.push_back( nodeIterator->second );
            }
        }
        
        std::vector<INode*>::iterator nextNode{};
        nextNode = postOrder.begin() + 1;
        std::vector<std::vector<uint32_t>> compiledShaders;
        std::vector<uint8_t>              compiledShaderTypes;
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
            /*
            0: "Vertex"
            1: "Tessellation Control"
            2: "Tessellation Evaluate"
            3: "Geometry"
            4: "Fragment"
            5: "Mesh"
            6: "Task"
            7: "Raygen"
            8: "Any"
            9: "Closest"
            a: "Miss"
            b: "Intersection"
            c: "Callable"
            */
            switch ( shader[shader.size() - 1] )
            {
                case '0':
                    extension  = ".vert";
                    shaderType = shaderc_vertex_shader;
                    break;
                case '1':
                    extension  = ".tess";
                    shaderType = shaderc_tess_control_shader;
                    break;
                case '2':
                    extension  = ".tess";
                    shaderType = shaderc_tess_evaluation_shader;
                    break;
                case '3':
                    extension  = ".geom";
                    shaderType = shaderc_geometry_shader;
                    break;
                case '4':
                    extension  = ".frag";
                    shaderType = shaderc_fragment_shader;
                    break;
                case '5':
                    extension  = ".mesh";
                    shaderType = shaderc_mesh_shader;
                    break;
                case '6':
                    extension  = ".task";
                    shaderType = shaderc_task_shader;
                    break;
                case '7':
                    extension  = ".raygen";
                    shaderType = shaderc_raygen_shader;
                    break;
                case '8':
                    extension  = ".Any";
                    shaderType = shaderc_anyhit_shader;
                    break;
                case '9':
                    extension  = ".Closest";
                    shaderType = shaderc_closesthit_shader;
                    break;
                case 'a':
                    extension  = ".Miss";
                    shaderType = shaderc_miss_shader;
                    break;
                case 'b':
                    extension  = ".Intersection";
                    shaderType = shaderc_intersection_shader;
                    break;
                case 'c':
                    extension  = ".callable";
                    shaderType = shaderc_callable_shader;
                    break;
                default:
                    break;
                
            }
            shaderc::SpvCompilationResult result = compiler.CompileGlslToSpv(
                    shader.c_str(),
                    shader.size() - 1,
                    shaderType,
                    ( "../Data/Editor/Shaders/" + m_currentShaderFile.fileName + extension ).c_str(),
                    options
            );
            if ( result.GetCompilationStatus() != shaderc_compilation_status_success )
            {
                //handle errors
                std::cout << shader << std::endl;
                std::cout << result.GetErrorMessage() << std::endl;
                assert( false );
            }
            const std::vector sprv( result.cbegin(), result.cend());
            compiledShaders.push_back( sprv );
            char shaderTypeChar{ shader[shader.size() - 1] };
            compiledShaderTypes.push_back( uint8_t(( shaderTypeChar > 57 )?(shaderTypeChar - 57):shaderTypeChar - 48));
        }
        
        std::string editorData{ SaveEditor() };
        BalEditor::Exporter::ExportShader(
                m_currentShaderFile.fileName,
                m_currentShaderFile.path,
                compiledShaders,
                compiledShaderTypes,
                m_currentShaderFile.uuid,
                editorData
        );
        m_wantsToSave = false;
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
        case EUiNodeType::GeometryOutput:           //Todo
        case EUiNodeType::TesselationControl:       //Todo
        case EUiNodeType::TesselationEvaluation:    //Todo
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
            return "Shaders/Vertex Output";
        case EUiNodeType::FragmentOutput:
            return "Shaders/Fragment Output";
        case EUiNodeType::ShaderNode:
            return "Shaders/Shader Output";
        case EUiNodeType::GeometryOutput:
            return "Shaders/Geometry Shader";
        case EUiNodeType::TesselationControl:
            return "Shaders/Tesselation Control Shader";
        case EUiNodeType::TesselationEvaluation:
            return "Shaders/Tesselation Evaluation Shader";
        case EUiNodeType::BrightContrast:
            return "Colour/BrightContrast";
        case EUiNodeType::Gamma:
            return "Colour/Gamma";
        case EUiNodeType::HueSaturationValue:
            return "Colour/Hue Saturation Value";
        case EUiNodeType::Invert:
            return "Colour/Invert";
        case EUiNodeType::Mix:
            return "Colour/Mix";
        case EUiNodeType::RGBtoBW:
            return "Colour/RGB To BW";
        case EUiNodeType::Clamp:
            return "Math/Clamp";
        case EUiNodeType::Math:
            return "Math/Math";
        case EUiNodeType::VectorMath:
            return "Math/Vector Math";
        case EUiNodeType::Bump:
            return "Texture/Bump";
        case EUiNodeType::Displacement:
            return "Texture/Displacement";
        case EUiNodeType::Mapping:
            return "Texture/Mapping";
        case EUiNodeType::NormalMap:
            return "Texture/NormalMap";
        case EUiNodeType::ImageTexture:
            return "Texture/Image Texture";
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
    
    m_currentShaderFile = shader;
    uint8_t type;
    BinaryReadWrite::Read( file, m_currentShaderFile.uuid );
    BinaryReadWrite::Read( file, type );
    if ( type != (uint8_t) EFileTypes::Shader )
    {
        return;
    }
    
    m_currentShaderFile.type = (EFileTypes) type;
    uint8_t              shaderCount;   //eg. 2 shaders
    uint64_t              shaderSize;    //eg. 965 char long
    uint8_t              shaderType;    //eg. vertex
    std::vector<uint64_t> shadersSizeToSkip;
    BinaryReadWrite::Read( file, shaderCount );
    for ( uint64_t i{}; i < shaderCount; ++i )
    {
        BinaryReadWrite::Read( file, shaderSize );
        BinaryReadWrite::Read( file, shaderType );
        shadersSizeToSkip.push_back( shaderSize );
    }
    for ( uint64_t i{}; i < shaderCount; ++i )
    {
        BinaryReadWrite::MoveCursor( file, (int64_t) ( shadersSizeToSkip[i] * sizeof( uint32_t )));
    }
    
    int32_t  leftoverSize;
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
    leftoverSize -= (int32_t) ( size * sizeof( int ) * 2 );
    BinaryReadWrite::Read( file, size );
    m_links.reserve( size );
    for ( int i = 0; i < size; ++i )
    {
        SLink link{};
        BinaryReadWrite::Read( file, link.id );
        BinaryReadWrite::Read( file, link.startAttr );
        BinaryReadWrite::Read( file, link.endAttr );
        BinaryReadWrite::Read( file, link.startNodeId );
        BinaryReadWrite::Read( file, link.endNodeId );
        AddLink( link );
    }
    leftoverSize -= (int32_t) ( size * sizeof( int ) * 5 );
    char* editor{ new char[leftoverSize] };
    BinaryReadWrite::Read( file, editor, leftoverSize );
    file.close();
    ImNodes::LoadCurrentEditorStateFromIniString( editor, leftoverSize );
}
void BalEditor::CShaderGraph::LoadDefaultEditor()
{
    AddNode( EUiNodeType::ShaderNode, glm::vec2{ 512, 128 } );
    if ( m_currentShaderFile.uuid == 0 )
    {
        m_currentShaderFile.uuid = (uint64_t) CUuid();
    }
    m_currentShaderFile.isFolder = true;
    m_currentShaderFile.path     = "../Data";
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
bool BalEditor::CShaderGraph::HasCycleStartingFrom( const INode* pStartNode )
{
    std::unordered_map<int, INode*> nodes;
    for ( const auto& [ id, node ] : m_nodes )
    {
        nodes[node->GetId()] = node;
    }
    
    std::unordered_set<int>  visited;
    std::stack<const INode*> stack;
    
    stack.push( pStartNode );
    while ( !stack.empty())
    {
        const INode* pCurrentNode = stack.top();
        stack.pop();
        
        if ( visited.count( pCurrentNode->GetId()))
        {
            return true;
        }
        visited.insert( pCurrentNode->GetId());
        
        std::vector connected{ GetNeighbors( pCurrentNode->GetId()) };
        for ( const SLink& neighbor : connected )
        {
            const INode* pNeighborNode = nodes[neighbor.startNodeId];
            stack.push( pNeighborNode );
            if ( pNeighborNode == pStartNode )
            {
                return true;
            }
        }
    }
    return false;
}
void BalEditor::CShaderGraph::AddLink( SLink& link )
{
    int        nodeId{ link.endNodeId };
    const auto it = std::ranges::find_if(
            m_nodes, [ &nodeId ]( const std::pair<int, INode*>& node )
            {
                return node.second->GetId() == nodeId;
            }
    );
    if ( it != m_nodes.end() && ( *it ).second->HasFreeAttachment( link.endAttr ))
    {
        ( *it ).second->Attach( link.endAttr );
        link.id = m_currentId++;
        m_links.push_back( link );
        if ( HasCycleStartingFrom(
                std::ranges::find_if(
                        m_nodes, [ nodeId ]( const std::pair<int, INode*>& node )
                        {
                            return node.second->GetId() == nodeId;
                        }
                )->second
        ))
        {
            m_links.pop_back();
            ( *it ).second->Detach( link.endAttr );
        }
    }
}
