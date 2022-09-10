//
// Created by Joran on 12/06/2022.
//
#include "EditorGui.h"

#include <imgui.h>
#include <imgui_internal.h>

void BalEditor::EditorGUI::DrawIntValue( const char* name, int value )
{
    ImGui::PushID( name );

    ImGui::Columns( 2 );
    ImGui::SetColumnWidth( 0, 200 );
    ImGui::Text( "%s", name );
    ImGui::NextColumn();

    ImGui::Value( "##V", value );

    ImGui::Columns( 1 );

    ImGui::PopID();
}

void BalEditor::EditorGUI::DrawInt2Value( const char* name, const int* value )
{
    const ImGuiIO& io = ImGui::GetIO();
    const auto boldFont = io.Fonts->Fonts[0];

    ImGui::PushID( name );

    ImGui::Columns( 2 );
    ImGui::SetColumnWidth( 0, 200 );
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

void BalEditor::EditorGUI::DrawInt3Value( const char* name, const int* value )
{
    const ImGuiIO& io = ImGui::GetIO();
    const auto boldFont = io.Fonts->Fonts[0];

    ImGui::PushID( name );

    ImGui::Columns( 2 );
    ImGui::SetColumnWidth( 0, 200 );
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

void BalEditor::EditorGUI::DrawInt4Value( const char* name, const int* value )
{
    const ImGuiIO& io = ImGui::GetIO();
    const auto boldFont = io.Fonts->Fonts[0];

    ImGui::PushID( name );

    ImGui::Columns( 2 );
    ImGui::SetColumnWidth( 0, 200 );
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

void BalEditor::EditorGUI::DrawFloatValue( const char* name, float value )
{
    ImGui::PushID( name );

    ImGui::Columns( 2 );
    ImGui::SetColumnWidth( 0, 200 );
    ImGui::Text( "%s", name );
    ImGui::NextColumn();

    ImGui::Value( "##V", value, "%.2f" );

    ImGui::Columns( 1 );

    ImGui::PopID();
}

void BalEditor::EditorGUI::DrawFloat2Value( const char* name, const float* value )
{

    const ImGuiIO& io = ImGui::GetIO();
    const auto boldFont = io.Fonts->Fonts[0];

    ImGui::PushID( name );

    ImGui::Columns( 2 );
    ImGui::SetColumnWidth( 0, 200 );
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

void BalEditor::EditorGUI::DrawFloat3Value( const char* name, const float* value )
{
    const ImGuiIO& io = ImGui::GetIO();
    const auto boldFont = io.Fonts->Fonts[0];

    ImGui::PushID( name );

    ImGui::Columns( 2 );
    ImGui::SetColumnWidth( 0, 200 );
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

void BalEditor::EditorGUI::DrawFloat4Value( const char* name, const float* value )
{
    const ImGuiIO& io = ImGui::GetIO();
    const auto boldFont = io.Fonts->Fonts[0];

    ImGui::PushID( name );

    ImGui::Columns( 2 );
    ImGui::SetColumnWidth( 0, 200 );
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

bool BalEditor::EditorGUI::DrawIntSlider( const char* name, int& value, int min, int max )
{
    bool hasChanged;
    ImGui::PushID( name );

    ImGui::Columns( 2 );
    ImGui::SetColumnWidth( 0, 200 );
    ImGui::Text( "%s", name );
    ImGui::NextColumn();

    hasChanged = ImGui::SliderInt( "##V", &value, min, max );

    ImGui::Columns( 1 );

    ImGui::PopID();
    return hasChanged;
}

bool BalEditor::EditorGUI::DrawFloatSlider( const char* name, float& value, float min, float max )
{
    bool hasChanged;
    ImGui::PushID( name );

    ImGui::Columns( 2 );
    ImGui::SetColumnWidth( 0, 200 );
    ImGui::Text( "%s", name );
    ImGui::NextColumn();

    hasChanged = ImGui::SliderFloat( "##V", &value, min, max, "%.2f" );

    ImGui::Columns( 1 );

    ImGui::PopID();
    return hasChanged;
}

bool BalEditor::EditorGUI::DrawFloat( const char* name, float& value, float steps )
{
    bool hasChanged;
    ImGui::PushID( name );

    ImGui::Columns( 2 );
    ImGui::SetColumnWidth( 0, 200.0 );
    ImGui::Text( "%s", name );
    ImGui::NextColumn();

    hasChanged = ImGui::DragFloat( "##V", &value, steps, 0.0f, 0.0f, "%.2f" );
    ImGui::Columns( 1 );

    ImGui::PopID();
    return hasChanged;
}

bool BalEditor::EditorGUI::DrawFloat2( const char* name, float* value, float steps, float resetValue )
{
    bool hasChanged{};
    const ImGuiIO& io = ImGui::GetIO();
    const auto boldFont = io.Fonts->Fonts[0];

    ImGui::PushID( name );

    ImGui::Columns( 2 );
    ImGui::SetColumnWidth( 0, 200.0 );
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

bool BalEditor::EditorGUI::DrawFloat3( const char* name, float* value, float steps, float resetValue )
{
    bool hasChanged{};
    const ImGuiIO& io = ImGui::GetIO();
    const auto boldFont = io.Fonts->Fonts[0];

    ImGui::PushID( name );

    ImGui::Columns( 2 );
    ImGui::SetColumnWidth( 0, 200.0 );
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

bool BalEditor::EditorGUI::DrawFloat4( const char* name, float* value, float steps, float resetValue )
{
    bool hasChanged{};
    const ImGuiIO& io = ImGui::GetIO();
    const auto boldFont = io.Fonts->Fonts[0];

    ImGui::PushID( name );

    ImGui::Columns( 2 );
    ImGui::SetColumnWidth( 0, 200.0 );
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

bool BalEditor::EditorGUI::DrawInt( const char* name, int& value, float steps )
{
    bool hasChanged;
    ImGui::PushID( name );

    ImGui::Columns( 2 );
    ImGui::SetColumnWidth( 0, 200.0 );
    ImGui::Text( "%s", name );
    ImGui::NextColumn();

    hasChanged = ImGui::DragInt( "##V", &value, steps );

    ImGui::Columns( 1 );

    ImGui::PopID();
    return hasChanged;
}

bool BalEditor::EditorGUI::DrawInt2( const char* name, int* value, float steps, int resetValue )
{
    bool hasChanged{};
    const ImGuiIO& io = ImGui::GetIO();
    const auto boldFont = io.Fonts->Fonts[0];

    ImGui::PushID( name );

    ImGui::Columns( 2 );
    ImGui::SetColumnWidth( 0, 200.0 );
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

bool BalEditor::EditorGUI::DrawInt3( const char* name, int* value, float steps, int resetValue )
{
    bool hasChanged{};
    const ImGuiIO& io = ImGui::GetIO();
    const auto boldFont = io.Fonts->Fonts[0];

    ImGui::PushID( name );

    ImGui::Columns( 2 );
    ImGui::SetColumnWidth( 0, 200.0 );
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

bool BalEditor::EditorGUI::DrawInt4( const char* name, int* value, float steps, int resetValue )
{
    bool hasChanged{};
    const ImGuiIO& io = ImGui::GetIO();
    const auto boldFont = io.Fonts->Fonts[0];

    ImGui::PushID( name );

    ImGui::Columns( 2 );
    ImGui::SetColumnWidth( 0, 200.0 );
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

bool BalEditor::EditorGUI::DrawToggle( const char* name, bool& value )
{
    bool hasChanged;
    ImGui::PushID( name );

    ImGui::Columns( 2 );
    ImGui::SetColumnWidth( 0, 200.0 );
    ImGui::Text( "%s", name );
    ImGui::NextColumn();

    hasChanged = ImGui::Checkbox( "##V", &value );

    ImGui::Columns( 1 );

    ImGui::PopID();
    return hasChanged;
}

bool BalEditor::EditorGUI::DrawButton( const char* name )
{
    bool hasChanged;
    ImGui::PushID( name );

    ImGui::Columns( 2 );
    ImGui::SetColumnWidth( 0, 200.0 );
    ImGui::Text( "%s", name );
    ImGui::NextColumn();

    hasChanged = ImGui::Button( name );
    ImGui::Columns( 1 );

    ImGui::PopID();
    return hasChanged;
}

bool BalEditor::EditorGUI::DrawComboBox( const char* name, std::string& currentValue, const std::vector<std::string>& elements )
{
    bool hasChanged{};

    if ( ImGui::BeginCombo( name,
                            currentValue.c_str())) // The second parameter is the label previewed before opening the combo.
    {
        for ( const auto& element : elements )
        {
            const bool isSelected = ( currentValue ==
                                      element ); // You can store your selection however you want, outside or inside your objects
            if ( ImGui::Selectable( element.c_str(), isSelected ))
            {
                currentValue = element;
                hasChanged   = true;
            }
            if ( isSelected )
            {
                ImGui::SetItemDefaultFocus();
            }   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
        }
        ImGui::EndCombo();
    }
    return hasChanged;
}

void BalEditor::EditorGUI::Spacing()
{
    ImGui::Spacing();
}

bool DrawInputText( const char* title, char* name, int maxLenth )
{
    return ImGui::InputText( title, name, maxLenth, ImGuiInputTextFlags_EnterReturnsTrue )
}