//
// Created by joran on 02/03/2023.
//

#pragma once
#include <set>
#include <unordered_set>
#include "../ShaderNode.h"
#include "../ShaderNode.h"

class COutputNode final : public CShaderNode
{
public:
    explicit COutputNode( int& id );
    ~COutputNode() override = default;
    COutputNode( const COutputNode& ) = delete;
    COutputNode( COutputNode&& ) = delete;
    COutputNode& operator=( const COutputNode& ) = delete;
    COutputNode& operator=( COutputNode&& ) = delete;
    
    void Draw() override;
    std::string GetCode() const override;
    void GetShaderInfo( SShaderInfo& shaderInfo ) const override;
    int32_t GetShaderTypes()const;
private:
    std::vector<shader_stage> m_shaders{ shader_stage_vertex };
    std::vector<std::string>  m_allShaderNames;
};


