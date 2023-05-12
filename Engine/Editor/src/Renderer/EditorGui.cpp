//
// Created by Joran on 12/06/2022.
//
#include "EditorGui.h"

#include "Imgui/imgui.h"
#include "Imgui/imgui_internal.h"

namespace FawnForge::Gui
{
    void DrawIntValue( std::string_view name, uint64_t value, float width )
    {
        ImGui::PushID( name.data());
        
        ImGui::BeginColumns( name.data(), 2, ImGuiOldColumnFlags_NoResize );
        ImGui::SetColumnWidth( 0, width );
        ImGui::Text( "%s", name.data());
        ImGui::NextColumn();
        ImGui::Text( "%llu", value );
        ImGui::EndColumns();
        
        ImGui::PopID();
    }
    void DrawIntValue( std::string_view name, int value, float width )
    {
        ImGui::PushID( name.data());
        
        ImGui::BeginColumns( name.data(), 2, ImGuiOldColumnFlags_NoResize );
        ImGui::SetColumnWidth( 0, width );
        ImGui::Text( "%s", name.data());
        ImGui::NextColumn();
        
        ImGui::Value( "##V", value );
        
        ImGui::EndColumns();
        
        ImGui::PopID();
    }
    void DrawInt2Value( std::string_view name, const int* value, float width )
    {
        const ImGuiIO& io = ImGui::GetIO();
        const auto boldFont = io.Fonts->Fonts[0];
        
        ImGui::PushID( name.data());
        
        ImGui::BeginColumns( name.data(), 2, ImGuiOldColumnFlags_NoResize );
        ImGui::SetColumnWidth( 0, width );
        ImGui::Text( "%s", name.data());
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
        
        ImGui::EndColumns();
        
        ImGui::PopID();
    }
    void DrawInt3Value( std::string_view name, const int* value, float width )
    {
        const ImGuiIO& io = ImGui::GetIO();
        const auto boldFont = io.Fonts->Fonts[0];
        
        ImGui::PushID( name.data());
        
        ImGui::BeginColumns( name.data(), 2, ImGuiOldColumnFlags_NoResize );
        ImGui::SetColumnWidth( 0, width );
        ImGui::Text( "%s", name.data());
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
        
        ImGui::EndColumns();
        
        ImGui::PopID();
    }
    void DrawInt4Value( std::string_view name, const int* value, float width )
    {
        const ImGuiIO& io = ImGui::GetIO();
        const auto boldFont = io.Fonts->Fonts[0];
        
        ImGui::PushID( name.data());
        
        ImGui::BeginColumns( name.data(), 2, ImGuiOldColumnFlags_NoResize );
        ImGui::SetColumnWidth( 0, width );
        ImGui::Text( "%s", name.data());
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
        
        ImGui::EndColumns();
        
        ImGui::PopID();
    }
    
    void DrawFloatValue( std::string_view name, float value, float width )
    {
        ImGui::PushID( name.data());
        
        ImGui::BeginColumns( name.data(), 2, ImGuiOldColumnFlags_NoResize );
        ImGui::SetColumnWidth( 0, width );
        ImGui::Text( "%s", name.data());
        ImGui::NextColumn();
        
        ImGui::Value( "##V", value, "%.2f" );
        
        ImGui::EndColumns();
        
        ImGui::PopID();
    }
    void DrawFloat2Value( std::string_view name, const float* value, float width )
    {
        
        const ImGuiIO& io = ImGui::GetIO();
        const auto boldFont = io.Fonts->Fonts[0];
        
        ImGui::PushID( name.data());
        
        ImGui::BeginColumns( name.data(), 2, ImGuiOldColumnFlags_NoResize );
        ImGui::SetColumnWidth( 0, width );
        ImGui::Text( "%s", name.data());
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
        
        ImGui::EndColumns();
        
        ImGui::PopID();
    }
    void DrawFloat3Value( std::string_view name, const float* value, float width )
    {
        const ImGuiIO& io = ImGui::GetIO();
        const auto boldFont = io.Fonts->Fonts[0];
        
        ImGui::PushID( name.data());
        
        ImGui::BeginColumns( name.data(), 2, ImGuiOldColumnFlags_NoResize );
        ImGui::SetColumnWidth( 0, width );
        ImGui::Text( "%s", name.data());
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
        
        ImGui::EndColumns();
        
        ImGui::PopID();
    }
    void DrawFloat4Value( std::string_view name, const float* value, float width )
    {
        const ImGuiIO& io = ImGui::GetIO();
        const auto boldFont = io.Fonts->Fonts[0];
        
        ImGui::PushID( name.data());
        
        ImGui::BeginColumns( name.data(), 2, ImGuiOldColumnFlags_NoResize );
        ImGui::SetColumnWidth( 0, width );
        ImGui::Text( "%s", name.data());
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
        
        ImGui::EndColumns();
        
        ImGui::PopID();
    }
    
    bool DrawIntSlider( std::string_view name, int& value, int min, int max, float width )
    {
        bool hasChanged;
        ImGui::PushID( name.data());
        
        ImGui::BeginColumns( name.data(), 2, ImGuiOldColumnFlags_NoResize );
        ImGui::SetColumnWidth( 0, width );
        ImGui::Text( "%s", name.data());
        ImGui::NextColumn();
        
        hasChanged = ImGui::SliderInt( "##V", &value, min, max );
        
        ImGui::EndColumns();
        
        ImGui::PopID();
        return hasChanged;
    }
    bool DrawFloatSlider( std::string_view name, float& value, float min, float max, float width, bool showName )
    {
        bool hasChanged;
        ImGui::PushID( name.data());
        if ( showName )
        {
            ImGui::BeginColumns( name.data(), 2, ImGuiOldColumnFlags_NoResize );
            ImGui::SetColumnWidth( 0, width );
            ImGui::Text( "%s", name.data());
            ImGui::NextColumn();
        }
        
        hasChanged = ImGui::SliderFloat( "##V", &value, min, max, "%.2f" );
        
        ImGui::EndColumns();
        
        ImGui::PopID();
        return hasChanged;
    }
    
    bool DrawFloat( std::string_view name, float& value, float steps, float width )
    {
        bool hasChanged;
        ImGui::PushID( name.data());
        
        ImGui::BeginColumns( name.data(), 2, ImGuiOldColumnFlags_NoResize );
        ImGui::SetColumnWidth( 0, width );
        ImGui::Text( "%s", name.data());
        ImGui::NextColumn();
        
        hasChanged = ImGui::DragFloat( "##V", &value, steps, 0.0f, 0.0f, "%.2f" );
        ImGui::EndColumns();
        
        ImGui::PopID();
        return hasChanged;
    }
    bool DrawFloat2( std::string_view name, float* value, float steps, float resetValue, float width )
    {
        bool hasChanged{};
        const ImGuiIO& io = ImGui::GetIO();
        const auto boldFont = io.Fonts->Fonts[0];
        
        ImGui::PushID( name.data());
        
        ImGui::BeginColumns( name.data(), 2, ImGuiOldColumnFlags_NoResize );
        ImGui::SetColumnWidth( 0, width );
        ImGui::Text( "%s", name.data());
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
        
        ImGui::EndColumns();
        
        ImGui::PopID();
        return hasChanged;
    }
    bool DrawFloat3( std::string_view name, float* value, float steps, float resetValue, float width )
    {
        bool hasChanged{};
        const ImGuiIO& io = ImGui::GetIO();
        const auto boldFont = io.Fonts->Fonts[0];
        
        ImGui::PushID( name.data());
        
        ImGui::BeginColumns( name.data(), 2, ImGuiOldColumnFlags_NoResize );
        ImGui::SetColumnWidth( 0, width );
        ImGui::Text( "%s", name.data());
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
        
        ImGui::EndColumns();
        
        ImGui::PopID();
        return hasChanged;
    }
    bool DrawFloat4( std::string_view name, float* value, float steps, float resetValue, float width )
    {
        bool hasChanged{};
        const ImGuiIO& io = ImGui::GetIO();
        const auto boldFont = io.Fonts->Fonts[0];
        
        ImGui::PushID( name.data());
        
        ImGui::BeginColumns( name.data(), 2, ImGuiOldColumnFlags_NoResize );
        ImGui::SetColumnWidth( 0, width );
        ImGui::Text( "%s", name.data());
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
        
        ImGui::EndColumns();
        
        ImGui::PopID();
        return hasChanged;
    }
    
    bool DrawInt( std::string_view name, int& value, float steps, int min, int max, float width )
    {
        bool hasChanged;
        ImGui::PushID( name.data());
        
        ImGui::BeginColumns( name.data(), 2, ImGuiOldColumnFlags_NoResize );
        ImGui::SetColumnWidth( 0, width );
        ImGui::Text( "%s", name.data());
        ImGui::NextColumn();
        
        hasChanged = ImGui::DragInt( "##V", &value, steps, min, max );
        
        ImGui::EndColumns();
        
        ImGui::PopID();
        return hasChanged;
    }
    bool DrawInt2( std::string_view name, int* value, float steps, int resetValue, float width )
    {
        bool hasChanged{};
        const ImGuiIO& io = ImGui::GetIO();
        const auto boldFont = io.Fonts->Fonts[0];
        
        ImGui::PushID( name.data());
        
        ImGui::BeginColumns( name.data(), 2, ImGuiOldColumnFlags_NoResize );
        ImGui::SetColumnWidth( 0, width );
        ImGui::Text( "%s", name.data());
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
        
        ImGui::EndColumns();
        
        ImGui::PopID();
        return hasChanged;
    }
    bool DrawInt3( std::string_view name, int* value, float steps, int resetValue, float width )
    {
        bool hasChanged{};
        const ImGuiIO& io = ImGui::GetIO();
        const auto boldFont = io.Fonts->Fonts[0];
        
        ImGui::PushID( name.data());
        
        ImGui::BeginColumns( name.data(), 2, ImGuiOldColumnFlags_NoResize );
        ImGui::SetColumnWidth( 0, width );
        ImGui::Text( "%s", name.data());
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
        
        ImGui::EndColumns();
        
        ImGui::PopID();
        return hasChanged;
    }
    bool DrawInt4( std::string_view name, int* value, float steps, int resetValue, float width )
    {
        bool hasChanged{};
        const ImGuiIO& io = ImGui::GetIO();
        const auto boldFont = io.Fonts->Fonts[0];
        
        ImGui::PushID( name.data());
        
        ImGui::BeginColumns( name.data(), 2, ImGuiOldColumnFlags_NoResize );
        ImGui::SetColumnWidth( 0, width );
        ImGui::Text( "%s", name.data());
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
        
        ImGui::EndColumns();
        
        ImGui::PopID();
        return hasChanged;
    }
    
    bool DrawToggle( std::string_view name, bool& value, float width )
    {
        bool hasChanged;
        ImGui::PushID( name.data());
        
        ImGui::BeginColumns( name.data(), 2, ImGuiOldColumnFlags_NoResize );
        ImGui::SetColumnWidth( 0, width );
        ImGui::Text( "%s", name.data());
        ImGui::NextColumn();
        
        hasChanged = ImGui::Checkbox( "##V", &value );
        
        ImGui::EndColumns();
        
        ImGui::PopID();
        return hasChanged;
    }
    
    bool DrawInputText( std::string_view name, char* text, int maxLength, float width, int flags )
    {
        bool hasChanged;
        const char* pName{ name.data() };
        const float w{ ImGui::CalcItemWidth() };
        ImGui::PushID( pName );
        ImGui::BeginColumns( pName, 2, ImGuiOldColumnFlags_NoResize );
        ImGui::SetColumnWidth( 0, width );
        ImGui::Text( "%s", pName );
        ImGui::NextColumn();
        ImGui::SetNextItemWidth( std::fmax( w - width * 1.15f, 50.f ));
        hasChanged = ImGui::InputText( "##input", text, maxLength, flags );
        ImGui::EndColumns();
        ImGui::PopID();
        return hasChanged;
    }
    bool DrawColor( std::string_view name, float color[3], float width )
    {
        bool hasChanged;
        ImGui::PushID( name.data());
        ImGui::BeginColumns( name.data(), 2, ImGuiOldColumnFlags_NoResize );
        ImGui::SetColumnWidth( 0, width );
        ImGui::Text( "%s", name.data());
        ImGui::NextColumn();
        hasChanged = ImGui::ColorEdit3( "##Col", color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel );
        ImGui::EndColumns();
        ImGui::PopID();
        return hasChanged;
    }
    void DrawText( std::string_view text )
    {
        ImGui::Text( "%s", text.data() );
    }
    bool DrawButton( std::string_view name, const glm::vec2& size )
    {
        return ImGui::Button( name.data(), { size.x, size.y } );
    }
    
    bool Gui::DrawComboBox( std::string_view name, uint64_t& currentIndex, const std::vector<std::string>& elements, const std::vector<int>& dividers, float width, bool inMenuBar )
    {
        if ( currentIndex >= elements.size())
        {
            return false;
        }
        
        bool hasChanged{};
        int  divider{};
        ImGui::PushID( name.data());
        if ( inMenuBar )
        {
            ImGui::SetNextItemWidth( width );
            if ( ImGui::BeginCombo( name.data(), elements[currentIndex].c_str()))
            {
                for ( uint64_t i{}; i < elements.size(); ++i )
                {
                    const bool isSelected = currentIndex == i;
                    if ( ImGui::Selectable( elements[i].c_str(), isSelected ))
                    {
                        currentIndex = i;
                        hasChanged   = true;
                    }
                    if ( isSelected )
                    {
                        ImGui::SetItemDefaultFocus();
                    }
                    if ( divider < dividers.size() && dividers[divider] == i )
                    {
                        ++divider;
                        FawnForge::Gui::Separator();
                    }
                }
                ImGui::EndCombo();
            }
        }
        else
        {
            const float w{ ImGui::CalcItemWidth() };
            ImGui::BeginColumns( name.data(), 2, ImGuiOldColumnFlags_NoResize );
            ImGui::SetColumnWidth( 0, width );
            ImGui::Text( "%s", name.data());
            ImGui::NextColumn();
            
            ImGui::SetNextItemWidth( std::fmax( w - width * 1.15f, 50.f ));
            if ( ImGui::BeginCombo( "##name", elements[currentIndex].c_str()))
            {
                for ( uint64_t i{}; i < elements.size(); ++i )
                {
                    const bool isSelected = currentIndex == i;
                    if ( ImGui::Selectable( elements[i].c_str(), isSelected ))
                    {
                        currentIndex = i;
                        hasChanged   = true;
                    }
                    if ( isSelected )
                    {
                        ImGui::SetItemDefaultFocus();
                    }
                }
                ImGui::EndCombo();
            }
            ImGui::EndColumns();
        }
        ImGui::PopID();
        return hasChanged;
    }
    uint64_t DrawPopupContextWindow( const std::string_view name, const std::vector<std::string>& options )
    {
        if ( ImGui::BeginPopupContextWindow( name.data()))
        {
            DrawText( name.data());
            Separator();
            
            std::vector<std::string> parents;
            std::vector<std::string> parentsCopy;
            
            for ( uint64_t n = 0; n < options.size(); n++ )
            {
                size_t      idx;
                std::string currentOption{ options[n] };
                bool        breakedEarly{};
                
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
                parents      = parentsCopy;
                while (( idx = currentOption.find( '/' )) != std::string::npos )
                {
                    std::string option = currentOption.substr( 0, idx );
                    currentOption.erase( 0, option.length() + 1 );
                    if ( !BeginMenu( option.c_str()))
                    {
                        breakedEarly = true;
                        break;
                    }
                    parents.push_back( option );
                }
                if ( !breakedEarly && MenuItem( currentOption ))
                {
                    while ( !parents.empty())
                    {
                        parents.pop_back();
                        EndMenu();
                    }
                    EndPopup();
                    return n;
                }
            }
            while ( !parents.empty())
            {
                parents.pop_back();
                EndMenu();
            }
            EndPopup();
        }
//        if ( ImGui::BeginPopupContextWindow(name.data()) )
//        {
//            DrawText(name.data());
//            Separator();
//
//            std::vector<std::string> parents;
//
//            for ( uint64_t n = 0; n < options.size(); n++ )
//            {
//                std::string_view currentOption{ options[n] };
//
//                for ( const std::string& str : parents )
//                {
//                    auto idx = currentOption.find(str);
//                    if ( idx == std::string_view::npos )
//                    {
//                        EndMenu();
//                    }
//                    else
//                    {
//                        currentOption = currentOption.substr(idx + str.size() + 1);
//                    }
//                }
//
//                while ( true )
//                {
//                    auto idx = currentOption.find_first_of('/');
//                    if ( idx == std::string_view::npos )
//                    {
//                        break;
//                    }
//                    std::string_view option = currentOption.substr(0, idx);
//                    currentOption = currentOption.substr(idx + 1);
//                    if ( !BeginMenu(option.data()) )
//                    {
//                        break;
//                    }
//                    parents.emplace_back(option);
//                }
//
//                if ( MenuItem(std::string{currentOption}) )
//                {
//                    while ( !parents.empty() )
//                    {
//                        parents.pop_back();
//                        EndMenu();
//                    }
//                    EndPopup();
//                    return n;
//                }
//            }
//
//            while ( !parents.empty() )
//            {
//                parents.pop_back();
//                EndMenu();
//            }
//            EndPopup();
//        }
        return std::numeric_limits<unsigned long long>::max();
    }
    
    bool StartPopup( std::string_view name, bool centered, glm::vec2 size )
    {
        ImGui::OpenPopup( name.data() );
        if ( centered )
        {
            ImVec2 center = ImGui::GetMainViewport()->GetCenter();
            ImGui::SetNextWindowPos( center, ImGuiCond_Appearing, ImVec2( 0.5f, 0.5f ));
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
        return ImGui::BeginPopupContextItem(( "##ContextMenu" + std::to_string( id )).c_str());
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
    bool BeginChild( std::string_view title, const glm::vec2& position, bool border, int flags )
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
        return ImGui::BeginMenu( text.data());
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
        hasChanged = ImGui::MenuItem( name.data());
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
        ImGui::SetWindowFocus( name.data());
    }
    
    bool TreeNodeEx( uint64_t id, int flags, std::string_view name )
    {
        return ImGui::TreeNodeEx((void*) id, flags, "%s", name.data() );
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
            ImGui::EndDragDropTarget();
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
    void DrawResourceItem( const SFile& file, VkDescriptorSet_T* descriptorSet, float imageSize )
    {
        if ( ImGui::BeginDragDropSource( ImGuiDragDropFlags_None ))
        {
            ImGui::SetDragDropPayload( ToString( file.type ), &file, sizeof( SFile ));
            ImGui::Text( "%s", file.fileName.c_str());
            ImGui::EndDragDropSource();
        }
        Gui::SameLine();
        ImGui::Image( descriptorSet, { imageSize, imageSize } );
        Gui::SameLine();
        ImGui::Text( "%s", file.fileName.c_str());
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
        return ImGui::BeginPopupContextWindow(( "##ContextMenu" + std::to_string( uuid )).c_str());
    }
    unsigned int GetId( std::string_view name )
    {
        return ImGui::GetID( name.data() );
    }
    unsigned int DockSpace( unsigned int id, const glm::vec2& size, int flags )
    {
        return ImGui::DockSpace( id, { size.x, size.y }, flags );
    }
    void SetNextWindowDockId( unsigned int id, int flags )
    {
        ImGui::SetNextWindowDockID( id, flags );
    }
    void DrawSelectable( int id, bool& isSelected, float height )
    {
        ImGui::Selectable(( "##file" + std::to_string( id )).c_str(), &isSelected, 1 << 2, { 0, height } );
        isSelected |= ImGui::IsItemHovered();
    }
    void SetTooltip( std::string_view tip )
    {
        if ( ImGui::IsItemHovered())
        {
            ImGui::SetTooltip( "%s", tip.data());
        }
    }
    bool DrawTristateToggle( const std::array<const char*, 3>& names, int32_t& currentIndex, float width, bool inMenuBar )
    {
        if ( inMenuBar )
        {
            bool hasChanged;
            ImGui::PushID( names[0] );
            
            hasChanged = ImGui::CheckBoxTristate( names[currentIndex + 1], &currentIndex );
            
            ImGui::PopID();
            return hasChanged;
        }
        bool hasChanged;
        ImGui::PushID( names[0] );
        
        ImGui::BeginColumns( names[0], 2, ImGuiOldColumnFlags_NoResize );
        ImGui::SetColumnWidth( 0, width );
        ImGui::Text( "%s", names[currentIndex + 1] );
        ImGui::NextColumn();
        
        hasChanged = ImGui::CheckBoxTristate( "##V", &currentIndex );
        
        ImGui::EndColumns();
        
        ImGui::PopID();
        return hasChanged;
    }
    bool CollapsingHeader(std::string_view name)
    {
        return ImGui::CollapsingHeader(name.data());
    }
    void Disable()
    {
        ImGui::BeginDisabled();
    }
    void Enable()
    {
        ImGui::EndDisabled();
    }
}