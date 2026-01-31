//
// Created by Joran on 12/06/2022.
//
#include "EditorGui.h"

#define IMGUI_DEFINE_MATH_OPERATORS
#include <array>
#include <format>

#include <imgui.h>
#include <imgui_internal.h>

namespace FawnForge::Gui
{
    void IntValue( const std::string_view name, const uint64_t value, const float width )
    {
        ImGui::PushID( name.data() );

        ImGui::BeginColumns( name.data(), 2, ImGuiOldColumnFlags_NoResize );
        ImGui::SetColumnWidth( 0, width );
        ImGui::Text( "%s", name.data() );
        ImGui::NextColumn();
        ImGui::Text( "%llu", value );
        ImGui::EndColumns();

        ImGui::PopID();
    }
    void IntValue( const std::string_view name, const int value, const float width )
    {
        ImGui::PushID( name.data() );

        ImGui::BeginColumns( name.data(), 2, ImGuiOldColumnFlags_NoResize );
        ImGui::SetColumnWidth( 0, width );
        ImGui::Text( "%s", name.data() );
        ImGui::NextColumn();

        ImGui::Value( "##V", value );

        ImGui::EndColumns();

        ImGui::PopID();
    }
    void Int2Value( const std::string_view name, const int* value, const float width )
    {
        const ImGuiIO& io   = ImGui::GetIO();
        const auto boldFont = io.Fonts->Fonts[ 0 ];

        ImGui::PushID( name.data() );

        ImGui::BeginColumns( name.data(), 2, ImGuiOldColumnFlags_NoResize );
        ImGui::SetColumnWidth( 0, width );
        ImGui::Text( "%s", name.data() );
        ImGui::NextColumn();

        ImGui::PushMultiItemsWidths( 2, ImGui::CalcItemWidth() );
        ImGui::PushStyleVar( ImGuiStyleVar_ItemSpacing, ImVec2 { 0, 0 } );

        ImGui::PushFont( boldFont );
        ImGui::Text( "X" );
        ImGui::PopFont();

        ImGui::SameLine();
        ImGui::Value( "##X", value[ 0 ] );
        ImGui::PopItemWidth();
        ImGui::SameLine();

        ImGui::PushFont( boldFont );
        ImGui::Text( "Y" );
        ImGui::PopFont();

        ImGui::SameLine();
        ImGui::Value( "##Y", value[ 1 ] );
        ImGui::PopItemWidth();
        ImGui::SameLine();

        ImGui::PopStyleVar();

        ImGui::EndColumns();

        ImGui::PopID();
    }
    void Int3Value( const std::string_view name, const int* value, const float width )
    {
        const ImGuiIO& io   = ImGui::GetIO();
        const auto boldFont = io.Fonts->Fonts[ 0 ];

        ImGui::PushID( name.data() );

        ImGui::BeginColumns( name.data(), 2, ImGuiOldColumnFlags_NoResize );
        ImGui::SetColumnWidth( 0, width );
        ImGui::Text( "%s", name.data() );
        ImGui::NextColumn();

        ImGui::PushMultiItemsWidths( 3, ImGui::CalcItemWidth() );
        ImGui::PushStyleVar( ImGuiStyleVar_ItemSpacing, ImVec2 { 0, 0 } );

        ImGui::PushFont( boldFont );
        ImGui::Text( "X" );
        ImGui::PopFont();

        ImGui::SameLine();
        ImGui::Value( "##X", value[ 0 ] );
        ImGui::PopItemWidth();
        ImGui::SameLine();

        ImGui::PushFont( boldFont );
        ImGui::Text( "Y" );
        ImGui::PopFont();

        ImGui::SameLine();
        ImGui::Value( "##Y", value[ 1 ] );
        ImGui::PopItemWidth();
        ImGui::SameLine();

        ImGui::PushFont( boldFont );
        ImGui::Text( "Z" );
        ImGui::PopFont();

        ImGui::SameLine();
        ImGui::Value( "##Z", value[ 2 ] );
        ImGui::PopItemWidth();

        ImGui::PopStyleVar();

        ImGui::EndColumns();

        ImGui::PopID();
    }
    void Int4Value( const std::string_view name, const int* value, const float width )
    {
        const ImGuiIO& io   = ImGui::GetIO();
        const auto boldFont = io.Fonts->Fonts[ 0 ];

        ImGui::PushID( name.data() );

        ImGui::BeginColumns( name.data(), 2, ImGuiOldColumnFlags_NoResize );
        ImGui::SetColumnWidth( 0, width );
        ImGui::Text( "%s", name.data() );
        ImGui::NextColumn();

        ImGui::PushMultiItemsWidths( 4, ImGui::CalcItemWidth() );
        ImGui::PushStyleVar( ImGuiStyleVar_ItemSpacing, ImVec2 { 0, 0 } );

        ImGui::PushFont( boldFont );
        ImGui::Text( "X" );
        ImGui::PopFont();

        ImGui::SameLine();
        ImGui::Value( "##X", value[ 0 ] );
        ImGui::PopItemWidth();
        ImGui::SameLine();

        ImGui::PushFont( boldFont );
        ImGui::Text( "Y" );
        ImGui::PopFont();

        ImGui::SameLine();
        ImGui::Value( "##Y", value[ 1 ] );
        ImGui::PopItemWidth();
        ImGui::SameLine();

        ImGui::PushFont( boldFont );
        ImGui::Text( "Z" );
        ImGui::PopFont();

        ImGui::SameLine();
        ImGui::Value( "##Z", value[ 2 ] );
        ImGui::PopItemWidth();

        ImGui::PushFont( boldFont );
        ImGui::Text( "W" );
        ImGui::PopFont();

        ImGui::SameLine();
        ImGui::Value( "##W", value[ 3 ] );
        ImGui::PopItemWidth();

        ImGui::PopStyleVar();

        ImGui::EndColumns();

        ImGui::PopID();
    }

    void FloatValue( const std::string_view name, const float value, const float width )
    {
        ImGui::PushID( name.data() );

        ImGui::BeginColumns( name.data(), 2, ImGuiOldColumnFlags_NoResize );
        ImGui::SetColumnWidth( 0, width );
        ImGui::Text( "%s", name.data() );
        ImGui::NextColumn();

        ImGui::Value( "##V", value, "%.2f" );

        ImGui::EndColumns();

        ImGui::PopID();
    }
    void Float2Value( const std::string_view name, const float* value, const float width )
    {

        const ImGuiIO& io   = ImGui::GetIO();
        const auto boldFont = io.Fonts->Fonts[ 0 ];

        ImGui::PushID( name.data() );

        ImGui::BeginColumns( name.data(), 2, ImGuiOldColumnFlags_NoResize );
        ImGui::SetColumnWidth( 0, width );
        ImGui::Text( "%s", name.data() );
        ImGui::NextColumn();

        ImGui::PushMultiItemsWidths( 2, ImGui::CalcItemWidth() );
        ImGui::PushStyleVar( ImGuiStyleVar_ItemSpacing, ImVec2 { 0, 0 } );

        ImGui::PushFont( boldFont );
        ImGui::Text( "X" );
        ImGui::PopFont();

        ImGui::SameLine();
        ImGui::Value( "##X", value[ 0 ], "%.2f" );
        ImGui::PopItemWidth();
        ImGui::SameLine();

        ImGui::PushFont( boldFont );
        ImGui::Text( "Y" );
        ImGui::PopFont();

        ImGui::SameLine();
        ImGui::Value( "##Y", value[ 1 ], "%.2f" );
        ImGui::PopItemWidth();
        ImGui::SameLine();

        ImGui::PopStyleVar();

        ImGui::EndColumns();

        ImGui::PopID();
    }
    void Float3Value( const std::string_view name, const float* value, const float width )
    {
        const ImGuiIO& io   = ImGui::GetIO();
        const auto boldFont = io.Fonts->Fonts[ 0 ];

        ImGui::PushID( name.data() );

        ImGui::BeginColumns( name.data(), 2, ImGuiOldColumnFlags_NoResize );
        ImGui::SetColumnWidth( 0, width );
        ImGui::Text( "%s", name.data() );
        ImGui::NextColumn();

        ImGui::PushMultiItemsWidths( 3, ImGui::CalcItemWidth() );
        ImGui::PushStyleVar( ImGuiStyleVar_ItemSpacing, ImVec2 { 0, 0 } );

        ImGui::PushFont( boldFont );
        ImGui::Text( "X" );
        ImGui::PopFont();

        ImGui::SameLine();
        ImGui::Value( "##X", value[ 0 ], "%.2f" );
        ImGui::PopItemWidth();
        ImGui::SameLine();

        ImGui::PushFont( boldFont );
        ImGui::Text( "Y" );
        ImGui::PopFont();

        ImGui::SameLine();
        ImGui::Value( "##Y", value[ 1 ], "%.2f" );
        ImGui::PopItemWidth();
        ImGui::SameLine();

        ImGui::PushFont( boldFont );
        ImGui::Text( "Z" );
        ImGui::PopFont();

        ImGui::SameLine();
        ImGui::Value( "##Z", value[ 2 ], "%.2f" );
        ImGui::PopItemWidth();

        ImGui::PopStyleVar();

        ImGui::EndColumns();

        ImGui::PopID();
    }
    void Float4Value( const std::string_view name, const float* value, const float width )
    {
        const ImGuiIO& io   = ImGui::GetIO();
        const auto boldFont = io.Fonts->Fonts[ 0 ];

        ImGui::PushID( name.data() );

        ImGui::BeginColumns( name.data(), 2, ImGuiOldColumnFlags_NoResize );
        ImGui::SetColumnWidth( 0, width );
        ImGui::Text( "%s", name.data() );
        ImGui::NextColumn();

        ImGui::PushMultiItemsWidths( 4, ImGui::CalcItemWidth() );
        ImGui::PushStyleVar( ImGuiStyleVar_ItemSpacing, ImVec2 { 0, 0 } );

        ImGui::PushFont( boldFont );
        ImGui::Text( "X" );
        ImGui::PopFont();

        ImGui::SameLine();
        ImGui::Value( "##X", value[ 0 ], "%.2f" );
        ImGui::PopItemWidth();
        ImGui::SameLine();

        ImGui::PushFont( boldFont );
        ImGui::Text( "Y" );
        ImGui::PopFont();

        ImGui::SameLine();
        ImGui::Value( "##Y", value[ 1 ], "%.2f" );
        ImGui::PopItemWidth();
        ImGui::SameLine();

        ImGui::PushFont( boldFont );
        ImGui::Text( "Z" );
        ImGui::PopFont();

        ImGui::SameLine();
        ImGui::Value( "##Z", value[ 2 ], "%.2f" );
        ImGui::PopItemWidth();

        ImGui::PushFont( boldFont );
        ImGui::Text( "W" );
        ImGui::PopFont();

        ImGui::SameLine();
        ImGui::Value( "##W", value[ 3 ], "%.2f" );
        ImGui::PopItemWidth();

        ImGui::PopStyleVar();

        ImGui::EndColumns();

        ImGui::PopID();
    }

    bool IntSlider( const std::string_view name, int& value, const int min, const int max, const float width )
    {
        ImGui::PushID( name.data() );

        ImGui::BeginColumns( name.data(), 2, ImGuiOldColumnFlags_NoResize );
        ImGui::SetColumnWidth( 0, width );
        ImGui::Text( "%s", name.data() );
        ImGui::NextColumn();

        const bool hasChanged = ImGui::SliderInt( "##V", &value, min, max );

        ImGui::EndColumns();

        ImGui::PopID();
        return hasChanged;
    }
    bool FloatSlider( const std::string_view name, float& value, const float min, const float max, const float width, const bool showName )
    {
        ImGui::PushID( name.data() );
        if ( showName )
        {
            ImGui::BeginColumns( name.data(), 2, ImGuiOldColumnFlags_NoResize );
            ImGui::SetColumnWidth( 0, width );
            ImGui::Text( "%s", name.data() );
            ImGui::NextColumn();
        }

        const bool hasChanged = ImGui::SliderFloat( "##V", &value, min, max, "%.2f" );

        ImGui::EndColumns();

        ImGui::PopID();
        return hasChanged;
    }

    bool Float( const std::string_view name, float& value, const float steps, const float width )
    {
        ImGui::PushID( name.data() );

        ImGui::BeginColumns( name.data(), 2, ImGuiOldColumnFlags_NoResize );
        ImGui::SetColumnWidth( 0, width );
        ImGui::Text( "%s", name.data() );
        ImGui::NextColumn();

        const bool hasChanged = ImGui::DragFloat( "##V", &value, steps, 0.0f, 0.0f, "%.2f" );
        ImGui::EndColumns();

        ImGui::PopID();
        return hasChanged;
    }
    bool Float2( const std::string_view name, float* value, const float steps, const float resetValue, const float width )
    {
        bool hasChanged {};
        const ImGuiIO& io   = ImGui::GetIO();
        const auto boldFont = io.Fonts->Fonts[ 0 ];

        ImGui::PushID( name.data() );

        ImGui::BeginColumns( name.data(), 2, ImGuiOldColumnFlags_NoResize );
        ImGui::SetColumnWidth( 0, width );
        ImGui::Text( "%s", name.data() );
        ImGui::NextColumn();

        ImGui::PushMultiItemsWidths( 2, ImGui::CalcItemWidth() );
        ImGui::PushStyleVar( ImGuiStyleVar_ItemSpacing, ImVec2 { 0, 0 } );

        const float lineHeight  = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
        const ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

        ImGui::PushFont( boldFont );
        if ( ImGui::Button( "X", buttonSize ) )
        {
            value[ 0 ] = resetValue;
            hasChanged = true;
        }
        ImGui::PopFont();

        ImGui::SameLine();
        hasChanged |= ImGui::DragFloat( "##X", &value[ 0 ], steps, 0.0f, 0.0f, "%.2f" );
        ImGui::PopItemWidth();
        ImGui::SameLine();

        ImGui::PushFont( boldFont );
        if ( ImGui::Button( "Y", buttonSize ) )
        {
            value[ 1 ] = resetValue;
            hasChanged = true;
        }
        ImGui::PopFont();

        ImGui::SameLine();
        hasChanged |= ImGui::DragFloat( "##Y", &value[ 1 ], steps, 0.0f, 0.0f, "%.2f" );
        ImGui::PopItemWidth();
        ImGui::SameLine();

        ImGui::PopStyleVar();

        ImGui::EndColumns();

        ImGui::PopID();
        return hasChanged;
    }
    bool Float3( const std::string_view name, float* value, const float steps, const float resetValue, const float width )
    {
        bool hasChanged {};
        const ImGuiIO& io   = ImGui::GetIO();
        const auto boldFont = io.Fonts->Fonts[ 0 ];

        ImGui::PushID( name.data() );

        ImGui::BeginColumns( name.data(), 2, ImGuiOldColumnFlags_NoResize );
        ImGui::SetColumnWidth( 0, width );
        ImGui::Text( "%s", name.data() );
        ImGui::NextColumn();

        ImGui::PushMultiItemsWidths( 3, ImGui::CalcItemWidth() );
        ImGui::PushStyleVar( ImGuiStyleVar_ItemSpacing, ImVec2 { 0, 0 } );

        const float lineHeight  = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
        const ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

        ImGui::PushFont( boldFont );
        if ( ImGui::Button( "X", buttonSize ) )
        {
            value[ 0 ] = resetValue;
            hasChanged = true;
        }
        ImGui::PopFont();

        ImGui::SameLine();
        hasChanged |= ImGui::DragFloat( "##X", &value[ 0 ], steps, 0.0f, 0.0f, "%.2f" );
        ImGui::PopItemWidth();
        ImGui::SameLine();

        ImGui::PushFont( boldFont );
        if ( ImGui::Button( "Y", buttonSize ) )
        {
            value[ 1 ] = resetValue;
            hasChanged = true;
        }
        ImGui::PopFont();

        ImGui::SameLine();
        hasChanged |= ImGui::DragFloat( "##Y", &value[ 1 ], steps, 0.0f, 0.0f, "%.2f" );
        ImGui::PopItemWidth();
        ImGui::SameLine();

        ImGui::PushFont( boldFont );
        if ( ImGui::Button( "Z", buttonSize ) )
        {
            value[ 2 ] = resetValue;
            hasChanged = true;
        }
        ImGui::PopFont();

        ImGui::SameLine();
        hasChanged |= ImGui::DragFloat( "##Z", &value[ 2 ], steps, 0.0f, 0.0f, "%.2f" );
        ImGui::PopItemWidth();

        ImGui::PopStyleVar();

        ImGui::EndColumns();

        ImGui::PopID();
        return hasChanged;
    }
    bool Float4( const std::string_view name, float* value, const float steps, const float resetValue, const float width )
    {
        bool hasChanged {};
        const ImGuiIO& io   = ImGui::GetIO();
        const auto boldFont = io.Fonts->Fonts[ 0 ];

        ImGui::PushID( name.data() );

        ImGui::BeginColumns( name.data(), 2, ImGuiOldColumnFlags_NoResize );
        ImGui::SetColumnWidth( 0, width );
        ImGui::Text( "%s", name.data() );
        ImGui::NextColumn();

        ImGui::PushMultiItemsWidths( 3, ImGui::CalcItemWidth() );
        ImGui::PushStyleVar( ImGuiStyleVar_ItemSpacing, ImVec2 { 0, 0 } );

        const float lineHeight  = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
        const ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

        ImGui::PushFont( boldFont );
        if ( ImGui::Button( "X", buttonSize ) )
        {
            value[ 0 ] = resetValue;
            hasChanged = true;
        }
        ImGui::PopFont();

        ImGui::SameLine();
        hasChanged |= ImGui::DragFloat( "##X", &value[ 0 ], steps, 0.0f, 0.0f, "%.2f" );
        ImGui::PopItemWidth();
        ImGui::SameLine();

        ImGui::PushFont( boldFont );
        if ( ImGui::Button( "Y", buttonSize ) )
        {
            value[ 1 ] = resetValue;
            hasChanged = true;
        }
        ImGui::PopFont();

        ImGui::SameLine();
        hasChanged |= ImGui::DragFloat( "##Y", &value[ 1 ], steps, 0.0f, 0.0f, "%.2f" );
        ImGui::PopItemWidth();
        ImGui::SameLine();

        ImGui::PushFont( boldFont );
        if ( ImGui::Button( "Z", buttonSize ) )
        {
            value[ 2 ] = resetValue;
            hasChanged = true;
        }
        ImGui::PopFont();

        ImGui::SameLine();
        hasChanged |= ImGui::DragFloat( "##Z", &value[ 2 ], steps, 0.0f, 0.0f, "%.2f" );
        ImGui::PopItemWidth();

        ImGui::PushFont( boldFont );
        if ( ImGui::Button( "W", buttonSize ) )
        {
            value[ 3 ] = resetValue;
            hasChanged = true;
        }
        ImGui::PopFont();

        ImGui::SameLine();
        hasChanged |= ImGui::DragFloat( "##W", &value[ 3 ], steps, 0.0f, 0.0f, "%.2f" );
        ImGui::PopItemWidth();

        ImGui::PopStyleVar();

        ImGui::EndColumns();

        ImGui::PopID();
        return hasChanged;
    }

    bool Int( const std::string_view name, int& value, const float steps, const int min, const int max, const float width )
    {
        ImGui::PushID( name.data() );

        ImGui::BeginColumns( name.data(), 2, ImGuiOldColumnFlags_NoResize );
        ImGui::SetColumnWidth( 0, width );
        ImGui::Text( "%s", name.data() );
        ImGui::NextColumn();

        const bool hasChanged = ImGui::DragInt( "##V", &value, steps, min, max );

        ImGui::EndColumns();

        ImGui::PopID();
        return hasChanged;
    }
    bool Int2( const std::string_view name, int* value, const float steps, const int resetValue, const float width )
    {
        bool hasChanged {};
        const ImGuiIO& io   = ImGui::GetIO();
        const auto boldFont = io.Fonts->Fonts[ 0 ];

        ImGui::PushID( name.data() );

        ImGui::BeginColumns( name.data(), 2, ImGuiOldColumnFlags_NoResize );
        ImGui::SetColumnWidth( 0, width );
        ImGui::Text( "%s", name.data() );
        ImGui::NextColumn();

        ImGui::PushMultiItemsWidths( 3, ImGui::CalcItemWidth() );
        ImGui::PushStyleVar( ImGuiStyleVar_ItemSpacing, ImVec2 { 0, 0 } );

        const float lineHeight  = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
        const ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

        ImGui::PushFont( boldFont );
        if ( ImGui::Button( "X", buttonSize ) )
        {
            value[ 0 ] = resetValue;
            hasChanged = true;
        }
        ImGui::PopFont();

        ImGui::SameLine();
        hasChanged |= ImGui::DragInt( "##X", &value[ 0 ], steps );
        ImGui::PopItemWidth();
        ImGui::SameLine();

        ImGui::PushFont( boldFont );
        if ( ImGui::Button( "Y", buttonSize ) )
        {
            value[ 1 ] = resetValue;
            hasChanged = true;
        }
        ImGui::PopFont();

        ImGui::SameLine();
        hasChanged |= ImGui::DragInt( "##Y", &value[ 1 ], steps );
        ImGui::PopItemWidth();
        ImGui::SameLine();

        ImGui::PopStyleVar();

        ImGui::EndColumns();

        ImGui::PopID();
        return hasChanged;
    }
    bool Int3( const std::string_view name, int* value, const float steps, const int resetValue, const float width )
    {
        bool hasChanged {};
        const ImGuiIO& io   = ImGui::GetIO();
        const auto boldFont = io.Fonts->Fonts[ 0 ];

        ImGui::PushID( name.data() );

        ImGui::BeginColumns( name.data(), 2, ImGuiOldColumnFlags_NoResize );
        ImGui::SetColumnWidth( 0, width );
        ImGui::Text( "%s", name.data() );
        ImGui::NextColumn();

        ImGui::PushMultiItemsWidths( 3, ImGui::CalcItemWidth() );
        ImGui::PushStyleVar( ImGuiStyleVar_ItemSpacing, ImVec2 { 0, 0 } );

        const float lineHeight  = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
        const ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

        ImGui::PushFont( boldFont );
        if ( ImGui::Button( "X", buttonSize ) )
        {
            value[ 0 ] = resetValue;
            hasChanged = true;
        }
        ImGui::PopFont();

        ImGui::SameLine();
        hasChanged |= ImGui::DragInt( "##X", &value[ 0 ], steps );
        ImGui::PopItemWidth();
        ImGui::SameLine();

        ImGui::PushFont( boldFont );
        if ( ImGui::Button( "Y", buttonSize ) )
        {
            value[ 1 ] = resetValue;
            hasChanged = true;
        }
        ImGui::PopFont();

        ImGui::SameLine();
        hasChanged |= ImGui::DragInt( "##Y", &value[ 1 ], steps );
        ImGui::PopItemWidth();
        ImGui::SameLine();

        ImGui::PushFont( boldFont );
        if ( ImGui::Button( "Z", buttonSize ) )
        {
            value[ 2 ] = resetValue;
            hasChanged = true;
        }
        ImGui::PopFont();

        ImGui::SameLine();
        hasChanged |= ImGui::DragInt( "##Z", &value[ 2 ], steps );
        ImGui::PopItemWidth();

        ImGui::PopStyleVar();

        ImGui::EndColumns();

        ImGui::PopID();
        return hasChanged;
    }
    bool Int4( const std::string_view name, int* value, const float steps, const int resetValue, const float width )
    {
        bool hasChanged {};
        const ImGuiIO& io   = ImGui::GetIO();
        const auto boldFont = io.Fonts->Fonts[ 0 ];

        ImGui::PushID( name.data() );

        ImGui::BeginColumns( name.data(), 2, ImGuiOldColumnFlags_NoResize );
        ImGui::SetColumnWidth( 0, width );
        ImGui::Text( "%s", name.data() );
        ImGui::NextColumn();

        ImGui::PushMultiItemsWidths( 3, ImGui::CalcItemWidth() );
        ImGui::PushStyleVar( ImGuiStyleVar_ItemSpacing, ImVec2 { 0, 0 } );

        const float lineHeight  = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
        const ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

        ImGui::PushFont( boldFont );
        if ( ImGui::Button( "X", buttonSize ) )
        {
            value[ 0 ] = resetValue;
            hasChanged = true;
        }
        ImGui::PopFont();

        ImGui::SameLine();
        hasChanged |= ImGui::DragInt( "##X", &value[ 0 ], steps );
        ImGui::PopItemWidth();
        ImGui::SameLine();

        ImGui::PushFont( boldFont );
        if ( ImGui::Button( "Y", buttonSize ) )
        {
            value[ 1 ] = resetValue;
            hasChanged = true;
        }
        ImGui::PopFont();

        ImGui::SameLine();
        hasChanged |= ImGui::DragInt( "##Y", &value[ 1 ], steps );
        ImGui::PopItemWidth();
        ImGui::SameLine();

        ImGui::PushFont( boldFont );
        if ( ImGui::Button( "Z", buttonSize ) )
        {
            value[ 2 ] = resetValue;
            hasChanged = true;
        }
        ImGui::PopFont();

        ImGui::SameLine();
        hasChanged |= ImGui::DragInt( "##Z", &value[ 2 ], steps );
        ImGui::PopItemWidth();

        ImGui::PushFont( boldFont );
        if ( ImGui::Button( "W", buttonSize ) )
        {
            value[ 2 ] = resetValue;
            hasChanged = true;
        }
        ImGui::PopFont();

        ImGui::SameLine();
        hasChanged |= ImGui::DragInt( "##W", &value[ 3 ], steps );
        ImGui::PopItemWidth();

        ImGui::PopStyleVar();

        ImGui::EndColumns();

        ImGui::PopID();
        return hasChanged;
    }

    bool Toggle( const std::string_view name, bool& value, const float width )
    {
        ImGui::PushID( name.data() );

        ImGui::BeginColumns( name.data(), 2, ImGuiOldColumnFlags_NoResize );
        ImGui::SetColumnWidth( 0, width );
        ImGui::Text( "%s", name.data() );
        ImGui::NextColumn();

        const bool hasChanged = ImGui::Checkbox( "##V", &value );

        ImGui::EndColumns();

        ImGui::PopID();
        return hasChanged;
    }

    bool InputText( const std::string_view name, char* text, const int maxLength, const float width, const int flags )
    {
        const char* pName { name.data() };
        const float w { ImGui::CalcItemWidth() };
        ImGui::PushID( pName );
        ImGui::BeginColumns( pName, 2, ImGuiOldColumnFlags_NoResize );
        ImGui::SetColumnWidth( 0, width );
        ImGui::Text( "%s", pName );
        ImGui::NextColumn();
        ImGui::SetNextItemWidth( std::fmax( w - width * 1.15f, 50.f ) );
        const bool hasChanged = ImGui::InputText( "##input", text, maxLength, flags );
        ImGui::EndColumns();
        ImGui::PopID();
        return hasChanged;
    }
    bool Color( const std::string_view name, float color[ 3 ], const float width )
    {
        ImGui::PushID( name.data() );
        ImGui::BeginColumns( name.data(), 2, ImGuiOldColumnFlags_NoResize );
        ImGui::SetColumnWidth( 0, width );
        ImGui::Text( "%s", name.data() );
        ImGui::NextColumn();
        const bool hasChanged = ImGui::ColorEdit3( "##Col", color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel );
        ImGui::EndColumns();
        ImGui::PopID();
        return hasChanged;
    }
    void Text( const std::string& text, const float maxLength )
    {
        if ( maxLength == -1 )
        {
            ImGui::Text( "%s", text.c_str() );
            return;
        }

        // If the text width exceeds the maximum width, truncate and display "..."
        if ( const ImVec2 textSize = ImGui::CalcTextSize( text.data() ); textSize.x > maxLength )
        {
            // Find the last character that fits within the maximum width
            int lastVisibleChar = 0;
            float widthSoFar    = 0.0f;
            for ( int i = 0; i < text.length(); i++ )
            {
                if ( const float charWidth = ImGui::CalcTextSize( &text[ i ], &text[ i + 1 ] ).x; widthSoFar + charWidth <= maxLength )
                {
                    lastVisibleChar = i;
                    widthSoFar += charWidth;
                }
                else
                {
                    break;
                }
            }

            // Truncate the text and add "..."
            std::string displayText = text.substr( 0, lastVisibleChar );
            displayText += "...";

            // Display the truncated text
            ImGui::Text( "%s", displayText.c_str() );

            // Show the full text as a tooltip when hovered
            if ( ImGui::IsItemHovered() )
            {
                ImGui::BeginTooltip();
                ImGui::Text( "%s", text.c_str() );
                ImGui::EndTooltip();
            }
        }
        else
        {
            // Display the full text since it fits within the maximum width
            ImGui::Text( "%s", text.c_str() );
        }
    }
    bool Button( const std::string_view name, const BambiMath::Vector2& size )
    {
        return ImGui::Button( name.data(), { size.x, size.y } );
    }

    bool HandleComboBoxItem( const uint64_t index, uint64_t& currentIndex, const std::vector<std::string>& elements, const std::vector<int>& dividers, int& divider )
    {
        const bool isSelected = currentIndex == index;
        bool hasChanged       = false;

        if ( ImGui::Selectable( elements[ index ].c_str(), isSelected ) )
        {
            currentIndex = index;
            hasChanged   = true;
        }

        if ( isSelected )
        {
            ImGui::SetItemDefaultFocus();
        }

        if ( divider < dividers.size() && dividers[ divider ] == static_cast<int>( index ) )
        {
            ++divider;
            FawnForge::Gui::Separator();
        }

        return hasChanged;
    }

    bool ComboBox( const std::string_view name, uint64_t& currentIndex, const std::vector<std::string>& elements, const std::vector<int>& dividers, const float width, const bool inMenuBar )
    {
        if ( currentIndex >= elements.size() )
        {
            return false;
        }

        bool hasChanged {};
        int divider {};
        ImGui::PushID( name.data() );

        if ( inMenuBar )
        {
            ImGui::SetNextItemWidth( width );
            if ( ImGui::BeginCombo( name.data(), elements[ currentIndex ].c_str() ) )
            {
                for ( uint64_t i = 0; i < elements.size(); ++i )
                {
                    hasChanged |= HandleComboBoxItem( i, currentIndex, elements, dividers, divider );
                }
                ImGui::EndCombo();
            }
        }
        else
        {
            const float w = ImGui::CalcItemWidth();
            ImGui::BeginColumns( name.data(), 2, ImGuiOldColumnFlags_NoResize );
            ImGui::SetColumnWidth( 0, width );
            ImGui::Text( "%s", name.data() );
            ImGui::NextColumn();

            ImGui::SetNextItemWidth( std::fmax( w - width * 1.15f, 50.f ) );
            if ( ImGui::BeginCombo( "##name", elements[ currentIndex ].c_str() ) )
            {
                for ( uint64_t i = 0; i < elements.size(); ++i )
                {
                    hasChanged |= HandleComboBoxItem( i, currentIndex, elements, dividers, divider );
                }
                ImGui::EndCombo();
            }
            ImGui::EndColumns();
        }
        ImGui::PopID();
        return hasChanged;
    }
    uint64_t PopupContextWindow( const std::string_view name, const std::vector<std::string>& options )
    {
        if ( !ImGui::BeginPopupContextWindow( name.data() ) )
        {
            return ~0u;
        }

        Text( name.data() );
        Separator();

        std::vector<std::string> parents;
        std::vector<std::string> parentsCopy;

        for ( uint64_t n = 0; n < options.size(); n++ )
        {
            std::size_t idx;
            std::string currentOption { options[ n ] };
            bool breakedEarly {};

            parentsCopy.clear();
            for ( const std::string& str : parents )
            {
                idx = currentOption.find( str );
                if ( idx == std::string::npos )
                {
                    EndMenu();
                }
                else
                {
                    currentOption = currentOption.erase( idx, str.size() + 1 );
                    parentsCopy.push_back( str );
                }
            }
            parents = parentsCopy;
            while ( ( idx = currentOption.find( '/' ) ) != std::string::npos )
            {
                std::string option = currentOption.substr( 0, idx );
                currentOption.erase( 0, option.length() + 1 );
                if ( !BeginMenu( option ) )
                {
                    breakedEarly = true;
                    break;
                }
                parents.push_back( option );
            }
            if ( !breakedEarly && MenuItem( currentOption ) )
            {
                while ( !parents.empty() )
                {
                    parents.pop_back();
                    EndMenu();
                }
                EndPopup();
                return n;
            }
        }

        while ( !parents.empty() )
        {
            parents.pop_back();
            EndMenu();
        }

        EndPopup();
        return ~0u;
    }

    bool StartPopup( const std::string_view name, const bool centered, BambiMath::Vector2 size )
    {
        ImGui::OpenPopup( name.data() );
        if ( centered )
        {
            const ImVec2 center = ImGui::GetMainViewport()->GetCenter();
            ImGui::SetNextWindowPos( center, ImGuiCond_Appearing, ImVec2( 0.5f, 0.5f ) );
            if ( size.x > 0 || size.y > 0 )
            {
                ImGui::SetNextWindowSize( { size.x, size.y }, ImGuiCond_Appearing );
                return ImGui::BeginPopupModal( name.data(), nullptr, ImGuiWindowFlags_NoResize );
            }
            return ImGui::BeginPopupModal( name.data(), nullptr, ImGuiWindowFlags_AlwaysAutoResize );
        }
        return true;
    }
    bool BeginPopup( const std::string_view name )
    {
        return ImGui::BeginPopup( name.data() );
    }
    bool BeginPopupContextItem( uint64_t id )
    {
        return ImGui::BeginPopupContextItem( std::format( "##ContextMenu{}", id ).c_str() );
    }
    void CloseCurrentPopup()
    {
        ImGui::CloseCurrentPopup();
    }
    void EndPopup()
    {
        ImGui::EndPopup();
    }

    bool Begin( const std::string_view title, bool& isVisible, const int flags )
    {
        if ( isVisible )
        {
            return ImGui::Begin( title.data(), &isVisible, flags );
        }
        return false;
    }
    bool BeginChild( const std::string_view title, const BambiMath::Vector2& position, const bool border, const int flags )
    {
        return ImGui::BeginChild( title.data(), { position.x, position.y }, border, flags );
    }
    void End()
    {
        ImGui::End();
    }
    void EndChild()
    {
        ImGui::EndChild();
    }

    bool BeginMenuBar()
    {
        return ImGui::BeginMenuBar();
    }
    bool BeginMenu( const std::string_view text )
    {
        return ImGui::BeginMenu( text.data() );
    }
    void EndMenuBar()
    {
        ImGui::EndMenuBar();
    }
    void EndMenu()
    {
        ImGui::EndMenu();
    }
    bool MenuItem( const std::string_view name, const bool isSelected )
    {
        if ( !isSelected )
        {
            ImGui::BeginDisabled( true );
        }
        const bool hasChanged = ImGui::MenuItem( name.data() );
        if ( !isSelected )
        {
            ImGui::EndDisabled();
        }
        return hasChanged;
    }

    void Spacing()
    {
        ImGui::Spacing();
    }
    void Separator()
    {
        ImGui::Separator();
    }
    void SameLine( const float d )
    {
        ImGui::SameLine( d );
    }

    void SetWindowFocus( const std::string_view name )
    {
        ImGui::SetWindowFocus( name.data() );
    }

    bool TreeNodeEx( const uint64_t id, const int flags, const std::string_view name )
    {
        return ImGui::TreeNodeEx( reinterpret_cast<void*>( id ), flags, "%s", name.data() );
    }
    void TreePop()
    {
        ImGui::TreePop();
    }

    void PushStyleVar( const int styleVar, const BambiMath::Vector2& val )
    {
        ImGui::PushStyleVar( styleVar, { val.x, val.y } );
    }
    void PopStyleVar()
    {
        ImGui::PopStyleVar();
    }

    BambiMath::Vector2 GetContentRegionAvail()
    {
        ImVec2 v = ImGui::GetContentRegionAvail();
        return { v.x, v.y };
    }
    float GetLineHeight()
    {
        return GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
    }

    void* ReceiveDragDrop( const std::string_view name )
    {
        if ( ImGui::BeginDragDropTarget() )
        {
            if ( const ImGuiPayload* payload = ImGui::AcceptDragDropPayload( name.data() ) )
            {
                ImGui::EndDragDropTarget();
                return payload->Data;
            }
            ImGui::EndDragDropTarget();
        }
        return nullptr;
    }

    BambiMath::Vector2 GetMousePos()
    {
        const ImVec2 pos = ImGui::GetMousePos();
        return BambiMath::Vector2 { pos.x, pos.y };
    }
    bool IsWindowHovered()
    {
        return ImGui::IsWindowHovered();
    }
    bool IsMouseDown( const int i )
    {
        return ImGui::IsMouseDown( i );
    }
    bool IsItemClicked()
    {
        return ImGui::IsItemClicked();
    }
    void Columns( const int nrOfColumns )
    {
        ImGui::Columns( nrOfColumns );
    }
    void NextColumn()
    {
        ImGui::NextColumn();
    }
    void MaxNextWindow()
    {
        const ImGuiIO io = ImGui::GetIO();
        ImGui::SetNextWindowSize( io.DisplaySize );
        ImGui::SetNextWindowPos( { 0, 0 } );
    }
    // void ResourceItem( const SFile& file, VkDescriptorSet descriptorSet, float imageSize )
    // {
    //     if ( ImGui::BeginDragDropSource( ImGuiDragDropFlags_None ) )
    //     {
    //         ImGui::SetDragDropPayload( ToString( file.type ), &file, sizeof( SFile ) );
    //         ImGui::Text( "%s", file.fileName.c_str() );
    //         ImGui::EndDragDropSource();
    //     }
    //     Gui::SameLine();
    //     if ( descriptorSet )
    //         ImGui::Image( descriptorSet, { imageSize, imageSize } );
    //     Gui::SameLine();
    //     ImGui::Text( "%s", file.fileName.c_str() );
    // }
    void PushId( const std::string_view id )
    {
        ImGui::PushID( id.data() );
    }
    void PushId( const int id )
    {
        ImGui::PushID( id );
    }
    void PopId()
    {
        ImGui::PopID();
    }
    bool IsMouseDoubleClicked( const int button )
    {
        return ImGui::IsMouseDoubleClicked( button );
    }
    void SetNextItemOpen( const bool open )
    {
        ImGui::SetNextItemOpen( open );
    }
    bool BeginPopupContextWindow( uint64_t uuid )
    {
        return ImGui::BeginPopupContextWindow( std::format( "##ContextMenu{}", uuid ).c_str() );
    }
    unsigned int GetId( const std::string_view name )
    {
        return ImGui::GetID( name.data() );
    }
    unsigned int DockSpace( const unsigned int id, const BambiMath::Vector2& size, const int flags )
    {
        return ImGui::DockSpace( id, { size.x, size.y }, flags );
    }
    void SetNextWindowDockId( const unsigned int id, const int flags )
    {
        ImGui::SetNextWindowDockID( id, flags );
    }
    void Selectable( int id, bool& isSelected, float height )
    {
        ImGui::Selectable( std::format( "##file{}", id ).c_str(), &isSelected, 1 << 2, { 0, height } );
        isSelected |= ImGui::IsItemHovered();
    }
    void SetTooltip( const std::string_view tip )
    {
        if ( ImGui::IsItemHovered() )
        {
            ImGui::SetTooltip( "%s", tip.data() );
        }
    }
    bool TristateToggle( const std::array<const char*, 3>& names, int32_t& currentIndex, const float width, const bool inMenu )
    {
        if ( inMenu )
        {
            bool hasChanged;
            ImGui::PushID( names[ 0 ] );

            if ( currentIndex == -1 )
            {
                bool b {};
                ImGui::PushItemFlag( ImGuiItemFlags_MixedValue, true );
                hasChanged = ImGui::Checkbox( names[ currentIndex + 1 ], &b );
                if ( hasChanged )
                    currentIndex = 1;
                ImGui::PopItemFlag();
            }
            else
            {
                bool b { currentIndex == 1 };
                hasChanged = ImGui::Checkbox( names[ currentIndex + 1 ], &b );
                if ( hasChanged )
                    --currentIndex;
            }

            ImGui::PopID();
            return hasChanged;
        }
        bool hasChanged;
        ImGui::PushID( names[ 0 ] );

        ImGui::BeginColumns( names[ 0 ], 2, ImGuiOldColumnFlags_NoResize );
        ImGui::SetColumnWidth( 0, width );
        ImGui::Text( "%s", names[ currentIndex + 1 ] );
        ImGui::NextColumn();

        if ( currentIndex == -1 )
        {
            bool b {};
            ImGui::PushItemFlag( ImGuiItemFlags_MixedValue, true );
            hasChanged = ImGui::Checkbox( names[ currentIndex + 1 ], &b );
            if ( hasChanged )
                currentIndex = 1;
            ImGui::PopItemFlag();
        }
        else
        {
            bool b { currentIndex == 1 };
            hasChanged = ImGui::Checkbox( names[ currentIndex + 1 ], &b );
            if ( hasChanged )
                --currentIndex;
        }

        ImGui::EndColumns();

        ImGui::PopID();
        return hasChanged;
    }
    bool CollapsingHeader( const std::string_view name )
    {
        return ImGui::CollapsingHeader( name.data() );
    }
    void Disable()
    {
        ImGui::BeginDisabled();
    }
    void Enable()
    {
        ImGui::EndDisabled();
    }
}// namespace FawnForge::Gui