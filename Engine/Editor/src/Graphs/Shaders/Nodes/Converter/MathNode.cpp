//
// Created by joran on 09/03/2023.
//


#include "MathNode.h"
#include "../Attribute.h"
#include "../../../../Renderer/EditorGui.h"

CMathNode::CMathNode( int& id )
        : CShaderNode{
        id,
        {
                SSocketType{ .type=SSocketType::var_type_float, .name="a", .uiName="Input" },
                SSocketType{ .type=SSocketType::var_type_float, .name="b", .uiName="Input" },
                SSocketType{ .type=SSocketType::var_type_float, .name="c", .uiName="Input" }},
        { SSocketType{ .type=SSocketType::var_type_float, .name="d", .uiName="outPut" }}}
{
    m_allModeNames.reserve( MathMode::max );
    for ( int n = 0; n < MathMode::max; ++n )
    {
        m_allModeNames.emplace_back( ToString( MathMode( n )));
    }
}

void CMathNode::Draw()
{
    int      inputCount{ 1 };
    int      id{ GetId() };
    uint64_t type{ (uint64_t) m_type };
    const std::vector<CShaderInput*>& in{ GetInputNodes() };
    
    StartNode( m_allModeNames[type].c_str(), GetId());
    if ( FawnForge::Gui::DrawComboBox( "mode", type, m_allModeNames, { 10, 18, 27, 37 }, 100.f, true ))
    {
        m_type      = (MathMode) type;
        switch ( m_type )
        {
            //2
            case MathMode::add:
            case MathMode::subtract:
            case MathMode::multiply:
            case MathMode::divide:
            case MathMode::power:
            case MathMode::logarithm:
            case MathMode::exponent:
            case MathMode::minimum:
            case MathMode::maximum:
            case MathMode::less_than:
            case MathMode::greater_than:
            case MathMode::fraction:
            case MathMode::modulo:
            case MathMode::snap:
            case MathMode::ping_pong:
            case MathMode::arctan2:
            {
                inputCount = 2;
                break;
            }
                //3
            case MathMode::multiply_add:
            case MathMode::smooth_minimum:
            case MathMode::smooth_maximum:
            case MathMode::floor:
            case MathMode::ceil:
            case MathMode::truncate:
            {
                inputCount = 3;
                break;
            }
            default:
                break;
        }
        m_inputSize = inputCount;
    }
    
    for ( int i{}; i < m_inputSize; ++i )
    {
        std::array<float, 3> val{ in[i]->GetValue() };
        DrawInput( in[i]->GetType(), in[i]->GetName(), val, in[i]->IsConnected(), in[i]->HasEditorValues(), id++ );
        in[i]->SetValue( val );
    }
    id = GetId() + (int) in.size();
    for ( const CShaderOutput* out : GetOutputNodes())
    {
        DrawOutput( out->GetType(), out->GetName(), id++ );
    }
    EndNode();
}

std::string CMathNode::GetCode() const
{
    switch ( m_type )
    {
        case MathMode::add:
            return "math_add(a,b,d);";
        case MathMode::subtract:
            return "math_subtract(a,b,d);";
        case MathMode::multiply:
            return "math_multiply(a,b,d);";
        case MathMode::divide:
            return "math_divide(a,b,d);";
        case MathMode::multiply_add:
            return "math_multiply_add(a,b,c,d);";
        case MathMode::power:
            return "math_power(a,b,d);";
        case MathMode::logarithm:
            return "math_logarithm(a,b,d);";
        case MathMode::square_root:
            return "math_square_root(a,d);";
        case MathMode::inverse_square_root:
            return "math_inverse_square_root(a,d);";
        case MathMode::absolute:
            return "math_absolute(a,d);";
        case MathMode::exponent:
            return "math_exponent(a,d);";
        case MathMode::minimum:
            return "math_minimum(a,b,d);";
        case MathMode::maximum:
            return "math_maximum(a,b,d);";
        case MathMode::less_than:
            return "math_less_than(a,b,d);";
        case MathMode::greater_than:
            return "math_greater_than(a,b,d);";
        case MathMode::sign:
            return "math_sign(a,d);";
        case MathMode::compare:
            return "math_compare(a,b,c,d);";
        case MathMode::smooth_minimum:
            return "math_smooth_minimum(a,b,c,d);";
        case MathMode::smooth_maximum:
            return "math_smooth_maximum(a,b,c,d);";
        case MathMode::round:
            return "math_round(a,d);";
        case MathMode::floor:
            return "math_floor(a,d);";
        case MathMode::ceil:
            return "math_ceil(a,d);";
        case MathMode::truncate:
            return "math_truncate(a,d);";
        case MathMode::fraction:
            return "math_fraction(a,d);";
        case MathMode::modulo:
            return "math_modulo(a,b,d);";
        case MathMode::wrap:
            return "math_wrap(a,b,d);";
        case MathMode::snap:
            return "math_snap(a,b,d);";
        case MathMode::ping_pong:
            return "math_ping_pong(a,b,d);";
        case MathMode::sine:
            return "math_sine(a,d);";
        case MathMode::cosine:
            return "math_cosine(a,d);";
        case MathMode::tangent:
            return "math_tangent(a,d);";
        case MathMode::arcsine:
            return "math_arcsine(a,d);";
        case MathMode::arccosine:
            return "math_arccosine(a,d);";
        case MathMode::arctangent:
            return "math_arctangent(a,d);";
        case MathMode::arctan2:
            return "math_arctan2(a,b,d);";
        case MathMode::hyperbolic_sine:
            return "math_hyperbolic_sine(a,d);";
        case MathMode::hyperbolic_cosine:
            return "math_hyperbolic_cosine(a,d);";
        case MathMode::hyperbolic_tangent:
            return "math_hyperbolic_tangent(a,d);";
        case MathMode::to_radians:
            return "math_to_radians(a,d);";
        case MathMode::to_degrees:
            return "math_to_degrees(a,d);";
        default:
            break;
    }
    return "";
}
void CMathNode::GetShaderInfo( SShaderInfo& shaderInfo ) const
{
    shaderInfo.AddInclude("node_math.glsl");
}
const char* CMathNode::ToString( CMathNode::MathMode type )
{
    switch ( type )
    {
        case MathMode::add:
            return "Add";
        case MathMode::subtract:
            return "Subtract";
        case MathMode::multiply:
            return "Multiply";
        case MathMode::divide:
            return "Divide";
        case MathMode::multiply_add:
            return "Multiply Add";
        case MathMode::power:
            return "Power";
        case MathMode::logarithm:
            return "Logarithm";
        case MathMode::square_root:
            return "Square Root";
        case MathMode::inverse_square_root:
            return "Inverse Square Root";
        case MathMode::absolute:
            return "Absolute";
        case MathMode::exponent:
            return "Exponent";
        case MathMode::minimum:
            return "Minimum";
        case MathMode::maximum:
            return "Maximum";
        case MathMode::less_than:
            return "Less Than";
        case MathMode::greater_than:
            return "Greater Than";
        case MathMode::sign:
            return "Sign";
        case MathMode::compare:
            return "Compare";
        case MathMode::smooth_minimum:
            return "Smooth Minimum";
        case MathMode::smooth_maximum:
            return "Smooth Maximum";
        case MathMode::round:
            return "Round";
        case MathMode::floor:
            return "Floor";
        case MathMode::ceil:
            return "Ceil";
        case MathMode::truncate:
            return "Truncate";
        case MathMode::fraction:
            return "Fraction";
        case MathMode::modulo:
            return "Modulo";
        case MathMode::wrap:
            return "Wrap";
        case MathMode::snap:
            return "Snap";
        case MathMode::ping_pong:
            return "Ping Pong";
        case MathMode::sine:
            return "Sine";
        case MathMode::cosine:
            return "Cosine";
        case MathMode::tangent:
            return "Tangent";
        case MathMode::arcsine:
            return "Arcsine";
        case MathMode::arccosine:
            return "Arccosine";
        case MathMode::arctangent:
            return "Arctangent";
        case MathMode::arctan2:
            return "Arctan2";
        case MathMode::hyperbolic_sine:
            return "Hyperbolic Sine";
        case MathMode::hyperbolic_cosine:
            return "Hyperbolic Cosine";
        case MathMode::hyperbolic_tangent:
            return "Hyperbolic Tangent";
        case MathMode::to_radians:
            return "To Radians";
        case MathMode::to_degrees:
            return "To Degrees";
        default:
            return nullptr;
    }
}
