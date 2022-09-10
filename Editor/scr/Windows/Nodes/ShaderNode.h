//
// Created by joran on 25/06/2022.
//

#ifndef GAME_SHADERNODE_H
#define GAME_SHADERNODE_H
#include "INode.h"

namespace BalEditor
{
    class CShaderNode : public INode
    {
    public:
        CShaderNode( int id, int& attributeStartId );
        ~CShaderNode() = default;
        CShaderNode( const CShaderNode& ) = delete;
        CShaderNode( CShaderNode&& ) = delete;
        CShaderNode& operator=( const CShaderNode& ) = delete;
        CShaderNode operator=( CShaderNode&& ) = delete;
        
        void Draw() override;
        void Attach( int endAttr ) override;
        void Detach( int endAttr ) override;
        std::string Evaluate( std::_Vector_iterator<std::_Vector_val<std::_Simple_types<INode*>>>& begin, std::set<std::string>& bindings, std::set<std::string>& includes, EAttributeType attributeType ) override;
        bool HasFreeAttachment( int endAttr ) const override;
        int GetId() const override;
        std::vector<int> GetInputs() const override;
        
    private:
        enum class ShaderCombos : uint8_t
        {
            Vertex,
            Geometry,
            Fragment,
            RTX
        }m_type;
        bool m_connections[8];
        int m_currentlyEvaluating;
        const char* ToString(ShaderCombos type);
    };
} // BalEditor

#endif //GAME_SHADERNODE_H
