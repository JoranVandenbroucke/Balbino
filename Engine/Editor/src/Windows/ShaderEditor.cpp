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
#include "../Graphs/Shaders/Nodes/Converter/MathNode.h"
#include "../Graphs/Shaders/Nodes/Converter/RGBtoBWNode.h"
#include "../Graphs/Shaders/Nodes/Converter/VectorMathNode.h"
#include "../Graphs/Shaders/Nodes/Shaders/FragmentOutput.h"
#include "../Graphs/Shaders/Nodes/Shaders/VertexOutput.h"
#include "../Graphs/Shaders/Nodes/Textures/ImageTextureNode.h"
#include "../Graphs/Shaders/Nodes/Vector/BumpNode.h"
#include "../Graphs/Shaders/Nodes/Vector/Displacement.h"
#include "../Graphs/Shaders/Nodes/Vector/MappingNode.h"
#include "../Graphs/Shaders/Nodes/Vector/NormalMapNode.h"

FawnForge::CShaderEditor::CShaderEditor()
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

void FawnForge::CShaderEditor::Draw()
{
    if ( Gui::Begin( "Shader Graph", m_isWindowVisible, 1 << 10 ))
    {
        if ( Gui::BeginChild( "Bindings", { 256, 0 }, true, 1 << 11 ))
        {
            std::vector<SShaderBinding>& bindings{ m_shaderGraph.GetBindings() };
            
            SShaderBinding* pPreviousBinding{ nullptr };
            bool           updateBindings{};
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
            if ( updateBindings )
            {
                m_shaderGraph.PushData();
            }
            Gui::EndChild();
        }
        Gui::SameLine();
        SLink link{};
        int   linkId;
        
        if ( Gui::BeginMenuBar())
        {
            m_wantsToSave |= Gui::DrawButton( "Save" );
            if ( Gui::DrawButton( "Save as ..." ))
            {
                m_currentShaderFile.fileName = "";
                m_currentShaderFile.isFolder = true;
                m_wantsToSave = true;
            }
            std::array<const char*, 3> value = { "Front Face Culling", "No Face Culling", "Back Face Culling" };
            Gui::DrawTristateToggle( value, m_cullMode, 200, true );
            Gui::EndMenuBar();
        }
        
        ImNodes::BeginNodeEditor();
        uint64_t idx = Gui::DrawPopupContextWindow( "Add Node", m_allNodeNames );
        if ( idx != -1 )
        {
            glm::vec2 mousePos = Gui::GetMousePos();
            AddNode( static_cast<ui_node_type>( idx + 1 ), mousePos );
        }
        m_shaderGraph.DrawNodes();
        for ( const SLink& linkToDraw : m_links )
        {
            ImNodes::Link( linkToDraw.id, linkToDraw.startAttr, linkToDraw.endAttr );
        }
        ImNodes::MiniMap();
        ImNodes::EndNodeEditor();
        
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
        
        Gui::End();
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

void FawnForge::CShaderEditor::ShowWindow( const SFile& shader )
{
    m_isWindowVisible = true;
    
    Gui::SetWindowFocus( "Shader Graph" );
    
    m_links.clear();
    m_currentShaderFile.uuid = 0;
    m_currentShaderFile.fileName = "";
    m_currentShaderFile.isFolder = true;
    m_wantsToSave = false;
    m_cullMode    = 0;
    (void) shader;
    
    m_shaderGraph.Cleanup();
    m_shaderGraph.Initialize();
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
    if(m_shaderGraph.Compile(m_currentShaderFile, m_cullMode))
    {
        std::cout << "Compilation successful" << std::endl;
    }
    
    m_wantsToSave = false;
}

void FawnForge::CShaderEditor::AddNode( const ui_node_type type, const glm::vec2& position )
{
    CShaderNode* pNode{ nullptr };
    switch ( type )
    {
        case ui_node_type::vertex_output:
            pNode = reinterpret_cast<CShaderNode*>(m_shaderGraph.CreateNode<CVertexOutputNode>());
            break;
        case ui_node_type::fragment_output:
            pNode = reinterpret_cast<CShaderNode*>(m_shaderGraph.CreateNode<CFragmentOutputNode>());
            break;
        case ui_node_type::math:
            pNode = reinterpret_cast<CShaderNode*>(m_shaderGraph.CreateNode<CMathNode>());
            break;
        case ui_node_type::bright_contrast:
            pNode = reinterpret_cast<CShaderNode*>(m_shaderGraph.CreateNode<CBrightContrastNode>());
            break;
        case ui_node_type::gamma:
            pNode = reinterpret_cast<CShaderNode*>(m_shaderGraph.CreateNode<CGammaNode>());
            break;
        case ui_node_type::hue_saturation_value:
            pNode = reinterpret_cast<CShaderNode*>(m_shaderGraph.CreateNode<CHueSaturationValueNode>());
            break;
        case ui_node_type::invert:
            pNode = reinterpret_cast<CShaderNode*>(m_shaderGraph.CreateNode<CColorInvertNode>());
            break;
        case ui_node_type::mix:
            pNode = reinterpret_cast<CShaderNode*>(m_shaderGraph.CreateNode<CColorMixNode>());
            break;
        case ui_node_type::image_texture:
            pNode = reinterpret_cast<CShaderNode*>(m_shaderGraph.CreateNode<CImageTextureNode>());
            break;
        case ui_node_type::bump:
            pNode = reinterpret_cast<CShaderNode*>(m_shaderGraph.CreateNode<CBumpNode>());
            break;
        case ui_node_type::displacement:
            pNode = reinterpret_cast<CShaderNode*>(m_shaderGraph.CreateNode<CDisplacementNode>());
            break;
        case ui_node_type::mapping:
            pNode = reinterpret_cast<CShaderNode*>(m_shaderGraph.CreateNode<CMappingNode>());
            break;
        case ui_node_type::normal_map:
            pNode = reinterpret_cast<CShaderNode*>(m_shaderGraph.CreateNode<CNormalMapNode>());
            break;
        case ui_node_type::clamp:
            pNode = reinterpret_cast<CShaderNode*>(m_shaderGraph.CreateNode<CClampNode>());
            break;
        case ui_node_type::rgb_to_bw:
            pNode = reinterpret_cast<CShaderNode*>(m_shaderGraph.CreateNode<CRGBtoBWNode>());
            break;
        case ui_node_type::vector_math:
            pNode = reinterpret_cast<CShaderNode*>(m_shaderGraph.CreateNode<CVectorMathNode>());
            break;
        case ui_node_type::shader_node:
        case ui_node_type::max:
            break;
    }
    ImNodes::SetNodeScreenSpacePos(( pNode ) ? pNode->GetId() : -1, position );
    m_shaderGraph.Add( pNode );
}
const char* FawnForge::CShaderEditor::ToString( const ui_node_type type )
{
    switch ( type )
    {
        case ui_node_type::vertex_output:
            return "Shaders/Vertex Output";
        case ui_node_type::fragment_output:
            return "Shaders/Fragment Output";
        case ui_node_type::math:
            return "Converter/Math";
        case ui_node_type::image_texture:
            return "Texture/Image Texture";
        case ui_node_type::bright_contrast:
            return "Colour/Brightness Contrast";
        case ui_node_type::gamma:
            return "Colour/Gamma";
        case ui_node_type::hue_saturation_value:
            return "Colour/Hue Saturation Value";
        case ui_node_type::invert:
            return "Colour/Invert";
        case ui_node_type::mix:
            return "Colour/Mix";
        case ui_node_type::bump:
            return "Vector/Mix";
        case ui_node_type::displacement:
            return "Vector/Displacement";
        case ui_node_type::mapping:
            return "Vector/Mapping";
        case ui_node_type::normal_map:
            return "Vector/Normal Map";
        case ui_node_type::clamp:
            return "Converter/Clamp";
        case ui_node_type::rgb_to_bw:
            return "Converter/RGB to BW";
        case ui_node_type::vector_math:
            return "Converter/Vector Math";
        case ui_node_type::shader_node:
        case ui_node_type::max:
            break;
    }
    return nullptr;
}

void FawnForge::CShaderEditor::AddLink( SLink& link )
{
    link.id = m_linkId++;
    m_shaderGraph.Connect( link.startNodeId, link.startAttr, link.endNodeId, link.endAttr );
    m_links.push_back( link );
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
        
        if ( binding.members.empty() )
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
