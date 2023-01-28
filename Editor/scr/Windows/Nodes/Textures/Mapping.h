#pragma once
#include <array>

#include "../INode.h"

class CMapping final : public INode
{
public:
    CMapping( int id, int& attributeStartId );
    
    ~CMapping() override;
    void Draw() override;
    void Attach( int endAttr ) override;
    void Detach( int endAttr ) override;
    std::string Evaluate( std::vector<INode*>::iterator& begin, std::set<std::string>& bindings, std::set<std::string>& includes, EAttributeType::Enum attributeType ) override;
    [[nodiscard]] bool HasFreeAttachment( int endAttr ) const override;
    [[nodiscard]] int GetId() const override;
    [[nodiscard]] std::vector<int> GetInputs() const override;
    [[nodiscard]] uint8_t GetVertexFlags() const override;
private:
    enum class EMode
    {
        Point,
        Texture,
        Vector,
        Normal,
        MaxIndex
    } m_type;
    
    bool                 m_connected[4];
    std::array<float, 3> m_vector;
    std::array<float, 3> m_position;
    std::array<float, 3> m_rotation;
    std::array<float, 3> m_scale;
    static const char* ToString( EMode type );
};
