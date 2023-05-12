#pragma once
#include <array>

#include "../ShaderNode.h"

class CVertexOutputNode final : public CShaderNode
{
public:
    explicit CVertexOutputNode( int& id );
    ~CVertexOutputNode() override = default;
    CVertexOutputNode( const CVertexOutputNode& ) = default;
    CVertexOutputNode( CVertexOutputNode&& ) = delete;
    CVertexOutputNode& operator=( const CVertexOutputNode& ) = delete;
    CVertexOutputNode& operator=( CVertexOutputNode&& ) = delete;
    
    void Draw() override;
    std::string GetCode() const override;
    void GetShaderInfo( SShaderInfo& shaderInfo ) const override;
    
    void SetFlag( SSocketType::compiler_define flags );
private:
    uint32_t m_flags{};
    bool m_useModelView{true};
};
