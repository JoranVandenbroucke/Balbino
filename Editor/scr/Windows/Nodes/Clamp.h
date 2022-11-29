#pragma once
#include "INode.h"

class CClamp : public INode
{
public:
	CClamp( int id, int& firstAttributeId );
	~CClamp() override = default;
	CClamp( const CClamp& ) = delete;
	CClamp( CClamp&& ) = delete;
	CClamp& operator=( const CClamp& ) = delete;
	CClamp& operator=( CClamp&& ) = delete;

	void Draw() override;
	void Attach( int endAttr ) override;
	void Detach( int endAttr ) override;
	std::string Evaluate(std::vector<INode*>::iterator& begin, std::set<std::string>& bindings, std::set<std::string>& includes, EAttributeType::Enum attributeType ) override;
	[[nodiscard]] bool HasFreeAttachment( int endAttr ) const override;
	[[nodiscard]] int GetId() const override;
	[[nodiscard]] std::vector<int> GetInputs() const override;
private:
	bool m_connected[3];
	float m_value[3];
};
