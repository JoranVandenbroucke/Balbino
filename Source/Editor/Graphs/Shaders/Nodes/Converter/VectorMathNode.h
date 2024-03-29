//
// Created by Joran Vandenbroucke on 12/03/2023.
//

#pragma once
#include "../ShaderNode.h"
#include <set>

class CVectorMathNode final : public CShaderNode
{
public:
    explicit CVectorMathNode( int& id );
    ~CVectorMathNode() override                          = default;
    CVectorMathNode( const CVectorMathNode& )            = delete;
    CVectorMathNode( CVectorMathNode&& )                 = delete;
    CVectorMathNode& operator=( const CVectorMathNode& ) = delete;
    CVectorMathNode& operator=( CVectorMathNode&& )      = delete;

    void Draw() noexcept override;
    [[nodiscard]] std::string GetCode() const override;
    void GetShaderInfo( SShaderInfo& shaderInfo ) const override;

private:
    enum math_mode
    {
        math_mode_add,
        math_mode_subtract,
        math_mode_multiply,
        math_mode_divide,
        multiply_add,
        math_mode_cross_product,
        math_mode_project,
        math_mode_reflect,
        math_mode_refract,
        math_mode_face_forward,
        math_mode_dot_product,
        math_mode_distance,
        math_mode_length,
        math_mode_scale,
        math_mode_normalize,
        math_mode_wrap,
        math_mode_snap,
        math_mode_floor,
        math_mode_ceil,
        math_mode_modulo,
        math_mode_fraction,
        math_mode_absolute,
        math_mode_minimum,
        math_mode_maximum,
        math_mode_sine,
        math_mode_cosine,
        math_mode_tangent,
        math_mode_max
    };
    math_mode m_type { math_mode_add };
    std::vector<std::string> m_allModeNames;

    static const char* ToString( math_mode type );
};
