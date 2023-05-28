//
// Created by joran on 12/03/2023.
//

#pragma once
#include <unordered_set>
#include <set>
#include "../ShaderNode.h"

class CRGBtoBWNode final : public CShaderNode
{
public:
    CRGBtoBWNode( int& id );
	~CRGBtoBWNode() override=default;
    CRGBtoBWNode( CRGBtoBWNode&& ) = default;
    CRGBtoBWNode( const CRGBtoBWNode& ) = delete;
    CRGBtoBWNode& operator=( const CRGBtoBWNode& ) = delete;
    CRGBtoBWNode& operator=( CRGBtoBWNode&& ) = delete;
    
    void Draw() override;
    [[nodiscard]] std::string GetCode() const override;
    void GetShaderInfo( SShaderInfo& shaderInfo ) const override;
};
