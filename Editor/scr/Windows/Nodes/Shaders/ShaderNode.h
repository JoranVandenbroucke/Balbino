//
// Created by joran on 25/06/2022.
//

#ifndef GAME_SHADERNODE_H
#define GAME_SHADERNODE_H

#include "../INode.h"

class CShaderNode : public INode
{
public:
    CShaderNode( int id, int& attributeStartId );
    ~CShaderNode() override = default;
    CShaderNode( const CShaderNode& ) = delete;
    CShaderNode( CShaderNode&& ) = delete;
    CShaderNode& operator=( const CShaderNode& ) = delete;
    CShaderNode& operator=( CShaderNode&& ) = delete;
    
    void Draw() override;
    void Attach( int endAttr ) override;
    void Detach( int endAttr ) override;
    std::string Evaluate( std::vector<INode*>::iterator& begin, std::set<std::string>& bindings, std::set<std::string>& includes, EAttributeType::Enum attributeType ) override;
    [[nodiscard]] bool HasFreeAttachment( int endAttr ) const override;
    [[nodiscard]] int GetId() const override;
    [[nodiscard]] std::vector<int> GetInputs() const override;
    [[nodiscard]] uint8_t GetVertexFlags() const override;
private:
    enum class EMode : uint8_t
    {
        Graphics,
        Mesh,
        RTX,
        MaxIndex
    };
    EMode m_type;
    bool m_connections[6];
    int m_currentlyEvaluating;
    const char* ToString( EMode type );
};

#endif //GAME_SHADERNODE_H
