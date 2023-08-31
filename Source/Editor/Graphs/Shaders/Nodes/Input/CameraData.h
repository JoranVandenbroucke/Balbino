//
// Created by Joran Vandenbroucke on 27/05/2023.
//

#pragma once
#include "../ShaderNode.h"

class CCameraData final : public CShaderNode
{
public:
    explicit CCameraData( int& id );
    ~CCameraData() override                      = default;
    CCameraData( const CCameraData& )            = delete;
    CCameraData( CCameraData&& )                 = delete;
    CCameraData& operator=( const CCameraData& ) = delete;
    CCameraData& operator=( CCameraData&& )      = delete;

    void Draw() noexcept override;
    [[nodiscard]] std::string GetCode() const override;
    void GetShaderInfo( SShaderInfo& shaderInfo ) const override;
};
