//
// Created by Joran Vandenbroucke on 12/03/2023.
//

#pragma once
#include "../ShaderNode.h"
#include <set>

class CClampNode : public CShaderNode
{
public:
    explicit CClampNode( int& id );
    ~CClampNode() override                     = default;
    CClampNode( const CClampNode& )            = delete;
    CClampNode( CClampNode&& )                 = delete;
    CClampNode& operator=( const CClampNode& ) = delete;
    CClampNode& operator=( CClampNode&& )      = delete;

    void Draw() noexcept override;
    [[nodiscard]] std::string GetCode() const override;
    void GetShaderInfo( SShaderInfo& shaderInfo ) const override;
};
