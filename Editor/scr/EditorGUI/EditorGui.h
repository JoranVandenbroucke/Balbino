//
// Created by Joran on 10/06/2022.
//

#ifndef GAME_EDITORGUI_H
#define GAME_EDITORGUI_H

#include <vector>
#include <string>

namespace BalEditor::EditorGUI
{
    void DrawIntValue( const char* name, int value );

    void DrawInt2Value( const char* name, const int value[2] );

    void DrawInt3Value( const char* name, const int value[3] );

    void DrawInt4Value( const char* name, const int value[4] );

    void DrawFloatValue( const char* name, float value );

    void DrawFloat2Value( const char* name, const float value[2] );

    void DrawFloat3Value( const char* name, const float value[3] );

    void DrawFloat4Value( const char* name, const float value[4] );

    bool DrawIntSlider( const char* name, int& value, int min = 0, int max = 10 );

    bool DrawFloatSlider( const char* name, float& value, float min = 0.f, float max = 10.f );

    bool DrawFloat( const char* name, float& value, float steps );

    bool DrawFloat2( const char* name, float value[2], float steps, float resetValue = 0.0f );

    bool DrawFloat3( const char* name, float value[3], float steps, float resetValue = 0.0f );

    bool DrawFloat4( const char* name, float value[4], float steps, float resetValue = 0.0f );

    bool DrawInt( const char* name, int& value, float steps );

    bool DrawInt2( const char* name, int value[2], float steps, int resetValue = 0 );

    bool DrawInt3( const char* name, int value[3], float steps, int resetValue = 0 );

    bool DrawInt4( const char* name, int value[4], float steps, int resetValue = 0 );

    bool DrawToggle( const char* name, bool& value );

    bool DrawButton( const char* name );

    bool DrawComboBox( const char* name, std::string& currentValue, const std::vector<std::string>& elements );

    void Spacing();

    bool DrawInputText( const char* title, char* name, int maxLenth );


}
#endif //GAME_EDITORGUI_H
