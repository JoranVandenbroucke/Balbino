//
// Created by joran on 27/05/2023.
//

#pragma once
#include "../ShaderNode.h"


class CCombineXYZ final : public CShaderNode
{
public:
    explicit CCombineXYZ(int& id);
    ~CCombineXYZ() override = default;
    CCombineXYZ( const CCombineXYZ& ) = delete;
    CCombineXYZ( CCombineXYZ&& ) = delete;
    CCombineXYZ& operator=( const CCombineXYZ& ) = delete;
    CCombineXYZ& operator=( CCombineXYZ&& ) = delete;
    
    void Draw() override;
    [[nodiscard]] std::string GetCode() const override;
    void GetShaderInfo( SShaderInfo& shaderInfo ) const override;
};
