#pragma once
#include "../ShaderNode.h"
#include <set>

class CBumpNode final : public CShaderNode
{
public:
    explicit CBumpNode( int& id );
    ~CBumpNode() override                    = default;
    CBumpNode( const CBumpNode& )            = delete;
    CBumpNode( CBumpNode&& )                 = delete;
    CBumpNode& operator=( CBumpNode&& )      = delete;
    CBumpNode& operator=( const CBumpNode& ) = delete;

    void Draw() noexcept override;
    std::string GetCode() const override;
    void GetShaderInfo( SShaderInfo& shaderInfo ) const override;

private:
    bool m_isInverted {};
};
