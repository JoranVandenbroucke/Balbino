#include "VertexOutput.h"

#include "../../../../Renderer/EditorGui.h"
#include "../Attribute.h"

CVertexOutputNode::CVertexOutputNode( int& id )
    : CShaderNode { id,
                    { SSocketType { .hasEditorValues = false, .type = SSocketType::var_type::vector3, .name = "offset", .uiName = "Position Offset" },
                      SSocketType { .hasEditorValues = false, .type = SSocketType::var_type::vector3, .name = "normal", .uiName = "Normal" },
                      SSocketType { .hasEditorValues = false, .type = SSocketType::var_type::vector4, .name = "tangent", .uiName = "Tangent" }, SSocketType { .hasEditorValues = false, .type = SSocketType::var_type::vector4, .name = "uv", .uiName = "UV" },
                      SSocketType { .type = SSocketType::var_type::color, .name = "vertex_colour", .uiName = "Vertex Color" } },
                    { SSocketType { .type = SSocketType::var_type::shader, .name = "vertex_shader", .uiName = "Vertex Shader" } } }
    , m_flags { SSocketType::compiler_define_uses_model_view }
{
    m_flags |= SSocketType::compiler_define_uses_world_position | SSocketType::compiler_define_uses_normal;//todo: remove and let it be part of 'flags'
}
void CVertexOutputNode::Draw() noexcept
{
    bool hasChanged;
    StartNode( "Vertex output", GetId() );
    hasChanged = FawnForge::Gui::Toggle( "Use Model View Matrix", m_useModelView );
    hasChanged |= FawnForge::Gui::Toggle( "Use Normals", m_useNormal );
    CShaderNode::Draw();
    EndNode();

    if ( hasChanged )
    {
        m_flags ^= SSocketType::compiler_define_uses_model_view & m_useModelView | SSocketType::compiler_define_uses_model_view & m_useNormal;
    }
}
std::string CVertexOutputNode::GetCode() const
{
    return "vertex(\n"
           "inPosition,\n"
           "modelBufferObject.proj,\n"
           "\n"
           "#if defined(USES_MODEL_VIEW)\n"
           "modelBufferObject.view,\n"
           "instanceModelMatrix,\n"
           "#endif\n"
           "\n"
           "#if defined(USES_WORLD_POSITION)\n"
           "outWorldPos,\n"
           "#endif\n"
           "\n"
           "#if defined(USES_NORMAL)\n"
           "inNormal,\n"
           "outNormal,\n"
           "#endif\n"
           "\n"
           "#if defined(USES_TANGENT)\n"
           "inTangent,\n"
           "outTangent,\n"
           "#endif\n"
           "\n"
           "#if defined(USES_COLOUR)\n"
           "inColor,\n"
           "outColor,\n"
           "#endif\n"
           "\n"
           "#if defined(USES_UV)\n"
           "inTexCoord,\n"
           "outTexCoord,\n"
           "#endif\n"
           "\n"
           "#if defined(USES_POSITION_OFFSET)\n"
           "offset,\n"
           "#endif\n"
           "\n"
           "gl_Position\n"
           ");";
}
void CVertexOutputNode::GetShaderInfo( SShaderInfo& shaderInfo ) const
{
    shaderInfo.shaderTypes = shader_stage_vertex;
    shaderInfo.AddInclude( "node_vertex.glsl" );
    shaderInfo.AddBinding( { SShaderBinding::layout::input, SShaderBinding::value_type::vec3, 0, 0, 1, "inPosition" }, -99 );
    if ( m_flags & SSocketType::compiler_define_uses_colour && !GetInput( 4 )->IsConnected() )
    {
        shaderInfo.AddBinding( { SShaderBinding::layout::input, SShaderBinding::value_type::vec4, 0, 1, 1, "inColor" }, -99 );
        shaderInfo.AddBinding( { SShaderBinding::layout::output, SShaderBinding::value_type::vec4, 0, 0, 1, "outColor" }, -99 );
    }
    if ( m_flags & SSocketType::compiler_define_uses_uv && !GetInput( 3 )->IsConnected() )
    {
        shaderInfo.AddBinding( { SShaderBinding::layout::input, SShaderBinding::value_type::vec2, 0, 2, 1, "inTexCoord" }, -99 );
        shaderInfo.AddBinding( { SShaderBinding::layout::output, SShaderBinding::value_type::vec2, 0, 1, 1, "outTexCoord" }, -99 );
    }
    if ( m_useNormal && m_flags & SSocketType::compiler_define_uses_normal && !GetInput( 1 )->IsConnected() )
    {
        shaderInfo.AddBinding( { SShaderBinding::layout::input, SShaderBinding::value_type::vec3, 0, 3, 1, "inNormal" }, -99 );
        shaderInfo.AddBinding( { SShaderBinding::layout::output, SShaderBinding::value_type::vec3, 0, 2, 1, "outNormal" }, -99 );
    }
    if ( m_flags & SSocketType::compiler_define_uses_tangent && !GetInput( 2 )->IsConnected() )
    {
        shaderInfo.AddBinding( { SShaderBinding::layout::input, SShaderBinding::value_type::vec4, 0, 4, 1, "inTangent" }, -99 );
        shaderInfo.AddBinding( { SShaderBinding::layout::output, SShaderBinding::value_type::vec4, 0, 3, 1, "outTangent" }, -99 );
    }
    if ( m_flags & SSocketType::compiler_define_uses_world_position )
    {
        shaderInfo.AddBinding( { SShaderBinding::layout::output, SShaderBinding::value_type::vec4, 0, 4, 1, "outWorldPos" }, -99 );
    }
    if ( m_flags & SSocketType::compiler_define_uses_position_offset )
    {
        shaderInfo.AddDefine( ToDefine( SSocketType::compiler_define_uses_position_offset ), "" );
    }
    if ( m_useModelView )
    {
        shaderInfo.AddDefine( ToDefine( SSocketType::compiler_define_uses_model_view ), "" );
        shaderInfo.AddBinding( { SShaderBinding::layout::input, SShaderBinding::value_type::mat4, 0, 5, 1, "instanceModelMatrix" }, -99 );
    }
}
void CVertexOutputNode::SetFlag( SSocketType::compiler_define flags ) noexcept
{
    bool hasValue = GetInput( 0 )->IsConnected() || ( std::abs( GetInput( 0 )->GetValue()[ 0 ] ) < 1e-5f && std::abs( GetInput( 0 )->GetValue()[ 1 ] ) < 1e-5f && std::abs( GetInput( 0 )->GetValue()[ 2 ] ) < 1e-5f );
    m_flags       = flags | ( m_useModelView ? SSocketType::compiler_define_uses_model_view : 0 ) | ( hasValue ? SSocketType::compiler_define_uses_position_offset : 0 );
}
