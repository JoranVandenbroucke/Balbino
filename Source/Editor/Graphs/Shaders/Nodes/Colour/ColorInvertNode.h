//
// Created by Joran Vandenbroucke on 12/03/2023.
//

#pragma once
#include "../ShaderNode.h"
#include <set>
#include <unordered_set>

class CColorInvertNode final : public CShaderNode
{
public:
    explicit CColorInvertNode( int& id );
    ~CColorInvertNode() override                           = default;
    CColorInvertNode( const CColorInvertNode& )            = delete;
    CColorInvertNode( CColorInvertNode&& )                 = delete;
    CColorInvertNode& operator=( const CColorInvertNode& ) = delete;
    CColorInvertNode& operator=( CColorInvertNode&& )      = delete;

    void Draw() noexcept override;
    [[nodiscard]] std::string GetCode() const override;
    void GetShaderInfo( SShaderInfo& shaderInfo ) const override;
};
