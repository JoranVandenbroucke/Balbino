//
// Created by joran on 25/02/2023.
//

#include <queue>
#include <stdexcept>
#include <cassert>
#include <format>
#include <regex>
#include <unordered_map>
#include <sstream>
#include <iostream>

#include "ShaderGraph.h"
#include "Nodes/ShaderNode.h"
#include "Nodes/Shaders/OutputNode.h"
#include "shaderc/shaderc.hpp"
#include "Shader.h"
#include "../../Tools/FilesSystem/Exporter.h"

void CShaderGraph::Initialize()
{
    Add( reinterpret_cast<CShaderNode*>(CreateNode<COutputNode>()));
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
void CShaderGraph::Connect( int fromNode, int fromAttr, int toNode, int toAttr )
{
    const std::list<CShaderNode*>::iterator to   = std::find_if(
            m_nodes.begin(), m_nodes.end(), [ toNode ]( const CShaderNode* pNode )
            {
                return pNode->GetId() == toNode;
            }
    );
    const std::list<CShaderNode*>::iterator from = std::find_if(
            m_nodes.begin(), m_nodes.end(), [ fromNode ]( const CShaderNode* pNode )
            {
                return pNode->GetId() == fromNode;
            }
    );
    
    Connect(( *from )->GetOutput( fromAttr ), ( *to )->GetInput( toAttr ));
}
void CShaderGraph::Connect( CShaderOutput* from, CShaderInput* to )
{
    if ( !from || !to )
    {
        return;
    }
    
    if ( to->IsConnected())
    {
        return;
    }
//    todo Convert value types
//    if( from->GetType() != to->GetType())
//    {
//        CShaderNode* converterNode;
//        CShaderInput* converterInput;
//    }
    to->Connect( from );
    from->Connect( to );
}
void CShaderGraph::Disconnect( int fromNode, int fromAttr, int toNode, int toAttr )
{
    const std::list<CShaderNode*>::iterator to   = std::find_if(
            m_nodes.begin(), m_nodes.end(), [ toNode ]( const CShaderNode* pNode )
            {
                return pNode->GetId() == toNode;
            }
    );
    const std::list<CShaderNode*>::iterator from = std::find_if(
            m_nodes.begin(), m_nodes.end(), [ fromNode ]( const CShaderNode* pNode )
            {
                return pNode->GetId() == fromNode;
            }
    );
    
    Disconnect(( *from )->GetOutput( fromAttr ));
    Disconnect(( *to )->GetInput( toAttr ));
}
void CShaderGraph::Disconnect( CShaderOutput* from )
{
    from->Disconnect();
}
void CShaderGraph::Disconnect( CShaderInput* to )
{
    to->Disconnect();
}
void CShaderGraph::Serialize( const char* filename )
{
    (void) filename;
}
void CShaderGraph::Deserialize( const char* filename )
{
    (void) filename;
}
void CShaderGraph::DrawNodes()
{
    for ( const auto& node : m_nodes )
    {
        node->Draw();
    }
}

bool CShaderGraph::Compile( const SFile& file, int cullMode )
{
    std::vector<const CShaderNode*>      evaluatedNodes;
    std::unordered_map<std::string, int> outputVarCount;
    SShaderInfo                          shaderInfo;
    std::string                          shader{
            Compile(
                    m_nodes.front(), evaluatedNodes, outputVarCount, shaderInfo
            )
    };
    
    // Short bindings, includes and defines
    std::sort(
            shaderInfo.defines.begin(), shaderInfo.defines.end(), []( SDefine& a, SDefine& b )
            {
                return a.priority < b.priority;
            }
    );
    std::sort(
            shaderInfo.includes.begin(), shaderInfo.includes.end(), []( SInclude& a, SInclude& b )
            {
                return a.priority < b.priority;
            }
    );
    std::sort(
            shaderInfo.bindings.begin(), shaderInfo.bindings.end(), []( SShaderBinding& a, SShaderBinding& b )
            {
                return a.priority < b.priority;
            }
    );
    std::sort(
            shaderInfo.bindings.begin(), shaderInfo.bindings.end(), []( SShaderBinding& a, SShaderBinding& b )
            {
                return a.layoutType > b.layoutType;
            }
    );
    
    std::vector<std::vector<uint32_t>> compiledShaders;
    const shaderc::Compiler            compiler;
    
    // Get all used shader types
    int32_t       shaders{ ((COutputNode*) m_nodes.front())->GetShaderTypes() };
    // for each used shader type
    // convert includes and bindings into string and append it to the glsl code
    for ( int32_t i{ shader_stage_vertex }, j{}; i < shader_stage_max; i <<= 1, ++j )
    {
        if (( shaders & i ) == 0 )
        {
            continue;
        }
        const std::string                            define{ ToDefine( shader_stage( i )) };
        std::unordered_map<uint32_t, uint32_t>       setBindings;
        uint32_t                                     inputAttachmentIndex{};
        std::vector<std::pair<int32_t, std::string>> glsCode;
        std::for_each(
                shaderInfo.includes.cbegin(), shaderInfo.includes.cend(), [ &glsCode, i ]( const SInclude& include )
                {
                    if (( include.shaderTypes & i ) == 0 )
                    {
                        return;
                    }
                    glsCode.emplace_back( include.priority, std::string( "#include \"" ) + include.file + "\"\n" );
                }
        );
        
        for ( const auto& binding : shaderInfo.bindings )
        {
            std::string bindingString;
            if (( binding.shaderTypes & i ) == 0 )
            {
                continue;
            }
            
            uint32_t setNr{
                    ( binding.set == SShaderBinding::autoConfig )
                    ? (uint32_t) binding.layoutType
                    : (uint32_t) binding.set
            };
            uint32_t bindingNr{
                    ( binding.binding == SShaderBinding::autoConfig )
                    ? setBindings[(uint32_t) binding.layoutType]++
                    : binding.binding
            };
            bindingString += "layout(";
            switch ( binding.layoutType )
            {
                case SShaderBinding::layout_input:
                case SShaderBinding::layout_output:
                    bindingString += "location = " + std::to_string( bindingNr ) + ") ";
                    break;
                case SShaderBinding::layout_subpass_input:
                    bindingString += "input_attachment_index = " + std::to_string( inputAttachmentIndex++ ) + ',';
                default:
                    bindingString += "set = " + std::to_string( setNr ) + ", binding = " + std::to_string( bindingNr ) + ") ";
                    break;
            }
            switch ( binding.layoutType )
            {
                case SShaderBinding::layout_input:
                    bindingString += "in ";
                    break;
                case SShaderBinding::layout_output:
                    bindingString += "out ";
                    break;
                case SShaderBinding::layout_subpass_input:
                    bindingString += "uniform subpassInput ";
                    break;
                default:
                    bindingString += "uniform ";
                    break;
            }
            bindingString += ConvertToGlslUniform( binding );
            glsCode.emplace_back( binding.priority,bindingString);
        }
        std::sort(
                glsCode.begin(),
                glsCode.end(),
                []( std::pair<int32_t, std::string>& a, std::pair<int32_t, std::string>& b )
                {
                    return a.first > b.first;
                }
        );
        std::for_each(
                glsCode.cbegin(), glsCode.cend(), [ &shader, define ]( const std::pair<int32_t, std::string>& a )
                {
                    shader.insert( shader.find( define ) + define.size() + 2, a.second );
                }
        );
    }
    // for each used shader type
    // set compiler settings and definitions
    // compile shader and report error
    for ( int     i{ shader_stage_vertex }; i < shader_stage_max; i <<= 1 )
    {
        if (( shaders & i ) == 0 )
        {
            continue;
        }
        const std::string   define{ ToDefine( shader_stage( i )) };
        shaderc_shader_kind shaderType{ Convert( shader_stage( i )) };
        const char* extension{ ".vert" };
        shaderc::CompileOptions options;
        
        options.SetTargetEnvironment( shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_3 );
        options.SetOptimizationLevel( shaderc_optimization_level_performance );
        options.SetWarningsAsErrors();
        options.SetTargetSpirv( shaderc_spirv_version_1_6 );
        options.SetIncluder( std::make_unique<FawnVision::CFileIncluder>());
        options.AddMacroDefinition( define );
        for ( const auto& def : shaderInfo.defines )
        {
            if (( def.shaderTypes | i ) == 0 )
            {
                continue;
            }
            options.AddMacroDefinition( def.name, def.value );
        }
        
        switch ( shaderType )
        {
            case shaderc_fragment_shader:
                extension = ".frag";
                break;
            case shaderc_geometry_shader:
                extension = ".geom";
                break;
            case shaderc_tess_control_shader:
            case shaderc_tess_evaluation_shader:
                extension = ".tess";
                break;
            case shaderc_raygen_shader:
                extension = ".rgen";
                break;
            case shaderc_anyhit_shader:
                extension = ".rhit";
                break;
            case shaderc_closesthit_shader:
                extension = ".rlco";
                break;
            case shaderc_miss_shader:
                extension = ".miss";
                break;
            case shaderc_intersection_shader:
                extension = ".inter";
                break;
            default:
                break;
        }
        std::string                   filePath{ std::string( "../Data/Editor/Shaders/" ) + file.fileName + extension };
        shaderc::SpvCompilationResult result = compiler.CompileGlslToSpv(
                shader.c_str(), shader.size(), shaderType, filePath.data(), "main", options
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
    }
    
    return FawnForge::Exporter::ExportShader(file.fileName, file.path, compiledShaders, shaders, cullMode, file.uuid );
}
std::string CShaderGraph::Compile( const CShaderNode* pNode, std::vector<const CShaderNode*>& evaluatedNodes, std::unordered_map<std::string, int>& outputVarCount, SShaderInfo& shaderInfo )
{
    pNode->GetShaderInfo( shaderInfo );
    if ( std::find( evaluatedNodes.cbegin(), evaluatedNodes.cend(), pNode ) != evaluatedNodes.cend())
    {
        return "";
    }
    
    std::string glslCode{ pNode->GetCode() };
    std::string otherCode;
    for ( const CShaderInput* pIn : pNode->GetInputNodes())
    {
        // find the input parameter with the same name
        std::string inputName = pIn->GetShaderName();
        std::regex  regex( "[\\s ,(](" + inputName + ")[;,)\\n ]" );
        std::smatch match;
        
        if ( std::regex_search( glslCode, match, regex ))
        {
            std::string valueStr{};
            if ( pIn->IsConnected())
            {
                // todo: this has a flaw, output parameter doesn't exist as a variable. detect what can be reused?
                valueStr = pIn->GetLink()->GetShaderName();
                if ( pIn->GetLink()->GetType() == SSocketType::var_type_shader )
                {
                    valueStr = Compile( pIn->GetLink()->GetParent(), evaluatedNodes, outputVarCount, shaderInfo );
                }
                else
                {
                    otherCode += Compile( pIn->GetLink()->GetParent(), evaluatedNodes, outputVarCount, shaderInfo );
                    valueStr += std::to_string( outputVarCount[valueStr] );
                }
            }
            else
            {
                switch ( pIn->GetType())
                {
                    case SSocketType::var_type_float:
                        valueStr = std::to_string( pIn->GetValue()[0] );
                        break;
                    case SSocketType::var_type_vector_3:
                        valueStr = std::format(
                                "vec3({},{},{})", pIn->GetValue()[0], pIn->GetValue()[1], pIn->GetValue()[2]
                        );
                        break;
                    case SSocketType::var_type_color:
                        valueStr = std::format(
                                "vec4({},{},{},1)", pIn->GetValue()[0], pIn->GetValue()[1], pIn->GetValue()[2]
                        );
                        break;
                    case SSocketType::var_type_vector_4:
                        valueStr = std::format(
                                "vec4({},{},{},0)", pIn->GetValue()[0], pIn->GetValue()[1], pIn->GetValue()[2]
                        );
                        break;
                    default:
                        assert( false && "input types can't Convert to valueStr" );
                        break;
                }
            }
            glslCode.replace( match.position( 1 ), inputName.length(), valueStr );
        }
    }
    std::string             variables;
    for ( const auto& pOut : pNode->GetOutputNodes())
    {
        const std::string& outputName = pOut->GetShaderName();
        int              & count      = outputVarCount[outputName];
        ++count;
        std::string outputVarName = outputName + std::to_string( count );
        
        // add the output variable definition to the generated code
        std::string outputVarType;
        switch ( pOut->GetType())
        {
            case SSocketType::var_type_float:
                outputVarType = "float";
                break;
            case SSocketType::var_type_vector_3:
                outputVarType = "vec3";
                break;
            case SSocketType::var_type_color:
            case SSocketType::var_type_vector_4:
                outputVarType = "vec4";
                break;
            default:
                outputVarName = "";
                break;
        }
        if ( !outputVarName.empty())
        {
            variables.append( outputVarType.append( " " + outputVarName + ";\n" ));
        }
        std::smatch match;
        std::regex  regex( "[\\s ,(](" + outputName + ")[;,)\\n ]" );
        
        if ( std::regex_search( glslCode, match, regex ))
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
    std::string       arrayStr  = ( binding.arraySize > 1 )
                                  ? "[" + std::to_string( binding.arraySize ) + "]"
                                  : ( binding.arraySizeString[0] != '\0' )
                                    ? std::string( "[" ) + binding.arraySizeString + ']'
                                    : "";
    std::string       typeStr   = ( binding.valueType == SShaderBinding::value_type_struct && !arrayStr.empty())
                                  ? binding.instanceName
                                  : SShaderBinding::ToString( binding.valueType );
    std::string       nameStr   = ( std::strcmp( binding.name, "" )) ? binding.name : "";
    std::string       structStr = ( !binding.members.empty()) ? binding.instanceName : "";
    
    if ( !binding.members.empty())
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
void CShaderGraph::PullNodeData()
{
    m_shaderInfo.Clear();
    for ( const CShaderNode* pNode : m_nodes )
    {
        pNode->GetShaderInfo( m_shaderInfo );
    }
    
    auto bindingComparator = []( const SShaderBinding& lhs, const SShaderBinding& rhs )
    {
        if ( lhs.layoutType != rhs.layoutType )
        {
            return lhs.layoutType > rhs.layoutType;
        }
        else
        {
            return std::strcmp( lhs.name, rhs.name ) < 0;
        }
    };
    std::sort( m_shaderInfo.bindings.begin(), m_shaderInfo.bindings.end(), bindingComparator );
}
void CShaderGraph::PushData()
{
    for ( CShaderNode* pNode : m_nodes )
    {
        pNode->SetBindings( m_shaderInfo.bindings );
    }
}
shaderc_shader_kind CShaderGraph::Convert( shader_stage stage )
{
    switch ( stage )
    {
        case shader_stage_vertex:
            return shaderc_vertex_shader;
        case shader_stage_tessellation_control:
            return shaderc_tess_control_shader;
        case shader_stage_tessellation_evaluation:
            return shaderc_tess_evaluation_shader;
        case shader_stage_geometry:
            return shaderc_geometry_shader;
        case shader_stage_fragment:
            return shaderc_fragment_shader;
        case shader_stage_compute:
            return shaderc_compute_shader;
        case shader_stage_raygen:
            return shaderc_raygen_shader;
        case shader_stage_any_hit:
            return shaderc_anyhit_shader;
        case shader_stage_closest_hit:
            return shaderc_closesthit_shader;
        case shader_stage_miss:
            return shaderc_miss_shader;
        case shader_stage_intersection:
            return shaderc_intersection_shader;
        case shader_stage_callable:
            return shaderc_callable_shader;
        case shader_stage_task:
            return shaderc_task_shader;
        case shader_stage_mesh:
            return shaderc_mesh_shader;
        case shader_stage_all:
        case shader_stage_max:
            break;
    }
    return shaderc_spirv_assembly;
}
