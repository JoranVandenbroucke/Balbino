#pragma once

#include <array>

#include "../../../Renderer/Imgui/imgui.h"
#include "../../../Renderer/Imgui/imnodes.h"
#include "ShaderNode.h"

inline void StartNode( const char* name, int id, float startWidth = 100 )
{
    ImGui::SetNextItemWidth( startWidth );
    ImNodes::BeginNode( id );
    
    ImNodes::BeginNodeTitleBar();
    ImGui::TextUnformatted( name );
    ImNodes::EndNodeTitleBar();
}
inline void EndNode()
{
    ImNodes::EndNode();
}

inline void DrawInputFloatAttribute( const char* name, float& value, bool connected, bool hasEditorValues, int id )
{
    ImNodes::PushColorStyle( ImNodesCol_Pin, 0xffa1a1a1 );
    ImNodes::PushColorStyle( ImNodesCol_PinHovered, 0xffa1a1a1 );
    ImNodes::BeginInputAttribute( id );
    if ( !connected && hasEditorValues )
    {
        ImGui::SetNextItemWidth( 128 );
        ImGui::DragFloat( name, &value );
    }
    else
    {
        ImGui::Text( "%s", name );
    }
    ImNodes::EndInputAttribute();
    ImNodes::PopColorStyle();
    ImNodes::PopColorStyle();
}
inline void DrawInputColorAttribute( const char* name, std::array<float, 3>& value, bool connected, bool hasEditorValues, int id )
{
    ImNodes::PushColorStyle( ImNodesCol_Pin, 0xff29c7c7 );
    ImNodes::PushColorStyle( ImNodesCol_PinHovered, 0xff29c7c7 );
    ImNodes::BeginInputAttribute( id );
    if ( !connected && hasEditorValues )
    {
        ImGui::SetNextItemWidth( 128 );
        ImGui::PushStyleColor( ImGuiCol_Button, ImVec4{ value[0], value[1], value[2], 1.0f } );
        ImGui::PushStyleColor( ImGuiCol_ButtonHovered, ImVec4{ value[0], value[1], value[2], 1.0f } );
        ImGui::PushStyleColor( ImGuiCol_ButtonActive, ImVec4{ value[0], value[1], value[2], 1.0f } );
        ImGui::ColorEdit3( name, value.data(), ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel );
        ImGui::SameLine();
        ImGui::Text( "%s", name );
        ImGui::PopStyleColor( 3 );
    }
    else
    {
        ImGui::Text( "%s", name );
    }
    ImNodes::EndInputAttribute();
    ImNodes::PopColorStyle();
    ImNodes::PopColorStyle();
}
inline void DrawInputVectorAttribute( const char* name, std::array<float, 3>& value, bool connected, bool hasEditorValues, int id )
{
    ImNodes::PushColorStyle( ImNodesCol_Pin, 0xffc76363 );
    ImNodes::PushColorStyle( ImNodesCol_PinHovered, 0xffc76363 );
    ImNodes::BeginInputAttribute( id );
    ImGui::Text( "%s", name );
    if ( !connected && hasEditorValues )
    {
        ImGui::PushItemWidth( 128 );
        ImGui::DragFloat( "X", &value[0] );
        ImGui::DragFloat( "Y", &value[1] );
        ImGui::DragFloat( "Z", &value[2] );
        ImGui::PopItemWidth();
    }
    ImNodes::EndInputAttribute();
    ImNodes::PopColorStyle();
    ImNodes::PopColorStyle();
}
inline void DrawInputShaderAttribute( const char* name, int id )
{
    ImNodes::PushColorStyle( ImNodesCol_Pin, 0xffc763c7 );
    ImNodes::PushColorStyle( ImNodesCol_PinHovered, 0xffc763c7 );
    ImNodes::BeginInputAttribute( id, ImNodesPinShape_QuadFilled );
    ImGui::Text( "%s", name );
    ImNodes::EndInputAttribute();
    ImNodes::PopColorStyle();
    ImNodes::PopColorStyle();
}

inline void DrawOutputFloatAttribute( const char* name, int id )
{
    ImNodes::PushColorStyle( ImNodesCol_Pin, 0xffa1a1a1 );
    ImNodes::PushColorStyle( ImNodesCol_PinHovered, 0xffa1a1a1 );
    ImNodes::BeginOutputAttribute( id );
    ImGui::Text( "%s", name );
    ImNodes::EndOutputAttribute();
    ImNodes::PopColorStyle();
    ImNodes::PopColorStyle();
}
inline void DrawOutputColorAttribute( const char* name, int id )
{
    ImNodes::PushColorStyle( ImNodesCol_Pin, 0xff29c7c7 );
    ImNodes::PushColorStyle( ImNodesCol_PinHovered, 0xff29c7c7 );
    ImNodes::BeginOutputAttribute( id );
    ImGui::Text( "%s", name );
    ImNodes::EndOutputAttribute();
    ImNodes::PopColorStyle();
    ImNodes::PopColorStyle();
}
inline void DrawOutputVectorAttribute( const char* name, int id )
{
    ImNodes::PushColorStyle( ImNodesCol_Pin, 0xffc76363 );
    ImNodes::PushColorStyle( ImNodesCol_PinHovered, 0xffc76363 );
    ImNodes::BeginOutputAttribute( id );
    ImGui::Text( "%s", name );
    ImNodes::EndOutputAttribute();
    ImNodes::PopColorStyle();
    ImNodes::PopColorStyle();
}
inline void DrawOutputShaderAttribute( const char* name, int id )
{
    ImNodes::PushColorStyle( ImNodesCol_Pin, 0xffc763c7 );
    ImNodes::PushColorStyle( ImNodesCol_PinHovered, 0xffc763c7 );
    ImNodes::BeginOutputAttribute( id, ImNodesPinShape_QuadFilled );
    ImGui::Text( "%s", name );
    ImNodes::EndOutputAttribute();
    ImNodes::PopColorStyle();
    ImNodes::PopColorStyle();
}

inline void DrawInput( const SSocketType::var_type& type, std::string_view name, std::array<float, 3>& value, bool isConnected, bool hasEditorValues, int id )
{
    switch ( type )
    {
        case SSocketType::var_type_none:
            break;
        case SSocketType::var_type_float:
            DrawInputFloatAttribute( name.data(), value[0], isConnected, hasEditorValues, id );
            break;
        case SSocketType::var_type_color:
            DrawInputColorAttribute( name.data(), value, isConnected, hasEditorValues, id );
            break;
        case SSocketType::var_type_vector_3:
        case SSocketType::var_type_vector_4:
            DrawInputVectorAttribute( name.data(), value, isConnected, hasEditorValues, id );
            break;
        case SSocketType::var_type_shader:
            DrawInputShaderAttribute( name.data(), id );
            break;
    }
}
inline void DrawOutput( const SSocketType::var_type& type, std::string_view name, int id )
{
    switch ( type )
    {
        case SSocketType::var_type_none:
            break;
        case SSocketType::var_type_float:
            DrawOutputFloatAttribute( name.data(), id );
            break;
        case SSocketType::var_type_color:
            DrawOutputColorAttribute( name.data(), id );
            break;
        case SSocketType::var_type_vector_3:
        case SSocketType::var_type_vector_4:
            DrawOutputVectorAttribute( name.data(), id );
            break;
        case SSocketType::var_type_shader:
            DrawOutputShaderAttribute( name.data(), id );
            break;
    }
}
