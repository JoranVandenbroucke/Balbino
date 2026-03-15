#include "ShaderEditor.h"

#include <array>
#include <filesystem>

#include "../Renderer/EditorGui.h"
#include <imnodes.h>

std::unordered_map<FawnForge::CShaderEditor::ui_node_type, const char*> FawnForge::CShaderEditor::m_nodeNameMap { { ui_node_type::vertex_output, "Shaders/Vertex Shader" },
                                                                                                                  { ui_node_type::fragment_output, "Shaders/Fragment/Lit Shader" },
                                                                                                                  { ui_node_type::fragment_unlit_output, "Shaders/Fragment/Unlit Shader" },
                                                                                                                  { ui_node_type::math, "Converter/Math" },
                                                                                                                  { ui_node_type::image_texture, "Texture/image Texture" },
                                                                                                                  { ui_node_type::bright_contrast, "Colour/Brightness Contrast" },
                                                                                                                  { ui_node_type::gamma, "Colour/Gamma" },
                                                                                                                  { ui_node_type::hue_saturation_value, "Colour/Hue Saturation Value" },
                                                                                                                  { ui_node_type::invert, "Colour/Invert" },
                                                                                                                  { ui_node_type::mix, "Colour/Mix" },
                                                                                                                  { ui_node_type::bump, "Vector/Bump" },
                                                                                                                  { ui_node_type::displacement, "Vector/Displacement" },
                                                                                                                  { ui_node_type::mapping, "Vector/Mapping" },
                                                                                                                  { ui_node_type::normal_map, "Vector/vertex_component_flag_normal Map" },
                                                                                                                  { ui_node_type::clamp, "Converter/Clamp" },
                                                                                                                  { ui_node_type::rgb_to_bw, "Converter/RGB to BW" },
                                                                                                                  { ui_node_type::vector_math, "Converter/Vector Math" },
                                                                                                                  { ui_node_type::camera_data, "input/Camera Data" },
                                                                                                                  { ui_node_type::combine_xyz, "Converter/Combine XYZ" },
                                                                                                                  { ui_node_type::combine_rgb, "Converter/Combine RGB" },
                                                                                                                  { ui_node_type::separate_xyz, "Converter/Separate XYZ" },
                                                                                                                  { ui_node_type::separate_rgb, "Converter/Separate RGB" } };

FawnForge::CShaderEditor::CShaderEditor()
{
    for ( int n { 1 }; n < (uint32_t)ui_node_type::max; ++n )
    {
        m_allNodeNames.emplace_back( ToString( (ui_node_type)n ) );
    }
}

FawnForge::CShaderEditor::~CShaderEditor()
{
    m_links.clear();
}

void FawnForge::CShaderEditor::ShowWindow( const SFile& shader )
{
    m_isWindowVisible = true;

    Gui::SetWindowFocus( "Shader Graph" );

    m_links.clear();
    m_currentShaderFile.uuid     = {};
    m_currentShaderFile.isFolder = true;
    m_currentShaderFile.fileName = shader.fileName;
    m_currentShaderFile.path     = shader.path;
    m_wantsToSave                = false;
    m_cullMode                   = 0;

    m_shaderGraph.Cleanup();
    m_shaderGraph.Initialize();
}

void FawnForge::CShaderEditor::Draw()
{
    bool updateBindings {};
    int linkId;
    uint64_t idx { ~0u };
    SLink link {};
    const SShaderBinding* pPreviousBinding { nullptr };
    constexpr std::array<const char*, 3> value = { "front Face Culling", "No Face Culling", "back Face Culling" };
    std::vector<SShaderBinding>& bindings { m_shaderGraph.GetBindings() };

    if ( Gui::Begin( "Shader Graph", m_isWindowVisible, 1 << 10 ) )
    {
        // GetDrawableObjects bindings
        if ( Gui::BeginChild( "Bindings", { 256, 0 }, true, 1 << 11 ) )
        {
            for ( uint32_t i {}; i < bindings.size(); pPreviousBinding = &bindings[ i ], ++i )
            {
                if ( pPreviousBinding )
                {
                    if ( std::strcmp( pPreviousBinding->name, bindings[ i ].name ) == 0 )
                    {
                        continue;
                    }
                    if ( SShaderBinding::IsBindingSimularEnough( *pPreviousBinding, bindings[ i ] ) )
                    {
                        bindings[ i ] = *pPreviousBinding;
                        continue;
                    }
                }
                updateBindings = DrawBinding( bindings[ i ] );
            }
            Gui::EndChild();
        }
        Gui::SameLine();

        if ( Gui::BeginMenuBar() )
        {
            m_wantsToSave |= Gui::Button( "Save" );
            if ( Gui::Button( "Save as ..." ) )
            {
                m_currentShaderFile.fileName = "";
                m_currentShaderFile.isFolder = true;
                m_wantsToSave                = true;
            }
            Gui::TristateToggle( value, m_cullMode, 200, true );
            Gui::EndMenuBar();
        }

        //GetDrawableObjects node editor
        ImNodes::BeginNodeEditor();
        idx = Gui::PopupContextWindow( "Add Node", m_allNodeNames );
        //GetDrawableObjects nodes
        m_shaderGraph.DrawNodes();
        //constexpr GetDrawableObjects links
        for ( const SLink& linkToDraw : m_links )
        {
            ImNodes::Link( linkToDraw.id, linkToDraw.startAttr, linkToDraw.endAttr );
        }
        ImNodes::MiniMap();
        ImNodes::EndNodeEditor();
        Gui::End();
    }

    if ( idx != ~0u )
    {
        BambiMath::Vector2 mousePos = Gui::GetMousePos();
        AddNode( static_cast<ui_node_type>( idx + 1 ), mousePos );
    }
    if ( ImNodes::IsLinkCreated( &link.startNodeId, &link.startAttr, &link.endNodeId, &link.endAttr ) )
    {
        AddLink( link );
    }
    if ( ImNodes::IsLinkDestroyed( &linkId ) )
    {
        const auto iterator = std::ranges::find_if( m_links, [ linkId ]( const SLink& lnk ) {
            return lnk.id == linkId;
        } );
        assert( iterator != m_links.end() );
        m_shaderGraph.Disconnect( iterator->startNodeId, iterator->startAttr, iterator->endNodeId, iterator->endAttr );
        m_links.erase( iterator );
    }

    if ( updateBindings )
    {
        m_shaderGraph.PushData();
    }
    if ( m_wantsToSave )
    {
        if ( m_currentShaderFile.isFolder )
        {
            EnterFileName();
        }
        else
        {
            Evaluate();
        }
    }
}
bool FawnForge::CShaderEditor::DrawBinding( SShaderBinding& binding ) noexcept
{
    bool hasValueChanged { false };
    Gui::PushId( (int)std::hash<SShaderBinding> {}( binding ) );
    if ( Gui::CollapsingHeader( "Binding" ) )
    {
        bool isInOut { binding.layoutType == SShaderBinding::layout::input || binding.layoutType == SShaderBinding::layout::output || binding.layoutType == SShaderBinding::layout::subpass_input
                       || binding.layoutType == SShaderBinding::layout::subpass_output };
        if ( isInOut )
        {
            Gui::Disable();
        }
        std::vector<std::string> layoutString;
        std::vector<std::string> valueString;
        uint64_t layoutType { (uint64_t)binding.layoutType };
        uint64_t valueType { (uint64_t)binding.valueType };
        layoutString.reserve( (uint8_t)SShaderBinding::layout::max );
        valueString.reserve( (uint8_t)SShaderBinding::value_type::max );
        for ( int i {}; i < (uint8_t)SShaderBinding::value_type::max; ++i )
        {
            if ( i < (uint8_t)SShaderBinding::layout::max )
            {
                layoutString.emplace_back( SShaderBinding::ToString( (SShaderBinding::layout)i ) );
            }
            valueString.emplace_back( SShaderBinding::ToString( (SShaderBinding::value_type)i ) );
        }
        hasValueChanged |= Gui::ComboBox( "Layout Type", layoutType, layoutString );
        hasValueChanged |= Gui::ComboBox( "Value Type", valueType, valueString );
        hasValueChanged |= Gui::Int( "Set", binding.set, 1 );
        hasValueChanged |= Gui::Int( "Binding", binding.binding, 1 );
        hasValueChanged |= Gui::Int( "Array Size", binding.arraySize, 1 );
        hasValueChanged |= Gui::InputText( "Name", binding.name, 64 );
        hasValueChanged |= Gui::InputText( "Instance Name", binding.instanceName, 64 );
        hasValueChanged |= Gui::InputText( "Array Size Name", binding.arraySizeString, 64 );

        if ( hasValueChanged )
        {
            binding.layoutType = (SShaderBinding::layout)layoutType;
            binding.valueType  = (SShaderBinding::value_type)valueType;
        }

        if ( binding.members.empty() )
        {
            Gui::Text( "Struct Members:" );
            for ( auto& member : binding.members )
            {
                hasValueChanged |= DrawBinding( member );
            }
        }
        if ( isInOut )
        {
            Gui::Enable();
        }
    }
    ImGui::PopID();
    return hasValueChanged;
}

std::vector<FawnForge::CShaderEditor::SLink> FawnForge::CShaderEditor::GetNeighbors( const int currentNode )
{
    std::vector<SLink> links;
    auto it = m_links.begin();

    while ( ( it = std::find_if( it, m_links.end(),
                                 [ &currentNode ]( const SLink& link ) {
                                     return link.endNodeId == currentNode;
                                 } ) )
            != m_links.end() )
    {
        links.push_back( *it );
        ++it;
    }
    std::ranges::reverse( links );
    return links;
}
void FawnForge::CShaderEditor::Evaluate() noexcept
{
    // find connected node order
    if ( m_shaderGraph.Compile( m_currentShaderFile, m_cullMode + 1 ) )
    {
        std::cout << "Compilation successful" << std::endl;
    }

    m_wantsToSave = false;
}

void FawnForge::CShaderEditor::AddNode( const ui_node_type type, const BambiMath::Vector2& position )
{
    CShaderNode* pNode { nullptr };

    if ( m_nodeCreationMap.contains( type ) )
    {
        pNode = m_nodeCreationMap.at( type )( m_shaderGraph );
    }
    ImNodes::SetNodeScreenSpacePos( pNode ? pNode->GetId() : -1, BambiMath::Vec2Converter(position) );
    m_shaderGraph.Add( pNode );
}
void FawnForge::CShaderEditor::AddLink( SLink& link ) noexcept
{
    const int nodeId { link.endNodeId };
    const int pinId { link.endAttr };
    const CShaderNode* pNode = m_shaderGraph.GetNode( nodeId );
    if ( pNode && pNode->GetInput( pinId - nodeId ) )
    {
        if ( pNode->GetInput( link.endAttr - link.endNodeId )->IsConnected() )
        {
            auto it = std::ranges::find_if( m_links, [ pinId ]( const SLink& lnk ) {
                return lnk.endAttr == pinId;
            } );
            m_shaderGraph.Disconnect( ( *it ).startNodeId, ( *it ).startAttr, ( *it ).endNodeId, ( *it ).endAttr );
            m_links.erase( it );
        }
        link.id = m_linkId++;
        m_shaderGraph.Connect( link.startNodeId, link.startAttr, link.endNodeId, link.endAttr );
        m_links.push_back( link );
    }
}

const char* FawnForge::CShaderEditor::ToString( const ui_node_type type )
{
    if ( m_nodeNameMap.contains( type ) )
    {
        return m_nodeNameMap.at( type );
    }
    return nullptr;
}

void FawnForge::CShaderEditor::EnterFileName() noexcept
{
    Gui::PushId( "SaveShader" );
    if ( Gui::StartPopup( "Save Shader", true, { 256, -1 } ) )
    {
        char name[ 64 ] {};
        bool inputChanged;
        bool saved;
        bool cancel;
        for ( int i {}; i < 64; ++i )
        {
            if ( i > m_currentShaderFile.fileName.size() )
            {
                break;
            }
            name[ i ] = m_currentShaderFile.fileName[ i ];
        }
        inputChanged = Gui::InputText( "new name", name, 64 );
        saved        = Gui::Button( "Save" );
        Gui::SameLine();
        cancel = Gui::Button( "Cancel" );
        Gui::EndPopup();
        if ( inputChanged )
        {
            m_currentShaderFile.fileName = "";
            m_currentShaderFile.fileName.append( name, std::find_if( name, name + 64, []( char c ) {
                                                     return c == '\0';
                                                 } ) );
        }
        if ( saved && !m_currentShaderFile.fileName.empty() )
        {
            m_currentShaderFile.isFolder = false;
        }
        if ( cancel )
        {
            m_currentShaderFile.isFolder = true;
            m_currentShaderFile.fileName = "";
            m_wantsToSave                = false;
        }
    }
    Gui::PopId();
}
