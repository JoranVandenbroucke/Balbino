#include "VectorMathNode.h"
#include "../Attribute.h"
#include "../../../../Renderer/EditorGui.h"

CVectorMathNode::CVectorMathNode( int& id )
        : CShaderNode{
        id, {
                SSocketType{ .type=SSocketType::var_type_vector_3, .name="v1", .uiName="Vector" },
                SSocketType{ .type=SSocketType::var_type_vector_3, .name="v2", .uiName="Vector" },
                SSocketType{ .type=SSocketType::var_type_vector_3, .name="v2", .uiName="Vector" },
                SSocketType{ .type=SSocketType::var_type_float, .name="eta", .uiName="val" }}, {
                SSocketType{ .type=SSocketType::var_type_vector_3, .name="v4", .uiName="Vector" },
                SSocketType{ .type=SSocketType::var_type_float, .name="f4", .uiName="Vector" }}}
{
    m_allModeNames.reserve( math_mode::math_mode_max );
    for ( int n = 0; n < math_mode::math_mode_max; ++n )
    {
        m_allModeNames.emplace_back( ToString( math_mode( n )));
    }
}
void CVectorMathNode::Draw()
{
    uint64_t type{ (uint64_t) m_type };
    int id{ GetId()};
    StartNode( m_allModeNames[type].c_str(), id);
    if ( FawnForge::Gui::DrawComboBox( "mode", type, m_allModeNames, {}, 100.f, true ))
    {
        m_type = (math_mode) type;
    }
    const auto& inputs{ GetInputNodes() };
    const auto& outputs{ GetOutputNodes() };
    std::array<float, 3> val1{ inputs[0]->GetValue() };
    std::array<float, 3> val2{ inputs[1]->GetValue() };
    std::array<float, 3> val3{ inputs[2]->GetValue() };
    std::array<float, 3> val4{ inputs[3]->GetValue() };
    DrawInput( inputs[0]->GetType(), inputs[0]->GetName(), val1, inputs[0]->IsConnected(), inputs[0]->HasEditorValues(), id++ );
    switch ( m_type )
    {
        case math_mode_add:
        case math_mode_subtract:
        case math_mode_multiply:
        case math_mode_divide:
        case math_mode_cross_product:
        case math_mode_project:
        case math_mode_reflect:
        case math_mode_snap:
        case math_mode_modulo:
        case math_mode_minimum:
        case math_mode_maximum:
            DrawInput( inputs[1]->GetType(), inputs[1]->GetName(), val2, inputs[1]->IsConnected(), inputs[1]->HasEditorValues(), id++ );
            DrawOutput( outputs[0]->GetType(), outputs[0]->GetName(), GetId() + 4);
            break;
        case multiply_add:
        case math_mode_face_forward:
        case math_mode_wrap:
            DrawInput( inputs[1]->GetType(), inputs[1]->GetName(), val2, inputs[1]->IsConnected(), inputs[1]->HasEditorValues(), id++ );
            DrawInput( inputs[2]->GetType(), inputs[2]->GetName(), val3, inputs[2]->IsConnected(), inputs[2]->HasEditorValues(), id++ );
            DrawOutput( outputs[0]->GetType(), outputs[0]->GetName(), GetId() + 4);
            break;
        case math_mode_refract:
            DrawInput( inputs[1]->GetType(), inputs[1]->GetName(), val2, inputs[1]->IsConnected(), inputs[1]->HasEditorValues(), id++ );
            DrawInput( inputs[3]->GetType(), inputs[3]->GetName(), val4, inputs[3]->IsConnected(), inputs[3]->HasEditorValues(), id++ );
            DrawOutput( outputs[0]->GetType(), outputs[0]->GetName(), GetId() + 4);
            break;
        case math_mode_dot_product:
        case math_mode_distance:
            DrawInput( inputs[1]->GetType(), inputs[1]->GetName(), val2, inputs[1]->IsConnected(), inputs[1]->HasEditorValues(), id++ );
        case math_mode_length:
            DrawOutput( outputs[1]->GetType(), outputs[1]->GetName(), GetId() + 5);
            break;
        case math_mode_scale:
        case math_mode_normalize:
        case math_mode_floor:
        case math_mode_ceil:
        case math_mode_fraction:
        case math_mode_absolute:
        case math_mode_sine:
        case math_mode_cosine:
        case math_mode_tangent:
            DrawOutput( outputs[0]->GetType(), outputs[0]->GetName(), GetId() + 4);
            break;
        case math_mode_max:
            break;
    }
    inputs[0]->SetValue(val1);
    inputs[1]->SetValue(val2);
    inputs[2]->SetValue(val3);
    inputs[3]->SetValue(val4);
    EndNode();
}

std::string CVectorMathNode::GetCode() const
{
    const std::string prefix{ "vector_math_" };
    switch ( m_type )
    {
        case math_mode_add:
            return prefix + "add(v1,v2,v4)";
        case math_mode_subtract:
            return prefix + "subtract(v1,v2,v4)";
        case math_mode_multiply:
            return prefix + "multiply(v1,v2,v4)";
        case math_mode_divide:
            return prefix + "divide(v1,v2,v4)";
        case multiply_add:
            return prefix + "multiply_add(v1,v2,v3,v4)";
        case math_mode_cross_product:
            return prefix + "cross_product(v1,v2,v4)";
        case math_mode_project:
            return prefix + "project(v1,v2,v4)";
        case math_mode_reflect:
            return prefix + "reflect(v1,v2,v4)";
        case math_mode_refract:
            return prefix + "refract(v1,v2,eta,v4)";
        case math_mode_face_forward:
            return prefix + "face_forward(v1,v2,v3,v4)";
        case math_mode_dot_product:
            return prefix + "dot_product(v1,v2,f4)";
        case math_mode_distance:
            return prefix + "distance(v1,v2,f4)";
        case math_mode_length:
            return prefix + "length(v1,v2,f4)";
        case math_mode_scale:
            return prefix + "scale(v1,eta,v4)";
        case math_mode_normalize:
            return prefix + "normalize(v1,v4)";
        case math_mode_wrap:
            return prefix + "wrap(v1,v2,v3,v4)";
        case math_mode_snap:
            return prefix + "snap(v1,v2,v4)";
        case math_mode_floor:
            return prefix + "floor(v1,v4)";
        case math_mode_ceil:
            return prefix + "ceil(v1,v4)";
        case math_mode_modulo:
            return prefix + "modulo(v1,v2,v4)";
        case math_mode_fraction:
            return prefix + "fraction(v1,v4)";
        case math_mode_absolute:
            return prefix + "absolute(v1,v4)";
        case math_mode_minimum:
            return prefix + "minimum(v1,v2,v4)";
        case math_mode_maximum:
            return prefix + "maximum(v1,v2,v4)";
        case math_mode_sine:
            return prefix + "sine(v1,v4)";
        case math_mode_cosine:
            return prefix + "cosine(v1,v4)";
        case math_mode_tangent:
            return prefix + "tangent(v1,v4)";
        case math_mode_max:
            break;
    }
    return "";
}
void CVectorMathNode::GetShaderInfo( SShaderInfo& shaderInfo ) const
{
    shaderInfo.AddInclude("node_vector_math.glsl");
}

const char* CVectorMathNode::ToString( math_mode type )
{
    switch ( type )
    {
        case math_mode::math_mode_add:
            return "Add";
        case math_mode::math_mode_subtract:
            return "Subtract";
        case math_mode::math_mode_multiply:
            return "Multiply";
        case math_mode::math_mode_divide:
            return "Divide";
        case math_mode::multiply_add:
            return "MultiplyAdd";
        case math_mode::math_mode_cross_product:
            return "CrossProduct";
        case math_mode::math_mode_project:
            return "Project";
        case math_mode::math_mode_reflect:
            return "Reflect";
        case math_mode::math_mode_refract:
            return "Refract";
        case math_mode::math_mode_face_forward:
            return "Face Forward";
        case math_mode::math_mode_dot_product:
            return "DotProduct";
        case math_mode::math_mode_distance:
            return "Distance";
        case math_mode::math_mode_length:
            return "Length";
        case math_mode::math_mode_scale:
            return "Scale";
        case math_mode::math_mode_normalize:
            return "Normalize";
        case math_mode::math_mode_wrap:
            return "Wrap";
        case math_mode::math_mode_snap:
            return "Snap";
        case math_mode::math_mode_floor:
            return "Floor";
        case math_mode::math_mode_ceil:
            return "Ceil";
        case math_mode::math_mode_modulo:
            return "Modulo";
        case math_mode::math_mode_fraction:
            return "Fraction";
        case math_mode::math_mode_absolute:
            return "Absolute";
        case math_mode::math_mode_minimum:
            return "Minimum";
        case math_mode::math_mode_maximum:
            return "Maximum";
        case math_mode::math_mode_sine:
            return "Sine";
        case math_mode::math_mode_cosine:
            return "Cosine";
        case math_mode::math_mode_tangent:
            return "Tangent";
        case math_mode::math_mode_max:
            break;
    }
    return "";
}
