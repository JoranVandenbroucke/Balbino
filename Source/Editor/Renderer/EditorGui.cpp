//
// Created by Joran on 12/06/2022.
//
#include "EditorGui.h"

#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui.h>
#include <imgui_internal.h>

namespace FawnForge::Gui
{
    void IntValue( std::string_view name, uint64_t value, float width )
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
    void IntValue( std::string_view name, int value, float width )
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
    void Int2Value( std::string_view name, const int* value, float width )
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
    void Int3Value( std::string_view name, const int* value, float width )
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
    void Int4Value( std::string_view name, const int* value, float width )
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

    void FloatValue( std::string_view name, float value, float width )
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
    void Float2Value( std::string_view name, const float* value, float width )
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
    void Float3Value( std::string_view name, const float* value, float width )
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
    void Float4Value( std::string_view name, const float* value, float width )
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

    bool IntSlider( std::string_view name, int& value, int min, int max, float width )
    {
        bool hasChanged;
        ImGui::PushID( name.data() );

        ImGui::BeginColumns( name.data(), 2, ImGuiOldColumnFlags_NoResize );
        ImGui::SetColumnWidth( 0, width );
        ImGui::Text( "%s", name.data() );
        ImGui::NextColumn();

        hasChanged = ImGui::SliderInt( "##V", &value, min, max );

        ImGui::EndColumns();

        ImGui::PopID();
        return hasChanged;
    }
    bool FloatSlider( std::string_view name, float& value, float min, float max, float width, bool showName )
    {
        bool hasChanged;
        ImGui::PushID( name.data() );
        if ( showName )
        {
            ImGui::BeginColumns( name.data(), 2, ImGuiOldColumnFlags_NoResize );
            ImGui::SetColumnWidth( 0, width );
            ImGui::Text( "%s", name.data() );
            ImGui::NextColumn();
        }

        hasChanged = ImGui::SliderFloat( "##V", &value, min, max, "%.2f" );

        ImGui::EndColumns();

        ImGui::PopID();
        return hasChanged;
    }

    bool Float( std::string_view name, float& value, float steps, float width )
    {
        bool hasChanged;
        ImGui::PushID( name.data() );

        ImGui::BeginColumns( name.data(), 2, ImGuiOldColumnFlags_NoResize );
        ImGui::SetColumnWidth( 0, width );
        ImGui::Text( "%s", name.data() );
        ImGui::NextColumn();

        hasChanged = ImGui::DragFloat( "##V", &value, steps, 0.0f, 0.0f, "%.2f" );
        ImGui::EndColumns();

        ImGui::PopID();
        return hasChanged;
    }
    bool Float2( std::string_view name, float* value, float steps, float resetValue, float width )
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
    bool Float3( std::string_view name, float* value, float steps, float resetValue, float width )
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
    bool Float4( std::string_view name, float* value, float steps, float resetValue, float width )
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

    bool Int( std::string_view name, int& value, float steps, int min, int max, float width )
    {
        bool hasChanged;
        ImGui::PushID( name.data() );

        ImGui::BeginColumns( name.data(), 2, ImGuiOldColumnFlags_NoResize );
        ImGui::SetColumnWidth( 0, width );
        ImGui::Text( "%s", name.data() );
        ImGui::NextColumn();

        hasChanged = ImGui::DragInt( "##V", &value, steps, min, max );

        ImGui::EndColumns();

        ImGui::PopID();
        return hasChanged;
    }
    bool Int2( std::string_view name, int* value, float steps, int resetValue, float width )
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
    bool Int3( std::string_view name, int* value, float steps, int resetValue, float width )
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
    bool Int4( std::string_view name, int* value, float steps, int resetValue, float width )
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

    bool Toggle( std::string_view name, bool& value, float width )
    {
        bool hasChanged;
        ImGui::PushID( name.data() );

        ImGui::BeginColumns( name.data(), 2, ImGuiOldColumnFlags_NoResize );
        ImGui::SetColumnWidth( 0, width );
        ImGui::Text( "%s", name.data() );
        ImGui::NextColumn();

        hasChanged = ImGui::Checkbox( "##V", &value );

        ImGui::EndColumns();

        ImGui::PopID();
        return hasChanged;
    }

    bool InputText( std::string_view name, char* text, int maxLength, float width, int flags )
    {
        bool hasChanged;
        const char* pName { name.data() };
        const float w { ImGui::CalcItemWidth() };
        ImGui::PushID( pName );
        ImGui::BeginColumns( pName, 2, ImGuiOldColumnFlags_NoResize );
        ImGui::SetColumnWidth( 0, width );
        ImGui::Text( "%s", pName );
        ImGui::NextColumn();
        ImGui::SetNextItemWidth( std::fmax( w - width * 1.15f, 50.f ) );
        hasChanged = ImGui::InputText( "##input", text, maxLength, flags );
        ImGui::EndColumns();
        ImGui::PopID();
        return hasChanged;
    }
    bool Color( std::string_view name, float color[ 3 ], float width )
    {
        bool hasChanged;
        ImGui::PushID( name.data() );
        ImGui::BeginColumns( name.data(), 2, ImGuiOldColumnFlags_NoResize );
        ImGui::SetColumnWidth( 0, width );
        ImGui::Text( "%s", name.data() );
        ImGui::NextColumn();
        hasChanged = ImGui::ColorEdit3( "##Col", color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel );
        ImGui::EndColumns();
        ImGui::PopID();
        return hasChanged;
    }
    void Text( const std::string& text, float maxLength )
    {
        if ( maxLength == -1 )
        {
            ImGui::Text( "%s", text.c_str() );
            return;
        }

        ImVec2 textSize = ImGui::CalcTextSize( text.data() );

        // If the text width exceeds the maximum width, truncate and display "..."
        if ( textSize.x > maxLength )
        {
            // Find the last character that fits within the maximum width
            int lastVisibleChar = 0;
            float widthSoFar    = 0.0f;
            for ( int i = 0; i < text.length(); i++ )
            {
                float charWidth = ImGui::CalcTextSize( &text[ i ], &text[ i + 1 ] ).x;
                if ( widthSoFar + charWidth <= maxLength )
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
    bool Button( std::string_view name, const BambiMath::Vector2& size )
    {
        return ImGui::Button( name.data(), { size.x, size.y } );
    }

    bool HandleComboBoxItem( uint64_t index, uint64_t& currentIndex, const std::vector<std::string>& elements, const std::vector<int>& dividers, int& divider )
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

    bool ComboBox( std::string_view name, uint64_t& currentIndex, const std::vector<std::string>& elements, const std::vector<int>& dividers, float width, bool inMenuBar )
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

    bool StartPopup( std::string_view name, bool centered, BambiMath::Vector2 size )
    {
        ImGui::OpenPopup( name.data() );
        if ( centered )
        {
            ImVec2 center = ImGui::GetMainViewport()->GetCenter();
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
    bool BeginPopup( std::string_view name )
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

    bool Begin( std::string_view title, bool& isVisible, int flags )
    {
        if ( isVisible )
        {
            return ImGui::Begin( title.data(), &isVisible, flags );
        }
        return false;
    }
    bool BeginChild( std::string_view title, const BambiMath::Vector2& position, bool border, int flags )
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
    bool BeginMenu( std::string_view text )
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
    bool MenuItem( const std::string_view name, bool isSelected )
    {
        bool hasChanged;
        if ( !isSelected )
        {
            ImGui::BeginDisabled( true );
        }
        hasChanged = ImGui::MenuItem( name.data() );
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
    void SameLine( float d )
    {
        ImGui::SameLine( d );
    }

    void SetWindowFocus( std::string_view name )
    {
        ImGui::SetWindowFocus( name.data() );
    }

    bool TreeNodeEx( uint64_t id, int flags, std::string_view name )
    {
        return ImGui::TreeNodeEx( (void*)id, flags, "%s", name.data() );
    }
    void TreePop()
    {
        ImGui::TreePop();
    }

    void PushStyleVar( int styleVar, const BambiMath::Vector2& val )
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

    void* ReceiveDragDrop( std::string_view name )
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
        ImVec2 pos = ImGui::GetMousePos();
        return BambiMath::Vector2 { pos.x, pos.y };
    }
    bool IsWindowHovered()
    {
        return ImGui::IsWindowHovered();
    }
    bool IsMouseDown( int i )
    {
        return ImGui::IsMouseDown( i );
    }
    bool IsItemClicked()
    {
        return ImGui::IsItemClicked();
    }
    void Columns( int nrOfColumns )
    {
        ImGui::Columns( nrOfColumns );
    }
    void NextColumn()
    {
        ImGui::NextColumn();
    }
    void MaxNextWindow()
    {
        ImGuiIO io = ImGui::GetIO();
        ImGui::SetNextWindowSize( io.DisplaySize );
        ImGui::SetNextWindowPos( { 0, 0 } );
    }
    void ResourceItem( const SFile& file, VkDescriptorSet descriptorSet, float imageSize )
    {
        if ( ImGui::BeginDragDropSource( ImGuiDragDropFlags_None ) )
        {
            ImGui::SetDragDropPayload( ToString( file.type ), &file, sizeof( SFile ) );
            ImGui::Text( "%s", file.fileName.c_str() );
            ImGui::EndDragDropSource();
        }
        Gui::SameLine();
        if ( descriptorSet )
            ImGui::Image( descriptorSet, { imageSize, imageSize } );
        Gui::SameLine();
        ImGui::Text( "%s", file.fileName.c_str() );
    }
    void PushId( std::string_view id )
    {
        ImGui::PushID( id.data() );
    }
    void PushId( int id )
    {
        ImGui::PushID( id );
    }
    void PopId()
    {
        ImGui::PopID();
    }
    bool IsMouseDoubleClicked( int button )
    {
        return ImGui::IsMouseDoubleClicked( button );
    }
    void SetNextItemOpen( bool open )
    {
        ImGui::SetNextItemOpen( open );
    }
    bool BeginPopupContextWindow( uint64_t uuid )
    {
        return ImGui::BeginPopupContextWindow( std::format( "##ContextMenu{}", uuid ).c_str() );
    }
    unsigned int GetId( std::string_view name )
    {
        return ImGui::GetID( name.data() );
    }
    unsigned int DockSpace( unsigned int id, const BambiMath::Vector2& size, int flags )
    {
        return ImGui::DockSpace( id, { size.x, size.y }, flags );
    }
    void SetNextWindowDockId( unsigned int id, int flags )
    {
        ImGui::SetNextWindowDockID( id, flags );
    }
    void Selectable( int id, bool& isSelected, float height )
    {
        ImGui::Selectable( std::format( "##file{}", id ).c_str(), &isSelected, 1 << 2, { 0, height } );
        isSelected |= ImGui::IsItemHovered();
    }
    void SetTooltip( std::string_view tip )
    {
        if ( ImGui::IsItemHovered() )
        {
            ImGui::SetTooltip( "%s", tip.data() );
        }
    }
    bool TristateToggle( const std::array<const char*, 3>& names, int32_t& currentIndex, float width, bool inMenuBar )
    {
        if ( inMenuBar )
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
    bool CollapsingHeader( std::string_view name )
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