//
// Created by Joran Vandenbroucke on 27/05/2023.
//

#pragma once
#include "../ShaderNode.h"

class CSeparateXyz final : public CShaderNode
{
public:
    explicit CSeparateXyz( int& id );
    ~CSeparateXyz() override                       = default;
    CSeparateXyz( const CSeparateXyz& )            = delete;
    CSeparateXyz( CSeparateXyz&& )                 = delete;
    CSeparateXyz& operator=( const CSeparateXyz& ) = delete;
    CSeparateXyz& operator=( CSeparateXyz&& )      = delete;

    void Draw() noexcept override;
    [[nodiscard]] std::string GetCode() const override;
    void GetShaderInfo( SShaderInfo& shaderInfo ) const override;
};
