//
// Created by Joran Vandenbroucke on 09/03/2023.
//

#pragma once
#include "../ShaderNode.h"
#include <set>
#include <unordered_set>

class CMathNode final : public CShaderNode
{
public:
    explicit CMathNode( int& id );
    ~CMathNode() override                    = default;
    CMathNode( const CMathNode& )            = delete;
    CMathNode( CMathNode&& )                 = delete;
    CMathNode& operator=( const CMathNode& ) = delete;
    CMathNode& operator=( CMathNode&& )      = delete;

    void Draw() noexcept override;
    [[nodiscard]] std::string GetCode() const override;
    void GetShaderInfo( SShaderInfo& shaderInfo ) const override;

private:
    enum math_mode
    {
        add,
        subtract,
        multiply,
        divide,
        multiply_add,
        power,
        logarithm,
        square_root,
        inverse_square_root,
        absolute,
        exponent,
        minimum,
        maximum,
        less_than,
        greater_than,
        sign,
        compare,
        smooth_minimum,
        smooth_maximum,
        round,
        floor,
        ceil,
        truncate,
        fraction,
        modulo,
        wrap,
        snap,
        ping_pong,
        sine,
        cosine,
        tangent,
        arcsine,
        arccosine,
        arctangent,
        arctan2,
        hyperbolic_sine,
        hyperbolic_cosine,
        hyperbolic_tangent,
        to_radians,
        to_degrees,
        max
    };
    math_mode m_type { math_mode::add };
    int m_inputSize { 2 };
    std::vector<std::string> m_allModeNames;

    std::unordered_map<CMathNode::math_mode, std::string_view> m_operationFunctionNames { { CMathNode::add, "math_add(a,b,d);" },
                                                                                          { CMathNode::subtract, "math_subtract(a,b,d);" },
                                                                                          { CMathNode::multiply, "math_multiply(a,b,d);" },
                                                                                          { CMathNode::divide, "math_divide(a,b,d);" },
                                                                                          { CMathNode::multiply_add, "math_multiply_add(a,b,c,d);" },
                                                                                          { CMathNode::power, "math_power(a,b,d);" },
                                                                                          { CMathNode::logarithm, "math_logarithm(a,b,d);" },
                                                                                          { CMathNode::square_root, "math_square_root(a,d);" },
                                                                                          { CMathNode::inverse_square_root, "math_inverse_square_root(a,d);" },
                                                                                          { CMathNode::absolute, "math_absolute(a,d);" },
                                                                                          { CMathNode::exponent, "math_exponent(a,d);" },
                                                                                          { CMathNode::minimum, "math_minimum(a,b,d);" },
                                                                                          { CMathNode::maximum, "math_maximum(a,b,d);" },
                                                                                          { CMathNode::less_than, "math_less_than(a,b,d);" },
                                                                                          { CMathNode::greater_than, "math_greater_than(a,b,d);" },
                                                                                          { CMathNode::sign, "math_sign(a,d);" },
                                                                                          { CMathNode::compare, "math_compare(a,b,c,d);" },
                                                                                          { CMathNode::smooth_minimum, "math_smooth_minimum(a,b,c,d);" },
                                                                                          { CMathNode::smooth_maximum, "math_smooth_maximum(a,b,c,d);" },
                                                                                          { CMathNode::round, "math_round(a,d);" },
                                                                                          { CMathNode::floor, "math_floor(a,d);" },
                                                                                          { CMathNode::ceil, "math_ceil(a,d);" },
                                                                                          { CMathNode::truncate, "math_truncate(a,d);" },
                                                                                          { CMathNode::fraction, "math_fraction(a,d);" },
                                                                                          { CMathNode::modulo, "math_modulo(a,b,d);" },
                                                                                          { CMathNode::wrap, "math_wrap(a,b,d);" },
                                                                                          { CMathNode::snap, "math_snap(a,b,d);" },
                                                                                          { CMathNode::ping_pong, "math_ping_pong(a,b,d);" },
                                                                                          { CMathNode::sine, "math_sine(a,d);" },
                                                                                          { CMathNode::cosine, "math_cosine(a,d);" },
                                                                                          { CMathNode::tangent, "math_tangent(a,d);" },
                                                                                          { CMathNode::arcsine, "math_arcsine(a,d);" },
                                                                                          { CMathNode::arccosine, "math_arccosine(a,d);" },
                                                                                          { CMathNode::arctangent, "math_arctangent(a,d);" },
                                                                                          { CMathNode::arctan2, "math_arctan2(a,b,d);" },
                                                                                          { CMathNode::hyperbolic_sine, "math_hyperbolic_sine(a,d);" },
                                                                                          { CMathNode::hyperbolic_cosine, "math_hyperbolic_cosine(a,d);" },
                                                                                          { CMathNode::hyperbolic_tangent, "math_hyperbolic_tangent(a,d);" },
                                                                                          { CMathNode::to_radians, "math_to_radians(a,d);" },
                                                                                          { CMathNode::to_degrees, "math_to_degrees(a,d);" } };

    static std::unordered_map<CMathNode::math_mode, std::string_view> m_operationNames;
    static const char* ToString( CMathNode::math_mode type );
};
