//
// Created by joran on 12/03/2023.
//

#ifndef ENGINE_CLAMPNODE_H
#define ENGINE_CLAMPNODE_H
#include <set>
#include "../ShaderNode.h"

class CClampNode : public CShaderNode
{
public:
    CClampNode( int& id );
	~CClampNode() override = default;
    CClampNode( const CClampNode& ) = delete;
    CClampNode( CClampNode&& ) = delete;
    CClampNode& operator=( const CClampNode& ) = delete;
    CClampNode& operator=( CClampNode&& ) = delete;

	void Draw() override;
    [[nodiscard]] std::string GetCode() const override;
    void GetShaderInfo( SShaderInfo& shaderInfo ) const override;
};
#endif //ENGINE_CLAMPNODE_H