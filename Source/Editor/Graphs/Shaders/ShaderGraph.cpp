//
// Created by joran on 25/02/2023.
//

#include <cassert>
#include <format>
#include <queue>
#include <regex>
#include <sstream>
#include <stdexcept>
#include <unordered_map>

#include "../../Tools/FilesSystem/Exporter.h"
#include "Nodes/ConverterNode.h"
#include "Nodes/ShaderNode.h"
#include "Nodes/Shaders/OutputNode.h"
#include "ShaderGraph.h"

void CShaderGraph::Initialize()
{
    Add( dynamic_cast<CShaderNode*>( CreateNode<COutputNode>() ) );
}
void CShaderGraph::Cleanup()
{
    for ( const auto* pNode : m_nodes )
    {
        DeleteNode( pNode );
    }
    m_nodes.clear();
}
CShaderNode* CShaderGraph::Add( CShaderNode* node )
{
    m_nodes.emplace_back( node );
    PullNodeData();
    return node;
}
void CShaderGraph::Connect( int fromNode, int fromAttr, int toNode, int toAttr ) noexcept
{
    const auto to   = std::ranges::find_if( m_nodes, [ toNode ]( const CShaderNode* pNode ) {
        return pNode->GetId() == toNode;
    } );
    const auto from = std::ranges::find_if( m_nodes, [ fromNode ]( const CShaderNode* pNode ) {
        return pNode->GetId() == fromNode;
    } );

    Connect( ( *from )->GetOutput( fromAttr - ( *from )->GetId() - (int)( *from )->GetInputNodes().size() ), ( *to )->GetInput( toAttr - ( *to )->GetId() ) );
}
void CShaderGraph::Connect( CShaderOutput* from, CShaderInput* to ) noexcept
{
    if ( !from || !to )
    {
        return;
    }

    if ( to->IsConnected() )
    {
        return;
    }
    //    todo:Convert mix_mode_value types
    if ( from->GetType() != to->GetType() )
    {
        const CShaderNode* converterNode;
        CShaderInput* converterInput;
        if ( to->GetType() != SSocketType::var_type::shader && to->GetType() != SSocketType::var_type::none )
        {
            converterNode  = Add( CreateNode<CConverterNode>( from->GetType(), to->GetType() ) );
            converterInput = converterNode->GetInput( 0 );

            from->Connect( converterInput );
            converterInput->Connect( from );
            converterNode->GetOutput( 0 )->Connect( to );
            to->Connect( converterNode->GetOutput( 0 ) );
            return;
        }
    }
    to->Connect( from );
    from->Connect( to );
}
void CShaderGraph::Disconnect( int fromNode, int fromAttr, int toNode, int toAttr ) noexcept
{
    const std::list<CShaderNode*>::iterator to   = std::ranges::find_if( m_nodes, [ toNode ]( const CShaderNode* pNode ) {
        return pNode->GetId() == toNode;
    } );
    const std::list<CShaderNode*>::iterator from = std::ranges::find_if( m_nodes, [ fromNode ]( const CShaderNode* pNode ) {
        return pNode->GetId() == fromNode;
    } );

    CShaderOutput* fromConnection { ( *from )->GetOutput( fromAttr - ( *from )->GetId() - (int)( *from )->GetInputNodes().size() ) };
    CShaderInput* toConnection { ( *to )->GetInput( toAttr - ( *to )->GetId() ) };

    // there should be a converter node here
    if ( toConnection->GetLink() != fromConnection )
    {
        if ( !( toConnection->GetLink() && toConnection->GetLink()->GetParent() ) )
        {
            return;
        }
        CShaderNode* converterNode { toConnection->GetLink()->GetParent() };
        Disconnect( converterNode->GetInput( 0 ) );
        Disconnect( converterNode->GetOutput( 0 ) );
        Remove( converterNode );
        DeleteNode( converterNode );
    }

    Disconnect( fromConnection );
    Disconnect( toConnection );
}
void CShaderGraph::Disconnect( CShaderOutput* from ) const noexcept
{
    from->Disconnect();
}
void CShaderGraph::Disconnect( CShaderInput* to ) const noexcept
{
    to->Disconnect();
}
void CShaderGraph::Serialize( const char* filename ) const
{
    (void)filename;
}
void CShaderGraph::Deserialize( const char* filename ) const
{
    (void)filename;
}
void CShaderGraph::DrawNodes() const noexcept
{
    for ( const auto& node : m_nodes )
    {
        node->Draw();
    }
}

bool CShaderGraph::Compile( const SFile& file, int cullMode )
{
    // Define local variables
    std::vector<const CShaderNode*> evaluatedNodes;
    std::vector<FawnVision::SShaderCreateInfo> createInfo;
    std::unordered_map<std::string_view, int> outputVarCount;
    SShaderInfo shaderInfo;
    const shaderc::Compiler compiler;

    std::string shader { Compile( m_nodes.front(), evaluatedNodes, outputVarCount, shaderInfo ) };

    // Sort bindings
    std::ranges::sort( shaderInfo.bindings, []( const SShaderBinding& a, const SShaderBinding& b ) {
        return a.binding > b.binding;
    } );
    std::ranges::sort( shaderInfo.bindings, []( const SShaderBinding& a, const SShaderBinding& b ) {
        return a.set > b.set;
    } );
    std::ranges::sort( shaderInfo.bindings, []( const SShaderBinding& a, const SShaderBinding& b ) {
        return a.layoutType > b.layoutType;
    } );

    // Get all used shader types
    int32_t shaders { ( (COutputNode*)m_nodes.front() )->GetShaderTypes() };

    // For each used shader type...
    for ( int32_t i { shader_stage_vertex }, j {}; i < shader_stage_max; i <<= 1, ++j )
    {
        // Skip if the shader type is not used
        if ( ( shaders & i ) == 0 )
        {
            continue;
        }
        const std::string define { ToDefine( shader_stage( i ) ) };
        std::unordered_map<uint32_t, uint32_t> setBindings;
        uint32_t inputAttachmentIndex {};
        std::vector<std::pair<int32_t, std::string>> glsCode;
        // Convert includes into string and append it to the GLSL code
        std::ranges::for_each( shaderInfo.includes, [ &glsCode, i ]( const SInclude& include ) {
            if ( ( include.shaderTypes & i ) == 0 )
            {
                return;
            }
            glsCode.emplace_back( include.priority, std::string( "#include \"" ) + include.file + "\"\n" );
        } );

        // constexpr Convert bindings into string and append it to the GLSL code
        for ( const auto& binding : shaderInfo.bindings )
        {
            // Skip if the binding is not used by this shader type
            if ( ( binding.shaderTypes & i ) == 0 )
            {
                continue;
            }

            // Get set and binding number
            uint32_t setNr { ( binding.set == SShaderBinding::autoConfig ) ? (uint32_t)binding.layoutType : (uint32_t)binding.set };
            uint32_t bindingNr { ( binding.binding == SShaderBinding::autoConfig ) ? setBindings[ (uint32_t)binding.layoutType ]++ : binding.binding };

            // Create binding string
            std::string bindingString { "layout(" };
            switch ( binding.layoutType )
            {
                case SShaderBinding::layout::input:
                case SShaderBinding::layout::output: bindingString += std::format( "location = {}) ", bindingNr ); break;
                case SShaderBinding::layout::subpass_input: bindingString += std::format( "input_attachment_index = {}, ", inputAttachmentIndex ); ++inputAttachmentIndex;
                default: bindingString += std::format( "set = {}, binding = {} ) ", setNr, bindingNr ); break;
            }
            switch ( binding.layoutType )
            {
                case SShaderBinding::layout::input: bindingString += "in "; break;
                case SShaderBinding::layout::output: bindingString += "out "; break;
                case SShaderBinding::layout::subpass_input: bindingString += "uniform subpassInput "; break;
                default: bindingString += "uniform "; break;
            }
            bindingString += ConvertToGlslUniform( binding );
            glsCode.emplace_back( binding.priority, bindingString );
        }
        // sort all on priority
        std::ranges::sort( glsCode, []( const std::pair<int32_t, std::string>& a, const std::pair<int32_t, std::string>& b ) {
            return a.first > b.first;
        } );
        // append each string to the shader code
        std::ranges::for_each( glsCode, [ &shader, &define ]( const std::pair<int32_t, std::string>& a ) {
            shader.insert( shader.find( define ) + define.size() + 2, a.second );
        } );
    }
    std::cout << shader << std::endl;
    //  for each used shader type
    for ( int i { shader_stage_vertex }; i < shader_stage_max; i <<= 1 )
    {
        // Skip if the shader type is not used
        if ( ( shaders & i ) == 0 )
        {
            continue;
        }
        const std::string define { ToDefine( shader_stage( i ) ) };
        shaderc_shader_kind shaderType { Convert( shader_stage( i ) ) };
        const char* extension { ".vert" };
        shaderc::CompileOptions options;

        options.SetTargetEnvironment( shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_3 );
        options.SetOptimizationLevel( shaderc_optimization_level_performance );
        options.SetWarningsAsErrors();
        options.SetTargetSpirv( shaderc_spirv_version_1_5 );
        options.SetIncluder( std::make_unique<CShaderFileIncluder>() );
        options.AddMacroDefinition( define );
        for ( const auto& def : shaderInfo.defines )
        {
            if ( ( def.shaderTypes | i ) == 0 )
            {
                continue;
            }
            options.AddMacroDefinition( def.name, def.value );
        }

        switch ( shaderType )
        {
            case shaderc_fragment_shader: extension = ".frag"; break;
            case shaderc_geometry_shader: extension = ".geom"; break;
            case shaderc_tess_control_shader:
            case shaderc_tess_evaluation_shader: extension = ".tess"; break;
            case shaderc_raygen_shader: extension = ".rgen"; break;
            case shaderc_anyhit_shader: extension = ".rhit"; break;
            case shaderc_closesthit_shader: extension = ".rlco"; break;
            case shaderc_miss_shader: extension = ".miss"; break;
            case shaderc_intersection_shader: extension = ".inter"; break;
            default: break;
        }
        std::string filePath { std::string( "../Data/Editor/Shaders/" ) + file.fileName + extension };
        shaderc::SpvCompilationResult result = compiler.CompileGlslToSpv( shader.c_str(), shader.size(), shaderType, filePath.data(), "main", options );
        if ( result.GetCompilationStatus() != shaderc_compilation_status_success )
        {
            //handle errors
            std::cout << result.GetErrorMessage() << std::endl;
            assert( false );
        }
        const std::vector sprv( result.cbegin(), result.cend() );
        createInfo.emplace_back( uint32_t( shaders & i ), (uint64_t)sprv.size(), (uint32_t*)sprv.data() );
    }
    return FawnForge::Exporter::ExportShader( file.fileName, file.path, createInfo, cullMode, file.uuid  );
}
std::string CShaderGraph::Compile( const CShaderNode* pNode, std::vector<const CShaderNode*>& evaluatedNodes, std::unordered_map<std::string_view, int>& outputVarCount, SShaderInfo& shaderInfo )
{
    pNode->GetShaderInfo( shaderInfo );
    if ( std::ranges::find( evaluatedNodes, pNode ) != evaluatedNodes.cend() )
    {
        return "";
    }

    std::string glslCode { pNode->GetCode() };
    std::string otherCode;
    for ( const CShaderInput* pIn : pNode->GetInputNodes() )
    {
        // find the input parameter with the same name
        std::string inputName = pIn->GetShaderName();
        std::regex regex( "[\\s ,(](" + inputName + ")[;,)\\n ]" );
        std::smatch match;

        if ( std::regex_search( glslCode, match, regex ) )
        {
            std::string valueStr {};
            if ( pIn->IsConnected() )
            {
                valueStr = pIn->GetLink()->GetShaderName();
                if ( pIn->GetLink()->GetType() == SSocketType::var_type::shader )
                {
                    valueStr = Compile( pIn->GetLink()->GetParent(), evaluatedNodes, outputVarCount, shaderInfo );
                }
                else
                {
                    otherCode += Compile( pIn->GetLink()->GetParent(), evaluatedNodes, outputVarCount, shaderInfo );
                    valueStr += std::to_string( outputVarCount[ valueStr ] );
                }
            }
            else
            {
                switch ( pIn->GetType() )
                {
                    case SSocketType::var_type::float_type: valueStr = std::to_string( pIn->GetValue()[ 0 ] ); break;
                    case SSocketType::var_type::vector2: valueStr = std::format( "vec2({},{})", pIn->GetValue()[ 0 ], pIn->GetValue()[ 1 ] ); break;
                    case SSocketType::var_type::vector3:
                    case SSocketType::var_type::color: valueStr = std::format( "vec3({},{},{})", pIn->GetValue()[ 0 ], pIn->GetValue()[ 1 ], pIn->GetValue()[ 2 ] ); break;
                    case SSocketType::var_type::vector4: valueStr = std::format( "vec4({},{},{},0)", pIn->GetValue()[ 0 ], pIn->GetValue()[ 1 ], pIn->GetValue()[ 2 ] ); break;
                    default: assert( false && "input types can't Convert to valueStr" ); break;
                }
            }
            glslCode.replace( match.position( 1 ), inputName.length(), valueStr );
        }
    }
    std::string variables;
    for ( const auto& pOut : pNode->GetOutputNodes() )
    {
        const std::string& outputName = pOut->GetShaderName();
        int& count                    = outputVarCount[ outputName ];
        ++count;
        std::string outputVarName = std::format( "{}{}", outputName, count );

        // mix_mode_add the output variable definition to the generated code
        std::string outputVarType;
        switch ( pOut->GetType() )
        {
            case SSocketType::var_type::float_type: outputVarType = "float"; break;
            case SSocketType::var_type::int_type: outputVarType = "int"; break;
            case SSocketType::var_type::vector2: outputVarType = "vec2"; break;
            case SSocketType::var_type::color:
            case SSocketType::var_type::vector3: outputVarType = "vec3"; break;
            case SSocketType::var_type::vector4: outputVarType = "vec4"; break;
            case SSocketType::var_type::none:
            case SSocketType::var_type::shader: outputVarName = ""; break;
        }
        if ( !outputVarName.empty() )
        {
            variables.append( outputVarType.append( " " + outputVarName + ";\n" ) );
        }
        std::smatch match;
        std::regex regex( "[\\s ,(](" + outputName + ")[;,)\\n ]" );

        if ( std::regex_search( glslCode, match, regex ) )
        {
            glslCode.replace( match.position( 1 ), outputName.length(), outputVarName );
        }
    }
    evaluatedNodes.push_back( pNode );
    return otherCode + '\n' + variables + glslCode;
}
std::string CShaderGraph::ConvertToGlslUniform( const SShaderBinding& binding )
{
    std::stringstream ss;
    std::string arrayStr  = ( binding.arraySize > 1 ) ? std::format( "[{}]", binding.arraySize ) : binding.arraySizeString[ 0 ] != '\0' ? std::format( "[{}]", binding.arraySizeString ) : "";
    std::string typeStr   = ( binding.valueType == SShaderBinding::value_type::struct_type && !arrayStr.empty() ) ? binding.instanceName : SShaderBinding::ToString( binding.valueType );
    std::string nameStr   = ( std::strcmp( binding.name, "" ) ) ? binding.name : "";
    std::string structStr = ( !binding.members.empty() ) ? binding.instanceName : "";

    if ( !binding.members.empty() )
    {
        ss << nameStr << "{" << std::endl;
        for ( auto& member : binding.members )
        {
            ss << ConvertToGlslUniform( member );
        }
        ss << "}" << structStr << ";" << std::endl;
    }
    else
    {
        ss << typeStr << arrayStr << " " << nameStr << ";" << std::endl;
    }

    return ss.str();
}

std::vector<SShaderBinding>& CShaderGraph::GetBindings()
{
    return m_shaderInfo.bindings;
}
void CShaderGraph::PullNodeData() noexcept
{
    m_shaderInfo.Clear();
    for ( const CShaderNode* pNode : m_nodes )
    {
        pNode->GetShaderInfo( m_shaderInfo );
    }

    auto bindingComparator = []( const SShaderBinding& lhs, const SShaderBinding& rhs ) constexpr {
        if ( lhs.layoutType != rhs.layoutType )
        {
            return lhs.layoutType > rhs.layoutType;
        }
        else
        {
            return std::strcmp( lhs.name, rhs.name ) < 0;
        }
    };
    std::ranges::sort( m_shaderInfo.bindings, bindingComparator );
}
void CShaderGraph::PushData() const noexcept
{
    for ( CShaderNode* pNode : m_nodes )
    {
        pNode->SetBindings( m_shaderInfo.bindings );
    }
}
shaderc_shader_kind CShaderGraph::Convert( shader_stage stage ) noexcept
{
    switch ( stage )
    {
        case shader_stage_vertex: return shaderc_vertex_shader;
        case shader_stage_tessellation_control: return shaderc_tess_control_shader;
        case shader_stage_tessellation_evaluation: return shaderc_tess_evaluation_shader;
        case shader_stage_geometry: return shaderc_geometry_shader;
        case shader_stage_fragment: return shaderc_fragment_shader;
        case shader_stage_compute: return shaderc_compute_shader;
        case shader_stage_raygen: return shaderc_raygen_shader;
        case shader_stage_any_hit: return shaderc_anyhit_shader;
        case shader_stage_closest_hit: return shaderc_closesthit_shader;
        case shader_stage_miss: return shaderc_miss_shader;
        case shader_stage_intersection: return shaderc_intersection_shader;
        case shader_stage_callable: return shaderc_callable_shader;
        case shader_stage_task: return shaderc_task_shader;
        case shader_stage_mesh: return shaderc_mesh_shader;
        case shader_stage_all:
        case shader_stage_max: break;
    }
    return shaderc_spirv_assembly;
}
void CShaderGraph::Remove( CShaderNode* node )
{
    m_nodes.remove( node );
}

shaderc_include_result* CShaderFileIncluder::GetInclude( const char* requestedSource, shaderc_include_type type, const char* requestingSource, size_t includeDepth )
{
    (void)includeDepth;
    std::filesystem::path src { requestingSource };
    src.remove_filename();
    const std::filesystem::path dest { src.remove_filename().string() + requestedSource };
    const std::string fullPath = ( type == shaderc_include_type_relative ) ? absolute( dest ).string() : requestedSource;
    if ( fullPath.empty() )
    {
        return nullptr;
    }

    std::ifstream newFileInfo( fullPath, std::ios::in | std::ios::binary );
    if ( !newFileInfo.is_open() )
    {
        return nullptr;
    }
    const std::string newFileInfoContent( ( std::istreambuf_iterator<char>( newFileInfo ) ), std::istreambuf_iterator<char>() );
    newFileInfo.close();

    m_includedFiles.insert( fullPath );

    auto pFilePath { new char[ fullPath.size() ] };
    auto pData { new char[ newFileInfoContent.size() ] };
    memcpy( pFilePath, fullPath.data(), fullPath.size() );
    memcpy( pData, newFileInfoContent.data(), newFileInfoContent.size() );
    return new shaderc_include_result { pFilePath, fullPath.size(), pData, newFileInfoContent.size(), nullptr };
}
void CShaderFileIncluder::ReleaseInclude( shaderc_include_result* includeResult )
{
    delete[] includeResult->source_name;
    delete[] includeResult->content;
    delete includeResult;
}
const std::unordered_set<std::string, StringHash, std::equal_to<>>& CShaderFileIncluder::FilePathTrace() const
{
    return m_includedFiles;
}
