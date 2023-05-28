#include "ShaderEditor.h"

#include <array>
#include <filesystem>

#include "../Renderer/Imgui/imnodes.h"
#include "../Renderer/EditorGui.h"

#include "../Graphs/Shaders/Nodes/Colour/BrightContrastNode.h"
#include "../Graphs/Shaders/Nodes/Colour/ColorInvertNode.h"
#include "../Graphs/Shaders/Nodes/Colour/ColorMixNode.h"
#include "../Graphs/Shaders/Nodes/Colour/GammaNode.h"
#include "../Graphs/Shaders/Nodes/Colour/HueSaturationValueNode.h"
#include "../Graphs/Shaders/Nodes/Converter/ClampNode.h"
#include "../Graphs/Shaders/Nodes/Converter/CombineRGB.h"
#include "../Graphs/Shaders/Nodes/Converter/CombineXYZ.h"
#include "../Graphs/Shaders/Nodes/Converter/MathNode.h"
#include "../Graphs/Shaders/Nodes/Converter/RGBtoBWNode.h"
#include "../Graphs/Shaders/Nodes/Converter/SeparateRGB.h"
#include "../Graphs/Shaders/Nodes/Converter/SeparateXYZ.h"
#include "../Graphs/Shaders/Nodes/Converter/VectorMathNode.h"
#include "../Graphs/Shaders/Nodes/Input/CameraData.h"
#include "../Graphs/Shaders/Nodes/Shaders/FragmentOutput.h"
#include "../Graphs/Shaders/Nodes/Shaders/FragmentUnlitNode.h"
#include "../Graphs/Shaders/Nodes/Shaders/VertexOutput.h"
#include "../Graphs/Shaders/Nodes/Textures/ImageTextureNode.h"
#include "../Graphs/Shaders/Nodes/Vector/BumpNode.h"
#include "../Graphs/Shaders/Nodes/Vector/Displacement.h"
#include "../Graphs/Shaders/Nodes/Vector/MappingNode.h"
#include "../Graphs/Shaders/Nodes/Vector/NormalMapNode.h"

std::unordered_map<FawnForge::CShaderEditor::ui_node_type, const char*> FawnForge::CShaderEditor::m_nodeNameMap{{ ui_node_type::vertex_output,         "Shaders/Vertex Output" },
                                                                                                                { ui_node_type::fragment_output,       "Shaders/Fragment/Specular Shader" },
                                                                                                                { ui_node_type::fragment_unlit_output, "Shaders/Fragment/Unlit Shader" },
                                                                                                                { ui_node_type::math,                  "Converter/Math" },
                                                                                                                { ui_node_type::image_texture,         "Texture/Image Texture" },
                                                                                                                { ui_node_type::bright_contrast,       "Colour/Brightness Contrast" },
                                                                                                                { ui_node_type::gamma,                 "Colour/Gamma" },
                                                                                                                { ui_node_type::hue_saturation_value,  "Colour/Hue Saturation Value" },
                                                                                                                { ui_node_type::invert,                "Colour/Invert" },
                                                                                                                { ui_node_type::mix,                   "Colour/Mix" },
                                                                                                                { ui_node_type::bump,                  "Vector/Bump" },
                                                                                                                { ui_node_type::displacement,          "Vector/Displacement" },
                                                                                                                { ui_node_type::mapping,               "Vector/Mapping" },
                                                                                                                { ui_node_type::normal_map,            "Vector/Normal Map" },
                                                                                                                { ui_node_type::clamp,                 "Converter/Clamp" },
                                                                                                                { ui_node_type::rgb_to_bw,             "Converter/RGB to BW" },
                                                                                                                { ui_node_type::vector_math,           "Converter/Vector Math" },
                                                                                                                { ui_node_type::camera_data,           "Input/Camera Data" },
                                                                                                                { ui_node_type::combine_xyz,           "Converter/Combine XYZ" },
                                                                                                                { ui_node_type::combine_rgb,           "Converter/Combine RGB" },
                                                                                                                { ui_node_type::separate_xyz,          "Converter/Separate XYZ" },
                                                                                                                { ui_node_type::separate_rgb,          "Converter/Separate RGB" }};

FawnForge::CShaderEditor::CShaderEditor()
        : m_nodeCreationMap{{
                                    ui_node_type::vertex_output,         []( CShaderGraph& graph )
                                                                         {
                                                                             return reinterpret_cast<CShaderNode*>(graph.CreateNode<CVertexOutputNode>());
                                                                         }},
                            {
                                    ui_node_type::fragment_output,       []( CShaderGraph& graph )
                                                                         {
                                                                             return reinterpret_cast<CShaderNode*>(graph.CreateNode<CFragmentOutputNode>());
                                                                         }},
                            {
                                    ui_node_type::math,                  []( CShaderGraph& graph )
                                                                         {
                                                                             return reinterpret_cast<CShaderNode*>(graph.CreateNode<CMathNode>());
                                                                         }},
                            {
                                    ui_node_type::bright_contrast,       []( CShaderGraph& graph )
                                                                         {
                                                                             return reinterpret_cast<CShaderNode*>(graph.CreateNode<CBrightContrastNode>());
                                                                         }},
                            {
                                    ui_node_type::gamma,                 []( CShaderGraph& graph )
                                                                         {
                                                                             return reinterpret_cast<CShaderNode*>(graph.CreateNode<CGammaNode>());
                                                                         }},
                            {
                                    ui_node_type::hue_saturation_value,  []( CShaderGraph& graph )
                                                                         {
                                                                             return reinterpret_cast<CShaderNode*>(graph.CreateNode<CHueSaturationValueNode>());
                                                                         }},
                            {
                                    ui_node_type::invert,                []( CShaderGraph& graph )
                                                                         {
                                                                             return reinterpret_cast<CShaderNode*>(graph.CreateNode<CColorInvertNode>());
                                                                         }},
                            {
                                    ui_node_type::mix,                   []( CShaderGraph& graph )
                                                                         {
                                                                             return reinterpret_cast<CShaderNode*>(graph.CreateNode<CColorMixNode>());
                                                                         }},
                            {
                                    ui_node_type::image_texture,         []( CShaderGraph& graph )
                                                                         {
                                                                             return reinterpret_cast<CShaderNode*>(graph.CreateNode<CImageTextureNode>());
                                                                         }},
                            {
                                    ui_node_type::bump,                  []( CShaderGraph& graph )
                                                                         {
                                                                             return reinterpret_cast<CShaderNode*>(graph.CreateNode<CBumpNode>());
                                                                         }},
                            {
                                    ui_node_type::displacement,          []( CShaderGraph& graph )
                                                                         {
                                                                             return reinterpret_cast<CShaderNode*>(graph.CreateNode<CDisplacementNode>());
                                                                         }},
                            {
                                    ui_node_type::mapping,               []( CShaderGraph& graph )
                                                                         {
                                                                             return reinterpret_cast<CShaderNode*>(graph.CreateNode<CMappingNode>());
                                                                         }},
                            {
                                    ui_node_type::normal_map,            []( CShaderGraph& graph )
                                                                         {
                                                                             return reinterpret_cast<CShaderNode*>(graph.CreateNode<CNormalMapNode>());
                                                                         }},
                            {
                                    ui_node_type::clamp,                 []( CShaderGraph& graph )
                                                                         {
                                                                             return reinterpret_cast<CShaderNode*>(graph.CreateNode<CClampNode>());
                                                                         }},
                            {
                                    ui_node_type::rgb_to_bw,             []( CShaderGraph& graph )
                                                                         {
                                                                             return reinterpret_cast<CShaderNode*>(graph.CreateNode<CRGBtoBWNode>());
                                                                         }},
                            {
                                    ui_node_type::vector_math,           []( CShaderGraph& graph )
                                                                         {
                                                                             return reinterpret_cast<CShaderNode*>(graph.CreateNode<CVectorMathNode>());
                                                                         }},
                            {
                                    ui_node_type::fragment_unlit_output, []( CShaderGraph& graph )
                                                                         {
                                                                             return reinterpret_cast<CShaderNode*>(graph.CreateNode<CFragmentUnlitNode>());
                                                                         }},
                            {
                                    ui_node_type::camera_data,           []( CShaderGraph& graph )
                                                                         {
                                                                             return reinterpret_cast<CShaderNode*>(graph.CreateNode<CCameraData>());
                                                                         }},
                            {
                                    ui_node_type::combine_xyz,           []( CShaderGraph& graph )
                                                                         {
                                                                             return reinterpret_cast<CShaderNode*>(graph.CreateNode<CCombineXYZ>());
                                                                         }},
                            {
                                    ui_node_type::combine_rgb,           []( CShaderGraph& graph )
                                                                         {
                                                                             return reinterpret_cast<CShaderNode*>(graph.CreateNode<CCombineRGB>());
                                                                         }},
                            {
                                    ui_node_type::separate_xyz,          []( CShaderGraph& graph )
                                                                         {
                                                                             return reinterpret_cast<CShaderNode*>(graph.CreateNode<CSeparateXYZ>());
                                                                         }},
                            {
                                    ui_node_type::separate_rgb,          []( CShaderGraph& graph )
                                                                         {
                                                                             return reinterpret_cast<CShaderNode*>(graph.CreateNode<CSeparateRGB>());
                                                                         }}}
{
    for ( int n{ 1 }; n < (uint32_t) ui_node_type::max; ++n )
    {
        m_allNodeNames.emplace_back( ToString((ui_node_type) n ));
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
    m_currentShaderFile.uuid     = 0;
    m_currentShaderFile.isFolder = true;
    m_currentShaderFile.fileName = shader.fileName;
    m_currentShaderFile.path     = shader.path;
    m_wantsToSave = false;
    m_cullMode    = 0;
    
    m_shaderGraph.Cleanup();
    m_shaderGraph.Initialize();
}

void FawnForge::CShaderEditor::Draw()
{
    bool     updateBindings{};
    int      linkId;
    uint64_t idx{ (uint64_t) -1 };
    SLink    link{};
    SShaderBinding* pPreviousBinding{ nullptr };
    constexpr std::array<const char*, 3> value = { "Front Face Culling", "No Face Culling", "Back Face Culling" };
    std::vector<SShaderBinding>& bindings{ m_shaderGraph.GetBindings() };
    
    if ( Gui::Begin( "Shader Graph", m_isWindowVisible, 1 << 10 ))
    {
        // Draw bindings
        if ( Gui::BeginChild( "Bindings", { 256, 0 }, true, 1 << 11 ))
        {
            for ( uint32_t i{}; i < bindings.size(); pPreviousBinding = &bindings[i++] )
            {
                if ( pPreviousBinding )
                {
                    if ( std::strcmp( pPreviousBinding->name, bindings[i].name ) == 0 )
                    {
                        continue;
                    }
                    if ( SShaderBinding::IsBindingSimularEnough( *pPreviousBinding, bindings[i] ))
                    {
                        bindings[i] = *pPreviousBinding;
                        continue;
                    }
                }
                updateBindings = DrawBinding( bindings[i] );
            }
            Gui::EndChild();
        }
        Gui::SameLine();
        
        if ( Gui::BeginMenuBar())
        {
            m_wantsToSave |= Gui::DrawButton( "Save" );
            if ( Gui::DrawButton( "Save as ..." ))
            {
                m_currentShaderFile.fileName = "";
                m_currentShaderFile.isFolder = true;
                m_wantsToSave = true;
            }
            Gui::DrawTristateToggle( value, m_cullMode, 200, true );
            Gui::EndMenuBar();
        }
        
        //Draw node editor
        ImNodes::BeginNodeEditor();
        idx = Gui::DrawPopupContextWindow( "Add Node", m_allNodeNames );
        //Draw nodes
        m_shaderGraph.DrawNodes();
        //Draw links
        for ( const SLink& linkToDraw : m_links )
        {
            ImNodes::Link( linkToDraw.id, linkToDraw.startAttr, linkToDraw.endAttr );
        }
        ImNodes::MiniMap();
        ImNodes::EndNodeEditor();
        Gui::End();
    }
    
    if ( idx != -1 )
    {
        glm::vec2 mousePos = Gui::GetMousePos();
        AddNode( static_cast<ui_node_type>( idx + 1 ), mousePos );
    }
    if ( ImNodes::IsLinkCreated( &link.startNodeId, &link.startAttr, &link.endNodeId, &link.endAttr ))
    {
        AddLink( link );
    }
    if ( ImNodes::IsLinkDestroyed( &linkId ))
    {
        const auto iterator = std::ranges::find_if(
                m_links, [ linkId ]( const SLink& link ) -> bool
                {
                    return link.id == linkId;
                }
        );
        assert( iterator != m_links.end());
        m_shaderGraph.Disconnect(
                iterator->startNodeId, iterator->startAttr, iterator->endNodeId, iterator->endAttr
        );
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
bool FawnForge::CShaderEditor::DrawBinding( SShaderBinding& binding )
{
    bool hasValueChanged{ false };
    Gui::PushId((int) std::hash<SShaderBinding>{}( binding ));
    if ( Gui::CollapsingHeader( "Binding" ))
    {
        bool isInOut{ binding.layoutType == SShaderBinding::layout_input || binding.layoutType == SShaderBinding::layout_output || binding.layoutType == SShaderBinding::layout_subpass_input || binding.layoutType == SShaderBinding::layout_subpass_output };
        if ( isInOut )
        {
            Gui::Disable();
        }
        std::vector<std::string> layoutString;
        std::vector<std::string> valueString;
        uint64_t                 layoutType{ (uint64_t) binding.layoutType };
        uint64_t                 valueType{ (uint64_t) binding.valueType };
        layoutString.reserve( SShaderBinding::layout_max );
        valueString.reserve( SShaderBinding::value_type_max );
        for ( int i{}; i < SShaderBinding::value_type::value_type_max; ++i )
        {
            if ( i < SShaderBinding::layout::layout_max )
            {
                layoutString.emplace_back( SShaderBinding::ToString((SShaderBinding::layout) i ));
            }
            valueString.emplace_back( SShaderBinding::ToString((SShaderBinding::value_type) i ));
        }
        hasValueChanged |= Gui::DrawComboBox( "Layout Type", layoutType, layoutString );
        hasValueChanged |= Gui::DrawComboBox( "Value Type", valueType, valueString );
        hasValueChanged |= Gui::DrawInt( "Set", binding.set, 1 );
        hasValueChanged |= Gui::DrawInt( "Binding", binding.binding, 1 );
        hasValueChanged |= Gui::DrawInt( "Array Size", binding.arraySize, 1 );
        hasValueChanged |= Gui::DrawInputText( "Name", binding.name, 64 );
        hasValueChanged |= Gui::DrawInputText( "Instance Name", binding.instanceName, 64 );
        hasValueChanged |= Gui::DrawInputText( "Array Size Name", binding.arraySizeString, 64 );
        
        if ( hasValueChanged )
        {
            binding.layoutType = (SShaderBinding::layout) layoutType;
            binding.valueType  = (SShaderBinding::value_type) valueType;
        }
        
        if ( binding.members.empty())
        {
            Gui::DrawText( "Struct Members:" );
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
    auto               it = m_links.begin();
    
    while (( it = std::find_if(
            it, m_links.end(), [ &currentNode ]( const SLink& link )
            {
                return link.endNodeId == currentNode;
            }
    )) != m_links.end())
    {
        links.push_back( *it );
        ++it;
    }
    std::ranges::reverse( links );
    return links;
}
void FawnForge::CShaderEditor::Evaluate()
{
    // find connected node order
    if ( m_shaderGraph.Compile( m_currentShaderFile, m_cullMode + 1 ))
    {
        std::cout << "Compilation successful" << std::endl;
    }
    
    m_wantsToSave = false;
}

void FawnForge::CShaderEditor::AddNode( const ui_node_type type, const glm::vec2& position )
{
    CShaderNode* pNode{ nullptr };
    
    if ( m_nodeCreationMap.contains( type ))
    {
        pNode = m_nodeCreationMap.at( type )( m_shaderGraph );
    }
    ImNodes::SetNodeScreenSpacePos(( pNode ) ? pNode->GetId() : -1, position );
    m_shaderGraph.Add( pNode );
}
void FawnForge::CShaderEditor::AddLink( SLink& link )
{
    int nodeId{ link.endNodeId };
    int pinId{ link.endAttr };
    CShaderNode* pNode = m_shaderGraph.GetNode( nodeId );
    if ( pNode && pNode->GetInput( pinId - nodeId ))
    {
        if ( pNode->GetInput( link.endAttr - link.endNodeId )->IsConnected())
        {
            auto it = std::find_if(
                    m_links.cbegin(), m_links.cend(), [ pinId]( const SLink& link )
                    {
                        return link.endAttr == pinId;
                    }
            );
            m_shaderGraph.Disconnect((*it).startNodeId, (*it).startAttr, (*it).endNodeId, (*it).endAttr);
            m_links.erase( it );
        }
        link.id = m_linkId++;
        m_shaderGraph.Connect( link.startNodeId, link.startAttr, link.endNodeId, link.endAttr );
        m_links.push_back( link );
    }
    
}

const char* FawnForge::CShaderEditor::ToString( const ui_node_type type )
{
    if ( m_nodeNameMap.contains( type ))
    {
        return m_nodeNameMap.at( type );
    }
    return nullptr;
}

void FawnForge::CShaderEditor::EnterFileName()
{
    Gui::PushId( "SaveShader" );
    if ( Gui::StartPopup( "Save Shader", true, { 256, -1 } ))
    {
        char      name[64]{};
        bool      inputChanged;
        bool      saved;
        bool      cancel;
        for ( int i{}; i < 64; ++i )
        {
            if ( i > m_currentShaderFile.fileName.size())
            {
                break;
            }
            name[i] = m_currentShaderFile.fileName[i];
        }
        inputChanged = Gui::DrawInputText( "new name", name, 64 );
        saved        = Gui::DrawButton( "Save" );
        Gui::SameLine();
        cancel = Gui::DrawButton( "Cancel" );
        Gui::EndPopup();
        if ( inputChanged )
        {
            m_currentShaderFile.fileName = "";
            m_currentShaderFile.fileName.append(
                    name, std::find_if(
                            name, name + 64, []( char c )
                            {
                                return c == '\0';
                            }
                    ));
        }
        if ( saved && !m_currentShaderFile.fileName.empty())
        {
            m_currentShaderFile.isFolder = false;
        }
        if ( cancel )
        {
            m_currentShaderFile.isFolder = true;
            m_currentShaderFile.fileName = "";
            m_wantsToSave = false;
        }
    }
    Gui::PopId();
}
