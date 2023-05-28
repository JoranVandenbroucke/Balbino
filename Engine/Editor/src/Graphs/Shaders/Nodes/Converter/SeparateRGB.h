//
// Created by joran on 27/05/2023.
//

#pragma once
#include "../ShaderNode.h"


class CSeparateRGB final : public CShaderNode
{
public:
    explicit CSeparateRGB(int& id);
    ~CSeparateRGB() override = default;
    CSeparateRGB( const CSeparateRGB& ) = delete;
    CSeparateRGB( CSeparateRGB&& ) = delete;
    CSeparateRGB& operator=( const CSeparateRGB& ) = delete;
    CSeparateRGB& operator=( CSeparateRGB&& ) = delete;
    
    void Draw() override;
    [[nodiscard]] std::string GetCode() const override;
    void GetShaderInfo( SShaderInfo& shaderInfo ) const override;
};
