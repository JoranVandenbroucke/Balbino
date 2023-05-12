#pragma once
#include <set>
#include "../ShaderNode.h"

class CNormalMapNode final : public CShaderNode
{
public:
    CNormalMapNode( int& id );
    ~CNormalMapNode() override = default;
    CNormalMapNode( const CNormalMapNode& ) = delete;
    CNormalMapNode( CNormalMapNode&& ) = delete;
    CNormalMapNode& operator=( CNormalMapNode&& ) = delete;
    CNormalMapNode& operator=( const CNormalMapNode& ) = delete;

	void Draw() override;
    std::string GetCode() const override;
    void GetShaderInfo( SShaderInfo& shaderInfo ) const override;
};
