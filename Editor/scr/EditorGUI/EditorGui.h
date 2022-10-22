//
// Created by Joran on 10/06/2022.
//

#ifndef GAME_EDITORGUI_H
#define GAME_EDITORGUI_H

#include <vector>
#include <string>
#include <glm/vec2.hpp>

namespace BalEditor::EditorGUI
{
    void DrawIntValue( const char* name, int value, float width = 100 );
    void DrawInt2Value( const char* name, const int value[2], float width = 100 );
    void DrawInt3Value( const char* name, const int value[3], float width = 100 );
    void DrawInt4Value( const char* name, const int value[4], float width = 100 );
    
    void DrawFloatValue( const char* name, float value, float width = 100 );
    void DrawFloat2Value( const char* name, const float value[2], float width = 100 );
    void DrawFloat3Value( const char* name, const float value[3], float width = 100 );
    void DrawFloat4Value( const char* name, const float value[4], float width = 100 );
    
    bool DrawIntSlider( const char* name, int& value, int min = 0, int max = 10, float width = 100 );
    bool DrawFloatSlider( const char* name, float& value, float min = 0.f, float max = 10.f, float width = 100 );
    
    bool DrawFloat( const char* name, float& value, float steps, float width = 100 );
    bool DrawFloat2( const char* name, float value[2], float steps, float resetValue = 0.0f, float width = 100 );
    bool DrawFloat3( const char* name, float value[3], float steps, float resetValue = 0.0f, float width = 100 );
    bool DrawFloat4( const char* name, float value[4], float steps, float resetValue = 0.0f, float width = 100 );
    
    bool DrawInt( const char* name, int& value, float steps, int min = 0, int max = 0, float width = 100 );
    bool DrawInt2( const char* name, int value[2], float steps, int resetValue = 0, float width = 100 );
    bool DrawInt3( const char* name, int value[3], float steps, int resetValue = 0, float width = 100 );
    bool DrawInt4( const char* name, int value[4], float steps, int resetValue = 0, float width = 100 );
    
    bool DrawToggle( const char* name, bool& value, float width = 100 );
    bool DrawComboBox( const char* name, std::string& currentValue, const std::vector<std::string>& elements, float width = 100 );
    bool DrawInputText( const char* name, char* inputText, int maxLength, float width = 100 );
    bool DrawColor( const char* name, float color[3], float width = 100 );
    void DrawText( const char* text );
    bool DrawButton( const char* name, const glm::vec2& size = { 0, 0 } );
    bool DrawInputText( std::string_view name, char* text, int maxLength, float width, int flags );
    
    int DrawPopupContextWindow( std::string_view name, const std::vector<std::string>& options );
    
    bool StartPopup( const char* name, bool centered = true, glm::vec2 size = { 0, 0 } );
    bool BeginPopup( const char* name );
    bool BeginPopupContextItem();
    void CloseCurrentPopup();
    void EndPopup();
    
    bool Begin( std::string_view title, bool& isVisible, int flags );
    void End();
    
    bool BeginMenuBar();
    void EndMenuBar();
    bool MenuItem( std::string_view name );
    
    void Separator();
    void Spacing();
    void SameLine( float d = 0 );
    
    void SetWindowFocus( std::string_view name );
    
    bool TreeNodeEx( uint64_t id, int flags, const char* name );
    void TreePop();
    
    void PushStyleVar( int styleVar, const glm::vec2& val );
    void PopStyleVar();
    
    void* ReceiveDragDrop( std::string_view name );
    glm::vec2 GetContentRegionAvail();
    float GetLineHeight();
    glm::vec2 GetMousePos();
    bool IsWindowHovered();
    bool IsMouseDown( int i );
    bool IsItemClicked();
}
#endif //GAME_EDITORGUI_H
