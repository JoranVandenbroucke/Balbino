#pragma once

#include <array>

#include "imgui.h"
#include "imnodes.h"


inline void DrawInputFloatAttribute( float& value, const int id, const bool connected, const char* title = "Value", const float min = 0.0f, const float max = 0.0f, const float speed = 0.01f )
{
    ImNodes::PushColorStyle( ImNodesCol_Pin, 0xffa1a1a1 );
    ImNodes::PushColorStyle( ImNodesCol_PinHovered, 0xffa1a1a1 );
    ImNodes::BeginInputAttribute( id );
    if ( !connected )
    {
        ImGui::SetNextItemWidth( 128 );
        ImGui::DragFloat( title, &value, speed, min, max );
    }
    else
    {
        ImGui::Text( "%s", title );
    }
    ImNodes::EndInputAttribute();
    ImNodes::PopColorStyle();
    ImNodes::PopColorStyle();
}

inline void DrawInputColorAttribute( std::array<float, 3>& value, int id, bool connected, const char* title = "Color" )
{
    ImNodes::PushColorStyle( ImNodesCol_Pin, 0xff29c7c7 );
    ImNodes::PushColorStyle( ImNodesCol_PinHovered, 0xff29c7c7 );
    ImNodes::BeginInputAttribute( id );
    if ( !connected )
    {
        ImGui::SetNextItemWidth( 128 );
        ImGui::PushStyleColor( ImGuiCol_Button, ImVec4{ value[0], value[1], value[2], 1.0f } );
        ImGui::PushStyleColor( ImGuiCol_ButtonHovered, ImVec4{ value[0], value[1], value[2], 1.0f } );
        ImGui::PushStyleColor( ImGuiCol_ButtonActive, ImVec4{ value[0], value[1], value[2], 1.0f } );
        ImGui::ColorEdit3( title, value.data(), ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel );
        ImGui::SameLine(); ImGui::Text( "%s", title );
        ImGui::PopStyleColor( 3 );
    }
    else
    {
        ImGui::Text( "%s", title );
    }
    ImNodes::EndInputAttribute();
    ImNodes::PopColorStyle();
    ImNodes::PopColorStyle();
}

inline void DrawInputVectorAttribute( std::array<float, 3>& value, int id, bool connected, const char* title = "Vector" )
{
    ImNodes::PushColorStyle( ImNodesCol_Pin, 0xffc76363 );
    ImNodes::PushColorStyle( ImNodesCol_PinHovered, 0xffc76363 );
    ImNodes::BeginInputAttribute( id );
    ImGui::Text( "%s", title );
    if ( !connected )
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


inline void DrawOutputFloatAttribute( int id, const char* title = "output:" )
{
    ImNodes::PushColorStyle( ImNodesCol_Pin, 0xffa1a1a1 );
    ImNodes::PushColorStyle( ImNodesCol_PinHovered, 0xffa1a1a1 );
    ImNodes::BeginOutputAttribute( id );
    ImGui::Text( "%s", title );
    ImNodes::EndOutputAttribute();
    ImNodes::PopColorStyle();
    ImNodes::PopColorStyle();
}

inline void DrawOutputColorAttribute( int id, const char* title = "output:" )
{
    ImNodes::PushColorStyle( ImNodesCol_Pin, 0xff29c7c7 );
    ImNodes::PushColorStyle( ImNodesCol_PinHovered, 0xff29c7c7 );
    ImNodes::BeginOutputAttribute( id );
    ImGui::Text( "%s", title );
    ImNodes::EndOutputAttribute();
    ImNodes::PopColorStyle();
    ImNodes::PopColorStyle();
}

inline void DrawOutputVectorAttribute( int id, const char* title = "output:" )
{
    ImNodes::PushColorStyle( ImNodesCol_Pin, 0xffc76363 );
    ImNodes::PushColorStyle( ImNodesCol_PinHovered, 0xffc76363 );
    ImNodes::BeginOutputAttribute( id );
    ImGui::Text( "%s", title );
    ImNodes::EndOutputAttribute();
    ImNodes::PopColorStyle();
    ImNodes::PopColorStyle();
}

inline void DrawOutputTextureAttribute( int id, const char* title = "output: " )
{
    ImNodes::PushColorStyle( ImNodesCol_Pin, 0xff29c7c7 );
    ImNodes::PushColorStyle( ImNodesCol_PinHovered, 0xff29c7c7 );
    ImNodes::BeginOutputAttribute( id );
    ImGui::Text( "%s", title );
    ImNodes::EndOutputAttribute();
    ImNodes::PopColorStyle();
    ImNodes::PopColorStyle();
}

inline void DrawInputShaderAttribute( const char* title, int id )
{
    ImNodes::PushColorStyle( ImNodesCol_Pin, 0xffc729c7 );
    ImNodes::PushColorStyle( ImNodesCol_PinHovered, 0xffc729c7 );
    ImNodes::BeginInputAttribute( id, ImNodesPinShape_QuadFilled );
    ImGui::Text( "%s", title );
    ImNodes::EndOutputAttribute();
    ImNodes::PopColorStyle();
    ImNodes::PopColorStyle();
}

inline void DrawOutputShaderAttribute( const char* title, int id )
{
    ImNodes::PushColorStyle( ImNodesCol_Pin, 0xffc729c7 );
    ImNodes::PushColorStyle( ImNodesCol_PinHovered, 0xffc729c7 );
    ImNodes::BeginOutputAttribute( id, ImNodesPinShape_QuadFilled );
    ImGui::Text( "%s", title );
    ImNodes::EndOutputAttribute();
    ImNodes::PopColorStyle();
    ImNodes::PopColorStyle();
}

inline void EndNode()
{
    ImNodes::EndNode();
}

inline void StartNode( const char* title, const int id, float startWidth = 100 )
{
    ImGui::SetNextItemWidth( startWidth );
    ImNodes::BeginNode( id );
    
    ImNodes::BeginNodeTitleBar();
    ImGui::TextUnformatted( title );
    ImNodes::EndNodeTitleBar();
}
