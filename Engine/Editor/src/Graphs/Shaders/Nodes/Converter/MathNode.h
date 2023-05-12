//
// Created by joran on 09/03/2023.
//

#ifndef ENGINE_MATHNODE_H
#define ENGINE_MATHNODE_H
#include <unordered_set>
#include <set>
#include "../ShaderNode.h"


class CMathNode final : public CShaderNode
{
public:
    explicit CMathNode( int& id );
    ~CMathNode() override = default;
    CMathNode( const CMathNode& ) = delete;
    CMathNode( CMathNode&& ) = delete;
    CMathNode& operator=( const CMathNode& ) = delete;
    CMathNode& operator=( CMathNode&& ) = delete;
    
    void Draw() override;
    [[nodiscard]] std::string GetCode() const override;
    void GetShaderInfo( SShaderInfo& shaderInfo ) const override;

private:
    enum MathMode
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
    MathMode m_type{ MathMode::add };
    int      m_inputSize{ 2 };
    std::vector<std::string> m_allModeNames;
    
    const char* ToString( CMathNode::MathMode type );
};


#endif //ENGINE_MATHNODE_H