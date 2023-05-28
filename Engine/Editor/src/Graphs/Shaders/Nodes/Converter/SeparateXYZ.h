//
// Created by joran on 27/05/2023.
//

#pragma once
#include "../ShaderNode.h"

class CSeparateXYZ final : public CShaderNode
{
public:
    explicit CSeparateXYZ(int& id);
    ~CSeparateXYZ() override = default;
    CSeparateXYZ( const CSeparateXYZ& ) = delete;
    CSeparateXYZ( CSeparateXYZ&& ) = delete;
    CSeparateXYZ& operator=( const CSeparateXYZ& ) = delete;
    CSeparateXYZ& operator=( CSeparateXYZ&& ) = delete;
    
    void Draw() override;
    [[nodiscard]] std::string GetCode() const override;
    void GetShaderInfo( SShaderInfo& shaderInfo ) const override;
};

