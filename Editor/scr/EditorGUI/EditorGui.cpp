//
// Created by Joran on 12/06/2022.
//
#include "EditorGui.h"

#include <imgui.h>
#include <imgui_internal.h>

namespace BalEditor::EditorGUI
{
    
    void DrawIntValue( const char* name, int value, float width )
    {
        ImGui::PushID( name );
        
        ImGui::Columns( 2 );
        ImGui::SetColumnWidth( 0, width );
        ImGui::Text( "%s", name );
        ImGui::NextColumn();
        
        ImGui::Value( "##V", value );
        
        ImGui::Columns( 1 );
        
        ImGui::PopID();
    }
    void DrawInt2Value( const char* name, const int* value, float width )
    {
        const ImGuiIO& io = ImGui::GetIO();
        const auto boldFont = io.Fonts->Fonts[0];
        
        ImGui::PushID( name );
        
        ImGui::Columns( 2 );
        ImGui::SetColumnWidth( 0, width );
        ImGui::Text( "%s", name );
        ImGui::NextColumn();
        
        ImGui::PushMultiItemsWidths( 2, ImGui::CalcItemWidth());
        ImGui::PushStyleVar( ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 } );
        
        ImGui::PushFont( boldFont );
        ImGui::Text( "X" );
        ImGui::PopFont();
        
        ImGui::SameLine();
        ImGui::Value( "##X", value[0] );
        ImGui::PopItemWidth();
        ImGui::SameLine();
        
        ImGui::PushFont( boldFont );
        ImGui::Text( "Y" );
        ImGui::PopFont();
        
        ImGui::SameLine();
        ImGui::Value( "##Y", value[1] );
        ImGui::PopItemWidth();
        ImGui::SameLine();
        
        ImGui::PopStyleVar();
        
        ImGui::Columns( 1 );
        
        ImGui::PopID();
    }
    void DrawInt3Value( const char* name, const int* value, float width )
    {
        const ImGuiIO& io = ImGui::GetIO();
        const auto boldFont = io.Fonts->Fonts[0];
        
        ImGui::PushID( name );
        
        ImGui::Columns( 2 );
        ImGui::SetColumnWidth( 0, width );
        ImGui::Text( "%s", name );
        ImGui::NextColumn();
        
        ImGui::PushMultiItemsWidths( 3, ImGui::CalcItemWidth());
        ImGui::PushStyleVar( ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 } );
        
        ImGui::PushFont( boldFont );
        ImGui::Text( "X" );
        ImGui::PopFont();
        
        ImGui::SameLine();
        ImGui::Value( "##X", value[0] );
        ImGui::PopItemWidth();
        ImGui::SameLine();
        
        ImGui::PushFont( boldFont );
        ImGui::Text( "Y" );
        ImGui::PopFont();
        
        ImGui::SameLine();
        ImGui::Value( "##Y", value[1] );
        ImGui::PopItemWidth();
        ImGui::SameLine();
        
        ImGui::PushFont( boldFont );
        ImGui::Text( "Z" );
        ImGui::PopFont();
        
        ImGui::SameLine();
        ImGui::Value( "##Z", value[2] );
        ImGui::PopItemWidth();
        
        ImGui::PopStyleVar();
        
        ImGui::Columns( 1 );
        
        ImGui::PopID();
    }
    void DrawInt4Value( const char* name, const int* value, float width )
    {
        const ImGuiIO& io = ImGui::GetIO();
        const auto boldFont = io.Fonts->Fonts[0];
        
        ImGui::PushID( name );
        
        ImGui::Columns( 2 );
        ImGui::SetColumnWidth( 0, width );
        ImGui::Text( "%s", name );
        ImGui::NextColumn();
        
        ImGui::PushMultiItemsWidths( 4, ImGui::CalcItemWidth());
        ImGui::PushStyleVar( ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 } );
        
        ImGui::PushFont( boldFont );
        ImGui::Text( "X" );
        ImGui::PopFont();
        
        ImGui::SameLine();
        ImGui::Value( "##X", value[0] );
        ImGui::PopItemWidth();
        ImGui::SameLine();
        
        ImGui::PushFont( boldFont );
        ImGui::Text( "Y" );
        ImGui::PopFont();
        
        ImGui::SameLine();
        ImGui::Value( "##Y", value[1] );
        ImGui::PopItemWidth();
        ImGui::SameLine();
        
        ImGui::PushFont( boldFont );
        ImGui::Text( "Z" );
        ImGui::PopFont();
        
        ImGui::SameLine();
        ImGui::Value( "##Z", value[2] );
        ImGui::PopItemWidth();
        
        ImGui::PushFont( boldFont );
        ImGui::Text( "W" );
        ImGui::PopFont();
        
        ImGui::SameLine();
        ImGui::Value( "##W", value[3] );
        ImGui::PopItemWidth();
        
        ImGui::PopStyleVar();
        
        ImGui::Columns( 1 );
        
        ImGui::PopID();
    }
    
    void DrawFloatValue( const char* name, float value, float width )
    {
        ImGui::PushID( name );
        
        ImGui::Columns( 2 );
        ImGui::SetColumnWidth( 0, width );
        ImGui::Text( "%s", name );
        ImGui::NextColumn();
        
        ImGui::Value( "##V", value, "%.2f" );
        
        ImGui::Columns( 1 );
        
        ImGui::PopID();
    }
    void DrawFloat2Value( const char* name, const float* value, float width )
    {
        
        const ImGuiIO& io = ImGui::GetIO();
        const auto boldFont = io.Fonts->Fonts[0];
        
        ImGui::PushID( name );
        
        ImGui::Columns( 2 );
        ImGui::SetColumnWidth( 0, width );
        ImGui::Text( "%s", name );
        ImGui::NextColumn();
        
        ImGui::PushMultiItemsWidths( 2, ImGui::CalcItemWidth());
        ImGui::PushStyleVar( ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 } );
        
        ImGui::PushFont( boldFont );
        ImGui::Text( "X" );
        ImGui::PopFont();
        
        ImGui::SameLine();
        ImGui::Value( "##X", value[0], "%.2f" );
        ImGui::PopItemWidth();
        ImGui::SameLine();
        
        ImGui::PushFont( boldFont );
        ImGui::Text( "Y" );
        ImGui::PopFont();
        
        ImGui::SameLine();
        ImGui::Value( "##Y", value[1], "%.2f" );
        ImGui::PopItemWidth();
        ImGui::SameLine();
        
        ImGui::PopStyleVar();
        
        ImGui::Columns( 1 );
        
        ImGui::PopID();
    }
    void DrawFloat3Value( const char* name, const float* value, float width )
    {
        const ImGuiIO& io = ImGui::GetIO();
        const auto boldFont = io.Fonts->Fonts[0];
        
        ImGui::PushID( name );
        
        ImGui::Columns( 2 );
        ImGui::SetColumnWidth( 0, width );
        ImGui::Text( "%s", name );
        ImGui::NextColumn();
        
        ImGui::PushMultiItemsWidths( 3, ImGui::CalcItemWidth());
        ImGui::PushStyleVar( ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 } );
        
        ImGui::PushFont( boldFont );
        ImGui::Text( "X" );
        ImGui::PopFont();
        
        ImGui::SameLine();
        ImGui::Value( "##X", value[0], "%.2f" );
        ImGui::PopItemWidth();
        ImGui::SameLine();
        
        ImGui::PushFont( boldFont );
        ImGui::Text( "Y" );
        ImGui::PopFont();
        
        ImGui::SameLine();
        ImGui::Value( "##Y", value[1], "%.2f" );
        ImGui::PopItemWidth();
        ImGui::SameLine();
        
        ImGui::PushFont( boldFont );
        ImGui::Text( "Z" );
        ImGui::PopFont();
        
        ImGui::SameLine();
        ImGui::Value( "##Z", value[2], "%.2f" );
        ImGui::PopItemWidth();
        
        ImGui::PopStyleVar();
        
        ImGui::Columns( 1 );
        
        ImGui::PopID();
    }
    void DrawFloat4Value( const char* name, const float* value, float width )
    {
        const ImGuiIO& io = ImGui::GetIO();
        const auto boldFont = io.Fonts->Fonts[0];
        
        ImGui::PushID( name );
        
        ImGui::Columns( 2 );
        ImGui::SetColumnWidth( 0, width );
        ImGui::Text( "%s", name );
        ImGui::NextColumn();
        
        ImGui::PushMultiItemsWidths( 4, ImGui::CalcItemWidth());
        ImGui::PushStyleVar( ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 } );
        
        ImGui::PushFont( boldFont );
        ImGui::Text( "X" );
        ImGui::PopFont();
        
        ImGui::SameLine();
        ImGui::Value( "##X", value[0], "%.2f" );
        ImGui::PopItemWidth();
        ImGui::SameLine();
        
        ImGui::PushFont( boldFont );
        ImGui::Text( "Y" );
        ImGui::PopFont();
        
        ImGui::SameLine();
        ImGui::Value( "##Y", value[1], "%.2f" );
        ImGui::PopItemWidth();
        ImGui::SameLine();
        
        ImGui::PushFont( boldFont );
        ImGui::Text( "Z" );
        ImGui::PopFont();
        
        ImGui::SameLine();
        ImGui::Value( "##Z", value[2], "%.2f" );
        ImGui::PopItemWidth();
        
        ImGui::PushFont( boldFont );
        ImGui::Text( "W" );
        ImGui::PopFont();
        
        ImGui::SameLine();
        ImGui::Value( "##W", value[3], "%.2f" );
        ImGui::PopItemWidth();
        
        ImGui::PopStyleVar();
        
        ImGui::Columns( 1 );
        
        ImGui::PopID();
    }
    
    bool DrawIntSlider( const char* name, int& value, int min, int max, float width )
    {
        bool hasChanged;
        ImGui::PushID( name );
        
        ImGui::Columns( 2 );
        ImGui::SetColumnWidth( 0, width );
        ImGui::Text( "%s", name );
        ImGui::NextColumn();
        
        hasChanged = ImGui::SliderInt( "##V", &value, min, max );
        
        ImGui::Columns( 1 );
        
        ImGui::PopID();
        return hasChanged;
    }
    bool DrawFloatSlider( const char* name, float& value, float min, float max, float width )
    {
        bool hasChanged;
        ImGui::PushID( name );
        
        ImGui::Columns( 2 );
        ImGui::SetColumnWidth( 0, width );
        ImGui::Text( "%s", name );
        ImGui::NextColumn();
        
        hasChanged = ImGui::SliderFloat( "##V", &value, min, max, "%.2f" );
        
        ImGui::Columns( 1 );
        
        ImGui::PopID();
        return hasChanged;
    }
    
    bool DrawFloat( const char* name, float& value, float steps, float width )
    {
        bool hasChanged;
        ImGui::PushID( name );
        
        ImGui::Columns( 2 );
        ImGui::SetColumnWidth( 0, width );
        ImGui::Text( "%s", name );
        ImGui::NextColumn();
        
        hasChanged = ImGui::DragFloat( "##V", &value, steps, 0.0f, 0.0f, "%.2f" );
        ImGui::Columns( 1 );
        
        ImGui::PopID();
        return hasChanged;
    }
    bool DrawFloat2( const char* name, float* value, float steps, float resetValue, float width )
    {
        bool hasChanged{};
        const ImGuiIO& io = ImGui::GetIO();
        const auto boldFont = io.Fonts->Fonts[0];
        
        ImGui::PushID( name );
        
        ImGui::Columns( 2 );
        ImGui::SetColumnWidth( 0, width );
        ImGui::Text( "%s", name );
        ImGui::NextColumn();
        
        ImGui::PushMultiItemsWidths( 2, ImGui::CalcItemWidth());
        ImGui::PushStyleVar( ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 } );
        
        const float  lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
        const ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };
        
        ImGui::PushFont( boldFont );
        if ( ImGui::Button( "X", buttonSize ))
        {
            value[0] = resetValue;
            hasChanged = true;
        }
        ImGui::PopFont();
        
        ImGui::SameLine();
        hasChanged |= ImGui::DragFloat( "##X", &value[0], steps, 0.0f, 0.0f, "%.2f" );
        ImGui::PopItemWidth();
        ImGui::SameLine();
        
        ImGui::PushFont( boldFont );
        if ( ImGui::Button( "Y", buttonSize ))
        {
            value[1] = resetValue;
            hasChanged = true;
        }
        ImGui::PopFont();
        
        ImGui::SameLine();
        hasChanged |= ImGui::DragFloat( "##Y", &value[1], steps, 0.0f, 0.0f, "%.2f" );
        ImGui::PopItemWidth();
        ImGui::SameLine();
        
        ImGui::PopStyleVar();
        
        ImGui::Columns( 1 );
        
        ImGui::PopID();
        return hasChanged;
    }
    bool DrawFloat3( const char* name, float* value, float steps, float resetValue, float width )
    {
        bool hasChanged{};
        const ImGuiIO& io = ImGui::GetIO();
        const auto boldFont = io.Fonts->Fonts[0];
        
        ImGui::PushID( name );
        
        ImGui::Columns( 2 );
        ImGui::SetColumnWidth( 0, width );
        ImGui::Text( "%s", name );
        ImGui::NextColumn();
        
        ImGui::PushMultiItemsWidths( 3, ImGui::CalcItemWidth());
        ImGui::PushStyleVar( ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 } );
        
        const float  lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
        const ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };
        
        ImGui::PushFont( boldFont );
        if ( ImGui::Button( "X", buttonSize ))
        {
            value[0] = resetValue;
            hasChanged = true;
        }
        ImGui::PopFont();
        
        ImGui::SameLine();
        hasChanged |= ImGui::DragFloat( "##X", &value[0], steps, 0.0f, 0.0f, "%.2f" );
        ImGui::PopItemWidth();
        ImGui::SameLine();
        
        ImGui::PushFont( boldFont );
        if ( ImGui::Button( "Y", buttonSize ))
        {
            value[1] = resetValue;
            hasChanged = true;
        }
        ImGui::PopFont();
        
        ImGui::SameLine();
        hasChanged |= ImGui::DragFloat( "##Y", &value[1], steps, 0.0f, 0.0f, "%.2f" );
        ImGui::PopItemWidth();
        ImGui::SameLine();
        
        ImGui::PushFont( boldFont );
        if ( ImGui::Button( "Z", buttonSize ))
        {
            value[2] = resetValue;
            hasChanged = true;
        }
        ImGui::PopFont();
        
        ImGui::SameLine();
        hasChanged |= ImGui::DragFloat( "##Z", &value[2], steps, 0.0f, 0.0f, "%.2f" );
        ImGui::PopItemWidth();
        
        ImGui::PopStyleVar();
        
        ImGui::Columns( 1 );
        
        ImGui::PopID();
        return hasChanged;
    }
    bool DrawFloat4( const char* name, float* value, float steps, float resetValue, float width )
    {
        bool hasChanged{};
        const ImGuiIO& io = ImGui::GetIO();
        const auto boldFont = io.Fonts->Fonts[0];
        
        ImGui::PushID( name );
        
        ImGui::Columns( 2 );
        ImGui::SetColumnWidth( 0, width );
        ImGui::Text( "%s", name );
        ImGui::NextColumn();
        
        ImGui::PushMultiItemsWidths( 3, ImGui::CalcItemWidth());
        ImGui::PushStyleVar( ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 } );
        
        const float  lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
        const ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };
        
        ImGui::PushFont( boldFont );
        if ( ImGui::Button( "X", buttonSize ))
        {
            value[0] = resetValue;
            hasChanged = true;
        }
        ImGui::PopFont();
        
        ImGui::SameLine();
        hasChanged |= ImGui::DragFloat( "##X", &value[0], steps, 0.0f, 0.0f, "%.2f" );
        ImGui::PopItemWidth();
        ImGui::SameLine();
        
        ImGui::PushFont( boldFont );
        if ( ImGui::Button( "Y", buttonSize ))
        {
            value[1] = resetValue;
            hasChanged = true;
        }
        ImGui::PopFont();
        
        ImGui::SameLine();
        hasChanged |= ImGui::DragFloat( "##Y", &value[1], steps, 0.0f, 0.0f, "%.2f" );
        ImGui::PopItemWidth();
        ImGui::SameLine();
        
        ImGui::PushFont( boldFont );
        if ( ImGui::Button( "Z", buttonSize ))
        {
            value[2] = resetValue;
            hasChanged = true;
        }
        ImGui::PopFont();
        
        ImGui::SameLine();
        hasChanged |= ImGui::DragFloat( "##Z", &value[2], steps, 0.0f, 0.0f, "%.2f" );
        ImGui::PopItemWidth();
        
        ImGui::PushFont( boldFont );
        if ( ImGui::Button( "W", buttonSize ))
        {
            value[3] = resetValue;
            hasChanged = true;
        }
        ImGui::PopFont();
        
        ImGui::SameLine();
        hasChanged |= ImGui::DragFloat( "##W", &value[3], steps, 0.0f, 0.0f, "%.2f" );
        ImGui::PopItemWidth();
        
        ImGui::PopStyleVar();
        
        ImGui::Columns( 1 );
        
        ImGui::PopID();
        return hasChanged;
    }
    
    bool DrawInt( const char* name, int& value, float steps, int min, int max, float width )
    {
        bool hasChanged;
        ImGui::PushID( name );
        
        ImGui::Columns( 2 );
        ImGui::SetColumnWidth( 0, width );
        ImGui::Text( "%s", name );
        ImGui::NextColumn();
        
        hasChanged = ImGui::DragInt( "##V", &value, steps, min, max );
        
        ImGui::Columns( 1 );
        
        ImGui::PopID();
        return hasChanged;
    }
    bool DrawInt2( const char* name, int* value, float steps, int resetValue, float width )
    {
        bool hasChanged{};
        const ImGuiIO& io = ImGui::GetIO();
        const auto boldFont = io.Fonts->Fonts[0];
        
        ImGui::PushID( name );
        
        ImGui::Columns( 2 );
        ImGui::SetColumnWidth( 0, width );
        ImGui::Text( "%s", name );
        ImGui::NextColumn();
        
        ImGui::PushMultiItemsWidths( 3, ImGui::CalcItemWidth());
        ImGui::PushStyleVar( ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 } );
        
        const float  lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
        const ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };
        
        ImGui::PushFont( boldFont );
        if ( ImGui::Button( "X", buttonSize ))
        {
            value[0] = resetValue;
            hasChanged = true;
        }
        ImGui::PopFont();
        
        ImGui::SameLine();
        hasChanged |= ImGui::DragInt( "##X", &value[0], steps );
        ImGui::PopItemWidth();
        ImGui::SameLine();
        
        ImGui::PushFont( boldFont );
        if ( ImGui::Button( "Y", buttonSize ))
        {
            value[1] = resetValue;
            hasChanged = true;
        }
        ImGui::PopFont();
        
        ImGui::SameLine();
        hasChanged |= ImGui::DragInt( "##Y", &value[1], steps );
        ImGui::PopItemWidth();
        ImGui::SameLine();
        
        ImGui::PopStyleVar();
        
        ImGui::Columns( 1 );
        
        ImGui::PopID();
        return hasChanged;
    }
    bool DrawInt3( const char* name, int* value, float steps, int resetValue, float width )
    {
        bool hasChanged{};
        const ImGuiIO& io = ImGui::GetIO();
        const auto boldFont = io.Fonts->Fonts[0];
        
        ImGui::PushID( name );
        
        ImGui::Columns( 2 );
        ImGui::SetColumnWidth( 0, width );
        ImGui::Text( "%s", name );
        ImGui::NextColumn();
        
        ImGui::PushMultiItemsWidths( 3, ImGui::CalcItemWidth());
        ImGui::PushStyleVar( ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 } );
        
        const float  lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
        const ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };
        
        ImGui::PushFont( boldFont );
        if ( ImGui::Button( "X", buttonSize ))
        {
            value[0] = resetValue;
            hasChanged = true;
        }
        ImGui::PopFont();
        
        ImGui::SameLine();
        hasChanged |= ImGui::DragInt( "##X", &value[0], steps );
        ImGui::PopItemWidth();
        ImGui::SameLine();
        
        ImGui::PushFont( boldFont );
        if ( ImGui::Button( "Y", buttonSize ))
        {
            value[1] = resetValue;
            hasChanged = true;
        }
        ImGui::PopFont();
        
        ImGui::SameLine();
        hasChanged |= ImGui::DragInt( "##Y", &value[1], steps );
        ImGui::PopItemWidth();
        ImGui::SameLine();
        
        ImGui::PushFont( boldFont );
        if ( ImGui::Button( "Z", buttonSize ))
        {
            value[2] = resetValue;
            hasChanged = true;
        }
        ImGui::PopFont();
        
        ImGui::SameLine();
        hasChanged |= ImGui::DragInt( "##Z", &value[2], steps );
        ImGui::PopItemWidth();
        
        ImGui::PopStyleVar();
        
        ImGui::Columns( 1 );
        
        ImGui::PopID();
        return hasChanged;
    }
    bool DrawInt4( const char* name, int* value, float steps, int resetValue, float width )
    {
        bool hasChanged{};
        const ImGuiIO& io = ImGui::GetIO();
        const auto boldFont = io.Fonts->Fonts[0];
        
        ImGui::PushID( name );
        
        ImGui::Columns( 2 );
        ImGui::SetColumnWidth( 0, width );
        ImGui::Text( "%s", name );
        ImGui::NextColumn();
        
        ImGui::PushMultiItemsWidths( 3, ImGui::CalcItemWidth());
        ImGui::PushStyleVar( ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 } );
        
        const float  lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
        const ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };
        
        ImGui::PushFont( boldFont );
        if ( ImGui::Button( "X", buttonSize ))
        {
            value[0] = resetValue;
            hasChanged = true;
        }
        ImGui::PopFont();
        
        ImGui::SameLine();
        hasChanged |= ImGui::DragInt( "##X", &value[0], steps );
        ImGui::PopItemWidth();
        ImGui::SameLine();
        
        ImGui::PushFont( boldFont );
        if ( ImGui::Button( "Y", buttonSize ))
        {
            value[1] = resetValue;
            hasChanged = true;
        }
        ImGui::PopFont();
        
        ImGui::SameLine();
        hasChanged |= ImGui::DragInt( "##Y", &value[1], steps );
        ImGui::PopItemWidth();
        ImGui::SameLine();
        
        ImGui::PushFont( boldFont );
        if ( ImGui::Button( "Z", buttonSize ))
        {
            value[2] = resetValue;
            hasChanged = true;
        }
        ImGui::PopFont();
        
        ImGui::SameLine();
        hasChanged |= ImGui::DragInt( "##Z", &value[2], steps );
        ImGui::PopItemWidth();
        
        ImGui::PushFont( boldFont );
        if ( ImGui::Button( "W", buttonSize ))
        {
            value[2] = resetValue;
            hasChanged = true;
        }
        ImGui::PopFont();
        
        ImGui::SameLine();
        hasChanged |= ImGui::DragInt( "##W", &value[3], steps );
        ImGui::PopItemWidth();
        
        ImGui::PopStyleVar();
        
        ImGui::Columns( 1 );
        
        ImGui::PopID();
        return hasChanged;
    }
    
    bool DrawToggle( const char* name, bool& value, float width )
    {
        bool hasChanged;
        ImGui::PushID( name );
        
        ImGui::Columns( 2 );
        ImGui::SetColumnWidth( 0, width );
        ImGui::Text( "%s", name );
        ImGui::NextColumn();
        
        hasChanged = ImGui::Checkbox( "##V", &value );
        
        ImGui::Columns( 1 );
        
        ImGui::PopID();
        return hasChanged;
    }
    bool DrawComboBox( const char* name, std::string& currentValue, const std::vector<std::string>& elements, float width )
    {
        bool hasChanged{};
        
        ImGui::PushID( name );
        ImGui::Columns( 2 );
        ImGui::SetColumnWidth( 0, width );
        ImGui::Text( "%s", name );
        ImGui::NextColumn();
        
        if ( ImGui::BeginCombo( "##combo", currentValue.c_str()))
        {
            for ( const auto& element : elements )
            {
                const bool isSelected = ( currentValue == element );
                if ( ImGui::Selectable( element.c_str(), isSelected ))
                {
                    currentValue = element;
                    hasChanged   = true;
                }
                if ( isSelected )
                {
                    ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndCombo();
        }
        ImGui::Columns( 1 );
        ImGui::PopID();
        return hasChanged;
    }
    bool DrawInputText( const char* name, char* inputText, int maxLength, float width )
    {
        bool hasChanged;
        ImGui::PushID( name );
        ImGui::Columns( 2 );
        ImGui::SetColumnWidth( 0, width );
        ImGui::Text( "%s", name );
        ImGui::NextColumn();
        hasChanged = ImGui::InputText( name, inputText, maxLength, ImGuiInputTextFlags_EnterReturnsTrue );
        ImGui::Columns( 1 );
        ImGui::PopID();
        return hasChanged;
    }
    bool DrawColor( const char* name, float color[3], float width )
    {
        bool hasChanged;
        ImGui::PushID( name );
        ImGui::Columns( 2 );
        ImGui::SetColumnWidth( 0, width );
        ImGui::Text( "%s", name );
        ImGui::NextColumn();
        hasChanged = ImGui::ColorEdit3( "##Col", color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel );
        ImGui::Columns( 1 );
        ImGui::PopID();
        return hasChanged;
    }
    void DrawText( const char* text )
    {
        ImGui::Text( "%s", text );
    }
    bool DrawButton( const char* name, const glm::vec2& size )
    {
        bool hasChanged;
        
        ImGui::PushID( name );
        hasChanged = ImGui::Button( name, { size.x, size.y } );
        ImGui::PopID();
        
        return hasChanged;
    }
    bool DrawInputText( std::string_view name, char* text, int maxLength, float width, int flags )
    {
        (void) name;
        (void) width;
        bool hasChanged;
        ImGui::PushID( name.data());
        
        ImGui::Columns( 2 );
        ImGui::SetColumnWidth( 0, width );
        ImGui::Text( "%s", name.data());
        ImGui::NextColumn();
        const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.x + ImGui::GetWindowContentRegionWidth();
        ImGui::SetColumnWidth( 1, footer_height_to_reserve );
        
        hasChanged = ImGui::InputText( "##v", text, maxLength, flags );
        ImGui::Columns( 1 );
        
        ImGui::PopID();
        return hasChanged;
    }
    
    int DrawPopupContextWindow( const std::string_view name, const std::vector<std::string>& options )
    {
        if ( ImGui::BeginPopupContextWindow( name.data(), 1, false ))
        {
            DrawText( name.data());
            Separator();
            for ( int n = 0; n < options.size(); n++ )
            {
                if ( MenuItem( options[n] ))
                {
                    EndPopup();
                    return n;
                }
            }
            EndPopup();
        }
        return -1;
    }
    
    bool StartPopup( const char* name, bool centered, glm::vec2 size )
    {
        ImGui::OpenPopup( name );
        if ( centered )
        {
            ImVec2 center = ImGui::GetMainViewport()->GetCenter();
            ImGui::SetNextWindowPos( center, ImGuiCond_Appearing, ImVec2( 0.5f, 0.5f ));
            if ( size.x > 0 || size.y > 0 )
            {
                ImGui::SetNextWindowSize( { size.x, size.y }, ImGuiCond_Appearing );
                return ImGui::BeginPopupModal( name, nullptr, ImGuiWindowFlags_NoResize );
            }
            return ImGui::BeginPopupModal( name, nullptr, ImGuiWindowFlags_AlwaysAutoResize );
        }
        return true;
    }
    bool BeginPopup( const char* name )
    {
        return ImGui::BeginPopup( name );
    }
    bool BeginPopupContextItem()
    {
        return ImGui::BeginPopupContextItem();
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
    void End()
    {
        ImGui::End();
    }
    
    bool BeginMenuBar()
    {
        return ImGui::BeginMenuBar();
    }
    void EndMenuBar()
    {
        ImGui::EndMenuBar();
    }
    bool MenuItem( const std::string_view name )
    {
        return ImGui::MenuItem( name.data());
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
        ImGui::SetWindowFocus( name.data());
    }
    
    bool TreeNodeEx( uint64_t id, int flags, const char* name )
    {
        return ImGui::TreeNodeEx((void*) id, flags, "%s", name );
    }
    void TreePop()
    {
        ImGui::TreePop();
    }
    
    void PushStyleVar( int styleVar, const glm::vec2& val )
    {
        ImGui::PushStyleVar( styleVar, { val.x, val.y } );
    }
    void PopStyleVar()
    {
        ImGui::PopStyleVar();
    }
    
    glm::vec2 GetContentRegionAvail()
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
        if ( ImGui::BeginDragDropTarget())
        {
            if ( const ImGuiPayload* payload = ImGui::AcceptDragDropPayload( name.data()))
            {
                ImGui::EndDragDropTarget();
                return payload->Data;
            }
        }
        return nullptr;
    }
    glm::vec2 GetMousePos()
    {
        ImVec2 pos = ImGui::GetMousePos();
        return glm::vec2{ pos.x, pos.y };
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
}