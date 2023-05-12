//
// Created by Joran on 10/06/2022.
//

#ifndef BALBINO_EDITORGUI_H
#define BALBINO_EDITORGUI_H

#include <vector>
#include <string>
#include <glm/vec2.hpp>
#include <FileParcer.h>

namespace FawnForge::Gui
{
    void DrawIntValue( std::string_view name, uint64_t value, float width = 100 );
    void DrawIntValue( std::string_view name, int value, float width = 100 );
    void DrawInt2Value( std::string_view name, const int value[2], float width = 100 );
    void DrawInt3Value( std::string_view name, const int value[3], float width = 100 );
    void DrawInt4Value( std::string_view name, const int value[4], float width = 100 );
    
    void DrawFloatValue( std::string_view name, float value, float width = 100 );
    void DrawFloat2Value( std::string_view name, const float value[2], float width = 100 );
    void DrawFloat3Value( std::string_view name, const float value[3], float width = 100 );
    void DrawFloat4Value( std::string_view name, const float value[4], float width = 100 );
    
    bool DrawIntSlider( std::string_view name, int& value, int min = 0, int max = 10, float width = 100 );
    bool DrawFloatSlider( std::string_view name, float& value, float min = 0.f, float max = 10.f, float width = 100, bool showName = true );
    
    bool DrawFloat( std::string_view name, float& value, float steps, float width = 100 );
    bool DrawFloat2( std::string_view name, float value[2], float steps, float resetValue = 0.0f, float width = 100 );
    bool DrawFloat3( std::string_view name, float value[3], float steps, float resetValue = 0.0f, float width = 100 );
    bool DrawFloat4( std::string_view name, float value[4], float steps, float resetValue = 0.0f, float width = 100 );
    
    bool DrawInt( std::string_view name, int& value, float steps, int min = 0, int max = 0, float width = 100 );
    bool DrawInt2( std::string_view name, int value[2], float steps, int resetValue = 0, float width = 100 );
    bool DrawInt3( std::string_view name, int value[3], float steps, int resetValue = 0, float width = 100 );
    bool DrawInt4( std::string_view name, int value[4], float steps, int resetValue = 0, float width = 100 );
    
    bool DrawToggle( std::string_view name, bool& value, float width = 100 );
    bool DrawTristateToggle( const std::array<const char*, 3>& names, int32_t& currentIndex, float width = 100, bool inMenu = false );
    bool DrawColor( std::string_view name, float color[3], float width = 100 );
    void DrawText( std::string_view text );
    bool DrawButton( std::string_view name, const glm::vec2& size = { 0, 0 } );
    bool DrawInputText( std::string_view name, char* text, int maxLength, float width = 100.f, int flags = 0.f );
    
    bool DrawComboBox( std::string_view name, uint64_t& currentIndex, const std::vector<std::string>& elements, const std::vector<int>& dividers = {}, float width = 100, bool inMenuBar = false );
    uint64_t DrawPopupContextWindow( std::string_view name, const std::vector<std::string>& options );
    
    bool StartPopup( std::string_view name, bool centered = true, glm::vec2 size = { 0, 0 } );
    bool BeginPopup( std::string_view name );
    bool BeginPopupContextItem( uint64_t id = 0 );
    void CloseCurrentPopup();
    void EndPopup();
    
    bool Begin( std::string_view title, bool& isVisible, int flags );
    bool BeginChild( std::string_view title, const glm::vec2& position, bool border, int flags );
    void End();
    void EndChild();
    
    bool BeginMenuBar();
    bool BeginMenu( std::string_view text );
    void EndMenuBar();
    void EndMenu();
    bool MenuItem( std::string_view name, bool isSelected = true );
    
    void Separator();
    void Spacing();
    void SameLine( float d = 0 );
    
    void SetWindowFocus( std::string_view name );
    
    bool TreeNodeEx( uint64_t id, int flags, std::string_view name );
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
    void Columns( int nrOfColumns );
    void NextColumn();
    void MaxNextWindow();
    void DrawResourceItem( const SFile& file, VkDescriptorSet_T* descriptorSet, float imageSize );
    void PushId( std::string_view id );
    void PushId( int id );
    void PopId();
    bool IsMouseDoubleClicked( int button );
    void SetNextItemOpen( bool open );
    bool BeginPopupContextWindow( uint64_t uuid );
    unsigned int GetId( std::string_view name );
    unsigned int DockSpace( unsigned int id, const glm::vec2& size, int flags );
    void SetNextWindowDockId( unsigned int id, int flags );
    void DrawSelectable( int id, bool& isSelected, float height );
    bool CollapsingHeader(std::string_view name);
    
    void SetTooltip( std::string_view tip );
    void Disable();
    void Enable();
}
#endif //BALBINO_EDITORGUI_H
