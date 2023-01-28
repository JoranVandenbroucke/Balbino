#include "VectorMath.h"

#include <string>
#include <format>

#include "../../../EditorGUI/EditorGui.h"
#include "../Attribute.h"

CVectorMath::CVectorMath( const int id, int& attributeStartId )
        : INode{ id, attributeStartId }
        , m_type{}
        , m_connected{}
        , m_vectorA{}
        , m_vectorB{}
        , m_vectorC{}
        , m_fac{ 0 }
{
    attributeStartId += 4;
}

CVectorMath::~CVectorMath() = default;

void CVectorMath::Draw()
{
    uint64_t                 current{ (uint64_t) m_type };
    std::vector<std::string> options;
    for ( int                n = 0; n < (int) EMode::MaxIndex; n++ )
    {
        options.emplace_back( ToString( EMode( n )));
    }
    ImNodes::BeginNode( m_id );
    
    ImNodes::BeginNodeTitleBar();
    FawnForge::GUI::DrawText( ToString( m_type ));
    ImNodes::EndNodeTitleBar();
    FawnForge::GUI::DrawComboBox( "Mode", current, options, { 10, 18, 27, 37 } );
    
    DrawInputVectorAttribute( m_vectorA, m_attributeStartId, m_connected[0] );
    m_type = (EMode)current;
    switch ( m_type )
    {
        case EMode::Add:
        case EMode::Subtract:
        case EMode::Multiply:
        case EMode::Divide:
        case EMode::CrossProduct:
        case EMode::Project:
        case EMode::Reflect:
        case EMode::DotProduct:
        case EMode::Distance:
        case EMode::Snap:
        case EMode::Modulo:
        case EMode::Minimum:
        case EMode::Maximum:
            DrawInputVectorAttribute( m_vectorB, m_attributeStartId + 1, m_connected[1] );
            break;
        case EMode::MultiplyAdd:
        case EMode::Faceforward:
        case EMode::Wrap:
            DrawInputVectorAttribute( m_vectorB, m_attributeStartId + 1, m_connected[1] );
            DrawInputVectorAttribute( m_vectorC, m_attributeStartId + 2, m_connected[2] );
            break;
        case EMode::Scale:
            DrawInputFloatAttribute( m_fac, m_attributeStartId + 1, m_connected[2] );
            break;
        case EMode::Refract:
            DrawInputVectorAttribute( m_vectorB, m_attributeStartId + 1, m_connected[1] );
            DrawInputFloatAttribute( m_fac, m_attributeStartId + 2, m_connected[2] );
            break;
        default:;
    }
    
    FawnForge::GUI::Spacing();
    DrawOutputVectorAttribute( m_attributeStartId + 3 );
    
    ImNodes::EndNode();
}

void CVectorMath::Attach( int endAttr )
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

void CVectorMath::Detach( int endAttr )
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

std::string CVectorMath::Evaluate( std::vector<INode*>::iterator& begin, std::set<std::string>& bindings, std::set<std::string>& includes, EAttributeType::Enum attributeType )
{
    (void) begin;
    (void) bindings;
    (void) attributeType;
    includes.insert( "vectorMath.glsl" );
    std::string shader{};
    switch ( m_type )
    {
        case EMode::Add:
        {
            shader = std::format(
                    "Add({},{})",
                    m_connected[0]
                    ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Vector )
                    : ( "vec3( " + std::to_string( m_vectorA[0] ) + ", " + std::to_string( m_vectorA[1] ) + ", " + std::to_string(
                            m_vectorA[2]
                    ) + " )" ),
                    m_connected[1]
                    ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Vector )
                    : ( "vec3( " + std::to_string( m_vectorB[0] ) + ", " + std::to_string( m_vectorB[1] ) + ", " + std::to_string(
                            m_vectorB[2]
                    ) + " )" ));
            break;
        }
        case EMode::Subtract:
        {
            shader = std::format(
                    "Subtract({},{})",
                    m_connected[0]
                    ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Vector )
                    : ( "vec3( " + std::to_string( m_vectorA[0] ) + ", " + std::to_string( m_vectorA[1] ) + ", " + std::to_string(
                            m_vectorA[2]
                    ) + " )" ),
                    m_connected[1]
                    ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Vector )
                    : ( "vec3( " + std::to_string( m_vectorB[0] ) + ", " + std::to_string( m_vectorB[1] ) + ", " + std::to_string(
                            m_vectorB[2]
                    ) + " )" ));
            break;
        }
        case EMode::Multiply:
        {
            shader = std::format(
                    "Multiply({},{})",
                    m_connected[0]
                    ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Vector )
                    : ( "vec3( " + std::to_string( m_vectorA[0] ) + ", " + std::to_string( m_vectorA[1] ) + ", " + std::to_string(
                            m_vectorA[2]
                    ) + " )" ),
                    m_connected[1]
                    ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Vector )
                    : ( "vec3( " + std::to_string( m_vectorB[0] ) + ", " + std::to_string( m_vectorB[1] ) + ", " + std::to_string(
                            m_vectorB[2]
                    ) + " )" ));
            break;
        }
        case EMode::Divide:
        {
            shader = std::format(
                    "Divide({},{})",
                    m_connected[0]
                    ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Vector )
                    : ( "vec3( " + std::to_string( m_vectorA[0] ) + ", " + std::to_string( m_vectorA[1] ) + ", " + std::to_string(
                            m_vectorA[2]
                    ) + " )" ),
                    m_connected[1]
                    ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Vector )
                    : ( "vec3( " + std::to_string( m_vectorB[0] ) + ", " + std::to_string( m_vectorB[1] ) + ", " + std::to_string(
                            m_vectorB[2]
                    ) + " )" ));
            break;
        }
        case EMode::MultiplyAdd:
        {
            shader = std::format(
                    "MultiplyAdd({},{},{})",
                    m_connected[0]
                    ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Vector )
                    : ( "vec3( " + std::to_string( m_vectorA[0] ) + ", " + std::to_string( m_vectorA[1] ) + ", " + std::to_string(
                            m_vectorA[2]
                    ) + " )" ),
                    m_connected[1]
                    ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Vector )
                    : ( "vec3( " + std::to_string( m_vectorB[0] ) + ", " + std::to_string( m_vectorB[1] ) + ", " + std::to_string(
                            m_vectorB[2]
                    ) + " )" ),
                    m_connected[2]
                    ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Vector )
                    : ( "vec3( " + std::to_string( m_vectorC[0] ) + ", " + std::to_string( m_vectorC[1] ) + ", " + std::to_string(
                            m_vectorC[2]
                    ) + " )" ));
            break;
        }
        case EMode::CrossProduct:
        {
            shader = std::format(
                    "CrossProduct({},{})",
                    m_connected[0]
                    ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Vector )
                    : ( "vec3( " + std::to_string( m_vectorA[0] ) + ", " + std::to_string( m_vectorA[1] ) + ", " + std::to_string(
                            m_vectorA[2]
                    ) + " )" ),
                    m_connected[1]
                    ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Vector )
                    : ( "vec3( " + std::to_string( m_vectorB[0] ) + ", " + std::to_string( m_vectorB[1] ) + ", " + std::to_string(
                            m_vectorB[2]
                    ) + " )" ));
            break;
        }
        case EMode::Project:
        {
            shader = std::format(
                    "Project({},{})",
                    m_connected[0]
                    ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Vector )
                    : ( "vec3( " + std::to_string( m_vectorA[0] ) + ", " + std::to_string( m_vectorA[1] ) + ", " + std::to_string(
                            m_vectorA[2]
                    ) + " )" ),
                    m_connected[1]
                    ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Vector )
                    : ( "vec3( " + std::to_string( m_vectorB[0] ) + ", " + std::to_string( m_vectorB[1] ) + ", " + std::to_string(
                            m_vectorB[2]
                    ) + " )" ));
            break;
        }
        case EMode::Reflect:
        {
            shader = std::format(
                    "Reflect({},{})",
                    m_connected[0]
                    ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Vector )
                    : ( "vec3( " + std::to_string( m_vectorA[0] ) + ", " + std::to_string( m_vectorA[1] ) + ", " + std::to_string(
                            m_vectorA[2]
                    ) + " )" ),
                    m_connected[1]
                    ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Vector )
                    : ( "vec3( " + std::to_string( m_vectorB[0] ) + ", " + std::to_string( m_vectorB[1] ) + ", " + std::to_string(
                            m_vectorB[2]
                    ) + " )" ));
            break;
        }
        case EMode::Refract:
        {
            shader = std::format(
                    "Refract({},{})",
                    m_connected[0]
                    ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Vector )
                    : ( "vec3( " + std::to_string( m_vectorA[0] ) + ", " + std::to_string( m_vectorA[1] ) + ", " + std::to_string(
                            m_vectorA[2]
                    ) + " )" ),
                    m_connected[1]
                    ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Vector )
                    : ( "vec3( " + std::to_string( m_vectorB[0] ) + ", " + std::to_string( m_vectorB[1] ) + ", " + std::to_string(
                            m_vectorB[2]
                    ) + " )" ));
            break;
        }
        case EMode::Faceforward:
        {
            shader = std::format(
                    "Faceforward({},{},{})",
                    m_connected[0]
                    ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Vector )
                    : ( "vec3( " + std::to_string( m_vectorA[0] ) + ", " + std::to_string( m_vectorA[1] ) + ", " + std::to_string(
                            m_vectorA[2]
                    ) + " )" ),
                    m_connected[1]
                    ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Vector )
                    : ( "vec3( " + std::to_string( m_vectorB[0] ) + ", " + std::to_string( m_vectorB[1] ) + ", " + std::to_string(
                            m_vectorB[2]
                    ) + " )" ),
                    m_connected[2]
                    ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Vector )
                    : ( "vec3( " + std::to_string( m_vectorC[0] ) + ", " + std::to_string( m_vectorC[1] ) + ", " + std::to_string(
                            m_vectorC[2]
                    ) + " )" ));
            break;
        }
        case EMode::DotProduct:
        {
            shader = std::format(
                    "EMode({},{})",
                    m_connected[0]
                    ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Vector )
                    : ( "vec3( " + std::to_string( m_vectorA[0] ) + ", " + std::to_string( m_vectorA[1] ) + ", " + std::to_string(
                            m_vectorA[2]
                    ) + " )" ),
                    m_connected[1]
                    ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Vector )
                    : ( "vec3( " + std::to_string( m_vectorB[0] ) + ", " + std::to_string( m_vectorB[1] ) + ", " + std::to_string(
                            m_vectorB[2]
                    ) + " )" ));
            break;
        }
        case EMode::Distance:
        {
            shader = std::format(
                    "Distance({},{})",
                    m_connected[0]
                    ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Vector )
                    : ( "vec3( " + std::to_string( m_vectorA[0] ) + ", " + std::to_string( m_vectorA[1] ) + ", " + std::to_string(
                            m_vectorA[2]
                    ) + " )" ),
                    m_connected[1]
                    ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Vector )
                    : ( "vec3( " + std::to_string( m_vectorB[0] ) + ", " + std::to_string( m_vectorB[1] ) + ", " + std::to_string(
                            m_vectorB[2]
                    ) + " )" ));
            break;
        }
        case EMode::Length:
        {
            shader = std::format(
                    "Length({})",
                    m_connected[0]
                    ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Vector )
                    : ( "vec3( " + std::to_string( m_vectorA[0] ) + ", " + std::to_string( m_vectorA[1] ) + ", " + std::to_string(
                            m_vectorA[2]
                    ) + " )" ));
            break;
        }
        case EMode::Scale:
        {
            shader = std::format(
                    "Scale({},{})",
                    m_connected[0]
                    ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Vector )
                    : ( "vec3( " + std::to_string( m_vectorA[0] ) + ", " + std::to_string( m_vectorA[1] ) + ", " + std::to_string(
                            m_vectorA[2]
                    ) + " )" ),
                    m_connected[1]
                    ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Float )
                    : std::to_string( m_fac ));
            break;
        }
        case EMode::Normalize:
        {
            shader = std::format(
                    "Normalize({})",
                    m_connected[0]
                    ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Vector )
                    : ( "vec3( " + std::to_string( m_vectorA[0] ) + ", " + std::to_string( m_vectorA[1] ) + ", " + std::to_string(
                            m_vectorA[2]
                    ) + " )" ));
            break;
        }
        case EMode::Wrap:
        {
            shader = std::format(
                    "Wrap({},{},{})",
                    m_connected[0]
                    ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Vector )
                    : ( "vec3( " + std::to_string( m_vectorA[0] ) + ", " + std::to_string( m_vectorA[1] ) + ", " + std::to_string(
                            m_vectorA[2]
                    ) + " )" ),
                    m_connected[1]
                    ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Vector )
                    : ( "vec3( " + std::to_string( m_vectorB[0] ) + ", " + std::to_string( m_vectorB[1] ) + ", " + std::to_string(
                            m_vectorB[2]
                    ) + " )" ),
                    m_connected[2]
                    ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Vector )
                    : ( "vec3( " + std::to_string( m_vectorC[0] ) + ", " + std::to_string( m_vectorC[1] ) + ", " + std::to_string(
                            m_vectorC[2]
                    ) + " )" ));
            break;
        }
        case EMode::Snap:
        {
            shader = std::format(
                    "Snap({},{})",
                    m_connected[0]
                    ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Vector )
                    : ( "vec3( " + std::to_string( m_vectorA[0] ) + ", " + std::to_string( m_vectorA[1] ) + ", " + std::to_string(
                            m_vectorA[2]
                    ) + " )" ),
                    m_connected[1]
                    ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Vector )
                    : ( "vec3( " + std::to_string( m_vectorB[0] ) + ", " + std::to_string( m_vectorB[1] ) + ", " + std::to_string(
                            m_vectorB[2]
                    ) + " )" ));
            break;
        }
        case EMode::Floor:
        {
            shader = std::format(
                    "Floor({})",
                    m_connected[0]
                    ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Vector )
                    : ( "vec3( " + std::to_string( m_vectorA[0] ) + ", " + std::to_string( m_vectorA[1] ) + ", " + std::to_string(
                            m_vectorA[2]
                    ) + " )" ));
            break;
        }
        case EMode::Ceil:
        {
            shader = std::format(
                    "Ceil({})",
                    m_connected[0]
                    ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Vector )
                    : ( "vec3( " + std::to_string( m_vectorA[0] ) + ", " + std::to_string( m_vectorA[1] ) + ", " + std::to_string(
                            m_vectorA[2]
                    ) + " )" ));
            break;
        }
        case EMode::Modulo:
        {
            shader = std::format(
                    "Modulo({},{})",
                    m_connected[0]
                    ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Vector )
                    : ( "vec3( " + std::to_string( m_vectorA[0] ) + ", " + std::to_string( m_vectorA[1] ) + ", " + std::to_string(
                            m_vectorA[2]
                    ) + " )" ),
                    m_connected[1]
                    ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Vector )
                    : ( "vec3( " + std::to_string( m_vectorB[0] ) + ", " + std::to_string( m_vectorB[1] ) + ", " + std::to_string(
                            m_vectorB[2]
                    ) + " )" ));
            break;
        }
        case EMode::Fraction:
        {
            shader = std::format(
                    "Fraction({})",
                    m_connected[0]
                    ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Vector )
                    : ( "vec3( " + std::to_string( m_vectorA[0] ) + ", " + std::to_string( m_vectorA[1] ) + ", " + std::to_string(
                            m_vectorA[2]
                    ) + " )" ));
            break;
        }
        case EMode::Absolute:
        {
            shader = std::format(
                    "Absolute({})",
                    m_connected[0]
                    ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Vector )
                    : ( "vec3( " + std::to_string( m_vectorA[0] ) + ", " + std::to_string( m_vectorA[1] ) + ", " + std::to_string(
                            m_vectorA[2]
                    ) + " )" ));
            break;
        }
        case EMode::Minimum:
        {
            shader = std::format(
                    "Minimum({},{})",
                    m_connected[0]
                    ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Vector )
                    : ( "vec3( " + std::to_string( m_vectorA[0] ) + ", " + std::to_string( m_vectorA[1] ) + ", " + std::to_string(
                            m_vectorA[2]
                    ) + " )" ),
                    m_connected[1]
                    ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Vector )
                    : ( "vec3( " + std::to_string( m_vectorB[0] ) + ", " + std::to_string( m_vectorB[1] ) + ", " + std::to_string(
                            m_vectorB[2]
                    ) + " )" ));
            break;
        }
        case EMode::Maximum:
        {
            shader = std::format(
                    "Maximum({},{})",
                    m_connected[0]
                    ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Vector )
                    : ( "vec3( " + std::to_string( m_vectorA[0] ) + ", " + std::to_string( m_vectorA[1] ) + ", " + std::to_string(
                            m_vectorA[2]
                    ) + " )" ),
                    m_connected[1]
                    ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Vector )
                    : ( "vec3( " + std::to_string( m_vectorB[0] ) + ", " + std::to_string( m_vectorB[1] ) + ", " + std::to_string(
                            m_vectorB[2]
                    ) + " )" ));
            break;
        }
        case EMode::Sine:
        {
            shader = std::format(
                    "Sine({})",
                    m_connected[0]
                    ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Vector )
                    : ( "vec3( " + std::to_string( m_vectorA[0] ) + ", " + std::to_string( m_vectorA[1] ) + ", " + std::to_string(
                            m_vectorA[2]
                    ) + " )" ));
            break;
        }
        case EMode::Cosine:
        {
            shader = std::format(
                    "Cosine({})",
                    m_connected[0]
                    ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Vector )
                    : ( "vec3( " + std::to_string( m_vectorA[0] ) + ", " + std::to_string( m_vectorA[1] ) + ", " + std::to_string(
                            m_vectorA[2]
                    ) + " )" ));
            break;
        }
        case EMode::Tangent:
        {
            shader = std::format(
                    "Tangent({})",
                    m_connected[0]
                    ? ( *( begin++ ))->Evaluate( begin, bindings, includes, EAttributeType::Vector )
                    : ( "vec3( " + std::to_string( m_vectorA[0] ) + ", " + std::to_string( m_vectorA[1] ) + ", " + std::to_string(
                            m_vectorA[2]
                    ) + " )" ));
            break;
        }
        case EMode::MaxIndex:
            break;
    }
    if ( attributeType == EAttributeType::Float )
    {
        shader += ".x";
    }
    return shader;
}

bool CVectorMath::HasFreeAttachment( int endAttr ) const
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

int CVectorMath::GetId() const
{
    return m_id;
}

std::vector<int> CVectorMath::GetInputs() const
{
    return { m_attributeStartId, m_attributeStartId + 1, m_attributeStartId + 2 };
}

const char* CVectorMath::ToString( EMode type )
{
    switch ( type )
    {
        case EMode::Add:
            return "Add";
        case EMode::Subtract:
            return "Subtract";
        case EMode::Multiply:
            return "Multiply";
        case EMode::Divide:
            return "Divide";
        case EMode::MultiplyAdd:
            return "MultiplyAdd";
        case EMode::CrossProduct:
            return "CrossProduct";
        case EMode::Project:
            return "Project";
        case EMode::Reflect:
            return "Reflect";
        case EMode::Refract:
            return "Refract";
        case EMode::Faceforward:
            return "Face Forward";
        case EMode::DotProduct:
            return "DotProduct";
        case EMode::Distance:
            return "Distance";
        case EMode::Length:
            return "Length";
        case EMode::Scale:
            return "Scale";
        case EMode::Normalize:
            return "Normalize";
        case EMode::Wrap:
            return "Wrap";
        case EMode::Snap:
            return "Snap";
        case EMode::Floor:
            return "Floor";
        case EMode::Ceil:
            return "Ceil";
        case EMode::Modulo:
            return "Modulo";
        case EMode::Fraction:
            return "Fraction";
        case EMode::Absolute:
            return "Absolute";
        case EMode::Minimum:
            return "Minimum";
        case EMode::Maximum:
            return "Maximum";
        case EMode::Sine:
            return "Sine";
        case EMode::Cosine:
            return "Cosine";
        case EMode::Tangent:
            return "Tangent";
        case EMode::MaxIndex:
            break;
    }
    return "";
}
uint8_t CVectorMath::GetVertexFlags() const
{
    return m_vertexFlags;
}
