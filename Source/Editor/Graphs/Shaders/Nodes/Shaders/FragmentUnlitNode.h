//
// Created by Joran Vandenbroucke on 27/05/2023.
//

#pragma once
#include "../ShaderNode.h"

class CFragmentUnlitNode final : public CShaderNode
{
public:
    explicit CFragmentUnlitNode( int& id );
    ~CFragmentUnlitNode() override                             = default;
    CFragmentUnlitNode( const CFragmentUnlitNode& )            = delete;
    CFragmentUnlitNode( CFragmentUnlitNode&& )                 = delete;
    CFragmentUnlitNode& operator=( const CFragmentUnlitNode& ) = delete;
    CFragmentUnlitNode& operator=( CFragmentUnlitNode&& )      = delete;

    void Draw() noexcept override;
    [[nodiscard]] std::string GetCode() const override;
    void GetShaderInfo( SShaderInfo& shaderInfo ) const override;
};
