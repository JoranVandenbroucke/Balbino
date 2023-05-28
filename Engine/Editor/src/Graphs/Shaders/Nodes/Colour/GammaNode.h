//
// Created by joran on 12/03/2023.
//

#pragma once
#include "../ShaderNode.h"


class CGammaNode final : public CShaderNode
{
public:
    CGammaNode(int& id);
    ~CGammaNode() override = default;
    CGammaNode( const CGammaNode& ) = delete;
    CGammaNode( CGammaNode&& ) = delete;
    CGammaNode& operator=( const CGammaNode& ) = delete;
    CGammaNode& operator=( CGammaNode&& ) = delete;
    
    void Draw() override;
    [[nodiscard]] std::string GetCode() const override;
    void GetShaderInfo( SShaderInfo& shaderInfo ) const override;
};


