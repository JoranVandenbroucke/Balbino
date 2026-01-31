//
// Created by Joran on 10/06/2022.
//

#pragma once
#include <string>
#include <vector>
#include <BambiMath.h>

namespace FawnForge::Gui
{
    void IntValue( std::string_view name, uint64_t value, float width = 100 );
    void IntValue( std::string_view name, int value, float width = 100 );
    void Int2Value( std::string_view name, const int value[ 2 ], float width = 100 );
    void Int3Value( std::string_view name, const int value[ 3 ], float width = 100 );
    void Int4Value( std::string_view name, const int value[ 4 ], float width = 100 );

    void FloatValue( std::string_view name, float value, float width = 100 );
    void Float2Value( std::string_view name, const float value[ 2 ], float width = 100 );
    void Float3Value( std::string_view name, const float value[ 3 ], float width = 100 );
    void Float4Value( std::string_view name, const float value[ 4 ], float width = 100 );

    bool IntSlider( std::string_view name, int& value, int min = 0, int max = 10, float width = 100 );
    bool FloatSlider( std::string_view name, float& value, float min = 0.f, float max = 10.f, float width = 100, bool showName = true );

    bool Float( std::string_view name, float& value, float steps, float width = 100 );
    bool Float2( std::string_view name, float value[ 2 ], float steps, float resetValue = 0.0f, float width = 100 );
    bool Float3( std::string_view name, float value[ 3 ], float steps, float resetValue = 0.0f, float width = 100 );
    bool Float4( std::string_view name, float value[ 4 ], float steps, float resetValue = 0.0f, float width = 100 );

    bool Int( std::string_view name, int& value, float steps, int min = 0, int max = 0, float width = 100 );
    bool Int2( std::string_view name, int value[ 2 ], float steps, int resetValue = 0, float width = 100 );
    bool Int3( std::string_view name, int value[ 3 ], float steps, int resetValue = 0, float width = 100 );
    bool Int4( std::string_view name, int value[ 4 ], float steps, int resetValue = 0, float width = 100 );

    bool Toggle( std::string_view name, bool& value, float width = 100 );
    bool TristateToggle( const std::array<const char*, 3>& names, int32_t& currentIndex, float width = 100, bool inMenu = false );
    bool Color( std::string_view name, float color[ 3 ], float width = 100 );
    void Text( const std::string& text, float maxLength = -1 );
    bool Button( std::string_view name, const BambiMath::Vector2& size = { 0, 0 } );
    bool InputText( std::string_view name, char* text, int maxLength, float width = 100.f, int flags = 0.f );

    bool ComboBox( std::string_view name, uint64_t& currentIndex, const std::vector<std::string>& elements, const std::vector<int>& dividers = {}, float width = 100, bool inMenuBar = false );
    uint64_t PopupContextWindow( std::string_view name, const std::vector<std::string>& options );

    bool StartPopup( std::string_view name, bool centered = true, BambiMath::Vector2 size = { 0, 0 } );
    bool BeginPopup( std::string_view name );
    bool BeginPopupContextItem( uint64_t id = 0 );
    void CloseCurrentPopup();
    void EndPopup();

    bool Begin( std::string_view title, bool& isVisible, int flags );
    bool BeginChild( std::string_view title, const BambiMath::Vector2& position, bool border, int flags );
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

    void PushStyleVar( int styleVar, const BambiMath::Vector2& val );
    void PopStyleVar();

    void* ReceiveDragDrop( std::string_view name );
    BambiMath::Vector2 GetContentRegionAvail();
    float GetLineHeight();
    BambiMath::Vector2 GetMousePos();
    bool IsWindowHovered();
    bool IsMouseDown( int i );
    bool IsItemClicked();
    void Columns( int nrOfColumns );
    void NextColumn();
    void MaxNextWindow();
    // void ResourceItem( const SFile& file, VkDescriptorSet descriptorSet, float imageSize );
    void PushId( std::string_view id );
    void PushId( int id );
    void PopId();
    bool IsMouseDoubleClicked( int button );
    void SetNextItemOpen( bool open );
    bool BeginPopupContextWindow( uint64_t uuid );
    unsigned int GetId( std::string_view name );
    unsigned int DockSpace( unsigned int id, const BambiMath::Vector2& size, int flags );
    void SetNextWindowDockId( unsigned int id, int flags );
    void Selectable( int id, bool& isSelected, float height );
    bool CollapsingHeader( std::string_view name );

    void SetTooltip( std::string_view tip );
    void Disable();
    void Enable();
}// namespace FawnForge::Gui
