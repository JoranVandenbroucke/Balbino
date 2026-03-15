//
// Created by Joran Vandenbroucke on 06/06/2023.
//

#pragma once
#include "ShaderNode.h"

class CConverterNode : public CShaderNode
{
public:
    CConverterNode( int& id, SSocketType::var_type left, SSocketType::var_type right );
    ~CConverterNode() override                         = default;
    CConverterNode( const CConverterNode& )            = delete;
    CConverterNode( CConverterNode&& )                 = delete;
    CConverterNode& operator=( const CConverterNode& ) = delete;
    CConverterNode& operator=( CConverterNode&& )      = delete;

    [[nodiscard]] std::string GetCode() const override;
    void GetShaderInfo( SShaderInfo& shaderInfo ) const override;
    void Draw() noexcept override;
};
