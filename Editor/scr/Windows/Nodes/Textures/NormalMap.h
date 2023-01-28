#pragma once
#include <array>

#include "../INode.h"

class CNormalMap final : public INode
{
public:
    CNormalMap( int id, int& attributeStartId );
    
    ~CNormalMap() override;
    void Draw() override;
    void Attach( int endAttr ) override;
    void Detach( int endAttr ) override;
    std::string Evaluate( std::vector<INode*>::iterator& begin, std::set<std::string>& bindings, std::set<std::string>& includes, EAttributeType::Enum attributeType ) override;
    [[nodiscard]] bool HasFreeAttachment( int endAttr ) const override;
    [[nodiscard]] int GetId() const override;
    [[nodiscard]] std::vector<int> GetInputs() const override;
    [[nodiscard]] uint8_t GetVertexFlags() const override;
private:
    bool                 m_connections[2];
    float                m_strength;
    std::array<float, 3> m_normal;
};
