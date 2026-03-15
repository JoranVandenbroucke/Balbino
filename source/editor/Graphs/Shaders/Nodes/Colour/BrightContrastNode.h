//
// Created by Joran Vandenbroucke on 11/03/2023.
//

#pragma once
#include "../ShaderNode.h"
#include <set>
#include <unordered_set>

class CBrightContrastNode final : public CShaderNode
{
public:
    explicit CBrightContrastNode( int& id );
    ~CBrightContrastNode() override                              = default;
    CBrightContrastNode( const CBrightContrastNode& )            = delete;
    CBrightContrastNode( CBrightContrastNode&& )                 = delete;
    CBrightContrastNode& operator=( const CBrightContrastNode& ) = delete;
    CBrightContrastNode& operator=( CBrightContrastNode&& )      = delete;

    void Draw() noexcept override;
    [[nodiscard]] std::string GetCode() const override;
    void GetShaderInfo( SShaderInfo& shaderInfo ) const override;
};
