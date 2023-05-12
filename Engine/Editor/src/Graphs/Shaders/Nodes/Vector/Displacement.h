#pragma once
#include "../ShaderNode.h"

#include <array>
#include <set>

class CDisplacementNode final : public CShaderNode
{
public:
	CDisplacementNode( int& id );
	~CDisplacementNode() override = default;
    CDisplacementNode( const CDisplacementNode& ) = delete;
    CDisplacementNode( CDisplacementNode&& ) = delete;
    CDisplacementNode& operator=( CDisplacementNode&& ) = delete;
    CDisplacementNode& operator=( const CDisplacementNode& ) = delete;
    
	void Draw() override;
    std::string GetCode() const override;
    void GetShaderInfo( SShaderInfo& shaderInfo ) const override;
};
