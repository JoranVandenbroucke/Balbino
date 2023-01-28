#include "ColorNodes.h"

#include <format>

#include "../../../EditorGUI/EditorGui.h"
#include "../Attribute.h"

CBrightContrast::CBrightContrast( const int id, int& attributeStartId )
        : INode{ id, attributeStartId }
        , m_connected{}
        , m_color{}
        , m_brightness{ 0 }
        , m_contrast{ 0 }
{
    attributeStartId += 4;
}

CBrightContrast::~CBrightContrast() = default;

void CBrightContrast::Draw()
{
    ImNodes::BeginNode( m_id );
    
    ImNodes::BeginNodeTitleBar();
    FawnForge::GUI::DrawText( "Brightness / Contrast" );
    ImNodes::EndNodeTitleBar();
    
    DrawInputColorAttribute( m_color, m_attributeStartId, m_connected[0], "Color" );
    DrawInputFloatAttribute( m_brightness, m_attributeStartId + 1, m_connected[1], "Brightness" );
    DrawInputFloatAttribute( m_contrast, m_attributeStartId + 2, m_connected[2], "Contrast" );
    
    FawnForge::GUI::Spacing();
    DrawOutputColorAttribute( m_attributeStartId + 3 );
    
    ImNodes::EndNode();
}

void CBrightContrast::Attach( int endAttr )
{
    if ( m_attributeStartId == endAttr )
    {
        m_connected[0] = true;
    }
    else if ( m_attributeStartId + 1 == endAttr )
    {
        m_connected[1] = true;
    }
    else if ( m_attributeStartId + 2 == endAttr )
    {
        m_connected[2] = true;
    }
}

void CBrightContrast::Detach( int endAttr )
{
    if ( m_attributeStartId == endAttr )
    {
        m_connected[0] = false;
    }
    else if ( m_attributeStartId + 1 == endAttr )
    {
        m_connected[1] = false;
    }
    else if ( m_attributeStartId + 2 == endAttr )
    {
        m_connected[2] = false;
    }
}

std::string CBrightContrast::Evaluate( std::vector<INode*>::iterator& begin, std::set<std::string>& bindings, std::set<std::string>& includes, EAttributeType::Enum attributeType )
{
    (void) begin;
    (void) bindings;
    (void) attributeType;
    includes.insert( "colorMath.glsl" );
    std::string shader;
    shader = std::format(
            "{}*{}+{}",
            m_connected[0]
            ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Color )
            : std::format(
                    "vec3({},{},{})",
                    std::to_string( m_color[0] ),
                    std::to_string( m_color[1] ),
                    std::to_string( m_color[2] )),
            m_connected[1]
            ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Float )
            : std::to_string( m_contrast ),
            m_connected[2]
            ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Float )
            : std::to_string( m_brightness ));
    
    if ( attributeType == EAttributeType::Float )
    {
        shader += ".x";
    }
    return shader;
}

bool CBrightContrast::HasFreeAttachment( int endAttr ) const
{
    if ( m_attributeStartId == endAttr )
    {
        return !m_connected[0];
    }
    if ( m_attributeStartId + 1 == endAttr )
    {
        return !m_connected[1];
    }
    if ( m_attributeStartId + 2 == endAttr )
    {
        return !m_connected[2];
    }
    return false;
}

int CBrightContrast::GetId() const
{
    return m_id;
}

std::vector<int> CBrightContrast::GetInputs() const
{
    return { m_attributeStartId, m_attributeStartId + 1, m_attributeStartId + 2 };
}
uint8_t  CBrightContrast::GetVertexFlags() const
{
    return m_vertexFlags;
}

CGamma::CGamma( const int id, int& attributeStartId )
        : INode{ id, attributeStartId }
        , m_connected{}
        , m_color{}
        , m_gamma{ 0 }
{
    attributeStartId += 3;
}

CGamma::~CGamma() = default;

void CGamma::Draw()
{
    ImNodes::BeginNode( m_id );
    
    ImNodes::BeginNodeTitleBar();
    FawnForge::GUI::DrawText( "Gamma" );
    ImNodes::EndNodeTitleBar();
    
    DrawInputColorAttribute( m_color, m_attributeStartId, m_connected[0], "Color" );
    DrawInputFloatAttribute( m_gamma, m_attributeStartId + 1, m_connected[1], "Gamma" );
    
    FawnForge::GUI::Spacing();
    DrawOutputColorAttribute( m_attributeStartId + 2 );
    
    ImNodes::EndNode();
}

void CGamma::Attach( int endAttr )
{
    if ( m_attributeStartId == endAttr )
    {
        m_connected[0] = true;
    }
    else if ( m_attributeStartId + 1 == endAttr )
    {
        m_connected[1] = true;
    }
}

void CGamma::Detach( int endAttr )
{
    if ( m_attributeStartId == endAttr )
    {
        m_connected[0] = false;
    }
    else if ( m_attributeStartId + 1 == endAttr )
    {
        m_connected[1] = false;
    }
}

std::string CGamma::Evaluate( std::vector<INode*>::iterator& begin, std::set<std::string>& bindings, std::set<std::string>& includes, EAttributeType::Enum attributeType )
{
    (void) begin;
    (void) bindings;
    (void) attributeType;
    includes.insert( "colorMath.glsl" );
    std::string shader;
    shader = std::format(
            "pow({},{})",
            m_connected[0]
            ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Color )
            : std::format(
                    "vec3({},{},{})",
                    std::to_string( m_color[0] ),
                    std::to_string( m_color[1] ),
                    std::to_string( m_color[2] )),
            m_connected[1]
            ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Float )
            : std::to_string( m_gamma ));
    
    if ( attributeType == EAttributeType::Float )
    {
        shader += ".x";
    }
    return shader;
}

bool CGamma::HasFreeAttachment( int endAttr ) const
{
    if ( m_attributeStartId == endAttr )
    {
        return !m_connected[0];
    }
    if ( m_attributeStartId + 1 == endAttr )
    {
        return !m_connected[1];
    }
    return false;
}

int CGamma::GetId() const
{
    return m_id;
}

std::vector<int> CGamma::GetInputs() const
{
    return { m_attributeStartId, m_attributeStartId + 1 };
}
uint8_t  CGamma::GetVertexFlags() const
{
    return m_vertexFlags;
}

CHueSaturationValue::CHueSaturationValue( const int id, int& attributeStartId )
        : INode{ id, attributeStartId }
        , m_connected{}
        , m_color{}
        , m_hue{ 0.5f }
        , m_saturation{ 0 }
        , m_value{ 0 }
        , m_factor{ 1 }
{
    attributeStartId += 6;
}

CHueSaturationValue::~CHueSaturationValue() = default;

void CHueSaturationValue::Draw()
{
    ImNodes::BeginNode( m_id );
    
    ImNodes::BeginNodeTitleBar();
    FawnForge::GUI::DrawText( "Hue Saturation Value" );
    ImNodes::EndNodeTitleBar();
    
    DrawInputColorAttribute( m_color, m_attributeStartId, m_connected[0], "Color" );
    DrawInputFloatAttribute( m_hue, m_attributeStartId + 1, m_connected[1], "Hue" );
    DrawInputFloatAttribute( m_saturation, m_attributeStartId + 2, m_connected[1], "Saturation" );
    DrawInputFloatAttribute( m_value, m_attributeStartId + 3, m_connected[1], "Value" );
    DrawInputFloatAttribute( m_factor, m_attributeStartId + 4, m_connected[1], "Factor", 0, 1 );
    
    FawnForge::GUI::Spacing();
    DrawOutputColorAttribute( m_attributeStartId + 5 );
    
    ImNodes::EndNode();
}

void CHueSaturationValue::Attach( int endAttr )
{
    if ( m_attributeStartId <= endAttr && m_attributeStartId + 6 > endAttr )
    {
        m_connected[endAttr - m_attributeStartId] = true;
    }
}

void CHueSaturationValue::Detach( int endAttr )
{
    if ( m_attributeStartId <= endAttr && m_attributeStartId + 6 > endAttr )
    {
        m_connected[endAttr - m_attributeStartId] = false;
    }
}

std::string CHueSaturationValue::Evaluate( std::vector<INode*>::iterator& begin, std::set<std::string>& bindings, std::set<std::string>& includes, EAttributeType::Enum attributeType )
{
    (void) begin;
    (void) bindings;
    (void) attributeType;
    includes.insert( "colorMath.glsl" );
    std::string shader;
    shader = std::format(
            "HueSatVal({},{},{},{})",
            m_connected[0]
            ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Float )
            : std::to_string( m_hue ),
            m_connected[1]
            ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Float )
            : std::to_string( m_saturation ),
            m_connected[2]
            ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Float )
            : std::to_string( m_value ),
            m_connected[3]
            ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Float )
            : std::to_string( m_factor ),
            m_connected[4]
            ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Color )
            : std::format(
                    "vec3({},{},{})",
                    std::to_string( m_color[0] ),
                    std::to_string( m_color[1] ),
                    std::to_string( m_color[2] )));
    
    if ( attributeType == EAttributeType::Float )
    {
        shader += ".x";
    }
    return shader;
}

bool CHueSaturationValue::HasFreeAttachment( int endAttr ) const
{
    if ( m_attributeStartId <= endAttr && m_attributeStartId + 6 > endAttr )
    {
        return !m_connected[endAttr - m_attributeStartId];
    }
    return false;
}

int CHueSaturationValue::GetId() const
{
    return m_id;
}

std::vector<int> CHueSaturationValue::GetInputs() const
{
    return {
            m_attributeStartId,
            m_attributeStartId + 1,
            m_attributeStartId + 2,
            m_attributeStartId + 3,
            m_attributeStartId + 4
    };
}
uint8_t  CHueSaturationValue::GetVertexFlags() const
{
    return m_vertexFlags;
}

CInvert::CInvert( const int id, int& attributeStartId )
        : INode{ id, attributeStartId }
        , m_connected{}
        , m_color{}
        , m_factor{ 0 }
{
    attributeStartId += 3;
}

CInvert::~CInvert() = default;

void CInvert::Draw()
{
    ImNodes::BeginNode( m_id );
    
    ImNodes::BeginNodeTitleBar();
    FawnForge::GUI::DrawText( "Invert" );
    ImNodes::EndNodeTitleBar();
    
    DrawInputColorAttribute( m_color, m_attributeStartId, m_connected[0], "Color" );
    DrawInputFloatAttribute( m_factor, m_attributeStartId + 1, m_connected[1], "Factor" );
    
    FawnForge::GUI::Spacing();
    DrawOutputColorAttribute( m_attributeStartId + 2 );
    
    ImNodes::EndNode();
}

void CInvert::Attach( int endAttr )
{
    if ( m_attributeStartId == endAttr )
    {
        m_connected[0] = true;
    }
    else if ( m_attributeStartId + 1 == endAttr )
    {
        m_connected[1] = true;
    }
}

void CInvert::Detach( int endAttr )
{
    if ( m_attributeStartId == endAttr )
    {
        m_connected[0] = false;
    }
    else if ( m_attributeStartId + 1 == endAttr )
    {
        m_connected[1] = false;
    }
}

std::string CInvert::Evaluate( std::vector<INode*>::iterator& begin, std::set<std::string>& bindings, std::set<std::string>& includes, EAttributeType::Enum attributeType )
{
    (void) begin;
    (void) bindings;
    (void) attributeType;
    includes.insert( "colorMath.glsl" );
    std::string shader;
    shader = std::format(
            "Invert({},{})",
            m_connected[0]
            ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Color )
            : std::format(
                    "vec3({},{},{})",
                    std::to_string( m_color[0] ),
                    std::to_string( m_color[1] ),
                    std::to_string( m_color[2] )),
            m_connected[1]
            ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Float )
            : std::to_string( m_factor ));
    
    if ( attributeType == EAttributeType::Float )
    {
        shader += ".x";
    }
    return shader;
}

bool CInvert::HasFreeAttachment( int endAttr ) const
{
    if ( m_attributeStartId == endAttr )
    {
        return !m_connected[0];
    }
    if ( m_attributeStartId + 1 == endAttr )
    {
        return !m_connected[1];
    }
    return false;
}

int CInvert::GetId() const
{
    return m_id;
}

std::vector<int> CInvert::GetInputs() const
{
    return { m_attributeStartId, m_attributeStartId + 1 };
}
uint8_t  CInvert::GetVertexFlags() const
{
    return m_vertexFlags;
}

CMix::CMix( const int id, int& attributeStartId )
        : INode{ id, attributeStartId }
        , m_type{ EMode::Multiply }
        , m_connected{}
        , m_color1{}
        , m_color2{}
        , m_factor{ 0 }
{
    attributeStartId += 4;
}

CMix::~CMix() = default;

void CMix::Draw()
{
    ImNodes::BeginNode( m_id );
    
    ImNodes::BeginNodeTitleBar();
    FawnForge::GUI::DrawText( ToString( m_type ));
    ImNodes::EndNodeTitleBar();
    {
        std::vector<std::string> names;
        uint64_t                 current{ uint64_t( m_type ) };
        
        for ( int n = 0; n < EMode::MaxIndex; n++ )
        {
            names.push_back( ToString( EMode::Enum( n )));
        }
        FawnForge::GUI::DrawComboBox( "Mode", current, names );
        m_type = EMode::Enum( current );
    }
    
    DrawInputColorAttribute( m_color1, m_attributeStartId, m_connected[0], "Color" );
    DrawInputColorAttribute( m_color2, m_attributeStartId + 1, m_connected[1], "Color" );
    DrawInputFloatAttribute( m_factor, m_attributeStartId + 2, m_connected[2], "Factor", 0, 1 );
    
    FawnForge::GUI::Spacing();
    DrawOutputColorAttribute( m_attributeStartId + 3 );
    
    ImNodes::EndNode();
}

void CMix::Attach( int endAttr )
{
    if ( m_attributeStartId == endAttr )
    {
        m_connected[0] = true;
    }
    else if ( m_attributeStartId + 1 == endAttr )
    {
        m_connected[1] = true;
    }
    else if ( m_attributeStartId + 2 == endAttr )
    {
        m_connected[2] = true;
    }
}

void CMix::Detach( int endAttr )
{
    if ( m_attributeStartId == endAttr )
    {
        m_connected[0] = false;
    }
    else if ( m_attributeStartId + 1 == endAttr )
    {
        m_connected[1] = false;
    }
    else if ( m_attributeStartId + 2 == endAttr )
    {
        m_connected[2] = false;
    }
}

std::string CMix::Evaluate( std::vector<INode*>::iterator& begin, std::set<std::string>& bindings, std::set<std::string>& includes, EAttributeType::Enum attributeType )
{
    (void) begin;
    (void) bindings;
    (void) attributeType;
    includes.insert( "colorMath.glsl" );
    
    std::string shader;
    switch ( m_type )
    {
        case EMode::Add:
            shader = std::format(
                    "Add({},{},{})",
                    m_connected[0]
                    ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Color )
                    : std::format(
                            "vec3({},{},{})",
                            std::to_string( m_color1[0] ),
                            std::to_string( m_color1[1] ),
                            std::to_string( m_color1[2] )),
                    m_connected[1]
                    ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Color )
                    : std::format(
                            "vec3({},{},{})",
                            std::to_string( m_color2[0] ),
                            std::to_string( m_color2[1] ),
                            std::to_string( m_color2[2] )),
                    m_connected[2]
                    ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Float )
                    : std::to_string( m_factor ));
            break;
        case EMode::Subtract:
            shader = std::format(
                    "Subtract({},{},{})",
                    m_connected[0]
                    ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Color )
                    : std::format(
                            "vec3({},{},{})",
                            std::to_string( m_color1[0] ),
                            std::to_string( m_color1[1] ),
                            std::to_string( m_color1[2] )),
                    m_connected[1]
                    ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Color )
                    : std::format(
                            "vec3({},{},{})",
                            std::to_string( m_color2[0] ),
                            std::to_string( m_color2[1] ),
                            std::to_string( m_color2[2] )),
                    m_connected[2]
                    ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Float )
                    : std::to_string( m_factor ));
            break;
        case EMode::Multiply:
            shader = std::format(
                    "Multiply({},{},{})",
                    m_connected[0]
                    ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Color )
                    : std::format(
                            "vec3({},{},{})",
                            std::to_string( m_color1[0] ),
                            std::to_string( m_color1[1] ),
                            std::to_string( m_color1[2] )),
                    m_connected[1]
                    ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Color )
                    : std::format(
                            "vec3({},{},{})",
                            std::to_string( m_color2[0] ),
                            std::to_string( m_color2[1] ),
                            std::to_string( m_color2[2] )),
                    m_connected[2]
                    ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Float )
                    : std::to_string( m_factor ));
            break;
        case EMode::Screen:
            shader = std::format(
                    "Screen({},{},{})",
                    m_connected[0]
                    ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Color )
                    : std::format(
                            "vec3({},{},{})",
                            std::to_string( m_color1[0] ),
                            std::to_string( m_color1[1] ),
                            std::to_string( m_color1[2] )),
                    m_connected[1]
                    ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Color )
                    : std::format(
                            "vec3({},{},{})",
                            std::to_string( m_color2[0] ),
                            std::to_string( m_color2[1] ),
                            std::to_string( m_color2[2] )),
                    m_connected[2]
                    ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Float )
                    : std::to_string( m_factor ));
            break;
        case EMode::Divide:
            shader = std::format(
                    "Divide({},{},{})",
                    m_connected[0]
                    ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Color )
                    : std::format(
                            "vec3({},{},{})",
                            std::to_string( m_color1[0] ),
                            std::to_string( m_color1[1] ),
                            std::to_string( m_color1[2] )),
                    m_connected[1]
                    ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Color )
                    : std::format(
                            "vec3({},{},{})",
                            std::to_string( m_color2[0] ),
                            std::to_string( m_color2[1] ),
                            std::to_string( m_color2[2] )),
                    m_connected[2]
                    ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Float )
                    : std::to_string( m_factor ));
            break;
        case EMode::Difference:
            shader = std::format(
                    "Difference({},{},{})",
                    m_connected[0]
                    ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Color )
                    : std::format(
                            "vec3({},{},{})",
                            std::to_string( m_color1[0] ),
                            std::to_string( m_color1[1] ),
                            std::to_string( m_color1[2] )),
                    m_connected[1]
                    ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Color )
                    : std::format(
                            "vec3({},{},{})",
                            std::to_string( m_color2[0] ),
                            std::to_string( m_color2[1] ),
                            std::to_string( m_color2[2] )),
                    m_connected[2]
                    ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Float )
                    : std::to_string( m_factor ));
            break;
        case EMode::Darken:
            shader = std::format(
                    "Darken({},{},{})",
                    m_connected[0]
                    ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Color )
                    : std::format(
                            "vec3({},{},{})",
                            std::to_string( m_color1[0] ),
                            std::to_string( m_color1[1] ),
                            std::to_string( m_color1[2] )),
                    m_connected[1]
                    ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Color )
                    : std::format(
                            "vec3({},{},{})",
                            std::to_string( m_color2[0] ),
                            std::to_string( m_color2[1] ),
                            std::to_string( m_color2[2] )),
                    m_connected[2]
                    ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Float )
                    : std::to_string( m_factor ));
            break;
        case EMode::Lighten:
            shader = std::format(
                    "Lighten({},{},{})",
                    m_connected[0]
                    ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Color )
                    : std::format(
                            "vec3({},{},{})",
                            std::to_string( m_color1[0] ),
                            std::to_string( m_color1[1] ),
                            std::to_string( m_color1[2] )),
                    m_connected[1]
                    ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Color )
                    : std::format(
                            "vec3({},{},{})",
                            std::to_string( m_color2[0] ),
                            std::to_string( m_color2[1] ),
                            std::to_string( m_color2[2] )),
                    m_connected[2]
                    ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Float )
                    : std::to_string( m_factor ));
            break;
        case EMode::Overlay:
            shader = std::format(
                    "Overlay({},{},{})",
                    m_connected[0]
                    ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Color )
                    : std::format(
                            "vec3({},{},{})",
                            std::to_string( m_color1[0] ),
                            std::to_string( m_color1[1] ),
                            std::to_string( m_color1[2] )),
                    m_connected[1]
                    ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Color )
                    : std::format(
                            "vec3({},{},{})",
                            std::to_string( m_color2[0] ),
                            std::to_string( m_color2[1] ),
                            std::to_string( m_color2[2] )),
                    m_connected[2]
                    ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Float )
                    : std::to_string( m_factor ));
            break;
        case EMode::ColorDodge:
            shader = std::format(
                    "ColorDodge({},{},{})",
                    m_connected[0]
                    ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Color )
                    : std::format(
                            "vec3({},{},{})",
                            std::to_string( m_color1[0] ),
                            std::to_string( m_color1[1] ),
                            std::to_string( m_color1[2] )),
                    m_connected[1]
                    ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Color )
                    : std::format(
                            "vec3({},{},{})",
                            std::to_string( m_color2[0] ),
                            std::to_string( m_color2[1] ),
                            std::to_string( m_color2[2] )),
                    m_connected[2]
                    ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Float )
                    : std::to_string( m_factor ));
            break;
        case EMode::ColorBurn:
            shader = std::format(
                    "ColorBurn({},{},{})",
                    m_connected[0]
                    ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Color )
                    : std::format(
                            "vec3({},{},{})",
                            std::to_string( m_color1[0] ),
                            std::to_string( m_color1[1] ),
                            std::to_string( m_color1[2] )),
                    m_connected[1]
                    ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Color )
                    : std::format(
                            "vec3({},{},{})",
                            std::to_string( m_color2[0] ),
                            std::to_string( m_color2[1] ),
                            std::to_string( m_color2[2] )),
                    m_connected[2]
                    ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Float )
                    : std::to_string( m_factor ));
            break;
        case EMode::Hue:
            shader = std::format(
                    "Hue({},{},{})",
                    m_connected[0]
                    ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Color )
                    : std::format(
                            "vec3({},{},{})",
                            std::to_string( m_color1[0] ),
                            std::to_string( m_color1[1] ),
                            std::to_string( m_color1[2] )),
                    m_connected[1]
                    ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Color )
                    : std::format(
                            "vec3({},{},{})",
                            std::to_string( m_color2[0] ),
                            std::to_string( m_color2[1] ),
                            std::to_string( m_color2[2] )),
                    m_connected[2]
                    ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Float )
                    : std::to_string( m_factor ));
            break;
        case EMode::Saturation:
            shader = std::format(
                    "Saturation({},{},{})",
                    m_connected[0]
                    ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Color )
                    : std::format(
                            "vec3({},{},{})",
                            std::to_string( m_color1[0] ),
                            std::to_string( m_color1[1] ),
                            std::to_string( m_color1[2] )),
                    m_connected[1]
                    ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Color )
                    : std::format(
                            "vec3({},{},{})",
                            std::to_string( m_color2[0] ),
                            std::to_string( m_color2[1] ),
                            std::to_string( m_color2[2] )),
                    m_connected[2]
                    ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Float )
                    : std::to_string( m_factor ));
            break;
        case EMode::Value:
            shader = std::format(
                    "Value({},{},{})",
                    m_connected[0]
                    ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Color )
                    : std::format(
                            "vec3({},{},{})",
                            std::to_string( m_color1[0] ),
                            std::to_string( m_color1[1] ),
                            std::to_string( m_color1[2] )),
                    m_connected[1]
                    ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Color )
                    : std::format(
                            "vec3({},{},{})",
                            std::to_string( m_color2[0] ),
                            std::to_string( m_color2[1] ),
                            std::to_string( m_color2[2] )),
                    m_connected[2]
                    ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Float )
                    : std::to_string( m_factor ));
            break;
        case EMode::ColorMix:
            shader = std::format(
                    "ColorMix({},{},{})",
                    m_connected[0]
                    ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Color )
                    : std::format(
                            "vec3({},{},{})",
                            std::to_string( m_color1[0] ),
                            std::to_string( m_color1[1] ),
                            std::to_string( m_color1[2] )),
                    m_connected[1]
                    ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Color )
                    : std::format(
                            "vec3({},{},{})",
                            std::to_string( m_color2[0] ),
                            std::to_string( m_color2[1] ),
                            std::to_string( m_color2[2] )),
                    m_connected[2]
                    ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Float )
                    : std::to_string( m_factor ));
            break;
        case EMode::SoftLight:
            shader = std::format(
                    "SoftLight({},{},{})",
                    m_connected[0]
                    ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Color )
                    : std::format(
                            "vec3({},{},{})",
                            std::to_string( m_color1[0] ),
                            std::to_string( m_color1[1] ),
                            std::to_string( m_color1[2] )),
                    m_connected[1]
                    ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Color )
                    : std::format(
                            "vec3({},{},{})",
                            std::to_string( m_color2[0] ),
                            std::to_string( m_color2[1] ),
                            std::to_string( m_color2[2] )),
                    m_connected[2]
                    ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Float )
                    : std::to_string( m_factor ));
            break;
        case EMode::LinearLight:
            shader = std::format(
                    "LinearLight({},{},{})",
                    m_connected[0]
                    ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Color )
                    : std::format(
                            "vec3({},{},{})",
                            std::to_string( m_color1[0] ),
                            std::to_string( m_color1[1] ),
                            std::to_string( m_color1[2] )),
                    m_connected[1]
                    ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Color )
                    : std::format(
                            "vec3({},{},{})",
                            std::to_string( m_color2[0] ),
                            std::to_string( m_color2[1] ),
                            std::to_string( m_color2[2] )),
                    m_connected[2]
                    ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Float )
                    : std::to_string( m_factor ));
            break;
        default:
            break;
    }
    
    return shader;
}

bool CMix::HasFreeAttachment( int endAttr ) const
{
    if ( m_attributeStartId == endAttr )
    {
        return !m_connected[0];
    }
    if ( m_attributeStartId + 1 == endAttr )
    {
        return !m_connected[1];
    }
    if ( m_attributeStartId + 2 == endAttr )
    {
        return !m_connected[2];
    }
    return false;
}

int CMix::GetId() const
{
    return m_id;
}

std::vector<int> CMix::GetInputs() const
{
    return { m_attributeStartId, m_attributeStartId + 1, m_attributeStartId + 2 };
}

const char* CMix::ToString( EMode::Enum mode )
{
    switch ( mode )
    {
        case EMode::Add:
            return "Add";
        case EMode::Subtract:
            return "Subtract";
        case EMode::Multiply:
            return "Multiply";
        case EMode::Screen:
            return "Screen";
        case EMode::Divide:
            return "Divide";
        case EMode::Difference:
            return "Difference";
        case EMode::Darken:
            return "Darken";
        case EMode::Lighten:
            return "Lighten";
        case EMode::Overlay:
            return "Overlay";
        case EMode::ColorDodge:
            return "ColorDodge";
        case EMode::ColorBurn:
            return "ColorBurn";
        case EMode::Hue:
            return "Hue";
        case EMode::Saturation:
            return "Saturation";
        case EMode::Value:
            return "Value";
        case EMode::ColorMix:
            return "Color Mix";
        case EMode::SoftLight:
            return "Soft Light";
        case EMode::LinearLight:
            return "Linear Light";
        default:
            return "";
    }
}
uint8_t  CMix::GetVertexFlags() const
{
    return m_vertexFlags;
}
