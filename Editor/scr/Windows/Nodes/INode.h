#pragma once
#include <ostream>
#include <vector>

enum class EAttributeType
{
	None,
	Float,
	Color,
	Vector
};

struct INode
{
	INode( int id, int& attributeStartId )
		:m_id{ id }
		,m_attributeStartId{ attributeStartId }
	{}
	virtual ~INode()=default;
    virtual void Draw() = 0;
    virtual void Attach( int endAttr ) = 0;
    virtual void Detach( int endAttr ) = 0;
	virtual void Evaluate( std::vector<INode*>::iterator& begin, const std::vector<INode*>::iterator& end, std::ostream& output, EAttributeType attributeType = EAttributeType::None) = 0;
    [[nodiscard]] virtual bool HasFreeAttachment( int endAttr ) const = 0;
    [[nodiscard]] virtual int GetId() const = 0;
    [[nodiscard]] virtual std::vector<int> GetInputs()const = 0;
protected:
	int m_id;
	int m_attributeStartId;
};
