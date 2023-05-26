//
// Created by joran on 02/03/2023.
//


#include "OutputNode.h"
#include "../Attribute.h"
#include "../../../../Renderer/EditorGui.h"

COutputNode::COutputNode( int& id )
        : CShaderNode{
        id, {
                SSocketType{
                        .type=SSocketType::var_type_shader, .name=ToString( shader_stage_vertex ), .uiName=ToStringUi(
                                shader_stage_vertex
                        )
                }}, {}}
{
    id += shader_stage_max - 1;
    m_allShaderNames.reserve( shader_stage_max );
    for ( uint16_t i{ shader_stage_vertex }; i < shader_stage_max; i <<= 1 )
    {
        m_allShaderNames.emplace_back( ToStringUi((shader_stage) i ));
    }
}
void COutputNode::Draw()
{
    int id{ GetId() };
    StartNode( "Output", id, 128 );
    for ( auto& shader : m_shaders )
    {
        CShaderInput* pIn = GetInput( id );
        std::array<float, 3> val{ pIn->GetValue() };
        DrawInput( pIn->GetType(), pIn->GetName(), val, pIn->IsConnected(), pIn->HasEditorValues(), id );
        pIn->SetValue( val );
        ImGui::SameLine();
        uint64_t shaderId{ (uint64_t) std::log2((uint64_t) shader ) };
        if ( FawnForge::Gui::DrawComboBox(
                std::to_string( id++ ), shaderId, m_allShaderNames, {}, 100.f, true
        ))
        {
            shader = (shader_stage) ( 1 << shaderId );
            pIn->SetSocketType(
                    SSocketType{
                            pIn->HasEditorValues(),
                            pIn->GetType(),
                            pIn->GetFlags(),
                            ToString( shader ),
                            m_allShaderNames[shaderId]
                    }
            );
        }
    }
    
    bool add{ ImGui::Button( "+" ) };
    ImGui::SameLine();
    bool remove{ ImGui::Button( "-" ) };
    EndNode();
    
    if ( add && m_shaders.size() < shader_stage_max )
    {
        m_shaders.emplace_back( shader_stage_vertex );
        AddInputNode(
                SSocketType{
                        .type=SSocketType::var_type_shader, .name=ToString( shader_stage_vertex ), .uiName=ToStringUi(
                                shader_stage_vertex
                        )
                }
        );
    }
    if ( remove && !m_shaders.empty())
    {
        m_shaders.pop_back();
        RemoveInputNode();
    }
    
}
std::string COutputNode::GetCode() const
{
    std::string shader{ "#version 450\n" };
    int         id{ GetId() };
    for ( auto& shaderType : m_shaders )
    {
        std::string define{ ToDefine( shaderType ) };
        shader += "#if defined(" + define + ")\n"\
                  "void main()\n{\n"\
                  "\t" + GetInput( id++ )->GetShaderName() + "\n"\
                  "}\n"\
                  "#endif\n";
    }
    return shader;
}
int32_t COutputNode::GetShaderTypes() const
{
    int32_t val{};
    for ( shader_stage shader : m_shaders )
    {
        val |= shader;
    }
    return val;
}
void COutputNode::GetShaderInfo( SShaderInfo& shaderInfo ) const
{
    SShaderBinding binding{
            SShaderBinding::layout_uniform,
            SShaderBinding::value_type_struct,
            0,
            0,
            1,
            "UniformBufferObject",
            "modelBufferObject"
    };
    
    // Add members to the struct
    binding.members.emplace_back(
            SShaderBinding::layout_max,
            SShaderBinding::value_type_mat_4,
            SShaderBinding::autoConfig,
            SShaderBinding::autoConfig,
            1,
            "view"
    );
    binding.members.emplace_back(
            SShaderBinding::layout_max,
            SShaderBinding::value_type_mat_4,
            SShaderBinding::autoConfig,
            SShaderBinding::autoConfig,
            1,
            "proj"
    );
    binding.members.emplace_back(
            SShaderBinding::layout_max,
            SShaderBinding::value_type_vec_4,
            SShaderBinding::autoConfig,
            SShaderBinding::autoConfig,
            1,
            "viewPos"
    );
    binding.members.emplace_back(
            SShaderBinding::layout_max,
            SShaderBinding::value_type_int,
            SShaderBinding::autoConfig,
            SShaderBinding::autoConfig,
            1,
            "displayDebugTarget"
    );
    shaderInfo.AddBinding( binding, -99, true );
    shaderInfo.AddInclude( "default.glsl", -1000000, true );
}
