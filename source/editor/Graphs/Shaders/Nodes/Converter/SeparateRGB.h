//
// Created by Joran Vandenbroucke on 27/05/2023.
//

#pragma once
#include "../ShaderNode.h"

class CSeparateRgb final : public CShaderNode
{
public:
    explicit CSeparateRgb( int& id );
    ~CSeparateRgb() override                       = default;
    CSeparateRgb( const CSeparateRgb& )            = delete;
    CSeparateRgb( CSeparateRgb&& )                 = delete;
    CSeparateRgb& operator=( const CSeparateRgb& ) = delete;
    CSeparateRgb& operator=( CSeparateRgb&& )      = delete;

    void Draw() noexcept override;
    [[nodiscard]] std::string GetCode() const override;
    void GetShaderInfo( SShaderInfo& shaderInfo ) const override;
};
