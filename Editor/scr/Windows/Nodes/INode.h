#pragma once
#include <ostream>
#include <vector>
#include <set>

enum class EAttributeType
{
	None,
	Float,
	Color,
	Vector,
    Texture
};

struct INode
{
	INode( int id, int& attributeStartId )
		: m_id{ id }
		, m_attributeStartId{ attributeStartId }
	{
	}

	virtual ~INode() = default;
	virtual void Draw() = 0;
	virtual void Attach( int endAttr ) = 0;
	virtual void Detach( int endAttr ) = 0;
	virtual std::string Evaluate(std::vector<INode*>::iterator& begin, std::set<std::string>& bindings, std::set<std::string>& includes, EAttributeType attributeType ) = 0;
	[[nodiscard]] virtual bool HasFreeAttachment( int endAttr ) const = 0;
	[[nodiscard]] virtual int GetId() const = 0;
	[[nodiscard]] virtual std::vector<int> GetInputs() const = 0;
protected:
	int m_id;
	int m_attributeStartId;
};
