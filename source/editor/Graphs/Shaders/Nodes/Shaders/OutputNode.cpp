//
// Created by Joran Vandenbroucke on 02/03/2023.
//

#include "OutputNode.h"
#include "../../../../Renderer/EditorGui.h"
#include "../Attribute.h"

COutputNode::COutputNode( int& id )
    : CShaderNode { id, { SSocketType { .type = SSocketType::var_type::shader, .name = ToString( shader_stage_vertex ), .uiName = ToStringUi( shader_stage_vertex ) } }, {} }
{
    id += shader_stage_max - 1;
    m_allShaderNames.reserve( shader_stage_max );
    for ( uint16_t i { shader_stage_vertex }; i < shader_stage_max; i <<= 1 )
    {
        m_allShaderNames.emplace_back( ToStringUi( (shader_stage)i ) );
    }
}
void COutputNode::Draw() noexcept
{
    int id { GetId() };
    StartNode( "Output", id, 128 );

    for ( int i {}; i < m_shaders.size(); ++i )
    {
        CShaderInput* pIn = GetInput( i );
        std::array<float, 3> val { pIn->GetValue() };
        DrawInput( pIn->GetType(), pIn->GetName(), val, pIn->IsConnected(), pIn->HasEditorValues(), id + i );
        pIn->SetValue( val );
        ImGui::SameLine();
        uint64_t shaderId { (uint64_t)std::log2( (uint64_t)m_shaders[ i ] ) };
        if ( FawnForge::Gui::ComboBox( std::to_string( id + i ), shaderId, m_allShaderNames, {}, 100.f, true ) )
        {
            m_shaders[ i ] = (shader_stage)( 1 << shaderId );
            pIn->SetSocketType( SSocketType { pIn->HasEditorValues(), pIn->GetType(), pIn->GetFlags(), ToString( m_shaders[ i ] ), m_allShaderNames[ shaderId ] } );
        }
    }

    bool add { ImGui::Button( "+" ) };
    ImGui::SameLine();
    bool remove { ImGui::Button( "-" ) };
    EndNode();

    if ( add && m_shaders.size() < shader_stage_max )
    {
        m_shaders.emplace_back( shader_stage_vertex );
        AddInputNode( SSocketType { .type = SSocketType::var_type::shader, .name = ToString( shader_stage_vertex ), .uiName = ToStringUi( shader_stage_vertex ) } );
    }
    if ( remove && !m_shaders.empty() )
    {
        m_shaders.pop_back();
        RemoveInputNode();
    }
}
std::string COutputNode::GetCode() const
{
    std::string shader { "#version 450\n" };
    int id { GetId() };
    for ( auto& shaderType : m_shaders )
    {
        std::string define { ToDefine( shaderType ) };
        shader += "#if defined(" + define
            + ")\n"
              "void main()\n{\n"
              "\t"
            + GetInput( id )->GetShaderName()
            + "\n"
              "}\n"
              "#endif\n";
        ++id;
    }
    return shader;
}
int32_t COutputNode::GetShaderTypes() const
{
    int32_t val {};
    for ( shader_stage shader : m_shaders )
    {
        val |= shader;
    }
    return val;
}
void COutputNode::GetShaderInfo( SShaderInfo& shaderInfo ) const
{
    SShaderBinding binding { SShaderBinding::layout::uniform, SShaderBinding::value_type::struct_type, 0, 0, 1, "UniformBufferObject", "modelBufferObject" };

    // Add members to the struct
    binding.members.emplace_back( SShaderBinding::layout::max, SShaderBinding::value_type::mat4, SShaderBinding::autoConfig, SShaderBinding::autoConfig, 1, "view" );
    binding.members.emplace_back( SShaderBinding::layout::max, SShaderBinding::value_type::mat4, SShaderBinding::autoConfig, SShaderBinding::autoConfig, 1, "proj" );
    binding.members.emplace_back( SShaderBinding::layout::max, SShaderBinding::value_type::vec4, SShaderBinding::autoConfig, SShaderBinding::autoConfig, 1, "viewPos" );
    binding.members.emplace_back( SShaderBinding::layout::max, SShaderBinding::value_type::int_type, SShaderBinding::autoConfig, SShaderBinding::autoConfig, 1, "displayDebugTarget" );
    shaderInfo.AddBinding( binding, -99, true );
    shaderInfo.AddInclude( "default.glsl", -1000000, true );
}
