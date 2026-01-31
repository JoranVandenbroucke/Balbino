#pragma once
#include "../ShaderNode.h"
#include <set>

class CNormalMapNode final : public CShaderNode
{
public:
    explicit CNormalMapNode( int& id );
    ~CNormalMapNode() override                         = default;
    CNormalMapNode( const CNormalMapNode& )            = delete;
    CNormalMapNode( CNormalMapNode&& )                 = delete;
    CNormalMapNode& operator=( CNormalMapNode&& )      = delete;
    CNormalMapNode& operator=( const CNormalMapNode& ) = delete;

    void Draw() noexcept override;
    [[nodiscard]] std::string GetCode() const override;
    void GetShaderInfo( SShaderInfo& shaderInfo ) const override;
};
