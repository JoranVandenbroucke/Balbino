//
// Created by joran on 11/03/2023.
//

#pragma once
#include <unordered_set>
#include <set>
#include "../ShaderNode.h"


class CBrightContrastNode final : public CShaderNode
{
public:
    CBrightContrastNode(int& id);
    ~CBrightContrastNode() override = default;
    CBrightContrastNode( const CBrightContrastNode& ) = delete;
    CBrightContrastNode( CBrightContrastNode&& ) = delete;
    CBrightContrastNode& operator=( const CBrightContrastNode& ) = delete;
    CBrightContrastNode& operator=( CBrightContrastNode&& ) = delete;
    
    void Draw() override;
    [[nodiscard]] std::string GetCode() const override;
    void GetShaderInfo( SShaderInfo& shaderInfo ) const override;
};


