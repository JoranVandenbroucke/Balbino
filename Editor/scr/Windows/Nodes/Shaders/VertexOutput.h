#pragma once
#include <array>

#include "../INode.h"

class CVertexOutputNode final : public INode
{
public:
    explicit CVertexOutputNode( int id, int& attributeStartId );
    ~CVertexOutputNode() override = default;
    CVertexOutputNode( const CVertexOutputNode& ) = default;
    CVertexOutputNode( CVertexOutputNode&& ) = delete;
    CVertexOutputNode& operator=( const CVertexOutputNode& ) = delete;
    CVertexOutputNode& operator=( CVertexOutputNode&& ) = delete;
    
    void Draw() override;
    void Attach( int endAttr ) override;
    void Detach( int endAttr ) override;
    std::string Evaluate( std::vector<INode*>::iterator& begin, std::set<std::string>& bindings, std::set<std::string>& includes, EAttributeType::Enum attributeType ) override;
    [[nodiscard]] bool HasFreeAttachment( int endAttr ) const override;
    [[nodiscard]] int GetId() const override;
    [[nodiscard]] std::vector<int> GetInputs() const override;
    [[nodiscard]] uint8_t GetVertexFlags() const override;
    void SetVertexFlags( uint8_t flags );

private:
    bool                 m_connections[5];
    bool                 m_useCameraTransform;
    uint8_t              m_inputFlags;
    std::array<float, 3> m_position;
    std::array<float, 3> m_uv;
    std::array<float, 3> m_color;
    std::array<float, 3> m_normal;
    std::array<float, 3> m_tangent;
    
    std::string m_inputString;
    std::string m_outputString;
    std::string m_calculationsString;
};
