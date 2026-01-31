//
// Created by Joran Vandenbroucke on 27/05/2023.
//

#pragma once
#include "../ShaderNode.h"

class CCombineRGB final : public CShaderNode
{
public:
    explicit CCombineRGB( int& id );
    ~CCombineRGB() override                      = default;
    CCombineRGB( const CCombineRGB& )            = delete;
    CCombineRGB( CCombineRGB&& )                 = delete;
    CCombineRGB& operator=( const CCombineRGB& ) = delete;
    CCombineRGB& operator=( CCombineRGB&& )      = delete;

    void Draw() noexcept override;
    [[nodiscard]] std::string GetCode() const override;
    void GetShaderInfo( SShaderInfo& shaderInfo ) const override;
};
