//
// Created by Joran Vandenbroucke on 25/02/2023.
//

#include "ShaderNode.h"
#include "Attribute.h"
#include <algorithm>

std::unordered_map<SShaderBinding::value_type, std::string_view> SShaderBinding::valueTypeStrings { { SShaderBinding::value_type::bool_type, "bool" },
                                                                                                    { SShaderBinding::value_type::int_type, "int" },
                                                                                                    { SShaderBinding::value_type::u_int, "uint" },
                                                                                                    { SShaderBinding::value_type::float_type, "float" },
                                                                                                    { SShaderBinding::value_type::double_type, "double" },
                                                                                                    { SShaderBinding::value_type::vec2, "vec2" },
                                                                                                    { SShaderBinding::value_type::vec3, "vec3" },
                                                                                                    { SShaderBinding::value_type::vec4, "vec4" },
                                                                                                    { SShaderBinding::value_type::dvec2, "dvec2" },
                                                                                                    { SShaderBinding::value_type::dvec3, "dvec3" },
                                                                                                    { SShaderBinding::value_type::dvec4, "dvec4" },
                                                                                                    { SShaderBinding::value_type::bvec2, "bvec2" },
                                                                                                    { SShaderBinding::value_type::bvec3, "bvec3" },
                                                                                                    { SShaderBinding::value_type::bvec4, "bvec4" },
                                                                                                    { SShaderBinding::value_type::ivec2, "ivec2" },
                                                                                                    { SShaderBinding::value_type::ivec3, "ivec3" },
                                                                                                    { SShaderBinding::value_type::ivec4, "ivec4" },
                                                                                                    { SShaderBinding::value_type::uvec2, "uvec2" },
                                                                                                    { SShaderBinding::value_type::uvec3, "uvec3" },
                                                                                                    { SShaderBinding::value_type::uvec4, "uvec4" },
                                                                                                    { SShaderBinding::value_type::mat2, "mat2" },
                                                                                                    { SShaderBinding::value_type::mat3, "mat3" },
                                                                                                    { SShaderBinding::value_type::mat4, "mat4" },
                                                                                                    { SShaderBinding::value_type::mat2x3, "mat2x3" },
                                                                                                    { SShaderBinding::value_type::mat2x4, "mat2x4" },
                                                                                                    { SShaderBinding::value_type::mat3x2, "mat3x2" },
                                                                                                    { SShaderBinding::value_type::mat3x4, "mat3x4" },
                                                                                                    { SShaderBinding::value_type::mat4x2, "mat4x2" },
                                                                                                    { SShaderBinding::value_type::mat4x3, "mat4x3" },
                                                                                                    { SShaderBinding::value_type::dmat2, "dmat2" },
                                                                                                    { SShaderBinding::value_type::dmat3, "dmat3" },
                                                                                                    { SShaderBinding::value_type::dmat4, "dmat4" },
                                                                                                    { SShaderBinding::value_type::dmat2x3, "dmat2x3" },
                                                                                                    { SShaderBinding::value_type::dmat2x4, "dmat2x4" },
                                                                                                    { SShaderBinding::value_type::dmat3x2, "dmat3x2" },
                                                                                                    { SShaderBinding::value_type::dmat3x4, "dmat3x4" },
                                                                                                    { SShaderBinding::value_type::dmat4x2, "dmat4x2" },
                                                                                                    { SShaderBinding::value_type::dmat4x3, "dmat4x3" },
                                                                                                    { SShaderBinding::value_type::sampler_1d, "sampler1D" },
                                                                                                    { SShaderBinding::value_type::sampler_2d, "sampler2D" },
                                                                                                    { SShaderBinding::value_type::sampler_3d, "sampler3D" },
                                                                                                    { SShaderBinding::value_type::sampler_cube, "samplerCube" },
                                                                                                    { SShaderBinding::value_type::sampler_2d_rect, "sampler2DRect" },
                                                                                                    { SShaderBinding::value_type::sampler_1d_array, "sampler1DArray" },
                                                                                                    { SShaderBinding::value_type::sampler_2d_array, "sampler2DArray" },
                                                                                                    { SShaderBinding::value_type::sampler_cube_array, "samplerCubeArray" },
                                                                                                    { SShaderBinding::value_type::sampler_1d_shadow, "sampler1DShadow" },
                                                                                                    { SShaderBinding::value_type::sampler_2d_shadow, "sampler2DShadow" },
                                                                                                    { SShaderBinding::value_type::sampler_cube_shadow, "samplerCubeShadow" },
                                                                                                    { SShaderBinding::value_type::sampler_2d_rect_shadow, "sampler2DRectShadow" },
                                                                                                    { SShaderBinding::value_type::sampler_1d_array_shadow, "sampler1DArrayShadow" },
                                                                                                    { SShaderBinding::value_type::sampler_2d_array_shadow, "sampler2DArrayShadow" },
                                                                                                    { SShaderBinding::value_type::sampler_cube_array_shadow, "samplerCubeArrayShadow" },
                                                                                                    { SShaderBinding::value_type::struct_type, "struct" } };

void CShaderInput::Disconnect() noexcept
{
    if ( m_pLink )
    {
        m_pLink->Disconnect( this );
    }
    m_pLink = nullptr;
}
void CShaderInput::Disconnect( const CShaderOutput* pOut ) noexcept
{
    if ( pOut == m_pLink )
    {
        m_pLink = nullptr;
    }
}

void CShaderInput::Connect( CShaderOutput* pOutput ) noexcept
{
    m_pLink = pOutput;
}
void CShaderInput::SetSocketType( const SSocketType& type )
{
    m_socketType = type;
}
void CShaderOutput::Disconnect() noexcept
{
    for ( CShaderInput* sock : m_links )
    {
        sock->Disconnect( this );
    }

    m_links.clear();
}
void CShaderOutput::Disconnect( CShaderInput* pIn ) noexcept
{
    auto ret = std::ranges::remove( m_links, pIn );
    m_links.erase( ret.begin(), ret.end() );
}
void CShaderOutput::Connect( CShaderInput* pInput ) noexcept
{
    m_links.emplace_back( pInput );
}

CShaderNode::CShaderNode( int& id, const std::vector<SSocketType>& inputs, const std::vector<SSocketType>& outputs )
    : m_id { id }
{
    for ( const SSocketType& type : inputs )
    {
        m_inputs.emplace_back( new CShaderInput( type, this ) );
    }
    for ( const SSocketType& type : outputs )
    {
        m_outputs.emplace_back( new CShaderOutput( type, this ) );
    }
    id += (int)inputs.size() + (int)outputs.size();
}
CShaderInput* CShaderNode::GetInput( const char* name ) const
{
    for ( CShaderInput* pInput : m_inputs )
    {
        if ( std::strcmp( name, pInput->GetName().c_str() ) != 0 )
        {
            return pInput;
        }
    }
    return nullptr;
}
CShaderOutput* CShaderNode::GetOutput( const char* name ) const
{
    for ( CShaderOutput* pOut : m_outputs )
    {
        if ( std::strcmp( name, pOut->GetName().c_str() ) != 0 )
        {
            return pOut;
        }
    }
    return nullptr;
}
CShaderInput* CShaderNode::GetInput( std::string_view name ) const
{
    return GetInput( name.data() );
}
CShaderOutput* CShaderNode::GetOutput( std::string_view name ) const
{
    return GetOutput( name.data() );
}
void CShaderNode::Draw() noexcept
{
    int id { m_id };
    for ( CShaderInput* in : m_inputs )
    {
        std::array<float, 3> val { in->GetValue() };
        DrawInput( in->GetType(), in->GetName(), val, in->IsConnected(), in->HasEditorValues(), id );
        in->SetValue( val );
        ++id;
    }
    for ( const CShaderOutput* out : m_outputs )
    {
        DrawOutput( out->GetType(), out->GetName(), id );
        ++id;
    }
}
CShaderInput* CShaderNode::GetInput( int id ) const
{
    if ( id < 0 || id >= m_inputs.size() )
    {
        return nullptr;
    }
    return m_inputs[ id ];
}
CShaderOutput* CShaderNode::GetOutput( int id ) const
{
    if ( id < 0 || id >= m_outputs.size() )
    {
        return nullptr;
    }
    return m_outputs[ id ];
}
void CShaderNode::AddInputNode( const SSocketType& type )
{
    m_inputs.emplace_back( new CShaderInput( type, this ) );
}
void CShaderNode::RemoveInputNode() noexcept
{
    m_inputs.pop_back();
}
void CShaderNode::SetBindings( const std::vector<SShaderBinding>& bindings )
{
    (void)bindings;
}

const char* SShaderBinding::ToString( SShaderBinding::layout type )
{
    switch ( type )
    {
        case layout::input: return "layout input";
        case layout::subpass_input: return "layout subpass input";
        case layout::output: return "layout output";
        case layout::subpass_output: return "layout subpass output";
        case layout::image: return "layout image";
        case layout::image_sampler: return "layout image sampler";
        case layout::image_storage: return "layout image storage";
        case layout::sampler: return "layout sampler";
        case layout::uniform: return "layout uniform";
        case layout::storage: return "layout storage";
        default: break;
    }
    return nullptr;
}
const char* SShaderBinding::ToString( SShaderBinding::value_type type )
{
    if ( valueTypeStrings.contains( type ) )
    {
        return valueTypeStrings.at( type ).data();
    }
    return nullptr;
}

const char* ToString( shader_stage type )
{
    switch ( type )
    {
        case shader_stage_vertex: return "vertex_shader";
        case shader_stage_tessellation_control: return "tesselation_control_shader";
        case shader_stage_tessellation_evaluation: return "tesselation_evaluation_shader";
        case shader_stage_geometry: return "geometry_shader";
        case shader_stage_fragment: return "fragment_shader";
        case shader_stage_compute: return "compute_shader";
        case shader_stage_raygen: return "raygen_shader";
        case shader_stage_any_hit: return "any_hit_shader";
        case shader_stage_closest_hit: return "closest_hit_shader";
        case shader_stage_miss: return "miss_shader";
        case shader_stage_intersection: return "intersection_shader";
        case shader_stage_callable: return "callable_shader";
        case shader_stage_task: return "task_shader";
        case shader_stage_mesh: return "mesh_shader";
        case shader_stage_all:
        case shader_stage_max: break;
    }
    return nullptr;
}
const char* ToStringUi( shader_stage type )
{
    switch ( type )
    {
        case shader_stage_vertex: return "Vertex Shader";
        case shader_stage_tessellation_control: return "Tesselation Control Shader";
        case shader_stage_tessellation_evaluation: return "Tesselation Evaluation Shader";
        case shader_stage_geometry: return "Geometry Shader";
        case shader_stage_fragment: return "Fragment Shader";
        case shader_stage_compute: return "Compute Shader";
        case shader_stage_raygen: return "Vertex Shader";
        case shader_stage_any_hit: return "Any Hit Shader";
        case shader_stage_closest_hit: return "Closest Hit Shader";
        case shader_stage_miss: return "Miss Shader";
        case shader_stage_intersection: return "Intersection Shader";
        case shader_stage_callable: return "Callable Shader";
        case shader_stage_task: return "Task Shader";
        case shader_stage_mesh: return "Mesh Shader";
        case shader_stage_all:
        case shader_stage_max: break;
    }
    return nullptr;
}
const char* ToDefine( shader_stage type )
{
    switch ( type )
    {
        case shader_stage_vertex: return "VERTEX_SHADER";
        case shader_stage_tessellation_control: return "TESSELLATION_CONTROL_SHADER";
        case shader_stage_tessellation_evaluation: return "TESSELLATION_EVALUATION_SHADER";
        case shader_stage_geometry: return "GEOMETRY_SHADER";
        case shader_stage_fragment: return "FRAGMENT_SHADER";
        case shader_stage_compute: return "COMPUTE_SHADER";
        case shader_stage_raygen: return "RAYGEN_SHADER";
        case shader_stage_any_hit: return "ANY_HIT_SHADER";
        case shader_stage_closest_hit: return "CLOSEST_HIT_SHADER";
        case shader_stage_miss: return "MISS_SHADER";
        case shader_stage_intersection: return "INTERSECTION_SHADER";
        case shader_stage_callable: return "CALLABLE_SHADER";
        case shader_stage_task: return "TASK_SHADER";
        case shader_stage_mesh: return "MESH_SHADER";
        case shader_stage_all:
        case shader_stage_max: break;
    }
    return nullptr;
}
const char* ToDefine( SSocketType::compiler_define type )
{
    switch ( type )
    {
        case SSocketType::compiler_define_uses_model_view: return "USES_MODEL_VIEW";
        case SSocketType::compiler_define_uses_world_position: return "USES_WORLD_POSITION";
        case SSocketType::compiler_define_uses_normal: return "USES_NORMAL";
        case SSocketType::compiler_define_uses_tangent: return "USES_TANGENT";
        case SSocketType::compiler_define_uses_colour: return "USES_COLOUR";
        case SSocketType::compiler_define_uses_uv: return "USES_UV";
        case SSocketType::compiler_define_uses_position_offset: return "USES_POSITION_OFFSET";
        case SSocketType::compiler_define_diffuse_lambert: return "0";
        case SSocketType::compiler_define_diffuse_burley: return "1";
        case SSocketType::compiler_define_diffuse_oren_nayar: return "2";
        case SSocketType::compiler_define_specular_blin_phong: return "0";
        case SSocketType::compiler_define_specular_beckmann: return "1";
        case SSocketType::compiler_define_specular_ggx: return "2";
        case SSocketType::compiler_define_specular_implicit: return "0";
        case SSocketType::compiler_define_specular_neumann: return "1";
        case SSocketType::compiler_define_specular_cook_torrance: return "2";
        case SSocketType::compiler_define_specular_kelemen: return "3";
        case SSocketType::compiler_define_specular_ggx_correlated: return "4";
        case SSocketType::compiler_define_specular_ggx_correlated_fast: return "5";
        case SSocketType::compiler_define_specular_kelemen_two: return "6";
        case SSocketType::compiler_define_specular_neubelt: return "7";
        case SSocketType::compiler_define_fresnel_cook_torrance: return "0";
        case SSocketType::compiler_define_fresnel_schlick: return "1";
    }
    return nullptr;
}

bool SShaderBinding::IsBindingSimularEnough( const SShaderBinding& b1, const SShaderBinding& b2 )
{
    uint8_t identicalCounter {};
    identicalCounter += uint8_t( b1.layoutType == b2.layoutType );
    identicalCounter += uint8_t( b1.valueType == b2.valueType );
    identicalCounter += uint8_t( b1.set == b2.set );
    identicalCounter += uint8_t( b1.binding == b2.binding );
    identicalCounter += uint8_t( b1.arraySize == b2.arraySize );
    identicalCounter += uint8_t( std::strcmp( b1.name, b2.name ) == 0 );
    identicalCounter += uint8_t( std::strcmp( b1.instanceName, b2.instanceName ) == 0 );
    identicalCounter += uint8_t( std::strcmp( b1.arraySizeString, b2.arraySizeString ) == 0 );
    identicalCounter += uint8_t( b1.members.size() == b2.members.size() );

    return identicalCounter >= 9u;
}