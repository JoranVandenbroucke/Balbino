#pragma once
#include <ostream>
#include <vector>
#include <set>

struct EAttributeType
{
    enum Enum
    {
        None,
        Float,
        Color,
        Vector,
        Texture
    };
};

struct INode
{
    INode( int id, int& attributeStartId )
            : m_id{ id }
            , m_attributeStartId{ attributeStartId }
            ,m_vertexFlags{}
    {
    }
    
    virtual ~INode() = default;
    virtual void Draw() = 0;
    virtual void Attach( int endAttr ) = 0;
    virtual void Detach( int endAttr ) = 0;
    virtual std::string Evaluate( std::vector<INode*>::iterator& begin, std::set<std::string>& bindings, std::set<std::string>& includes, EAttributeType::Enum attributeType ) = 0;
    [[nodiscard]] virtual bool HasFreeAttachment( int endAttr ) const = 0;
    [[nodiscard]] virtual int GetId() const = 0;
    [[nodiscard]] virtual std::vector<int> GetInputs() const = 0;
    [[nodiscard]] virtual uint8_t GetVertexFlags() const = 0;
protected:
    int     m_id;
    int     m_attributeStartId;
    uint8_t m_vertexFlags;  //0:Colour, 1:UV, 2:Normal, 3:Tangent, 4: World
};
