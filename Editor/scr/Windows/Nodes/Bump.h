#pragma once
#include "INode.h"

#include <array>

class CBump final : public INode
{
public:
	CBump( int id, int& attributeStartId );

	~CBump() override;
	void Draw() override;
	void Attach( int endAttr ) override;
	void Detach( int endAttr ) override;
	void Evaluate( std::vector<INode*>::iterator& begin, const std::vector<INode*>::iterator& end, std::ostream& output, EAttributeType attributeType ) override;
	[[nodiscard]] bool HasFreeAttachment( int endAttr ) const override;
	[[nodiscard]] int GetId() const override;
	[[nodiscard]] std::vector<int> GetInputs() const override;
private:
	bool m_invert;
	bool m_connections[4];
	float m_strength;
	float m_distance;
	float m_height;
	std::array<float, 3> m_normal;
};
