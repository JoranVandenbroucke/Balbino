#pragma once
#include "../INode.h"

#include <array>

class CVectorMath final : public INode
{
public:
    CVectorMath( int id, int& attributeStartId );
    ~CVectorMath() override;
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
        Add,
        Subtract,
        Multiply,
        Divide,
        MultiplyAdd,
        CrossProduct,
        Project,
        Reflect,
        Refract,
        Faceforward,
        DotProduct,
        Distance,
        Length,
        Scale,
        Normalize,
        Wrap,
        Snap,
        Floor,
        Ceil,
        Modulo,
        Fraction,
        Absolute,
        Minimum,
        Maximum,
        Sine,
        Cosine,
        Tangent,
        MaxIndex
    } m_type;
    
    bool                 m_connected[3];
    std::array<float, 3> m_vectorA;
    std::array<float, 3> m_vectorB;
    std::array<float, 3> m_vectorC;
    float                m_fac;
    
    static const char* ToString( EMode type );
};
