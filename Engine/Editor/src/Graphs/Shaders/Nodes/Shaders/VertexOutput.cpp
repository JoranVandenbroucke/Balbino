#include <unordered_set>
#include "VertexOutput.h"

#include "../Attribute.h"
#include "../../../../Renderer/EditorGui.h"

CVertexOutputNode::CVertexOutputNode( int& id )
        : CShaderNode{
        id,
        {
                SSocketType{ .hasEditorValues=false, .type=SSocketType::var_type_vector_3, .name="offset", .uiName="Position Offset" },
                SSocketType{ .hasEditorValues=false, .type=SSocketType::var_type_vector_3, .name="normal", .uiName="Normal" },
                SSocketType{ .hasEditorValues=false, .type=SSocketType::var_type_vector_4, .name="tangent", .uiName="Tangent" },
                SSocketType{ .hasEditorValues=false, .type=SSocketType::var_type_vector_4, .name="uv", .uiName="UV" },
                SSocketType{ .type=SSocketType::var_type_color, .name="vertex_colour", .uiName="Vertex Color" }},
        { SSocketType{ .type=SSocketType::var_type_shader, .name="vertex_shader", .uiName="Vertex Shader" }}}
        , m_flags{ SSocketType::compiler_define_uses_model_view }
{
}
void CVertexOutputNode::Draw()
{
    bool hasChanged;
    StartNode( "Vertex Output", GetId());
    hasChanged = FawnForge::Gui::DrawToggle( "Use Model View Matrix", m_useModelView );
    CShaderNode::Draw();
    EndNode();
    
    if ( hasChanged )
    {
        m_flags ^= SSocketType::compiler_define_uses_model_view;
    }
}
std::string CVertexOutputNode::GetCode() const
{
    return "vertex(\n"
           "inPosition,\n"
           "modelBufferObject.proj,\n"
           "\n"
           "#if defined(USED_MODEL_VIEW)\n"
           "modelBufferObject.view,\n"
           "instanceModelMatrix,\n"
           "#endif\n"
           "\n"
           "#if defined(USES_WORD_POSITION)\n"
           "worldPos,\n"
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
    shaderInfo.AddBinding(
            { SShaderBinding::layout_input, SShaderBinding::value_type_vec_3, 0, 0, 1,"inPosition" }
    );
    if ( m_flags & SSocketType::compiler_define_uses_colour && !GetInput( 4 )->IsConnected())
    {
        shaderInfo.AddBinding(
                { SShaderBinding::layout_input, SShaderBinding::value_type_vec_4, 0, 1, 1,"inColor" }
        );
        shaderInfo.AddBinding(
                { SShaderBinding::layout_output, SShaderBinding::value_type_vec_4, 0, 0, 1,"outColor" }
        );
    }
    if ( m_flags & SSocketType::compiler_define_uses_uv && !GetInput( 3 )->IsConnected())
    {
        shaderInfo.AddBinding(
                { SShaderBinding::layout_input, SShaderBinding::value_type_vec_2, 0, 2, 1,"inTexCoord" }
        );
        shaderInfo.AddBinding(
                { SShaderBinding::layout_output, SShaderBinding::value_type_vec_2, 0, 1, 1,"outTexCoord" }
        );
    }
    if ( m_flags & SSocketType::compiler_define_uses_normal && !GetInput( 1 )->IsConnected())
    {
        shaderInfo.AddBinding(
                { SShaderBinding::layout_input, SShaderBinding::value_type_vec_3, 0, 3, 1,"inNormal" }
        );
        shaderInfo.AddBinding(
                { SShaderBinding::layout_output, SShaderBinding::value_type_vec_3, 0, 2, 1,"outNormal" }
        );
    }
    if ( m_flags & SSocketType::compiler_define_uses_tangent && !GetInput( 2 )->IsConnected())
    {
        shaderInfo.AddBinding(
                { SShaderBinding::layout_input, SShaderBinding::value_type_vec_4, 0, 4, 1,"inTangent" }
        );
        shaderInfo.AddBinding(
                { SShaderBinding::layout_output, SShaderBinding::value_type_vec_4, 0, 3, 1,"outTangent" }
        );
    }
    if ( m_flags & SSocketType::compiler_define_uses_word_position )
    {
        shaderInfo.AddBinding(
                { SShaderBinding::layout_output, SShaderBinding::value_type_vec_4, 0, 4, 1,"outWorldPos" }
        );
    }
    if(m_flags & SSocketType::compiler_define_uses_position_offset)
    {
        shaderInfo.AddDefine( ToDefine(SSocketType::compiler_define_uses_position_offset), "" );
    }
    shaderInfo.AddBinding(
            { SShaderBinding::layout_input, SShaderBinding::value_type_mat_4, 0, 5, 1,"instanceModelMatrix" }
    );
}
void CVertexOutputNode::SetFlag( SSocketType::compiler_define flags )
{
    bool hasValue = GetInput(0)->IsConnected() || (std::abs(GetInput(0)->GetValue()[0]) < 1e-5f && std::abs(GetInput(0)->GetValue()[1]) < 1e-5f && std::abs(GetInput(0)->GetValue()[2]) < 1e-5f);
    m_flags = flags | ( m_useModelView ? SSocketType::compiler_define_uses_model_view : 0 ) | (hasValue?SSocketType::compiler_define_uses_position_offset:0);
}
