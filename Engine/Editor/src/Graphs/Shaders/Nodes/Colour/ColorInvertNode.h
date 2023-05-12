//
// Created by joran on 12/03/2023.
//

#ifndef ENGINE_COLORINVERTNODE_H
#define ENGINE_COLORINVERTNODE_H
#include <unordered_set>
#include <set>
#include "../ShaderNode.h"


class CColorInvertNode final : public CShaderNode
{
public:
    explicit CColorInvertNode(int& id);
    ~CColorInvertNode() override = default;
    CColorInvertNode( const CColorInvertNode& ) = delete;
    CColorInvertNode( CColorInvertNode&& ) = delete;
    CColorInvertNode& operator=( const CColorInvertNode& ) = delete;
    CColorInvertNode& operator=( CColorInvertNode&& ) = delete;
    
    void Draw() override;
    [[nodiscard]] std::string GetCode() const override;
    void GetShaderInfo( SShaderInfo& shaderInfo ) const override;
};


#endif //ENGINE_COLORINVERTNODE_H