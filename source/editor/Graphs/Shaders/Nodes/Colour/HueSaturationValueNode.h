//
// Created by Joran Vandenbroucke on 12/03/2023.
//

#pragma once
#include "../ShaderNode.h"

class CHueSaturationValueNode final : public CShaderNode
{
public:
    explicit CHueSaturationValueNode( int& id );
    ~CHueSaturationValueNode() override                                  = default;
    CHueSaturationValueNode( const CHueSaturationValueNode& )            = delete;
    CHueSaturationValueNode( CHueSaturationValueNode&& )                 = delete;
    CHueSaturationValueNode& operator=( const CHueSaturationValueNode& ) = delete;
    CHueSaturationValueNode& operator=( CHueSaturationValueNode&& )      = delete;

    void Draw() noexcept override;
    [[nodiscard]] std::string GetCode() const override;
    void GetShaderInfo( SShaderInfo& shaderInfo ) const override;
};
