//
// Created by Joran Vandenbroucke on 09/03/2023.
//

#include "MathNode.h"
#include "../../../../Renderer/EditorGui.h"
#include "../Attribute.h"

std::unordered_map<CMathNode::math_mode, std::string_view> CMathNode::m_operationNames { { CMathNode::add, "Add" },
                                                                                         { CMathNode::subtract, "Subtract" },
                                                                                         { CMathNode::multiply, "Multiply" },
                                                                                         { CMathNode::divide, "Divide" },
                                                                                         { CMathNode::multiply_add, "Multiply Add" },
                                                                                         { CMathNode::power, "Power" },
                                                                                         { CMathNode::logarithm, "Logarithm" },
                                                                                         { CMathNode::square_root, "Square Root" },
                                                                                         { CMathNode::inverse_square_root, "Inverse Square Root" },
                                                                                         { CMathNode::absolute, "Absolute" },
                                                                                         { CMathNode::exponent, "Exponent" },
                                                                                         { CMathNode::minimum, "Minimum" },
                                                                                         { CMathNode::maximum, "Maximum" },
                                                                                         { CMathNode::less_than, "Less Than" },
                                                                                         { CMathNode::greater_than, "Greater Than" },
                                                                                         { CMathNode::sign, "Sign" },
                                                                                         { CMathNode::compare, "Compare" },
                                                                                         { CMathNode::smooth_minimum, "Smooth Minimum" },
                                                                                         { CMathNode::smooth_maximum, "Smooth Maximum" },
                                                                                         { CMathNode::round, "Round" },
                                                                                         { CMathNode::floor, "Floor" },
                                                                                         { CMathNode::ceil, "Ceil" },
                                                                                         { CMathNode::truncate, "Truncate" },
                                                                                         { CMathNode::fraction, "Fraction" },
                                                                                         { CMathNode::modulo, "Modulo" },
                                                                                         { CMathNode::wrap, "Wrap" },
                                                                                         { CMathNode::snap, "Snap" },
                                                                                         { CMathNode::ping_pong, "Ping Pong" },
                                                                                         { CMathNode::sine, "Sine" },
                                                                                         { CMathNode::cosine, "Cosine" },
                                                                                         { CMathNode::tangent, "Tangent" },
                                                                                         { CMathNode::arcsine, "Arcsine" },
                                                                                         { CMathNode::arccosine, "Arccosine" },
                                                                                         { CMathNode::arctangent, "Arctangent" },
                                                                                         { CMathNode::arctan2, "Arctan2" },
                                                                                         { CMathNode::hyperbolic_sine, "Hyperbolic Sine" },
                                                                                         { CMathNode::hyperbolic_cosine, "Hyperbolic Cosine" },
                                                                                         { CMathNode::hyperbolic_tangent, "Hyperbolic Tangent" },
                                                                                         { CMathNode::to_radians, "To Radians" },
                                                                                         { CMathNode::to_degrees, "To Degrees" } };

CMathNode::CMathNode( int& id )
    : CShaderNode { id,
                    { SSocketType { .type = SSocketType::var_type::float_type, .name = "a", .uiName = "input" }, SSocketType { .type = SSocketType::var_type::float_type, .name = "b", .uiName = "input" },
                      SSocketType { .type = SSocketType::var_type::float_type, .name = "c", .uiName = "input" } },
                    { SSocketType { .type = SSocketType::var_type::float_type, .name = "d", .uiName = "outPut" } } }
{
    m_allModeNames.reserve( math_mode::max );
    for ( int n = 0; n < math_mode::max; ++n )
    {
        m_allModeNames.emplace_back( ToString( math_mode( n ) ) );
    }
}

void CMathNode::Draw() noexcept
{
    int inputCount { 1 };
    int id { GetId() };
    uint64_t type { (uint64_t)m_type };
    const std::vector<CShaderInput*>& in { GetInputNodes() };

    StartNode( m_allModeNames[ type ].c_str(), GetId() );
    if ( FawnForge::Gui::ComboBox( "mode", type, m_allModeNames, { 10, 18, 27, 37 }, 100.f, true ) )
    {
        m_type = (math_mode)type;
        switch ( m_type )
        {
            //2
            case math_mode::add:
            case math_mode::subtract:
            case math_mode::multiply:
            case math_mode::divide:
            case math_mode::power:
            case math_mode::logarithm:
            case math_mode::exponent:
            case math_mode::minimum:
            case math_mode::maximum:
            case math_mode::less_than:
            case math_mode::greater_than:
            case math_mode::fraction:
            case math_mode::modulo:
            case math_mode::snap:
            case math_mode::ping_pong:
            case math_mode::arctan2: {
                inputCount = 2;
                break;
            }
                //3
            case math_mode::multiply_add:
            case math_mode::smooth_minimum:
            case math_mode::smooth_maximum:
            case math_mode::floor:
            case math_mode::ceil:
            case math_mode::truncate: {
                inputCount = 3;
                break;
            }
            default: break;
        }
        m_inputSize = inputCount;
    }

    for ( int i {}; i < m_inputSize; ++i )
    {
        std::array<float, 3> val { in[ i ]->GetValue() };
        DrawInput( in[ i ]->GetType(), in[ i ]->GetName(), val, in[ i ]->IsConnected(), in[ i ]->HasEditorValues(), id );
        ++id;
        in[ i ]->SetValue( val );
    }
    id = GetId() + (int)in.size();
    for ( const CShaderOutput* out : GetOutputNodes() )
    {
        DrawOutput( out->GetType(), out->GetName(), id );
        ++id;
    }
    EndNode();
}

std::string CMathNode::GetCode() const
{
    if ( m_operationFunctionNames.contains( m_type ) )
        return m_operationFunctionNames.at( m_type ).data();
    return "";
}
void CMathNode::GetShaderInfo( SShaderInfo& shaderInfo ) const
{
    shaderInfo.AddInclude( "node_math.glsl" );
}
const char* CMathNode::ToString( CMathNode::math_mode type )
{
    if ( m_operationNames.contains( type ) )
        return m_operationNames.at( type ).data();
    return nullptr;
}
