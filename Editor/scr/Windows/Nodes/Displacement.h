#pragma once
#include "INode.h"

#include <array>

class CDisplacement final : public INode
{
public:
	CDisplacement( int id, int& attributeStartId );

	~CDisplacement() override;
	void Draw() override;
	void Attach( int endAttr ) override;
	void Detach( int endAttr ) override;
	void Evaluate( std::vector<INode*>::iterator& begin, const std::vector<INode*>::iterator& end, std::ostream& output, EAttributeType attributeType ) override;
	[[nodiscard]] bool HasFreeAttachment( int endAttr ) const override;
	[[nodiscard]] int GetId() const override;
	[[nodiscard]] std::vector<int> GetInputs() const override;
private:
	bool m_connections[4];
	float m_height;
	float m_midLevel;
	float m_strength;
	std::array<float, 3> m_normal;
};
