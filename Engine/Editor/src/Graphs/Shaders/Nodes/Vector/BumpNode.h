#pragma once
#include <set>
#include "../ShaderNode.h"

class CBumpNode final : public CShaderNode
{
public:
    CBumpNode( int& id );
    ~CBumpNode() override = default;
    CBumpNode( const CBumpNode& ) = delete;
    CBumpNode( CBumpNode&& ) = delete;
    CBumpNode& operator=( CBumpNode&& ) = delete;
    CBumpNode& operator=( const CBumpNode& ) = delete;
    
    void Draw() override;
    std::string GetCode() const override;
    void GetShaderInfo( SShaderInfo& shaderInfo ) const override;

private:
    bool m_isInverted{};
};
